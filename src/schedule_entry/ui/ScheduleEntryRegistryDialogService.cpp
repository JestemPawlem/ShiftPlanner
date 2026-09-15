#include <QWidget>

#include "ConfirmDialog.h"
#include "InfoDialog.h"
#include "InputDialog.h"
#include "ScheduleEntryRegistryDialogService.h"
#include "ScheduleEntryRegistryDialog.h"



ScheduleEntryRegistryDialogService::ScheduleEntryRegistryDialogService(QWidget* parent) :
	_parent{ parent }
{}



void ScheduleEntryRegistryDialogService::showDialog(IScheduleEntryRegistryManager& manager)
{
	ScheduleEntryRegistryDialog dialog{ manager, this, _parent };
	dialog.exec();
}

std::optional<QString> ScheduleEntryRegistryDialogService::requestTemplateName(const QString& currentName)
{
	return InputDialog::getText(
		_parent,
		"Save Definitions As Template",
		"Template name:",
		currentName);
}

bool ScheduleEntryRegistryDialogService::confirmRemoval(const QString& name)
{
	return ConfirmDialog::confirm(
		_parent,
		"Delete Template",
		QStringLiteral("Are you sure you want to delete template '%1'?").arg(name));
}

void ScheduleEntryRegistryDialogService::showMessage(const QString& title, const QString& message)
{
	InfoDialog::showMessage(_parent, title, message);
}