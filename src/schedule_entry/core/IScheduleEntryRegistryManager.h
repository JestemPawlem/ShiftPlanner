#pragma once

#include <QList>
#include <QString>

#include "CommonTypes.h"
#include "ScheduleEntryRegistry.h"


class IScheduleEntryRegistryManager
{
public:
	virtual ~IScheduleEntryRegistryManager() = default;

	virtual const ScheduleEntryRegistry* registry(const QString& name) const = 0;
	virtual bool hasRegistry(const QString& name) const = 0;
	virtual QList<QString> registriesNames() const = 0;

	virtual void setRegistry(ScheduleEntryRegistry registry) = 0;
	virtual bool renameRegistry(const QString& oldName, const QString& newName) = 0;
	virtual bool removeRegistry(const QString& name) = 0;

	virtual result_t<void> saveRegistry(const QString& name) = 0;
	virtual result_t<void> saveAll() = 0;
	virtual result_t<void> loadAll() = 0;
	virtual result_t<void> deleteFromDisk(const QString& name) = 0;
};