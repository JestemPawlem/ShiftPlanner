#include <algorithm>
#include <utility>
#include <QVariantMap>
#include <QVariantList>

#include "ScheduleEntryRegistry.h"



ScheduleEntryRegistry::ScheduleEntryRegistry(const QString& name) :
	_name(name)
{}



QString ScheduleEntryRegistry::name() const
{
	return _name;
}

void ScheduleEntryRegistry::setName(const QString& name)
{
	_name = name;
}



qulonglong ScheduleEntryRegistry::lastId() const
{
	return _lastId;
}

QList<qulonglong> ScheduleEntryRegistry::ids() const
{
	QList<qulonglong> result;
	result.reserve(_entities.size());
	for (const ScheduleEntry& entity : _entities)
		result.append(entity.id());
	return result;
}



const ScheduleEntry* ScheduleEntryRegistry::entity(qulonglong id) const
{
	auto it = findById(id);
	if (it == _entities.cend())
		return nullptr;

	return &(*it);
}

const QList<ScheduleEntry>& ScheduleEntryRegistry::entities() const
{
	return _entities;
}



std::optional<qulonglong> ScheduleEntryRegistry::add(const ScheduleEntryData& data)
{
	qulonglong newId = _lastId + 1;
	if (!add(newId, data))
		return std::nullopt;

	return newId;
}

bool ScheduleEntryRegistry::add(qulonglong id, const ScheduleEntryData& data)
{
	if (id == 0 || findById(id) != _entities.cend())
		return false;

	if (id > _lastId)
		_lastId = id;

	_entities.append(ScheduleEntry(id, data));
	return true;
}

bool ScheduleEntryRegistry::remove(qulonglong id)
{
	auto it = findById(id);
	if (it == _entities.cend())
		return false;

	_entities.erase(it);
	return true;
}

bool ScheduleEntryRegistry::modify(qulonglong id, const ScheduleEntryData& data)
{
	auto it = findById(id);
	if (it == _entities.end())
		return false;

	it->setData(data);
	return true;
}

bool ScheduleEntryRegistry::move(qsizetype from, qsizetype to)
{
	if (from < 0 || from >= _entities.size() || to < 0 || to >= _entities.size() || from == to)
		return false;

	_entities.move(from, to);
	return true;
}



QVariant ScheduleEntryRegistry::exportState() const
{
	QVariantMap root;
	root["name"] = _name;
	root["lastId"] = _lastId;

	QVariantList entitiesList;
	entitiesList.reserve(_entities.size());

	for (const ScheduleEntry& entity : _entities)
	{
		QVariantMap entityObject;
		entityObject["id"] = entity.id();
		entityObject["data"] = entity.exportState();
		entitiesList.append(entityObject);
	}

	root["entities"] = entitiesList;
	return root;
}

result_t<void> ScheduleEntryRegistry::importState(const QVariant& data)
{
	const QVariantMap root = data.toMap();
	const QVariantList entitiesList = root["entities"].toList();

	QList<ScheduleEntry> tempEntities;
	tempEntities.reserve(entitiesList.size());

	for (const QVariant& itemVar : entitiesList)
	{
		const QVariantMap entityObject = itemVar.toMap();
		qulonglong id = entityObject["id"].toULongLong();

		ScheduleEntry entity(id);
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



QList<ScheduleEntry>::const_iterator ScheduleEntryRegistry::findById(qulonglong id) const
{
	return std::find_if(_entities.cbegin(), _entities.cend(), [id](const ScheduleEntry& entity) {
		return entity.id() == id;
		});
}

QList<ScheduleEntry>::iterator ScheduleEntryRegistry::findById(qulonglong id)
{
	return std::find_if(_entities.begin(), _entities.end(), [id](const ScheduleEntry& entity) {
		return entity.id() == id;
		});
}