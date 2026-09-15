#include "ScheduleManager.h"



ScheduleManager::ScheduleManager(std::unique_ptr<IStorage> storage) :
	_storage(std::move(storage))
{}



void ScheduleManager::createNew(const ScheduleData& data)
{
	_activeSchedule = std::make_unique<Schedule>(data);
	emit scheduleOpened(*_activeSchedule);
}

result_t<void> ScheduleManager::open(const QString& path)
{
	auto tempSchedule = std::make_unique<Schedule>();

	result_t<void> result = _storage->load(*tempSchedule, path);
	if (!result)
		return result;

	_activeSchedule = std::move(tempSchedule);
	emit scheduleOpened(*_activeSchedule);
	return {};
}

result_t<void> ScheduleManager::save(const QString& path)
{
	if (!_activeSchedule)
		return std::unexpected("No active schedule");

	result_t<void> result = _storage->save(*_activeSchedule, path);
	if (!result)
		return result;

	emit scheduleSaved(path);
	return {};
}

void ScheduleManager::close()
{
	_activeSchedule.reset();
	emit scheduleClosed();
}



const Schedule* ScheduleManager::schedule() const
{
	return _activeSchedule.get();
}

QString ScheduleManager::name() const
{
	if (!_activeSchedule)
		return {};

	return _activeSchedule->name();
}

bool ScheduleManager::setName(const QString& name)
{
	if (!_activeSchedule)
		return false;

	_activeSchedule->setName(name);
	emit nameChanged(name);
	return true;
}



const EmployeeRegistry* ScheduleManager::employeeRegistry() const
{
	if (!_activeSchedule)
		return nullptr;

	return &_activeSchedule->employeeRegistry();
}

QString ScheduleManager::employeeRegistryName() const
{
	if (!_activeSchedule)
		return {};

	return _activeSchedule->employeeRegistryName();
}

bool ScheduleManager::setEmployeeRegistryName(const QString& name)
{
	if (!_activeSchedule)
		return false;

	_activeSchedule->setEmployeeRegistryName(name);
	emit employeeRegistryNameChanged(name);
	return true;
}

QList<Employee> ScheduleManager::employees() const
{
	if (!_activeSchedule)
		return {};

	return _activeSchedule->employees();
}

const Employee* ScheduleManager::employee(qulonglong id) const
{
	if (!_activeSchedule)
		return nullptr;

	return _activeSchedule->employee(id);
}

std::optional<qulonglong> ScheduleManager::registerEmployee(const EmployeeData& data)
{
	if (!_activeSchedule)
		return std::nullopt;

	std::optional<qulonglong> idOpt = _activeSchedule->registerEmployee(data);
	if (!idOpt)
		return std::nullopt;

	emit employeeRegistered(*idOpt, data);
	return idOpt;
}

bool ScheduleManager::unregisterEmployee(qulonglong id)
{
	if (!_activeSchedule || !_activeSchedule->unregisterEmployee(id))
		return false;

	emit employeeUnregistered(id);
	return true;
}

bool ScheduleManager::modifyEmployee(qulonglong id, const EmployeeData& data)
{
	if (!_activeSchedule || !_activeSchedule->modifyEmployee(id, data))
		return false;

	emit employeeModified(id, data);
	return true;
}

bool ScheduleManager::moveEmployee(qsizetype from, qsizetype to)
{
	if (!_activeSchedule || !_activeSchedule->moveEmployee(from, to))
		return false;

	emit employeeMoved(from, to);
	return true;
}



const ScheduleEntryRegistry* ScheduleManager::scheduleEntryRegistry() const
{
	if (!_activeSchedule)
		return nullptr;

	return &_activeSchedule->scheduleEntryRegistry();
}

QString ScheduleManager::scheduleEntryRegistryName() const
{
	if (!_activeSchedule)
		return {};

	return _activeSchedule->scheduleEntryRegistryName();
}

bool ScheduleManager::setScheduleEntryRegistryName(const QString& name)
{
	if (!_activeSchedule)
		return false;

	_activeSchedule->setScheduleEntryRegistryName(name);
	emit scheduleEntryRegistryNameChanged(name);
	return true;
}

QList<ScheduleEntry> ScheduleManager::scheduleEntries() const
{
	if (!_activeSchedule)
		return {};

	return _activeSchedule->scheduleEntries();
}

const ScheduleEntry* ScheduleManager::scheduleEntry(qulonglong id) const
{
	if (!_activeSchedule)
		return nullptr;

	return _activeSchedule->scheduleEntry(id);
}

std::optional<qulonglong> ScheduleManager::registerScheduleEntry(const ScheduleEntryData& data)
{
	if (!_activeSchedule)
		return std::nullopt;

	std::optional<qulonglong> idOpt = _activeSchedule->registerScheduleEntry(data);
	if (!idOpt)
		return std::nullopt;

	emit scheduleEntryRegistered(*idOpt, data);
	return idOpt;
}

bool ScheduleManager::unregisterScheduleEntry(qulonglong id)
{
	if (!_activeSchedule || !_activeSchedule->unregisterScheduleEntry(id))
		return false;

	emit scheduleEntryUnregistered(id);
	return true;
}

bool ScheduleManager::modifyScheduleEntry(qulonglong id, const ScheduleEntryData& data)
{
	if (!_activeSchedule || !_activeSchedule->modifyScheduleEntry(id, data))
		return false;

	emit scheduleEntryModified(id, data);
	return true;
}

bool ScheduleManager::moveScheduleEntry(qsizetype from, qsizetype to)
{
	if (!_activeSchedule || !_activeSchedule->moveScheduleEntry(from, to))
		return false;

	emit scheduleEntryMoved(from, to);
	return true;
}



const ScheduleAssignments* ScheduleManager::scheduleAssignments() const
{
	if (!_activeSchedule)
		return nullptr;

	return &_activeSchedule->scheduleAssignments();
}

bool ScheduleManager::isAvailable(qulonglong employeeId, const QDate& date) const
{
	if (!_activeSchedule)
		return false;

	return _activeSchedule->isAvailable(employeeId, date);
}

std::optional<qulonglong> ScheduleManager::assignment(qulonglong employeeId, const QDate& date) const
{
	if (!_activeSchedule)
		return std::nullopt;

	return _activeSchedule->assignment(employeeId, date);
}

qsizetype ScheduleManager::assignedCount(qulonglong scheduleEntryId, const QDate& date) const
{
	if (!_activeSchedule)
		return 0;

	return _activeSchedule->assignedCount(scheduleEntryId, date);
}

bool ScheduleManager::assignEntry(qulonglong employeeId, const QDate& date, qulonglong scheduleEntryId)
{
	if (!_activeSchedule || !_activeSchedule->assignEntry(employeeId, date, scheduleEntryId))
		return false;

	emit assignmentChanged(employeeId, date, scheduleEntryId);
	return true;
}

void ScheduleManager::unassignEntry(qulonglong employeeId, const QDate& date)
{
	if (!_activeSchedule)
		return;

	_activeSchedule->unassignEntry(employeeId, date);
	emit assignmentChanged(employeeId, date, std::nullopt);
}



const ScheduleDemands* ScheduleManager::scheduleDemands() const
{
	if (!_activeSchedule)
		return nullptr;

	return &_activeSchedule->scheduleDemands();
}

qsizetype ScheduleManager::demand(qulonglong scheduleEntryId, const QDate& date) const
{
	if (!_activeSchedule)
		return 0;

	return _activeSchedule->demand(scheduleEntryId, date);
}

bool ScheduleManager::setDemand(qulonglong scheduleEntryId, const QDate& date, qsizetype count)
{
	if (!_activeSchedule || !_activeSchedule->setDemand(scheduleEntryId, date, count))
		return false;

	emit demandChanged(scheduleEntryId, date, count);
	return true;
}

void ScheduleManager::removeDemand(qulonglong scheduleEntryId, const QDate& date)
{
	if (!_activeSchedule)
		return;

	_activeSchedule->removeDemand(scheduleEntryId, date);
	emit demandChanged(scheduleEntryId, date, 0);
}