#pragma once

#include <memory>
#include <QWidget>

class QAction;
class Schedule;
class ScheduleManager;
class ScheduleDemandsTableModel;
class ScheduleDemandsTableView;
class IScheduleDemandsDialogService;


class ScheduleDemandsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ScheduleDemandsWidget(
		ScheduleManager* scheduleManager,
		std::unique_ptr<IScheduleDemandsDialogService> dialogService,
		QWidget* parent = nullptr);
	~ScheduleDemandsWidget() override;

	QAction* setWeeklyDemandsAction() const;

	ScheduleDemandsTableView* view() const;
	ScheduleDemandsTableModel* model() const;

public slots:
	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

private slots:
	void onSetWeeklyDemandsRequested();

private:
	void setupUi();
	void setupActions();
	void setupConnections();
	void updateActionStates();

	ScheduleManager* _scheduleManager;
	std::unique_ptr<IScheduleDemandsDialogService> _dialogService;

	ScheduleDemandsTableView* _view = nullptr;
	ScheduleDemandsTableModel* _model = nullptr;

	QAction* _actionSetWeeklyDemands = nullptr;

	bool _hasActiveSchedule = false;
};