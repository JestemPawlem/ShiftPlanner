#pragma once

#include <memory>
#include <QObject>

class QAction;
class Schedule;
class ScheduleManager;
class IEmployeeRegistryManager;
class IEmployeeRegistryDialogService;


class EmployeeRegistryController : public QObject
{
	Q_OBJECT

public:
	explicit EmployeeRegistryController(
		ScheduleManager* scheduleManager,
		IEmployeeRegistryManager* registryManager,
		std::unique_ptr<IEmployeeRegistryDialogService> dialogService,
		QObject* parent = nullptr);
	~EmployeeRegistryController() override;

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
	IEmployeeRegistryManager* _registryManager;
	std::unique_ptr<IEmployeeRegistryDialogService> _dialogService;

	QAction* _actionSave = nullptr;
	QAction* _actionSaveAs = nullptr;
	QAction* _actionManage = nullptr;

	bool _hasActiveSchedule = false;
};