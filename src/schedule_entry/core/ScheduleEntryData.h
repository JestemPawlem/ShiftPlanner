#pragma once

#include <QString>
#include <QTime>
#include <QtTypes>

#include "CommonTypes.h"
#include "ColorScheme.h"
#include "ScheduleEntryCategory.h"


struct ScheduleEntryData
{
	QString code;
	QString name;
	ColorScheme colorScheme;
	ScheduleEntryCategory category = ScheduleEntryCategory::Work;

	optional<QTime> startTime;
	optional<QTime> endTime;

	double paymentRate = 1.0;
	bool isCountedAsWork = true;
};