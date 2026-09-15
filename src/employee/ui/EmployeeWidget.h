#pragma once

#include <memory>
#include <QWidget>

class QAction;
class EmployeeListModel;
class EmployeeListView;
class IEmployeeDialogService;
class Schedule;
class ScheduleManager;


class EmployeeWidget : public QWidget
{
	Q_OBJECT

public:
	explicit EmployeeWidget(ScheduleManager* scheduleManager, std::unique_ptr<IEmployeeDialogService> dialogService, QWidget* parent = nullptr);
	~EmployeeWidget() override;

	QAction* addAction() const;
	QAction* modifyAction() const;
	QAction* removeAction() const;
	EmployeeListView* listView() const;

public slots:
	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

private slots:
	void onCurrentEmployeeChanged(qulonglong id);

	void onAddRequested();
	void onModifyRequested();
	void onRemoveRequested();

private:
	void setupUi();
	void setupActions();
	void setupConnections();
	void updateActionStates();

	ScheduleManager* _scheduleManager;
	std::unique_ptr<IEmployeeDialogService> _dialogService;

	EmployeeListView* _view = nullptr;
	EmployeeListModel* _model = nullptr;

	QAction* _actionAdd = nullptr;
	QAction* _actionModify = nullptr;
	QAction* _actionRemove = nullptr;

	qulonglong _selectedId = 0;
	bool _hasActiveSchedule = false;
};