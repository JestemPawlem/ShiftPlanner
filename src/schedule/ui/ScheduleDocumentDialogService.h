#pragma once

#include "IScheduleDocumentDialogService.h"

class QWidget;


class ScheduleDocumentDialogService : public IScheduleDocumentDialogService
{
public:
	explicit ScheduleDocumentDialogService(QWidget* parent);

	std::optional<ScheduleData> requestNewScheduleData(
		EmployeeRegistryManager* employeeManager,
		ScheduleEntryRegistryManager* entryManager) override;

	std::optional<QString> requestOpenFile(const QString& defaultDir) override;
	std::optional<QString> requestSaveFile(const QString& initialPath) override;

	void showMessage(const QString& title, const QString& message) override;

private:
	QWidget* _parent;
};