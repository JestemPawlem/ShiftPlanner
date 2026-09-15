#include "ScheduleDemandsDialogService.h"
#include "ScheduleDemandsWeeklyDialog.h"



ScheduleDemandsDialogService::ScheduleDemandsDialogService(QWidget* parent) :
	_parent{ parent }
{}



void ScheduleDemandsDialogService::showWeeklyDemandsDialog(ScheduleManager& manager)
{
	ScheduleDemandsWeeklyDialog::execute(manager, _parent);
}