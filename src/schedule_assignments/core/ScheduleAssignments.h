#pragma once

#include <QDate>
#include <QMap>
#include <QVariant>
#include <optional>
#include <QtTypes>

#include "CommonTypes.h"
#include "IStorable.h"

class ScheduleAssignments : public IStorable
{
public:
	ScheduleAssignments() = default;

	const QMap<qulonglong, QMap<QDate, qulonglong>>& assignments() const;

	bool isAvailable(qulonglong employeeId, const QDate& date) const;
	std::optional<qulonglong> assignment(qulonglong employeeId, const QDate& date) const;
	qsizetype assignedCount(qulonglong scheduleEntryId, const QDate& date) const;

	void setAssignment(qulonglong employeeId, const QDate& date, qulonglong scheduleEntryId);
	void removeAssignment(qulonglong employeeId, const QDate& date);

	void removeEmployee(qulonglong employeeId);
	void removeScheduleEntry(qulonglong scheduleEntryId);

	void clear();

	QVariant exportState() const override;
	result_t<void> importState(const QVariant& data) override;

private:
	void decrementCount(qulonglong scheduleEntryId, const QDate& date);

	QMap<qulonglong, QMap<QDate, qulonglong>> _assignments;
	QMap<QDate, QMap<qulonglong, qsizetype>> _assignedCounts;
};