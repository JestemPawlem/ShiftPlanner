#include <QAction>
#include <QVBoxLayout>

#include "IScheduleEntryDialogService.h"
#include "Schedule.h"
#include "ScheduleEntry.h"
#include "ScheduleEntryData.h"
#include "ScheduleEntryListModel.h"
#include "ScheduleEntryListView.h"
#include "ScheduleEntryWidget.h"
#include "ScheduleManager.h"



ScheduleEntryWidget::ScheduleEntryWidget(ScheduleManager* scheduleManager, std::unique_ptr<IScheduleEntryDialogService> dialogService, QWidget* parent) :
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

ScheduleEntryWidget::~ScheduleEntryWidget() = default;



QAction* ScheduleEntryWidget::addAction() const
{
	return _actionAdd;
}

QAction* ScheduleEntryWidget::modifyAction() const
{
	return _actionModify;
}

QAction* ScheduleEntryWidget::removeAction() const
{
	return _actionRemove;
}

ScheduleEntryListView* ScheduleEntryWidget::listView() const
{
	return _view;
}

qulonglong ScheduleEntryWidget::activeId() const
{
	return _selectedId;
}



void ScheduleEntryWidget::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	_hasActiveSchedule = true;
	_selectedId = 0;
	updateActionStates();
	emit activeScheduleEntryChanged(0);
}

void ScheduleEntryWidget::onScheduleClosed()
{
	_hasActiveSchedule = false;
	_selectedId = 0;
	updateActionStates();
	emit activeScheduleEntryChanged(0);
}



void ScheduleEntryWidget::onCurrentScheduleEntryChanged(qulonglong id)
{
	_selectedId = id;
	updateActionStates();
	emit activeScheduleEntryChanged(_selectedId);
}



void ScheduleEntryWidget::onAddRequested()
{
	if (!_hasActiveSchedule)
		return;

	if (std::optional<ScheduleEntryData> data = _dialogService->requestAddScheduleEntry())
		_scheduleManager->registerScheduleEntry(*data);
}

void ScheduleEntryWidget::onModifyRequested()
{
	if (!_hasActiveSchedule || !_selectedId)
		return;

	const ScheduleEntry* entry = _scheduleManager->scheduleEntry(_selectedId);
	if (!entry)
		return;

	if (std::optional<ScheduleEntryData> data = _dialogService->requestModifyScheduleEntry(entry->data()))
		_scheduleManager->modifyScheduleEntry(_selectedId, *data);
}

void ScheduleEntryWidget::onRemoveRequested()
{
	if (!_hasActiveSchedule || !_selectedId)
		return;

	const ScheduleEntry* entry = _scheduleManager->scheduleEntry(_selectedId);
	if (entry && _dialogService->confirmRemoval(entry->data()))
		_scheduleManager->unregisterScheduleEntry(_selectedId);
}



void ScheduleEntryWidget::setupUi()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_view = new ScheduleEntryListView(this);
	_model = new ScheduleEntryListModel(this);
	_view->setModel(_model);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(_view);
}

void ScheduleEntryWidget::setupActions()
{
	_actionAdd = new QAction("Add Schedule Entry", this);
	_actionModify = new QAction("Edit Schedule Entry", this);
	_actionRemove = new QAction("Remove Schedule Entry", this);

	_view->addAction(_actionAdd);
	_view->addAction(_actionModify);
	_view->addAction(_actionRemove);
}

void ScheduleEntryWidget::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &ScheduleEntryWidget::onScheduleOpened);

	connect(_actionAdd, &QAction::triggered,
		this, &ScheduleEntryWidget::onAddRequested, Qt::QueuedConnection);
	connect(_actionModify, &QAction::triggered,
		this, &ScheduleEntryWidget::onModifyRequested, Qt::QueuedConnection);
	connect(_actionRemove, &QAction::triggered,
		this, &ScheduleEntryWidget::onRemoveRequested, Qt::QueuedConnection);

	connect(_view, &QListView::doubleClicked,
		_actionModify, &QAction::trigger);
	connect(_view, &ScheduleEntryListView::currentScheduleEntryChanged,
		this, &ScheduleEntryWidget::onCurrentScheduleEntryChanged);

	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		_view, &ScheduleEntryListView::onScheduleOpened);

	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		_model, &ScheduleEntryListModel::onScheduleOpened);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryRegistered,
		_model, &ScheduleEntryListModel::onScheduleEntryRegistered);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryUnregistered,
		_model, &ScheduleEntryListModel::onScheduleEntryUnregistered);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryModified,
		_model, &ScheduleEntryListModel::onScheduleEntryModified);
	connect(_scheduleManager, &ScheduleManager::scheduleEntryMoved,
		_model, &ScheduleEntryListModel::onScheduleEntryMoved);

	connect(_model, &ScheduleEntryListModel::scheduleEntryMoveRequested,
		_scheduleManager, &ScheduleManager::moveScheduleEntry);
}

void ScheduleEntryWidget::updateActionStates()
{
	_actionAdd->setEnabled(_hasActiveSchedule);
	_actionModify->setEnabled(_hasActiveSchedule && _selectedId);
	_actionRemove->setEnabled(_hasActiveSchedule && _selectedId);
}