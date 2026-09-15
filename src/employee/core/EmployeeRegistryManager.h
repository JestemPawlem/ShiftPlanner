#pragma once

#include <memory>
#include <QMap>
#include <QString>

#include "IEmployeeRegistryManager.h"
#include "IStorage.h"


class EmployeeRegistryManager : public IEmployeeRegistryManager
{
public:
	explicit EmployeeRegistryManager(std::unique_ptr<IStorage> storage, QString dirPath);

	QString getDirectoryPath() const;

	const EmployeeRegistry* registry(const QString& name) const override;
	bool hasRegistry(const QString& name) const override;
	QList<QString> registriesNames() const override;

	void setRegistry(EmployeeRegistry registry) override;
	bool renameRegistry(const QString& oldName, const QString& newName) override;
	bool removeRegistry(const QString& name) override;

	result_t<void> saveRegistry(const QString& name) override;
	result_t<void> saveAll() override;
	result_t<void> loadAll() override;
	result_t<void> deleteFromDisk(const QString& name) override;

private:
	QString filePath(const QString& name) const;

	std::unique_ptr<IStorage> _storage;
	QString _dirPath;
	QMap<QString, EmployeeRegistry> _registries;
};