#include "ScheduleDemandsTableModel.h"
#include "ScheduleRoles.h"

#include <QLocale>



ScheduleDemandsTableModel::ScheduleDemandsTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{}



void ScheduleDemandsTableModel::setSchedule(const Schedule* schedule)
{
    if (_schedule == schedule)
        return;

    beginResetModel();
    _schedule = schedule;
    reloadEntryIds();
    endResetModel();
}



int ScheduleDemandsTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return _schedule ? static_cast<int>(_scheduleEntryIds.size()) : 0;
}

int ScheduleDemandsTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return _schedule ? static_cast<int>(_schedule->startDate().daysTo(_schedule->endDate()) + 1) : 0;
}



QVariant ScheduleDemandsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !_schedule)
        return {};

    qulonglong entryId = _scheduleEntryIds[index.row()];
    QDate date = _schedule->startDate().addDays(index.column());

    const ScheduleEntry* entry = _schedule->scheduleEntry(entryId);
    if (!entry)
        return {};

    qsizetype demandValue = _schedule->demand(entryId, date);
    qsizetype assignedValue = _schedule->assignedCount(entryId, date);

    switch (role)
    {
    case Qt::DisplayRole:
        return QString("%1/%2").arg(assignedValue).arg(demandValue);
    case Qt::EditRole:
        return demandValue > 0 ? QVariant::fromValue(demandValue) : QVariant{};

    case Qt::UserRole:
        return entryId;

    case ScheduleRoles::AssignedCount:
        return QVariant::fromValue(assignedValue);

    case ScheduleRoles::DemandCount:
        return QVariant::fromValue(demandValue);

    case ScheduleRoles::IsWeekend:
    {
        int dayOfWeek = date.dayOfWeek();
        return (dayOfWeek == 6 || dayOfWeek == 7);
    }

    default:
        return {};
    }
}

bool ScheduleDemandsTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || !_schedule)
        return false;

    qulonglong entryId = _scheduleEntryIds[index.row()];
    QDate date = _schedule->startDate().addDays(index.column());

    bool ok = false;
    qsizetype demand = value.toLongLong(&ok);
    if (!ok || demand < 0)
        demand = 0;

    emit demandChangeRequested(entryId, date, demand);
    return true;
}

QVariant ScheduleDemandsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!_schedule)
        return {};

    if (orientation == Qt::Horizontal)
    {
        QDate date = _schedule->startDate().addDays(section);
        int dayOfWeek = date.dayOfWeek();

        switch (role)
        {
        case Qt::DisplayRole:
        {
            static const QLocale englishLocale(QLocale::English);
            return QString("%1\n%2")
                .arg(date.toString("dd.MM"))
                .arg(englishLocale.dayName(dayOfWeek, QLocale::ShortFormat).toUpper());
        }
        case ScheduleRoles::IsWeekend:
            return (dayOfWeek == 6 || dayOfWeek == 7);
        default:
            return {};
        }
    }
    else if (orientation == Qt::Vertical)
    {
        if (section < 0 || section >= _scheduleEntryIds.size())
            return {};

        const ScheduleEntry* entry = _schedule->scheduleEntry(_scheduleEntryIds[section]);
        if (!entry)
            return {};

        if (role == Qt::DisplayRole)
            return entry->name();
        if (role == Qt::ForegroundRole)
            return entry->colorScheme().foreground;
        if (role == Qt::BackgroundRole)
            return entry->colorScheme().background;
    }

    return {};
}

Qt::ItemFlags ScheduleDemandsTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}



qulonglong ScheduleDemandsTableModel::getScheduleEntryIdFromRow(int row) const
{
    if (row < 0 || row >= _scheduleEntryIds.size())
        return 0;
    return _scheduleEntryIds[row];
}

QDate ScheduleDemandsTableModel::getDateFromCol(int col) const
{
    if (!_schedule || col < 0 || col >= columnCount())
        return {};
    return _schedule->startDate().addDays(col);
}

int ScheduleDemandsTableModel::findRowByScheduleEntryId(qulonglong id) const
{
    return static_cast<int>(_scheduleEntryIds.indexOf(id));
}

int ScheduleDemandsTableModel::findColByDate(const QDate& date) const
{
    if (!_schedule || date < _schedule->startDate() || date > _schedule->endDate())
        return -1;
    return static_cast<int>(_schedule->startDate().daysTo(date));
}



void ScheduleDemandsTableModel::onScheduleEntryRegistered(qulonglong id, const ScheduleEntryData& data)
{
    if (data.category != ScheduleEntryCategory::Work)
        return;

    int newRow = static_cast<int>(_scheduleEntryIds.size());
    beginInsertRows({}, newRow, newRow);
    _scheduleEntryIds.append(id);
    endInsertRows();
}

void ScheduleDemandsTableModel::onScheduleEntryUnregistered(qulonglong id)
{
    int row = findRowByScheduleEntryId(id);
    if (row < 0)
        return;

    beginRemoveRows({}, row, row);
    _scheduleEntryIds.removeAt(row);
    endRemoveRows();
}

void ScheduleDemandsTableModel::onScheduleEntryModified(qulonglong id, const ScheduleEntryData& data)
{
    int row = findRowByScheduleEntryId(id);

    if (data.category == ScheduleEntryCategory::Work)
    {
        if (row >= 0)
        {
            emit headerDataChanged(Qt::Vertical, row, row);
            emit dataChanged(index(row, 0), index(row, columnCount() - 1));
        }
        else
        {
            int newRow = static_cast<int>(_scheduleEntryIds.size());
            beginInsertRows({}, newRow, newRow);
            _scheduleEntryIds.append(id);
            endInsertRows();
        }
    }
    else
    {
        if (row >= 0)
        {
            beginRemoveRows({}, row, row);
            _scheduleEntryIds.removeAt(row);
            endRemoveRows();
        }
    }
}

void ScheduleDemandsTableModel::onScheduleEntryMoved(qsizetype from, qsizetype to)
{
    if (!_schedule)
        return;

    const auto& entries = _schedule->scheduleEntries();
    if (from < 0 || from >= entries.size() || to < 0 || to >= entries.size())
        return;

    if (entries[to].data().category != ScheduleEntryCategory::Work)
        return;

    beginResetModel();
    reloadEntryIds();
    endResetModel();
}

void ScheduleDemandsTableModel::onDemandChanged(qulonglong scheduleEntryId, const QDate& date, qsizetype count)
{
    Q_UNUSED(count);

    int row = findRowByScheduleEntryId(scheduleEntryId);
    int col = findColByDate(date);

    if (row < 0 || col < 0)
        return;

    QModelIndex cellIndex = index(row, col);
    emit dataChanged(cellIndex, cellIndex, { Qt::DisplayRole, Qt::EditRole, ScheduleRoles::DemandCount });
}

void ScheduleDemandsTableModel::onAssignmentChanged(qulonglong employeeId, const QDate& date, std::optional<qulonglong> scheduleEntryId)
{
    Q_UNUSED(employeeId);
    Q_UNUSED(scheduleEntryId);

    if (!_schedule)
        return;

    int col = findColByDate(date);
    if (col < 0 || _scheduleEntryIds.isEmpty())
        return;

    QModelIndex top = index(0, col);
    QModelIndex bottom = index(static_cast<int>(_scheduleEntryIds.size() - 1), col);
    emit dataChanged(top, bottom, { Qt::DisplayRole, ScheduleRoles::AssignedCount });
}



void ScheduleDemandsTableModel::reloadEntryIds()
{
    _scheduleEntryIds.clear();

    if (!_schedule)
        return;

    for (const ScheduleEntry& entry : _schedule->scheduleEntries())
    {
        if (entry.data().category == ScheduleEntryCategory::Work)
            _scheduleEntryIds.append(entry.id());
    }
}