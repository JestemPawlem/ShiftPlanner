#pragma once

#include <QListView>

class Schedule;
class QMouseEvent;


class EmployeeListView : public QListView
{
	Q_OBJECT

public:
	explicit EmployeeListView(QWidget* parent = nullptr);

	void setModel(QAbstractItemModel* model);

public slots:
	void onScheduleOpened(const Schedule& schedule);

signals:
	void currentEmployeeChanged(qulonglong id);

private slots:
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
};