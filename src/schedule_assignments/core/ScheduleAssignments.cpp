#include <QVariantMap>

#include "ScheduleAssignments.h"


const QMap<qulonglong, QMap<QDate, qulonglong>>& ScheduleAssignments::assignments() const
{
	return _assignments;
}



bool ScheduleAssignments::isAvailable(qulonglong employeeId, const QDate& date) const
{
	auto it = _assignments.find(employeeId);
	return (it == _assignments.end()) || !it.value().contains(date);
}

std::optional<qulonglong> ScheduleAssignments::assignment(qulonglong employeeId, const QDate& date) const
{
	auto it = _assignments.find(employeeId);
	if (it == _assignments.end())
		return std::nullopt;

	auto assignIt = it.value().find(date);
	if (assignIt == it.value().end())
		return std::nullopt;

	return assignIt.value();
}

qsizetype ScheduleAssignments::assignedCount(qulonglong scheduleEntryId, const QDate& date) const
{
	auto it = _assignedCounts.find(date);
	if (it == _assignedCounts.end())
		return 0;

	return it.value().value(scheduleEntryId, 0);
}



void ScheduleAssignments::setAssignment(qulonglong employeeId, const QDate& date, qulonglong scheduleEntryId)
{
	if (employeeId == 0 || scheduleEntryId == 0)
		return;

	auto& empDates = _assignments[employeeId];
	auto it = empDates.find(date);

	if (it != empDates.end())
	{
		qulonglong oldEntryId = it.value();
		if (oldEntryId == scheduleEntryId)
			return;

		decrementCount(oldEntryId, date);
	}

	empDates.insert(date, scheduleEntryId);
	_assignedCounts[date][scheduleEntryId]++;
}

void ScheduleAssignments::removeAssignment(qulonglong employeeId, const QDate& date)
{
	auto it = _assignments.find(employeeId);
	if (it == _assignments.end())
		return;

	auto dateIt = it.value().find(date);
	if (dateIt == it.value().end())
		return;

	decrementCount(dateIt.value(), date);
	it.value().erase(dateIt);

	if (it.value().isEmpty())
		_assignments.erase(it);
}



void ScheduleAssignments::removeEmployee(qulonglong employeeId)
{
	auto it = _assignments.find(employeeId);
	if (it == _assignments.end())
		return;

	for (auto dateIt = it.value().cbegin(); dateIt != it.value().cend(); ++dateIt)
		decrementCount(dateIt.value(), dateIt.key());

	_assignments.erase(it);
}

void ScheduleAssignments::removeScheduleEntry(qulonglong scheduleEntryId)
{
	for (auto it = _assignments.begin(); it != _assignments.end();)
	{
		it.value().removeIf([this, scheduleEntryId](auto entryIt)
			{
				if (entryIt.value() == scheduleEntryId)
				{
					decrementCount(scheduleEntryId, entryIt.key());
					return true;
				}
				return false;
			});

		if (it.value().isEmpty())
			it = _assignments.erase(it);
		else
			++it;
	}
}



void ScheduleAssignments::clear()
{
	_assignments.clear();
	_assignedCounts.clear();
}



QVariant ScheduleAssignments::exportState() const
{
	QVariantMap root;
	QVariantMap assignmentsMap;

	for (auto empIt = _assignments.cbegin(); empIt != _assignments.cend(); ++empIt)
	{
		QVariantMap datesMap;
		for (auto dateIt = empIt.value().cbegin(); dateIt != empIt.value().cend(); ++dateIt)
			datesMap.insert(dateIt.key().toString(Qt::ISODate), dateIt.value());

		assignmentsMap.insert(QString::number(empIt.key()), datesMap);
	}

	root["assignments"] = assignmentsMap;
	return root;
}

result_t<void> ScheduleAssignments::importState(const QVariant& data)
{
	if (!data.canConvert<QVariantMap>())
		return std::unexpected("Illegal assignments data format");

	const QVariantMap root = data.toMap();
	const QVariantMap assignmentsMap = root["assignments"].toMap();
	QMap<qulonglong, QMap<QDate, qulonglong>> tempAssignments;
	QMap<QDate, QMap<qulonglong, qsizetype>> tempCounts;

	for (auto empIt = assignmentsMap.cbegin(); empIt != assignmentsMap.cend(); ++empIt)
	{
		qulonglong employeeId = empIt.key().toULongLong();
		const QVariantMap datesMap = empIt.value().toMap();
		QMap<QDate, qulonglong> tempDates;

		for (auto dateIt = datesMap.cbegin(); dateIt != datesMap.cend(); ++dateIt)
		{
			QDate date = QDate::fromString(dateIt.key(), Qt::ISODate);
			if (!date.isValid())
				continue;

			qulonglong scheduleEntryId = dateIt.value().toULongLong();
			if (scheduleEntryId != 0)
			{
				tempDates.insert(date, scheduleEntryId);
				tempCounts[date][scheduleEntryId]++;
			}
		}

		if (!tempDates.isEmpty())
			tempAssignments.insert(employeeId, tempDates);
	}

	_assignments = std::move(tempAssignments);
	_assignedCounts = std::move(tempCounts);
	return {};
}



void ScheduleAssignments::decrementCount(qulonglong scheduleEntryId, const QDate& date)
{
	auto dateIt = _assignedCounts.find(date);
	if (dateIt == _assignedCounts.end())
		return;

	auto countIt = dateIt.value().find(scheduleEntryId);
	if (countIt == dateIt.value().end())
		return;

	if (--countIt.value() <= 0)
		dateIt.value().erase(countIt);

	if (dateIt.value().isEmpty())
		_assignedCounts.erase(dateIt);
}