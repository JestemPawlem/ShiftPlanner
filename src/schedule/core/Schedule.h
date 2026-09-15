#pragma once

#include <optional>
#include <QDate>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QtTypes>

#include "Employee.h"
#include "EmployeeData.h"
#include "EmployeeRegistry.h"
#include "ScheduleEntry.h"
#include "ScheduleEntryData.h"
#include "ScheduleEntryRegistry.h"
#include "ScheduleData.h"
#include "ScheduleAssignments.h"
#include "ScheduleDemands.h"
#include "IStorable.h"
#include "CommonTypes.h"


class Schedule : public IStorable
{
public:
	Schedule() = default;
	explicit Schedule(const ScheduleData& data);

	// --- PODSTAWOWE ---
	ScheduleData data() const;
	QString name() const;
	void setName(const QString& name);
	QDate startDate() const;
	QDate endDate() const;

	// --- PRACOWNICY ---
	const EmployeeRegistry& employeeRegistry() const;
	QString employeeRegistryName() const;
	void setEmployeeRegistryName(const QString& name);
	const QList<Employee>& employees() const;
	const Employee* employee(qulonglong id) const;
	std::optional<qulonglong> registerEmployee(const EmployeeData& data);
	bool unregisterEmployee(qulonglong id);
	bool modifyEmployee(qulonglong id, const EmployeeData& data);
	bool moveEmployee(qsizetype from, qsizetype to);

	// --- DEFINICJE ZMIAN ---
	const ScheduleEntryRegistry& scheduleEntryRegistry() const;
	QString scheduleEntryRegistryName() const;
	void setScheduleEntryRegistryName(const QString& name);
	const QList<ScheduleEntry>& scheduleEntries() const;
	const ScheduleEntry* scheduleEntry(qulonglong id) const;
	std::optional<qulonglong> registerScheduleEntry(const ScheduleEntryData& data);
	bool unregisterScheduleEntry(qulonglong id);
	bool modifyScheduleEntry(qulonglong id, const ScheduleEntryData& data);
	bool moveScheduleEntry(qsizetype from, qsizetype to);

	// --- PRZYPISANIA ---
	const ScheduleAssignments& scheduleAssignments() const;
	bool isAvailable(qulonglong employeeId, const QDate& date) const;
	std::optional<qulonglong> assignment(qulonglong employeeId, const QDate& date) const;
	qsizetype assignedCount(qulonglong scheduleEntryId, const QDate& date) const;
	bool assignEntry(qulonglong employeeId, const QDate& date, qulonglong scheduleEntryId);
	void unassignEntry(qulonglong employeeId, const QDate& date);

	// --- ZAPOTRZEBOWANIE ---
	const ScheduleDemands& scheduleDemands() const;
	qsizetype demand(qulonglong scheduleEntryId, const QDate& date) const;
	bool setDemand(qulonglong scheduleEntryId, const QDate& date, qsizetype count);
	void removeDemand(qulonglong scheduleEntryId, const QDate& date);

	// --- SERIALIZACJA ---
	QVariant exportState() const override;
	result_t<void> importState(const QVariant& data) override;

private:
	bool isDateInRange(const QDate& date) const;

	QString _name;
	QDate _startDate;
	QDate _endDate;

	EmployeeRegistry _employeeRegistry;
	ScheduleEntryRegistry _scheduleEntryRegistry;
	ScheduleAssignments _scheduleAssignments;
	ScheduleDemands _scheduleDemands;
};