#pragma once

#include <QString>
#include <QList>

#include "CommonTypes.h"


class IRegistryManager
{
public:
	virtual ~IRegistryManager() = default;

	virtual QList<QString> registriesNames() const = 0;
	virtual bool hasRegistry(const QString& name) const = 0;
	virtual bool renameRegistry(const QString& oldName, const QString& newName) = 0;
	virtual bool removeRegistry(const QString& name) = 0;

	virtual result_t<void> deleteFromDisk(const QString& name) = 0;
	virtual result_t<void> saveRegistry(const QString& name) = 0;
};