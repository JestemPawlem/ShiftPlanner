#pragma once

#include <optional>
#include <QString>

#include "ScheduleData.h"

class EmployeeRegistryManager;
class ScheduleEntryRegistryManager;


class IScheduleDocumentDialogService
{
public:
	virtual ~IScheduleDocumentDialogService() = default;

	virtual std::optional<ScheduleData> requestNewScheduleData(
		EmployeeRegistryManager* employeeManager,
		ScheduleEntryRegistryManager* entryManager) = 0;

	virtual std::optional<QString> requestOpenFile(const QString& defaultDir) = 0;
	virtual std::optional<QString> requestSaveFile(const QString& initialPath) = 0;

	virtual void showMessage(const QString& title, const QString& message) = 0;
};