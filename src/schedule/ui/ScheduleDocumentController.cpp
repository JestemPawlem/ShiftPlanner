#include <utility>
#include <QAction>
#include <QDir>
#include <QStandardPaths>

#include "EmployeeRegistryManager.h"
#include "IScheduleDocumentDialogService.h"
#include "Schedule.h"
#include "ScheduleData.h"
#include "ScheduleDocumentController.h"
#include "ScheduleEntryRegistryManager.h"
#include "ScheduleManager.h"



ScheduleDocumentController::ScheduleDocumentController(
	ScheduleManager* scheduleManager,
	EmployeeRegistryManager* employeeRegistryManager,
	ScheduleEntryRegistryManager* scheduleEntryRegistryManager,
	std::unique_ptr<IScheduleDocumentDialogService> dialogService,
	QObject* parent) :
	QObject{ parent },
	_scheduleManager{ scheduleManager },
	_employeeRegistryManager{ employeeRegistryManager },
	_scheduleEntryRegistryManager{ scheduleEntryRegistryManager },
	_dialogService{ std::move(dialogService) }
{
	Q_ASSERT(_scheduleManager != nullptr);
	Q_ASSERT(_employeeRegistryManager != nullptr);
	Q_ASSERT(_scheduleEntryRegistryManager != nullptr);
	Q_ASSERT(_dialogService != nullptr);

	setupActions();
	setupConnections();
	updateActionStates();
}

ScheduleDocumentController::~ScheduleDocumentController() = default;



QAction* ScheduleDocumentController::newAction() const
{
	return _actionNew;
}

QAction* ScheduleDocumentController::openAction() const
{
	return _actionOpen;
}

QAction* ScheduleDocumentController::saveAction() const
{
	return _actionSave;
}

QAction* ScheduleDocumentController::saveAsAction() const
{
	return _actionSaveAs;
}

std::optional<QString> ScheduleDocumentController::currentPath() const
{
	return _currentPath;
}



bool ScheduleDocumentController::createNew()
{
	std::optional<ScheduleData> data = _dialogService->requestNewScheduleData(
		_employeeRegistryManager,
		_scheduleEntryRegistryManager);

	if (!data)
		return false;

	_currentPath = std::nullopt;
	_scheduleManager->createNew(*data);

	return true;
}

bool ScheduleDocumentController::open()
{
	QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	std::optional<QString> fileName = _dialogService->requestOpenFile(defaultDir);

	if (!fileName)
		return false;

	auto result = _scheduleManager->open(*fileName);
	if (!result)
	{
		_dialogService->showMessage("Error", "Failed to open schedule: " + result.error());
		return false;
	}

	_currentPath = fileName;
	return true;
}

bool ScheduleDocumentController::save()
{
	if (!_currentPath)
		return saveAs();

	auto result = _scheduleManager->save(*_currentPath);
	if (!result)
	{
		_dialogService->showMessage("Error", "Failed to save schedule: " + result.error());
		return false;
	}

	return true;
}

bool ScheduleDocumentController::saveAs()
{
	QString initialPath = resolveInitialSavePath();
	std::optional<QString> fileName = _dialogService->requestSaveFile(initialPath);

	if (!fileName)
		return false;

	auto result = _scheduleManager->save(*fileName);
	if (!result)
	{
		_dialogService->showMessage("Error", "Failed to save schedule: " + result.error());
		return false;
	}

	_currentPath = fileName;
	return true;
}

void ScheduleDocumentController::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	_hasActiveSchedule = true;
	updateActionStates();
}

void ScheduleDocumentController::onScheduleClosed()
{
	_hasActiveSchedule = false;
	_currentPath = std::nullopt;
	updateActionStates();
}



void ScheduleDocumentController::setupActions()
{
	_actionNew = new QAction("New Schedule", this);
	_actionOpen = new QAction("Open Schedule...", this);
	_actionSave = new QAction("Save Schedule", this);
	_actionSaveAs = new QAction("Save Schedule As...", this);
}

void ScheduleDocumentController::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &ScheduleDocumentController::onScheduleOpened);

	connect(_actionNew, &QAction::triggered,
		this, &ScheduleDocumentController::createNew, Qt::QueuedConnection);
	connect(_actionOpen, &QAction::triggered,
		this, &ScheduleDocumentController::open, Qt::QueuedConnection);
	connect(_actionSave, &QAction::triggered,
		this, &ScheduleDocumentController::save, Qt::QueuedConnection);
	connect(_actionSaveAs, &QAction::triggered,
		this, &ScheduleDocumentController::saveAs, Qt::QueuedConnection);
}

void ScheduleDocumentController::updateActionStates()
{
	_actionSave->setEnabled(_hasActiveSchedule);
	_actionSaveAs->setEnabled(_hasActiveSchedule);
}

QString ScheduleDocumentController::resolveInitialSavePath() const
{
	const Schedule* schedule = _scheduleManager->schedule();
	QString fileName = (schedule && !schedule->name().isEmpty()) ? schedule->name() + ".json" : "new_schedule.json";
	QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

	return _currentPath ? *_currentPath : QDir(defaultDir).filePath(fileName);
}