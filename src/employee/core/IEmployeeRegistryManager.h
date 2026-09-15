#pragma once

#include <QList>
#include <QString>

#include "CommonTypes.h"
#include "EmployeeRegistry.h"


class IEmployeeRegistryManager
{
public:
	virtual ~IEmployeeRegistryManager() = default;

	virtual const EmployeeRegistry* registry(const QString& name) const = 0;
	virtual bool hasRegistry(const QString& name) const = 0;
	virtual QList<QString> registriesNames() const = 0;

	virtual void setRegistry(EmployeeRegistry registry) = 0;
	virtual bool renameRegistry(const QString& oldName, const QString& newName) = 0;
	virtual bool removeRegistry(const QString& name) = 0;

	virtual result_t<void> saveRegistry(const QString& name) = 0;
	virtual result_t<void> saveAll() = 0;
	virtual result_t<void> loadAll() = 0;
	virtual result_t<void> deleteFromDisk(const QString& name) = 0;
};