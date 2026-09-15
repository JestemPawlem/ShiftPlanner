#pragma once

#include <QListView>

class Schedule;


class ScheduleEntryListView : public QListView
{
	Q_OBJECT

public:
	explicit ScheduleEntryListView(QWidget* parent = nullptr);

	void setModel(QAbstractItemModel* model) override;

	qulonglong selectedScheduleEntryId() const;

public slots:
	void onScheduleOpened(const Schedule& schedule);

signals:
	void currentScheduleEntryChanged(qulonglong id);

private slots:
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
};