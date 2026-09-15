#pragma once

#include <optional>
#include <QString>

#include "ScheduleEntryData.h"


class IScheduleEntryDialogService
{
public:
	virtual ~IScheduleEntryDialogService() = default;

	virtual std::optional<ScheduleEntryData> requestAddScheduleEntry() = 0;
	virtual std::optional<ScheduleEntryData> requestModifyScheduleEntry(const ScheduleEntryData& current) = 0;

	virtual bool confirmRemoval(const ScheduleEntryData& data) = 0;
	virtual void showMessage(const QString& title, const QString& message) = 0;
};