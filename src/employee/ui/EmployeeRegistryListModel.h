#pragma once

#include <QAbstractListModel>
#include <QStringList>

class IEmployeeRegistryManager;


class EmployeeRegistryListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit EmployeeRegistryListModel(IEmployeeRegistryManager& manager, QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

	void refresh();

signals:
	void operationFailed(const QString& message);

private:
	IEmployeeRegistryManager& _manager;
	QStringList _templates;
};