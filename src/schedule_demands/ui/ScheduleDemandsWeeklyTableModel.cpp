#include "ScheduleDemandsWeeklyTableModel.h"
#include "Schedule.h"
#include "ScheduleRoles.h"


ScheduleDemandsWeeklyTableModel::ScheduleDemandsWeeklyTableModel(const Schedule* schedule, QObject* parent)
    : QAbstractTableModel(parent)
    , _schedule(schedule)
{
    Q_ASSERT(_schedule != nullptr);

    for (const ScheduleEntry& entry : _schedule->scheduleEntries())
    {
        if (entry.data().category == ScheduleEntryCategory::Work)
        {
            _entryIds.append(entry.id());
            _grid.insert(entry.id(), QVector<qsizetype>(7, 0));
        }
    }
}


int ScheduleDemandsWeeklyTableModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(_entryIds.size());
}

int ScheduleDemandsWeeklyTableModel::columnCount(const QModelIndex& parent) const
{
    return 7;
}


QVariant ScheduleDemandsWeeklyTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= _entryIds.size() || index.column() >= 7)
        return {};

    qulonglong entryId = _entryIds[index.row()];
    int dayOfWeek = index.column() + 1;
    bool isWeekend = (dayOfWeek == 6 || dayOfWeek == 7);

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        qsizetype val = _grid[entryId][index.column()];
        return val > 0 ? QVariant::fromValue(val) : QVariant{};
    }
    case ScheduleRoles::IsWeekend:
        return isWeekend;
    default:
        return {};
    }
}

bool ScheduleDemandsWeeklyTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    qulonglong entryId = _entryIds[index.row()];
    bool ok = false;
    qsizetype count = value.toLongLong(&ok);
    if (!ok || count < 0)
        count = 0;

    _grid[entryId][index.column()] = count;
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
    return true;
}

QVariant ScheduleDemandsWeeklyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (section < 0 || section >= 7)
            return {};

        int dayOfWeek = section + 1;

        if (role == Qt::DisplayRole)
        {
            static const QLocale englishLocale(QLocale::English);
            return englishLocale.dayName(dayOfWeek, QLocale::ShortFormat).toUpper();
        }

        if (role == ScheduleRoles::IsWeekend)
            return (dayOfWeek == 6 || dayOfWeek == 7);
    }
    else if (orientation == Qt::Vertical)
    {
        if (section < 0 || section >= _entryIds.size())
            return {};

        const ScheduleEntry* entry = _schedule->scheduleEntry(_entryIds[section]);
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

Qt::ItemFlags ScheduleDemandsWeeklyTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

const QMap<qulonglong, QVector<qsizetype>>& ScheduleDemandsWeeklyTableModel::templateGrid() const
{
    return _grid;
}

const QList<qulonglong>& ScheduleDemandsWeeklyTableModel::entryIds() const
{
    return _entryIds;
}