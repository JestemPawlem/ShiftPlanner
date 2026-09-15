#pragma once

#include <optional>
#include <QString>
#include <QDate>

#include "EmployeeRegistry.h"
#include "ScheduleEntryRegistry.h"
#include "ScheduleAssignments.h"
#include "ScheduleDemands.h"


struct ScheduleData
{
	QString name;
	QDate startDate;
	QDate endDate;

	std::optional<EmployeeRegistry> employeeRegistry = std::nullopt;
	std::optional<ScheduleEntryRegistry> scheduleEntryRegistry = std::nullopt;
	std::optional<ScheduleAssignments> scheduleAssignments = std::nullopt;
	std::optional<ScheduleDemands> scheduleDemands = std::nullopt;
};