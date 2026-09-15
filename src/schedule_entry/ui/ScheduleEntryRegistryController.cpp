#include <utility>
#include <QAction>

#include "IScheduleEntryRegistryDialogService.h"
#include "IScheduleEntryRegistryManager.h"
#include "Schedule.h"
#include "ScheduleEntryRegistry.h"
#include "ScheduleEntryRegistryController.h"
#include "ScheduleManager.h"



ScheduleEntryRegistryController::ScheduleEntryRegistryController(
	ScheduleManager* scheduleManager,
	IScheduleEntryRegistryManager* registryManager,
	std::unique_ptr<IScheduleEntryRegistryDialogService> dialogService,
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

ScheduleEntryRegistryController::~ScheduleEntryRegistryController() = default;



QAction* ScheduleEntryRegistryController::saveAction() const
{
	return _actionSave;
}

QAction* ScheduleEntryRegistryController::saveAsAction() const
{
	return _actionSaveAs;
}

QAction* ScheduleEntryRegistryController::manageAction() const
{
	return _actionManage;
}



void ScheduleEntryRegistryController::save()
{
	if (!_hasActiveSchedule)
		return;

	QString name = _scheduleManager->scheduleEntryRegistryName();
	if (name.isEmpty() || !_registryManager->hasRegistry(name))
	{
		saveAs();
		return;
	}

	ScheduleEntryRegistry copy = *_scheduleManager->scheduleEntryRegistry();
	_registryManager->setRegistry(std::move(copy));

	auto res = _registryManager->saveRegistry(name);
	if (!res)
		_dialogService->showMessage("Error", "Failed to save template: " + res.error());
}

void ScheduleEntryRegistryController::saveAs()
{
	if (!_hasActiveSchedule)
		return;

	const ScheduleEntryRegistry* registry = _scheduleManager->scheduleEntryRegistry();
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

	ScheduleEntryRegistry copy = *registry;
	copy.setName(templateName);
	_registryManager->setRegistry(std::move(copy));

	auto res = _registryManager->saveRegistry(templateName);
	if (!res)
	{
		_dialogService->showMessage("Error", "Failed to save template: " + res.error());
	}
	else
	{
		_scheduleManager->setScheduleEntryRegistryName(templateName);
		_dialogService->showMessage("Success", "Template '" + templateName + "' saved successfully.");
	}
}

void ScheduleEntryRegistryController::manage()
{
	_dialogService->showDialog(*_registryManager);
}

void ScheduleEntryRegistryController::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	_hasActiveSchedule = true;
	updateActionStates();
}

void ScheduleEntryRegistryController::onScheduleClosed()
{
	_hasActiveSchedule = false;
	updateActionStates();
}



void ScheduleEntryRegistryController::setupActions()
{
	_actionSave = new QAction("Save Schedule Entry Template", this);
	_actionSaveAs = new QAction("Save Schedule Entry Template As...", this);
	_actionManage = new QAction("Manage Schedule Entry Templates...", this);
}

void ScheduleEntryRegistryController::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &ScheduleEntryRegistryController::onScheduleOpened);

	connect(_actionSave, &QAction::triggered,
		this, &ScheduleEntryRegistryController::save, Qt::QueuedConnection);
	connect(_actionSaveAs, &QAction::triggered,
		this, &ScheduleEntryRegistryController::saveAs, Qt::QueuedConnection);
	connect(_actionManage, &QAction::triggered,
		this, &ScheduleEntryRegistryController::manage, Qt::QueuedConnection);
}

void ScheduleEntryRegistryController::updateActionStates()
{
	_actionSave->setEnabled(_hasActiveSchedule);
	_actionSaveAs->setEnabled(_hasActiveSchedule);
	_actionManage->setEnabled(true);
}