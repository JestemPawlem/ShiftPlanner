#include <utility>
#include <QDir>
#include <QDirListing>
#include <QFile>

#include "EmployeeRegistryManager.h"



EmployeeRegistryManager::EmployeeRegistryManager(std::unique_ptr<IStorage> storage, QString dirPath) :
	_storage{ std::move(storage) },
	_dirPath{ std::move(dirPath) }
{}



QString EmployeeRegistryManager::getDirectoryPath() const
{
	return _dirPath;
}

const EmployeeRegistry* EmployeeRegistryManager::registry(const QString& name) const
{
	auto it = _registries.find(name);
	if (it == _registries.end())
		return nullptr;
	return &it.value();
}

bool EmployeeRegistryManager::hasRegistry(const QString& name) const
{
	return _registries.contains(name);
}

QList<QString> EmployeeRegistryManager::registriesNames() const
{
	return _registries.keys();
}

void EmployeeRegistryManager::setRegistry(EmployeeRegistry registry)
{
	QString name = registry.name();
	_registries.insert(name, std::move(registry));
}

bool EmployeeRegistryManager::renameRegistry(const QString& oldName, const QString& newName)
{
	if (oldName == newName)
		return false;

	if (!_registries.contains(oldName) || _registries.contains(newName))
		return false;

	EmployeeRegistry item = _registries.take(oldName);
	item.setName(newName);
	_registries.insert(newName, std::move(item));

	return true;
}

bool EmployeeRegistryManager::removeRegistry(const QString& name)
{
	if (!_registries.contains(name))
		return false;

	_registries.remove(name);
	return true;
}

result_t<void> EmployeeRegistryManager::saveRegistry(const QString& name)
{
	auto it = _registries.find(name);
	if (it == _registries.end())
		return std::unexpected(QStringLiteral("Registry not found: %1").arg(name));

	QDir dir;
	if (!dir.mkpath(_dirPath))
		return std::unexpected(QStringLiteral("Could not create path: %1").arg(_dirPath));

	return _storage->save(it.value(), filePath(name));
}

result_t<void> EmployeeRegistryManager::saveAll()
{
	QDir dir;
	if (!dir.mkpath(_dirPath))
		return std::unexpected(QStringLiteral("Could not create path: %1").arg(_dirPath));

	for (auto it = _registries.begin(); it != _registries.end(); ++it)
	{
		auto result = _storage->save(it.value(), filePath(it.key()));
		if (!result)
			return result;
	}

	return {};
}

result_t<void> EmployeeRegistryManager::loadAll()
{
	QDir dir{ _dirPath };
	if (!dir.exists())
		return {};

	QMap<QString, EmployeeRegistry> tempRegistries;

	for (const auto& dirEntry : QDirListing(_dirPath, QStringList{ "*.json" }, QDirListing::IteratorFlag::FilesOnly))
	{
		EmployeeRegistry item;
		QString currentFilePath = dirEntry.filePath();

		auto result = _storage->load(item, currentFilePath);
		if (!result)
			return result;

		QString name = item.name();
		if (name.isEmpty())
		{
			name = dirEntry.baseName();
			item.setName(name);
		}

		tempRegistries.insert(name, std::move(item));
	}

	_registries = std::move(tempRegistries);
	return {};
}

result_t<void> EmployeeRegistryManager::deleteFromDisk(const QString& name)
{
	QString fileName = filePath(name);

	if (QFile::exists(fileName) && !QFile::remove(fileName))
		return std::unexpected(QStringLiteral("Could not remove file from disk: %1").arg(fileName));

	return {};
}



QString EmployeeRegistryManager::filePath(const QString& name) const
{
	return QDir{ _dirPath }.absoluteFilePath(name + ".json");
}