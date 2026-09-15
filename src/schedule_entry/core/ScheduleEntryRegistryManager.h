#pragma once

#include <memory>
#include <QMap>
#include <QString>

#include "IScheduleEntryRegistryManager.h"
#include "IStorage.h"


class ScheduleEntryRegistryManager : public IScheduleEntryRegistryManager
{
public:
	explicit ScheduleEntryRegistryManager(std::unique_ptr<IStorage> storage, QString dirPath);

	QString getDirectoryPath() const;

	const ScheduleEntryRegistry* registry(const QString& name) const override;
	bool hasRegistry(const QString& name) const override;
	QList<QString> registriesNames() const override;

	void setRegistry(ScheduleEntryRegistry registry) override;
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
	QMap<QString, ScheduleEntryRegistry> _registries;
};