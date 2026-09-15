#include <QDataStream>
#include <QIODevice>

#include "Employee.h"
#include "EmployeeData.h"
#include "EmployeeListModel.h"
#include "EmployeeRegistry.h"
#include "Schedule.h"



EmployeeListModel::EmployeeListModel(QObject* parent) :
	QAbstractListModel{ parent }
{}



int EmployeeListModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	return _ids.size();
}

QVariant EmployeeListModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	if (!_registry || row < 0 || row >= _ids.size())
		return {};

	qulonglong id = _ids[row];
	const Employee* employee = _registry->entity(id);
	if (!employee)
		return {};

	if (role == Qt::DisplayRole)
		return employee->fullName();
	if (role == Qt::UserRole)
		return employee->id();

	return {};
}



Qt::ItemFlags EmployeeListModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
	if (index.isValid())
		return defaultFlags | Qt::ItemIsDragEnabled;

	return defaultFlags | Qt::ItemIsDropEnabled;
}

Qt::DropActions EmployeeListModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

QStringList EmployeeListModel::mimeTypes() const
{
	return { "application/x-employee-row" };
}

QMimeData* EmployeeListModel::mimeData(const QModelIndexList& indexes) const
{
	if (indexes.isEmpty())
		return nullptr;

	QMimeData* mime = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	stream << indexes.first().row();
	mime->setData("application/x-employee-row", encodedData);
	return mime;
}

bool EmployeeListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	Q_UNUSED(column);

	if (action != Qt::MoveAction || !data->hasFormat("application/x-employee-row"))
		return false;

	QByteArray encodedData = data->data("application/x-employee-row");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	int sourceRow;
	stream >> sourceRow;

	int destinationRow = row;
	if (destinationRow == -1)
		destinationRow = parent.isValid() ? parent.row() : _ids.size();

	int targetIndex = destinationRow;
	if (destinationRow > sourceRow)
		targetIndex--;

	if (sourceRow == targetIndex)
		return false;

	emit employeeMoveRequested(sourceRow, targetIndex);
	return true;
}



qulonglong EmployeeListModel::getIdAt(int row) const
{
	return _ids.value(row, 0);
}

int EmployeeListModel::getRowById(qulonglong id) const
{
	return _ids.indexOf(id);
}



void EmployeeListModel::onScheduleOpened(const Schedule& schedule)
{
	setRegistry(&schedule.employeeRegistry());
}

void EmployeeListModel::onScheduleClosed()
{
	setRegistry(nullptr);
}



void EmployeeListModel::onEmployeeRegistered(qulonglong id, const EmployeeData& data)
{
	Q_UNUSED(data);

	beginInsertRows(QModelIndex(), _ids.size(), _ids.size());
	_ids.append(id);
	endInsertRows();
}

void EmployeeListModel::onEmployeeUnregistered(qulonglong id)
{
	int row = _ids.indexOf(id);
	if (row != -1)
	{
		beginRemoveRows(QModelIndex(), row, row);
		_ids.removeAt(row);
		endRemoveRows();
	}
}

void EmployeeListModel::onEmployeeModified(qulonglong id, const EmployeeData& data)
{
	Q_UNUSED(data);

	int row = _ids.indexOf(id);
	if (row != -1)
	{
		QModelIndex idx = index(row);
		emit dataChanged(idx, idx, { Qt::DisplayRole });
	}
}

void EmployeeListModel::onEmployeeMoved(qsizetype from, qsizetype to)
{
	int destChild = (to > from) ? to + 1 : to;
	beginMoveRows(QModelIndex(), from, from, QModelIndex(), destChild);
	_ids.move(from, to);
	endMoveRows();
}



void EmployeeListModel::setRegistry(const EmployeeRegistry* registry)
{
	beginResetModel();
	_registry = registry;
	_ids.clear();
	if (_registry)
		_ids = _registry->ids();
	endResetModel();
}