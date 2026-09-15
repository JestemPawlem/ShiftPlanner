#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QMimeData>

#include "ScheduleEntryRegistry.h"

class Schedule;


class ScheduleEntryListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit ScheduleEntryListModel(QObject* parent = nullptr);

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

	void onScheduleEntryRegistered(qulonglong id, const ScheduleEntryData& data);
	void onScheduleEntryUnregistered(qulonglong id);
	void onScheduleEntryModified(qulonglong id, const ScheduleEntryData& data);
	void onScheduleEntryMoved(qsizetype from, qsizetype to);

signals:
	void scheduleEntryMoveRequested(qsizetype from, qsizetype to);

private:
	void setRegistry(const ScheduleEntryRegistry* registry);

	const ScheduleEntryRegistry* _registry = nullptr;
	QList<qulonglong> _ids;
};