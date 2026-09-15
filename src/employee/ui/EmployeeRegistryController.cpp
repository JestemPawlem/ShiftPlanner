#include <utility>
#include <QAction>

#include "EmployeeRegistry.h"
#include "EmployeeRegistryController.h"
#include "IEmployeeRegistryDialogService.h"
#include "IEmployeeRegistryManager.h"
#include "Schedule.h"
#include "ScheduleManager.h"



EmployeeRegistryController::EmployeeRegistryController(
	ScheduleManager* scheduleManager,
	IEmployeeRegistryManager* registryManager,
	std::unique_ptr<IEmployeeRegistryDialogService> dialogService,
	QObject* parent) :
	QObject{ parent },
	_scheduleManager{ scheduleManager },
	_registryManager{ registryManager },
	_dialogService{ std::move(dialogService) }
{
	Q_ASSERT(_scheduleManager != nullptr);
	Q_ASSERT(_registryManager != nullptr);
	Q_ASSERT(_dialogService != nullptr);

	setupActions();
	setupConnections();
	updateActionStates();
}

EmployeeRegistryController::~EmployeeRegistryController() = default;



QAction* EmployeeRegistryController::saveAction() const
{
	return _actionSave;
}

QAction* EmployeeRegistryController::saveAsAction() const
{
	return _actionSaveAs;
}

QAction* EmployeeRegistryController::manageAction() const
{
	return _actionManage;
}



void EmployeeRegistryController::save()
{
	if (!_hasActiveSchedule)
		return;

	QString name = _scheduleManager->employeeRegistryName();
	if (name.isEmpty() || !_registryManager->hasRegistry(name))
	{
		saveAs();
		return;
	}

	EmployeeRegistry copy = *_scheduleManager->employeeRegistry();
	_registryManager->setRegistry(std::move(copy));

	auto res = _registryManager->saveRegistry(name);
	if (!res)
		_dialogService->showMessage("Error", "Failed to save template: " + res.error());
}

void EmployeeRegistryController::saveAs()
{
	if (!_hasActiveSchedule)
		return;

	const EmployeeRegistry* registry = _scheduleManager->employeeRegistry();
	if (!registry)
		return;

	QString currentName = registry->name();
	auto templateNameOpt = _dialogService->requestTemplateName(currentName);
	if (!templateNameOpt)
		return;

	QString templateName = templateNameOpt->trimmed();
	if (templateName.isEmpty())
	{
		_dialogService->showMessage("Error", "Template name cannot be empty.");
		return;
	}

	if (_registryManager->hasRegistry(templateName))
	{
		_dialogService->showMessage("Error", "A template with name '" + templateName + "' already exists.");
		return;
	}

	EmployeeRegistry copy = *registry;
	copy.setName(templateName);
	_registryManager->setRegistry(std::move(copy));

	auto res = _registryManager->saveRegistry(templateName);
	if (!res)
	{
		_dialogService->showMessage("Error", "Failed to save template: " + res.error());
	}
	else
	{
		_scheduleManager->setEmployeeRegistryName(templateName);
		_dialogService->showMessage("Success", "Template '" + templateName + "' saved successfully.");
	}
}

void EmployeeRegistryController::manage()
{
	_dialogService->showDialog(*_registryManager);
}

void EmployeeRegistryController::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	_hasActiveSchedule = true;
	updateActionStates();
}

void EmployeeRegistryController::onScheduleClosed()
{
	_hasActiveSchedule = false;
	updateActionStates();
}



void EmployeeRegistryController::setupActions()
{
	_actionSave = new QAction("Save Employee Template", this);
	_actionSaveAs = new QAction("Save Employee Template As...", this);
	_actionManage = new QAction("Manage Employee Templates...", this);
}

void EmployeeRegistryController::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &EmployeeRegistryController::onScheduleOpened);

	connect(_actionSave, &QAction::triggered,
		this, &EmployeeRegistryController::save, Qt::QueuedConnection);
	connect(_actionSaveAs, &QAction::triggered,
		this, &EmployeeRegistryController::saveAs, Qt::QueuedConnection);
	connect(_actionManage, &QAction::triggered,
		this, &EmployeeRegistryController::manage, Qt::QueuedConnection);
}

void EmployeeRegistryController::updateActionStates()
{
	_actionSave->setEnabled(_hasActiveSchedule);
	_actionSaveAs->setEnabled(_hasActiveSchedule);
	_actionManage->setEnabled(true);
}