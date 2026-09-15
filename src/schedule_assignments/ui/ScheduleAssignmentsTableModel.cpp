#include <QFont>
#include <QColor>

#include "ScheduleAssignmentsTableModel.h"
#include "ScheduleRoles.h"


ScheduleAssignmentsTableModel::ScheduleAssignmentsTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{}


void ScheduleAssignmentsTableModel::setSchedule(const Schedule* schedule)
{
    beginResetModel();
    _schedule = schedule;
    _employeeIds.clear();
    if (_schedule)
        _employeeIds = _schedule->employeeRegistry().ids();
    endResetModel();
}


int ScheduleAssignmentsTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return _schedule ? static_cast<int>(_employeeIds.size()) : 0;
}

int ScheduleAssignmentsTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return _schedule ? static_cast<int>(_schedule->startDate().daysTo(_schedule->endDate())) + 1 : 0;
}


QVariant ScheduleAssignmentsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !_schedule)
        return {};

    QDate date = getDateFromColumn(index.column());
    int dayOfWeek = date.dayOfWeek();
    bool isWeekend = (dayOfWeek == 6 || dayOfWeek == 7);

    if (role == ScheduleRoles::IsWeekend)
        return isWeekend;

    qulonglong empId = getEmployeeIdFromRow(index.row());
    std::optional<qulonglong> entryIdOpt = _schedule->assignment(empId, date);

    if (!entryIdOpt || *entryIdOpt == 0)
    {
        if (role == Qt::UserRole)
            return static_cast<qulonglong>(0);

        return {};
    }

    const ScheduleEntry* entry = _schedule->scheduleEntry(*entryIdOpt);
    if (!entry)
        return {};

    switch (role)
    {
    case Qt::DisplayRole:
        return entry->code();
    case Qt::BackgroundRole:
        return entry->colorScheme().background;
    case Qt::ForegroundRole:
        return entry->colorScheme().foreground;
    case Qt::UserRole:
        return entry->id();
    default:
        return {};
    }
}

QVariant ScheduleAssignmentsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!_schedule || section < 0 || section >= _employeeIds.size())
        return {};

    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        const Employee* emp = _schedule->employee(getEmployeeIdFromRow(section));
        if (!emp)
            return {};

        return emp->fullName();
    }

    return {};
}


qulonglong ScheduleAssignmentsTableModel::getEmployeeIdFromRow(int row) const
{
    return (row >= 0 && row < _employeeIds.size()) ? _employeeIds[row] : 0;
}

QDate ScheduleAssignmentsTableModel::getDateFromColumn(int col) const
{
    if (!_schedule || col < 0 || col >= columnCount())
        return QDate();

    return _schedule->startDate().addDays(col);
}

int ScheduleAssignmentsTableModel::findRowByEmployeeId(qulonglong id) const
{
    return static_cast<int>(_employeeIds.indexOf(id));
}

int ScheduleAssignmentsTableModel::findColByDate(const QDate& date) const
{
    if (!_schedule || !date.isValid() || date < _schedule->startDate() || date > _schedule->endDate())
        return -1;

    return static_cast<int>(_schedule->startDate().daysTo(date));
}


void ScheduleAssignmentsTableModel::onEmployeeRegistered(qulonglong id, const EmployeeData& data)
{
    Q_UNUSED(data);

    int row = static_cast<int>(_employeeIds.size());
    beginInsertRows({}, row, row);
    _employeeIds.append(id);
    endInsertRows();
}

void ScheduleAssignmentsTableModel::onEmployeeUnregistered(qulonglong id)
{
    int row = findRowByEmployeeId(id);
    if (row < 0)
        return;

    beginRemoveRows({}, row, row);
    _employeeIds.removeAt(row);
    endRemoveRows();
}

void ScheduleAssignmentsTableModel::onEmployeeModified(qulonglong id, const EmployeeData& data)
{
    Q_UNUSED(data);

    int row = findRowByEmployeeId(id);
    if (row >= 0)
        emit headerDataChanged(Qt::Vertical, row, row);
}

void ScheduleAssignmentsTableModel::onEmployeeMoved(qsizetype from, qsizetype to)
{
    if (from < 0 || from >= _employeeIds.size() || to < 0 || to >= _employeeIds.size() || from == to)
        return;

    int sourceRow = static_cast<int>(from);
    int destRow = static_cast<int>(to);
    int destChild = (destRow > sourceRow) ? destRow + 1 : destRow;

    if (!beginMoveRows({}, sourceRow, sourceRow, {}, destChild))
        return;

    _employeeIds.move(from, to);
    endMoveRows();
}

void ScheduleAssignmentsTableModel::onEntryModified(qulonglong id, const ScheduleEntryData& data)
{
    Q_UNUSED(id);
    Q_UNUSED(data);

    if (rowCount() > 0 && columnCount() > 0)
        emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void ScheduleAssignmentsTableModel::onAssignmentChanged(qulonglong employeeId, const QDate& date, std::optional<qulonglong> scheduleEntryId)
{
    Q_UNUSED(scheduleEntryId);

    int row = findRowByEmployeeId(employeeId);
    int col = findColByDate(date);

    if (row >= 0 && col >= 0)
    {
        QModelIndex idx = index(row, col);
        emit dataChanged(idx, idx, { Qt::DisplayRole, Qt::BackgroundRole, Qt::ForegroundRole, Qt::UserRole });
    }
}