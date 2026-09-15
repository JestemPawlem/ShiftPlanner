#pragma once

#include "IScheduleEntryDialogService.h"


class QWidget;

class ScheduleEntryDialogService : public IScheduleEntryDialogService
{
public:
	explicit ScheduleEntryDialogService(QWidget* parent);

	std::optional<ScheduleEntryData> requestAddScheduleEntry() override;
	std::optional<ScheduleEntryData> requestModifyScheduleEntry(const ScheduleEntryData& current) override;

	bool confirmRemoval(const ScheduleEntryData& data) override;
	void showMessage(const QString& title, const QString& message) override;

private:
	QWidget* _parent;
};