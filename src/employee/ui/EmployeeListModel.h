#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QMimeData>

struct EmployeeData;
class EmployeeRegistry;
class Schedule;


class EmployeeListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit EmployeeListModel(QObject* parent = nullptr);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;
	Qt::DropActions supportedDropActions() const override;
	QStringList mimeTypes() const override;
	QMimeData* mimeData(const QModelIndexList& indexes) const override;
	bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

	qulonglong getIdAt(int row) const;
	int getRowById(qulonglong id) const;

public slots:
	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

	void onEmployeeRegistered(qulonglong id, const EmployeeData& data);
	void onEmployeeUnregistered(qulonglong id);
	void onEmployeeModified(qulonglong id, const EmployeeData& data);
	void onEmployeeMoved(qsizetype from, qsizetype to);

signals:
	void employeeMoveRequested(qsizetype from, qsizetype to);

private:
	void setRegistry(const EmployeeRegistry* registry);

	const EmployeeRegistry* _registry = nullptr;
	QList<qulonglong> _ids;
};