#include "Schedule.h"



Schedule::Schedule(const ScheduleData& data) :
	_name(data.name),
	_startDate(data.startDate),
	_endDate(data.endDate)
{
	Q_ASSERT(_startDate.isValid() && _endDate.isValid() && _startDate <= _endDate);

	if (data.employeeRegistry)
		_employeeRegistry = *data.employeeRegistry;

	if (data.scheduleEntryRegistry)
		_scheduleEntryRegistry = *data.scheduleEntryRegistry;

	if (data.scheduleAssignments)
		_scheduleAssignments = *data.scheduleAssignments;

	if (data.scheduleDemands)
		_scheduleDemands = *data.scheduleDemands;
}



ScheduleData Schedule::data() const
{
	return {
		_name,
		_startDate,
		_endDate,
		_employeeRegistry,
		_scheduleEntryRegistry,
		_scheduleAssignments,
		_scheduleDemands
	};
}

QString Schedule::name() const
{
	return _name;
}

void Schedule::setName(const QString& name)
{
	_name = name;
}

QDate Schedule::startDate() const
{
	return _startDate;
}

QDate Schedule::endDate() const
{
	return _endDate;
}



const EmployeeRegistry& Schedule::employeeRegistry() const
{
	return _employeeRegistry;
}

QString Schedule::employeeRegistryName() const
{
	return _employeeRegistry.name();
}

void Schedule::setEmployeeRegistryName(const QString& name)
{
	_employeeRegistry.setName(name);
}

const QList<Employee>& Schedule::employees() const
{
	return _employeeRegistry.entities();
}

const Employee* Schedule::employee(qulonglong id) const
{
	return _employeeRegistry.entity(id);
}

std::optional<qulonglong> Schedule::registerEmployee(const EmployeeData& data)
{
	return _employeeRegistry.add(data);
}

bool Schedule::unregisterEmployee(qulonglong id)
{
	if (!_employeeRegistry.remove(id))
		return false;

	_scheduleAssignments.removeEmployee(id);
	return true;
}

bool Schedule::modifyEmployee(qulonglong id, const EmployeeData& data)
{
	return _employeeRegistry.modify(id, data);
}

bool Schedule::moveEmployee(qsizetype from, qsizetype to)
{
	return _employeeRegistry.move(from, to);
}



const ScheduleEntryRegistry& Schedule::scheduleEntryRegistry() const
{
	return _scheduleEntryRegistry;
}

QString Schedule::scheduleEntryRegistryName() const
{
	return _scheduleEntryRegistry.name();
}

void Schedule::setScheduleEntryRegistryName(const QString& name)
{
	_scheduleEntryRegistry.setName(name);
}

const QList<ScheduleEntry>& Schedule::scheduleEntries() const
{
	return _scheduleEntryRegistry.entities();
}

const ScheduleEntry* Schedule::scheduleEntry(qulonglong id) const
{
	return _scheduleEntryRegistry.entity(id);
}

std::optional<qulonglong> Schedule::registerScheduleEntry(const ScheduleEntryData& data)
{
	return _scheduleEntryRegistry.add(data);
}

bool Schedule::unregisterScheduleEntry(qulonglong id)
{
	if (!_scheduleEntryRegistry.remove(id))
		return false;

	_scheduleAssignments.removeScheduleEntry(id);
	_scheduleDemands.removeScheduleEntry(id);
	return true;
}

bool Schedule::modifyScheduleEntry(qulonglong id, const ScheduleEntryData& data)
{
	return _scheduleEntryRegistry.modify(id, data);
}

bool Schedule::moveScheduleEntry(qsizetype from, qsizetype to)
{
	return _scheduleEntryRegistry.move(from, to);
}



const ScheduleAssignments& Schedule::scheduleAssignments() const
{
	return _scheduleAssignments;
}

bool Schedule::isAvailable(qulonglong employeeId, const QDate& date) const
{
	return _scheduleAssignments.isAvailable(employeeId, date);
}

std::optional<qulonglong> Schedule::assignment(qulonglong employeeId, const QDate& date) const
{
	return _scheduleAssignments.assignment(employeeId, date);
}

qsizetype Schedule::assignedCount(qulonglong scheduleEntryId, const QDate& date) const
{
	return _scheduleAssignments.assignedCount(scheduleEntryId, date);
}

bool Schedule::assignEntry(qulonglong employeeId, const QDate& date, qulonglong scheduleEntryId)
{
	if (!isDateInRange(date) || !_employeeRegistry.entity(employeeId) || !_scheduleEntryRegistry.entity(scheduleEntryId))
		return false;

	_scheduleAssignments.setAssignment(employeeId, date, scheduleEntryId);
	return true;
}

void Schedule::unassignEntry(qulonglong employeeId, const QDate& date)
{
	_scheduleAssignments.removeAssignment(employeeId, date);
}



const ScheduleDemands& Schedule::scheduleDemands() const
{
	return _scheduleDemands;
}

qsizetype Schedule::demand(qulonglong scheduleEntryId, const QDate& date) const
{
	return _scheduleDemands.demand(scheduleEntryId, date);
}

bool Schedule::setDemand(qulonglong scheduleEntryId, const QDate& date, qsizetype count)
{
	if (!isDateInRange(date) || !_scheduleEntryRegistry.entity(scheduleEntryId))
		return false;

	_scheduleDemands.setDemand(scheduleEntryId, date, count);
	return true;
}

void Schedule::removeDemand(qulonglong scheduleEntryId, const QDate& date)
{
	_scheduleDemands.removeDemand(scheduleEntryId, date);
}



QVariant Schedule::exportState() const
{
	QVariantMap root;

	root["name"] = _name;
	root["startDate"] = _startDate.toString(Qt::ISODate);
	root["endDate"] = _endDate.toString(Qt::ISODate);
	root["employeeRegistry"] = _employeeRegistry.exportState();
	root["scheduleEntryRegistry"] = _scheduleEntryRegistry.exportState();
	root["scheduleAssignments"] = _scheduleAssignments.exportState();
	root["scheduleDemands"] = _scheduleDemands.exportState();

	return root;
}

result_t<void> Schedule::importState(const QVariant& data)
{
	if (!data.canConvert<QVariantMap>())
		return std::unexpected("Illegal schedule data format");

	const QVariantMap root = data.toMap();

	QString tempName = root["name"].toString();
	QDate tempStartDate = QDate::fromString(root["startDate"].toString(), Qt::ISODate);
	QDate tempEndDate = QDate::fromString(root["endDate"].toString(), Qt::ISODate);

	if (!tempStartDate.isValid() || !tempEndDate.isValid() || tempStartDate > tempEndDate)
		return std::unexpected("Invalid date range in schedule data");

	EmployeeRegistry tempEmployeeRegistry;
	ScheduleEntryRegistry tempScheduleEntryRegistry;
	ScheduleAssignments tempScheduleAssignments;
	ScheduleDemands tempScheduleDemands;

	result_t<void> result = tempEmployeeRegistry.importState(root["employeeRegistry"]);
	if (!result)
		return result;

	result = tempScheduleEntryRegistry.importState(root["scheduleEntryRegistry"]);
	if (!result)
		return result;

	result = tempScheduleAssignments.importState(root["scheduleAssignments"]);
	if (!result)
		return result;

	if (root.contains("scheduleDemands"))
	{
		result = tempScheduleDemands.importState(root["scheduleDemands"]);
		if (!result)
			return result;
	}

	_name = tempName;
	_startDate = tempStartDate;
	_endDate = tempEndDate;

	_employeeRegistry = std::move(tempEmployeeRegistry);
	_scheduleEntryRegistry = std::move(tempScheduleEntryRegistry);
	_scheduleAssignments = std::move(tempScheduleAssignments);
	_scheduleDemands = std::move(tempScheduleDemands);

	return {};
}



bool Schedule::isDateInRange(const QDate& date) const
{
	return date.isValid() && date >= _startDate && date <= _endDate;
}