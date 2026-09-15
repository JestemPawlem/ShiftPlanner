#pragma once

#include <memory>
#include <QObject>

class QAction;
class Schedule;
class ScheduleManager;
class IScheduleEntryRegistryManager;
class IScheduleEntryRegistryDialogService;


class ScheduleEntryRegistryController : public QObject
{
	Q_OBJECT

public:
	explicit ScheduleEntryRegistryController(
		ScheduleManager* scheduleManager,
		IScheduleEntryRegistryManager* registryManager,
		std::unique_ptr<IScheduleEntryRegistryDialogService> dialogService,
		QObject* parent = nullptr);
	~ScheduleEntryRegistryController() override;

	QAction* saveAction() const;
	QAction* saveAsAction() const;
	QAction* manageAction() const;

public slots:
	void save();
	void saveAs();
	void manage();

	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

private:
	void setupActions();
	void setupConnections();
	void updateActionStates();

	ScheduleManager* _scheduleManager;
	IScheduleEntryRegistryManager* _registryManager;
	std::unique_ptr<IScheduleEntryRegistryDialogService> _dialogService;

	QAction* _actionSave = nullptr;
	QAction* _actionSaveAs = nullptr;
	QAction* _actionManage = nullptr;

	bool _hasActiveSchedule = false;
};