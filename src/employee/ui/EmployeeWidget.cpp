#include <QAction>
#include <QVBoxLayout>

#include "Employee.h"
#include "EmployeeData.h"
#include "EmployeeListModel.h"
#include "EmployeeListView.h"
#include "EmployeeWidget.h"
#include "IEmployeeDialogService.h"
#include "Schedule.h"
#include "ScheduleManager.h"



EmployeeWidget::EmployeeWidget(ScheduleManager* scheduleManager, std::unique_ptr<IEmployeeDialogService> dialogService, QWidget* parent) :
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

EmployeeWidget::~EmployeeWidget() = default;



QAction* EmployeeWidget::addAction() const
{
	return _actionAdd;
}

QAction* EmployeeWidget::modifyAction() const
{
	return _actionModify;
}

QAction* EmployeeWidget::removeAction() const
{
	return _actionRemove;
}

EmployeeListView* EmployeeWidget::listView() const
{
	return _view;
}



void EmployeeWidget::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	_hasActiveSchedule = true;
	_selectedId = 0;
	updateActionStates();
}

void EmployeeWidget::onScheduleClosed()
{
	_hasActiveSchedule = false;
	_selectedId = 0;
	updateActionStates();
}



void EmployeeWidget::onCurrentEmployeeChanged(qulonglong id)
{
	_selectedId = id;
	updateActionStates();
}



void EmployeeWidget::onAddRequested()
{
	if (!_hasActiveSchedule)
		return;

	if (std::optional<EmployeeData> data = _dialogService->requestAddEmployee())
		_scheduleManager->registerEmployee(*data);
}

void EmployeeWidget::onModifyRequested()
{
	if (!_hasActiveSchedule || !_selectedId)
		return;

	const Employee* employee = _scheduleManager->employee(_selectedId);
	if (!employee)
		return;

	if (std::optional<EmployeeData> data = _dialogService->requestModifyEmployee(employee->data()))
		_scheduleManager->modifyEmployee(_selectedId, *data);
}

void EmployeeWidget::onRemoveRequested()
{
	if (!_hasActiveSchedule || !_selectedId)
		return;

	const Employee* employee = _scheduleManager->employee(_selectedId);
	if (employee && _dialogService->confirmRemoval(employee->data()))
		_scheduleManager->unregisterEmployee(_selectedId);
}



void EmployeeWidget::setupUi()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_view = new EmployeeListView(this);
	_model = new EmployeeListModel(this);
	_view->setModel(_model);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_view);
}

void EmployeeWidget::setupActions()
{
	_actionAdd = new QAction("Add Employee", this);
	_actionModify = new QAction("Edit Employee", this);
	_actionRemove = new QAction("Remove Employee", this);

	_view->addAction(_actionAdd);
	_view->addAction(_actionModify);
	_view->addAction(_actionRemove);
}

void EmployeeWidget::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened, this, &EmployeeWidget::onScheduleOpened);

	connect(_actionAdd, &QAction::triggered,
		this, &EmployeeWidget::onAddRequested, Qt::QueuedConnection);
	connect(_actionModify, &QAction::triggered,
		this, &EmployeeWidget::onModifyRequested, Qt::QueuedConnection);
	connect(_actionRemove, &QAction::triggered,
		this, &EmployeeWidget::onRemoveRequested, Qt::QueuedConnection);

	connect(_view, &QListView::doubleClicked,
		_actionModify, &QAction::trigger);
	connect(_view, &EmployeeListView::currentEmployeeChanged,
		this, &EmployeeWidget::onCurrentEmployeeChanged);

	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		_view, &EmployeeListView::onScheduleOpened);

	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		_model, &EmployeeListModel::onScheduleOpened);
	connect(_scheduleManager, &ScheduleManager::employeeRegistered,
		_model, &EmployeeListModel::onEmployeeRegistered);
	connect(_scheduleManager, &ScheduleManager::employeeUnregistered,
		_model, &EmployeeListModel::onEmployeeUnregistered);
	connect(_scheduleManager, &ScheduleManager::employeeModified,
		_model, &EmployeeListModel::onEmployeeModified);
	connect(_scheduleManager, &ScheduleManager::employeeMoved,
		_model, &EmployeeListModel::onEmployeeMoved);

	connect(_model, &EmployeeListModel::employeeMoveRequested,
		_scheduleManager, &ScheduleManager::moveEmployee);
}

void EmployeeWidget::updateActionStates()
{
	_actionAdd->setEnabled(_hasActiveSchedule);
	_actionModify->setEnabled(_hasActiveSchedule && _selectedId);
	_actionRemove->setEnabled(_hasActiveSchedule && _selectedId);
}