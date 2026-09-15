#include <algorithm>
#include <utility>
#include <QVariantMap>
#include <QVariantList>

#include "EmployeeRegistry.h"



EmployeeRegistry::EmployeeRegistry(const QString& name) :
	_name(name)
{}



QString EmployeeRegistry::name() const
{
	return _name;
}

void EmployeeRegistry::setName(const QString& name)
{
	_name = name;
}



qulonglong EmployeeRegistry::lastId() const
{
	return _lastId;
}

QList<qulonglong> EmployeeRegistry::ids() const
{
	QList<qulonglong> result;
	result.reserve(_entities.size());
	for (const Employee& entity : _entities)
		result.append(entity.id());
	return result;
}



const Employee* EmployeeRegistry::entity(qulonglong id) const
{
	auto it = findById(id);
	if (it == _entities.cend())
		return nullptr;

	return &(*it);
}

const QList<Employee>& EmployeeRegistry::entities() const
{
	return _entities;
}



std::optional<qulonglong> EmployeeRegistry::add(const EmployeeData& data)
{
	qulonglong newId = _lastId + 1;
	if (!add(newId, data))
		return std::nullopt;

	return newId;
}

bool EmployeeRegistry::add(qulonglong id, const EmployeeData& data)
{
	if (id == 0 || findById(id) != _entities.cend())
		return false;

	if (id > _lastId)
		_lastId = id;

	_entities.append(Employee(id, data));
	return true;
}

bool EmployeeRegistry::remove(qulonglong id)
{
	auto it = findById(id);
	if (it == _entities.cend())
		return false;

	_entities.erase(it);
	return true;
}

bool EmployeeRegistry::modify(qulonglong id, const EmployeeData& data)
{
	auto it = findById(id);
	if (it == _entities.end())
		return false;

	it->setData(data);
	return true;
}

bool EmployeeRegistry::move(qsizetype from, qsizetype to)
{
	if (from < 0 || from >= _entities.size() || to < 0 || to >= _entities.size() || from == to)
		return false;

	_entities.move(from, to);
	return true;
}



QVariant EmployeeRegistry::exportState() const
{
	QVariantMap root;
	root["name"] = _name;
	root["lastId"] = _lastId;

	QVariantList entitiesList;
	entitiesList.reserve(_entities.size());

	for (const Employee& entity : _entities)
	{
		QVariantMap entityObject;
		entityObject["id"] = entity.id();
		entityObject["data"] = entity.exportState();
		entitiesList.append(entityObject);
	}

	root["entities"] = entitiesList;
	return root;
}

result_t<void> EmployeeRegistry::importState(const QVariant& data)
{
	const QVariantMap root = data.toMap();
	const QVariantList entitiesList = root["entities"].toList();

	QList<Employee> tempEntities;
	tempEntities.reserve(entitiesList.size());

	for (const QVariant& itemVar : entitiesList)
	{
		const QVariantMap entityObject = itemVar.toMap();
		qulonglong id = entityObject["id"].toULongLong();

		Employee entity(id);
		result_t<void> result = entity.importState(entityObject["data"]);
		if (!result)
			return result;

		tempEntities.append(std::move(entity));
	}

	_name = root["name"].toString();
	_lastId = root["lastId"].toULongLong();
	_entities = std::move(tempEntities);

	return {};
}



QList<Employee>::const_iterator EmployeeRegistry::findById(qulonglong id) const
{
	return std::find_if(_entities.cbegin(), _entities.cend(), [id](const Employee& entity) {
		return entity.id() == id;
		});
}

QList<Employee>::iterator EmployeeRegistry::findById(qulonglong id)
{
	return std::find_if(_entities.begin(), _entities.end(), [id](const Employee& entity) {
		return entity.id() == id;
		});
}