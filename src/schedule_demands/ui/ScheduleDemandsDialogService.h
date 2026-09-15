#pragma once

#include "IScheduleDemandsDialogService.h"

class QWidget;


class ScheduleDemandsDialogService : public IScheduleDemandsDialogService
{
public:
	explicit ScheduleDemandsDialogService(QWidget* parent);

	void showWeeklyDemandsDialog(ScheduleManager& manager) override;

private:
	QWidget* _parent;
};