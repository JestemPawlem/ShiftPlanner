#pragma once

#include <QDate>
#include <QWidget>

class Schedule;
class ScheduleManager;
class ScheduleAssignmentsTableModel;
class ScheduleAssignmentsTableView;


class ScheduleAssignmentsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ScheduleAssignmentsWidget(ScheduleManager* scheduleManager, QWidget* parent = nullptr);

	ScheduleAssignmentsTableView* view() const;
	ScheduleAssignmentsTableModel* model() const;

public slots:
	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();
	void onActiveScheduleEntryIdChanged(qulonglong entryId);

private slots:
	void onAssignmentChangeRequested(qulonglong employeeId, const QDate& date);
	void onAssignmentRemovalRequested(qulonglong employeeId, const QDate& date);

private:
	void setupUi();
	void setupConnections();

	ScheduleManager* _scheduleManager;

	ScheduleAssignmentsTableView* _view = nullptr;
	ScheduleAssignmentsTableModel* _model = nullptr;

	qulonglong _activeEntryId = 0;
};