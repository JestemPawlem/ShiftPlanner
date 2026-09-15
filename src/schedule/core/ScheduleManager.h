#pragma once

#include <memory>
#include <optional>
#include <QDate>
#include <QList>
#include <QObject>
#include <QString>
#include <QtTypes>

#include "CommonTypes.h"
#include "Schedule.h"
#include "ScheduleData.h"
#include "IStorage.h"


class ScheduleManager : public QObject
{
	Q_OBJECT
public:
	explicit ScheduleManager(std::unique_ptr<IStorage> storage);

	// --- PODSTAWOWE / CYKL ŻYCIA ---
	void createNew(const ScheduleData& data);
	result_t<void> open(const QString& path);
	result_t<void> save(const QString& path);
	void close();

	// --- GRAFIK ---
	const Schedule* schedule() const;
	QString name() const;
	bool setName(const QString& name);

	// --- PRACOWNICY ---
	const EmployeeRegistry* employeeRegistry() const;
	QString employeeRegistryName() const;
	bool setEmployeeRegistryName(const QString& name);
	QList<Employee> employees() const;
	const Employee* employee(qulonglong id) const;
	std::optional<qulonglong> registerEmployee(const EmployeeData& data);
	bool unregisterEmployee(qulonglong id);
	bool modifyEmployee(qulonglong id, const EmployeeData& data);
	bool moveEmployee(qsizetype from, qsizetype to);

	// --- DEFINICJE ZMIAN ---
	const ScheduleEntryRegistry* scheduleEntryRegistry() const;
	QString scheduleEntryRegistryName() const;
	bool setScheduleEntryRegistryName(const QString& name);
	QList<ScheduleEntry> scheduleEntries() const;
	const ScheduleEntry* scheduleEntry(qulonglong id) const;
	std::optional<qulonglong> registerScheduleEntry(const ScheduleEntryData& data);
	bool unregisterScheduleEntry(qulonglong id);
	bool modifyScheduleEntry(qulonglong id, const ScheduleEntryData& data);
	bool moveScheduleEntry(qsizetype from, qsizetype to);

	// --- PRZYPISANIA ---
	const ScheduleAssignments* scheduleAssignments() const;
	bool isAvailable(qulonglong employeeId, const QDate& date) const;
	std::optional<qulonglong> assignment(qulonglong employeeId, const QDate& date) const;
	qsizetype assignedCount(qulonglong scheduleEntryId, const QDate& date) const;
	bool assignEntry(qulonglong employeeId, const QDate& date, qulonglong scheduleEntryId);
	void unassignEntry(qulonglong employeeId, const QDate& date);

	// --- ZAPOTRZEBOWANIE ---
	const ScheduleDemands* scheduleDemands() const;
	qsizetype demand(qulonglong scheduleEntryId, const QDate& date) const;
	bool setDemand(qulonglong scheduleEntryId, const QDate& date, qsizetype count);
	void removeDemand(qulonglong scheduleEntryId, const QDate& date);

signals:
	void scheduleOpened(const Schedule& schedule);
	void scheduleSaved(const QString& path);
	void scheduleClosed();
	void nameChanged(const QString& name);

	void employeeRegistryNameChanged(const QString& name);
	void employeeRegistered(qulonglong id, const EmployeeData& data);
	void employeeUnregistered(qulonglong id);
	void employeeModified(qulonglong id, const EmployeeData& data);
	void employeeMoved(qsizetype from, qsizetype to);

	void scheduleEntryRegistryNameChanged(const QString& name);
	void scheduleEntryRegistered(qulonglong id, const ScheduleEntryData& data);
	void scheduleEntryUnregistered(qulonglong id);
	void scheduleEntryModified(qulonglong id, const ScheduleEntryData& data);
	void scheduleEntryMoved(qsizetype from, qsizetype to);

	void assignmentChanged(qulonglong employeeId, const QDate& date, std::optional<qulonglong> scheduleEntryId);
	void demandChanged(qulonglong scheduleEntryId, const QDate& date, qsizetype count);

private:
	std::unique_ptr<IStorage> _storage;
	std::unique_ptr<Schedule> _activeSchedule;
};