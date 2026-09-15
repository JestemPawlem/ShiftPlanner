#include <QVBoxLayout>

#include "Schedule.h"
#include "ScheduleAssignmentsTableModel.h"
#include "ScheduleAssignmentsTableView.h"
#include "ScheduleAssignmentsWidget.h"
#include "ScheduleManager.h"



ScheduleAssignmentsWidget::ScheduleAssignmentsWidget(ScheduleManager* scheduleManager, QWidget* parent) :
	QWidget{ parent },
	_scheduleManager{ scheduleManager }
{
	Q_ASSERT(_scheduleManager != nullptr);

	setupUi();
	setupConnections();
}



ScheduleAssignmentsTableView* ScheduleAssignmentsWidget::view() const
{
	return _view;
}

ScheduleAssignmentsTableModel* ScheduleAssignmentsWidget::model() const
{
	return _model;
}



void ScheduleAssignmentsWidget::onScheduleOpened(const Schedule& schedule)
{
	_activeEntryId = 0;
	_model->setSchedule(&schedule);
}

void ScheduleAssignmentsWidget::onScheduleClosed()
{
	_activeEntryId = 0;
	_model->setSchedule(nullptr);
}

void ScheduleAssignmentsWidget::onActiveScheduleEntryIdChanged(qulonglong entryId)
{
	_activeEntryId = entryId;
}



void ScheduleAssignmentsWidget::onAssignmentChangeRequested(qulonglong employeeId, const QDate& date)
{
	if (_activeEntryId == 0)
		return;

	_scheduleManager->assignEntry(employeeId, date, _activeEntryId);
}

void ScheduleAssignmentsWidget::onAssignmentRemovalRequested(qulonglong employeeId, const QDate& date)
{
	_scheduleManager->unassignEntry(employeeId, date);
}



void ScheduleAssignmentsWidget::setupUi()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_view = new ScheduleAssignmentsTableView(this);
	_model = new ScheduleAssignmentsTableModel(this);
	_view->setModel(_model);

	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_view);
}

void ScheduleAssignmentsWidget::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &ScheduleAssignmentsWidget::onScheduleOpened);

	connect(_scheduleManager, &ScheduleManager::employeeRegistered,
		_model, &ScheduleAssignmentsTableModel::onEmployeeRegistered);
	connect(_scheduleManager, &ScheduleManager::employeeUnregistered,
		_model, &ScheduleAssignmentsTableModel::onEmployeeUnregistered);
	connect(_scheduleManager, &ScheduleManager::employeeModified,
		_model, &ScheduleAssignmentsTableModel::onEmployeeModified);
	connect(_scheduleManager, &ScheduleManager::employeeMoved,
		_model, &ScheduleAssignmentsTableModel::onEmployeeMoved);

	connect(_scheduleManager, &ScheduleManager::scheduleEntryModified,
		_model, &ScheduleAssignmentsTableModel::onEntryModified);
	connect(_scheduleManager, &ScheduleManager::assignmentChanged,
		_model, &ScheduleAssignmentsTableModel::onAssignmentChanged);

	connect(_view, &ScheduleAssignmentsTableView::assignmentChangeRequested,
		this, &ScheduleAssignmentsWidget::onAssignmentChangeRequested);
	connect(_view, &ScheduleAssignmentsTableView::assignmentRemovalRequested,
		this, &ScheduleAssignmentsWidget::onAssignmentRemovalRequested);
}