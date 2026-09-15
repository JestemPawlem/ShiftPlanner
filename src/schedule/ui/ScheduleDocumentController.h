#pragma once

#include <memory>
#include <optional>
#include <QObject>
#include <QString>

class QAction;
class EmployeeRegistryManager;
class IScheduleDocumentDialogService;
class Schedule;
class ScheduleEntryRegistryManager;
class ScheduleManager;


class ScheduleDocumentController : public QObject
{
	Q_OBJECT

public:
	explicit ScheduleDocumentController(
		ScheduleManager* scheduleManager,
		EmployeeRegistryManager* employeeRegistryManager,
		ScheduleEntryRegistryManager* scheduleEntryRegistryManager,
		std::unique_ptr<IScheduleDocumentDialogService> dialogService,
		QObject* parent = nullptr);
	~ScheduleDocumentController() override;

	QAction* newAction() const;
	QAction* openAction() const;
	QAction* saveAction() const;
	QAction* saveAsAction() const;

	std::optional<QString> currentPath() const;

public slots:
	bool createNew();
	bool open();
	bool save();
	bool saveAs();

	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

private:
	void setupActions();
	void setupConnections();
	void updateActionStates();
	QString resolveInitialSavePath() const;

	ScheduleManager* _scheduleManager;
	EmployeeRegistryManager* _employeeRegistryManager;
	ScheduleEntryRegistryManager* _scheduleEntryRegistryManager;
	std::unique_ptr<IScheduleDocumentDialogService> _dialogService;

	QAction* _actionNew = nullptr;
	QAction* _actionOpen = nullptr;
	QAction* _actionSave = nullptr;
	QAction* _actionSaveAs = nullptr;

	std::optional<QString> _currentPath;
	bool _hasActiveSchedule = false;
};