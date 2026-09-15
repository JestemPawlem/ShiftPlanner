#pragma once

class ScheduleManager;


class IScheduleDemandsDialogService
{
public:
	virtual ~IScheduleDemandsDialogService() = default;

	virtual void showWeeklyDemandsDialog(ScheduleManager& manager) = 0;
};