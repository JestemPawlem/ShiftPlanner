#pragma once

#include <memory>
#include <QWidget>

class QAction;
class IScheduleEntryDialogService;
class Schedule;
class ScheduleEntryListModel;
class ScheduleEntryListView;
class ScheduleManager;


class ScheduleEntryWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ScheduleEntryWidget(ScheduleManager* scheduleManager, std::unique_ptr<IScheduleEntryDialogService> dialogService, QWidget* parent = nullptr);
	~ScheduleEntryWidget() override;

	QAction* addAction() const;
	QAction* modifyAction() const;
	QAction* removeAction() const;
	ScheduleEntryListView* listView() const;
	qulonglong activeId() const;

public slots:
	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

signals:
	void activeScheduleEntryChanged(qulonglong id);

private slots:
	void onCurrentScheduleEntryChanged(qulonglong id);

	void onAddRequested();
	void onModifyRequested();
	void onRemoveRequested();

private:
	void setupUi();
	void setupActions();
	void setupConnections();
	void updateActionStates();

	ScheduleManager* _scheduleManager;
	std::unique_ptr<IScheduleEntryDialogService> _dialogService;

	ScheduleEntryListView* _view = nullptr;
	ScheduleEntryListModel* _model = nullptr;

	QAction* _actionAdd = nullptr;
	QAction* _actionModify = nullptr;
	QAction* _actionRemove = nullptr;

	qulonglong _selectedId = 0;
	bool _hasActiveSchedule = false;
};