#pragma once

#include <optional>
#include <QList>
#include <QString>
#include <QtTypes>

#include "CommonTypes.h"
#include "IStorable.h"
#include "ScheduleEntry.h"


struct ScheduleEntryData;

class ScheduleEntryRegistry : public IStorable
{
public:
	ScheduleEntryRegistry() = default;
	explicit ScheduleEntryRegistry(const QString& name);

	QString name() const;
	void setName(const QString& name);

	qulonglong lastId() const;
	QList<qulonglong> ids() const;

	const ScheduleEntry* entity(qulonglong id) const;
	const QList<ScheduleEntry>& entities() const;

	std::optional<qulonglong> add(const ScheduleEntryData& data);
	bool add(qulonglong id, const ScheduleEntryData& data);
	bool remove(qulonglong id);
	bool modify(qulonglong id, const ScheduleEntryData& data);
	bool move(qsizetype from, qsizetype to);

	QVariant exportState() const override;
	result_t<void> importState(const QVariant& data) override;

private:
	QList<ScheduleEntry>::const_iterator findById(qulonglong id) const;
	QList<ScheduleEntry>::iterator findById(qulonglong id);

	QString _name;
	qulonglong _lastId = 0;
	QList<ScheduleEntry> _entities;
};