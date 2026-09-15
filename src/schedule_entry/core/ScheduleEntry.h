#pragma once

#include <QString>
#include <QTime>
#include <QVariant>
#include <QtTypes>

#include "CommonTypes.h"
#include "IStorable.h"
#include "ScheduleEntryCategory.h"
#include "ScheduleEntryData.h"


class ScheduleEntry : public IStorable
{
public:
	ScheduleEntry(qulonglong id);
	ScheduleEntry(qulonglong id, const ScheduleEntryData& data);

	bool operator==(const ScheduleEntry& other) const;
	bool operator!=(const ScheduleEntry& other) const;

	ScheduleEntryData data() const;
	void setData(const ScheduleEntryData& data);

	qulonglong id() const;
	QString code() const;
	QString name() const;
	ColorScheme colorScheme() const;
	ScheduleEntryCategory category() const;

	optional<QTime> startTime() const;
	optional<QTime> endTime() const;
	double paymentRate() const;
	bool countedAsWork() const;

	bool isWork() const;

	QVariant exportState() const override;
	result_t<void> importState(const QVariant& data) override;

private:
	qulonglong _id = 0;

	QString _code;
	QString _name;
	ColorScheme _colorScheme;
	ScheduleEntryCategory _category = ScheduleEntryCategory::Work;

	optional<QTime> _startTime;
	optional<QTime> _endTime;
	double _paymentRate = 1.0;
	bool _isCountedAsWork = true;
};