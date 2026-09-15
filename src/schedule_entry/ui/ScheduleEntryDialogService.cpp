#include <QWidget>

#include "ConfirmDialog.h"
#include "InfoDialog.h"
#include "ScheduleEntryDialog.h"
#include "ScheduleEntryDialogService.h"



ScheduleEntryDialogService::ScheduleEntryDialogService(QWidget* parent) :
	_parent{ parent }
{}



std::optional<ScheduleEntryData> ScheduleEntryDialogService::requestAddScheduleEntry()
{
	return ScheduleEntryDialog::getEntryData(_parent);
}

std::optional<ScheduleEntryData> ScheduleEntryDialogService::requestModifyScheduleEntry(const ScheduleEntryData& current)
{
	return ScheduleEntryDialog::getEntryData(_parent, current);
}



bool ScheduleEntryDialogService::confirmRemoval(const ScheduleEntryData& data)
{
	return ConfirmDialog::confirm(_parent, "Schedule Entry Removal",
		QStringLiteral("Are you sure you want to remove schedule entry '%1'?").arg(data.name));
}

void ScheduleEntryDialogService::showMessage(const QString& title, const QString& message)
{
	InfoDialog::showMessage(_parent, title, message);
}