#include "IScheduleEntryRegistryManager.h"
#include "ScheduleEntryRegistryListModel.h"



ScheduleEntryRegistryListModel::ScheduleEntryRegistryListModel(IScheduleEntryRegistryManager& manager, QObject* parent) :
	QAbstractListModel{ parent },
	_manager{ manager }
{
	_templates = _manager.registriesNames();
}



int ScheduleEntryRegistryListModel::rowCount(const QModelIndex& parent) const
{
	return parent.isValid() ? 0 : static_cast<int>(_templates.size());
}

QVariant ScheduleEntryRegistryListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= _templates.size())
		return {};

	if (role == Qt::DisplayRole || role == Qt::EditRole)
		return _templates[index.row()];

	return {};
}

Qt::ItemFlags ScheduleEntryRegistryListModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool ScheduleEntryRegistryListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid() || role != Qt::EditRole)
		return false;

	QString newName = value.toString().trimmed();
	QString oldName = _templates[index.row()];

	if (newName.isEmpty() || newName == oldName)
		return false;

	if (_manager.hasRegistry(newName))
	{
		emit operationFailed(QStringLiteral("Template '%1' already exists.").arg(newName));
		return false;
	}

	if (!_manager.renameRegistry(oldName, newName))
	{
		emit operationFailed("Failed to rename template in memory.");
		return false;
	}

	_manager.deleteFromDisk(oldName);
	auto saveRes = _manager.saveRegistry(newName);
	if (!saveRes)
	{
		emit operationFailed(saveRes.error());
		_manager.renameRegistry(newName, oldName);
		return false;
	}

	_templates[index.row()] = newName;
	emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
	return true;
}

bool ScheduleEntryRegistryListModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if (row < 0 || row + count > _templates.size())
		return false;

	beginRemoveRows(parent, row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		QString name = _templates[row];
		_manager.removeRegistry(name);
		_manager.deleteFromDisk(name);
		_templates.removeAt(row);
	}
	endRemoveRows();

	return true;
}

void ScheduleEntryRegistryListModel::refresh()
{
	beginResetModel();
	_templates = _manager.registriesNames();
	endResetModel();
}