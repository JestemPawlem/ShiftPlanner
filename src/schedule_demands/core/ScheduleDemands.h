#pragma once

#include <QDate>
#include <QMap>
#include <QVariant>
#include <QtTypes>

#include "CommonTypes.h"
#include "IStorable.h"


class ScheduleDemands : public IStorable
{
public:
	ScheduleDemands() = default;

	const QMap<QDate, QMap<qulonglong, qsizetype>>& demands() const;

	qsizetype demand(qulonglong scheduleEntryId, const QDate& date) const;
	void setDemand(qulonglong scheduleEntryId, const QDate& date, qsizetype count);
	void removeDemand(qulonglong scheduleEntryId, const QDate& date);

	void removeScheduleEntry(qulonglong scheduleEntryId);

	void clear();

	QVariant exportState() const override;
	result_t<void> importState(const QVariant& data) override;

private:
	QMap<QDate, QMap<qulonglong, qsizetype>> _demands;
};