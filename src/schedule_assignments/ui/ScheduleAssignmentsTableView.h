#pragma once

#include <QTableView>
#include <QDate>


class ScheduleAssignmentsTableModel;

class ScheduleAssignmentsTableView : public QTableView
{
	Q_OBJECT

public:
	explicit ScheduleAssignmentsTableView(QWidget* parent = nullptr);

	void setModel(ScheduleAssignmentsTableModel* model);

signals:
	void assignmentChangeRequested(qulonglong employeeId, const QDate& date);
	void assignmentRemovalRequested(qulonglong employeeId, const QDate& date);

private slots:
	void onCellClicked(const QModelIndex& index);
	void onContextMenu(const QPoint& pos);

private:
	ScheduleAssignmentsTableModel* _model = nullptr;
};