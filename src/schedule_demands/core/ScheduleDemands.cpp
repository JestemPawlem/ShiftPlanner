#include <QVariantMap>

#include "ScheduleDemands.h"


const QMap<QDate, QMap<qulonglong, qsizetype>>& ScheduleDemands::demands() const
{
	return _demands;
}


qsizetype ScheduleDemands::demand(qulonglong scheduleEntryId, const QDate& date) const
{
	auto it = _demands.find(date);
	if (it == _demands.end())
		return 0;

	return it.value().value(scheduleEntryId, 0);
}

void ScheduleDemands::setDemand(qulonglong scheduleEntryId, const QDate& date, qsizetype count)
{
	if (scheduleEntryId == 0 || count < 0)
		return;

	_demands[date].insert(scheduleEntryId, count);
}

void ScheduleDemands::removeDemand(qulonglong scheduleEntryId, const QDate& date)
{
	auto it = _demands.find(date);
	if (it == _demands.end())
		return;

	it.value().remove(scheduleEntryId);
	if (it.value().isEmpty())
		_demands.erase(it);
}


void ScheduleDemands::removeScheduleEntry(qulonglong scheduleEntryId)
{
	for (auto it = _demands.begin(); it != _demands.end();)
	{
		it.value().remove(scheduleEntryId);

		if (it.value().isEmpty())
			it = _demands.erase(it);
		else
			++it;
	}
}


void ScheduleDemands::clear()
{
	_demands.clear();
}


QVariant ScheduleDemands::exportState() const
{
	QVariantMap root;
	QVariantMap demandsMap;

	for (auto dateIt = _demands.cbegin(); dateIt != _demands.cend(); ++dateIt)
	{
		QVariantMap entriesMap;
		for (auto entryIt = dateIt.value().cbegin(); entryIt != dateIt.value().cend(); ++entryIt)
			entriesMap.insert(QString::number(entryIt.key()), static_cast<qint64>(entryIt.value()));

		demandsMap.insert(dateIt.key().toString(Qt::ISODate), entriesMap);
	}

	root["demands"] = demandsMap;
	return root;
}

result_t<void> ScheduleDemands::importState(const QVariant& data)
{
	if (!data.canConvert<QVariantMap>())
		return std::unexpected("Illegal demands data format");

	const QVariantMap root = data.toMap();
	const QVariantMap demandsMap = root["demands"].toMap();
	QMap<QDate, QMap<qulonglong, qsizetype>> tempDemands;

	for (auto dateIt = demandsMap.cbegin(); dateIt != demandsMap.cend(); ++dateIt)
	{
		QDate date = QDate::fromString(dateIt.key(), Qt::ISODate);
		if (!date.isValid())
			continue;

		const QVariantMap entriesMap = dateIt.value().toMap();
		QMap<qulonglong, qsizetype> tempEntries;

		for (auto entryIt = entriesMap.cbegin(); entryIt != entriesMap.cend(); ++entryIt)
		{
			qulonglong scheduleEntryId = entryIt.key().toULongLong();
			qsizetype count = static_cast<qsizetype>(entryIt.value().toLongLong());
			if (scheduleEntryId != 0 && count > 0)
				tempEntries.insert(scheduleEntryId, count);
		}

		if (!tempEntries.isEmpty())
			tempDemands.insert(date, tempEntries);
	}

	_demands = std::move(tempDemands);
	return {};
}