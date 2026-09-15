#include <QFileDialog>
#include <QWidget>

#include "CreateScheduleDialog.h"
#include "InfoDialog.h"
#include "ScheduleDocumentDialogService.h"



ScheduleDocumentDialogService::ScheduleDocumentDialogService(QWidget* parent) :
	_parent{ parent }
{}



std::optional<ScheduleData> ScheduleDocumentDialogService::requestNewScheduleData(
	EmployeeRegistryManager* employeeManager,
	ScheduleEntryRegistryManager* entryManager)
{
	return CreateScheduleDialog::getScheduleData(employeeManager, entryManager, _parent);
}

std::optional<QString> ScheduleDocumentDialogService::requestOpenFile(const QString& defaultDir)
{
	QString path = QFileDialog::getOpenFileName(
		_parent,
		"Open Schedule",
		defaultDir,
		"Schedule Files (*.json)");

	if (path.isEmpty())
		return std::nullopt;

	return path;
}

std::optional<QString> ScheduleDocumentDialogService::requestSaveFile(const QString& initialPath)
{
	QString path = QFileDialog::getSaveFileName(
		_parent,
		"Save Schedule",
		initialPath,
		"Schedule Files (*.json)");

	if (path.isEmpty())
		return std::nullopt;

	return path;
}

void ScheduleDocumentDialogService::showMessage(const QString& title, const QString& message)
{
	InfoDialog::showMessage(_parent, title, message);
}