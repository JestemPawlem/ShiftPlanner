#include "FileJsonStorage.h"

#include <QFile>
#include <QJsonDocument>


result_t<void> FileJsonStorage::save(
	const IStorable& data,
	const QString& identifier)
{
	QFile file(identifier);
	if (!file.open(QIODevice::WriteOnly))
		return std::unexpected("Could not open file for writing: " + identifier);

	QJsonDocument doc = QJsonDocument::fromVariant(data.exportState());
	file.write(doc.toJson());

	return {};
}

result_t<void> FileJsonStorage::load(
	IStorable& data,
	const QString& identifier)
{
	QFile file(identifier);
	if (!file.open(QIODevice::ReadOnly))
		return std::unexpected("Could not open file for reading: " + identifier);

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (doc.isNull())
		return std::unexpected("Invalid JSON format in file: " + identifier);

	return data.importState(doc.toVariant());
}
