#include <utility>
#include <QAction>
#include <QVBoxLayout>

#include "IScheduleDemandsDialogService.h"
#include "Schedule.h"
#include "ScheduleDemandsTableModel.h"
#include "ScheduleDemandsTableView.h"
#include "ScheduleDemandsWidget.h"
#include "ScheduleManager.h"



ScheduleDemandsWidget::ScheduleDemandsWidget(
	ScheduleManager* scheduleManager,
	std::unique_ptr<IScheduleDemandsDialogService> dialogService,
	QWidget* parent) :
	QWidget{ parent },
	_scheduleManager{ scheduleManager },
	_dialogService{ std::move(dialogService) }
{
	Q_ASSERT(_scheduleManager != nullptr);
	Q_ASSERT(_dialogService != nullptr);

	setupUi();
	setupActions();
	setupConnections();
	updateActionStates();
}

ScheduleDemandsWidget::~ScheduleDemandsWidget() = default;



QAction* ScheduleDemandsWidget::setWeeklyDemandsAction() const
{
	return _actionSetWeeklyDemands;
}



ScheduleDemandsTableView* ScheduleDemandsWidget::view() const
{
	return _view;
}

ScheduleDemandsTableModel* ScheduleDemandsWidget::model() const
{
	return _model;
}



void ScheduleDemandsWidget::onScheduleOpened(const Schedule& schedule)
{
	_hasActiveSchedule = true;
	_model->setSchedule(&schedule);
	updateActionStates();
}

void ScheduleDemandsWidget::onScheduleClosed()
{
	_hasActiveSchedule = false;
	_model->setSchedule(nullptr);
	updateActionStates();
}

void ScheduleDemandsWidget::onSetWeeklyDemandsRequested()
{
	if (!_hasActiveSchedule)
		return;

	_dialogService->showWeeklyDemandsDialog(*_scheduleManager);
}



void ScheduleDemandsWidget::setupUi()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	_view = new ScheduleDemandsTableView(this);
	_model = new ScheduleDemandsTableModel(this);
	_view->setModel(_model);

	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_view);
}

void ScheduleDemandsWidget::setupActions()
{
	_actionSetWeeklyDemands = new QAction("Set Weekly Demands...", this);
}

void ScheduleDemandsWidget::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &ScheduleDemandsWidget::onScheduleOpened);

	connect(_scheduleManager, &ScheduleManager::scheduleEntryRegistered,
		_model, &ScheduleDemandsTableModel::onScheduleEntryRegistered);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryUnregistered,
		_model, &ScheduleDemandsTableModel::onScheduleEntryUnregistered);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryModified,
		_model, &ScheduleDemandsTableModel::onScheduleEntryModified);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryMoved,
		_model, &ScheduleDemandsTableModel::onScheduleEntryMoved);

	connect(_scheduleManager, &ScheduleManager::demandChanged,
		_model, &ScheduleDemandsTableModel::onDemandChanged);
	connect(_model, &ScheduleDemandsTableModel::demandChangeRequested,
		_scheduleManager, &ScheduleManager::setDemand);
	connect(_scheduleManager, &ScheduleManager::assignmentChanged,
		_model, &ScheduleDemandsTableModel::onAssignmentChanged);

	connect(_actionSetWeeklyDemands, &QAction::triggered,
		this, &ScheduleDemandsWidget::onSetWeeklyDemandsRequested, Qt::QueuedConnection);
}

void ScheduleDemandsWidget::updateActionStates()
{
	_actionSetWeeklyDemands->setEnabled(_hasActiveSchedule);
}