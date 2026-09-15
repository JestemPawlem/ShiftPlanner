#pragma once

#include <memory>
#include <QMainWindow>

class EmployeeRegistryController;
class EmployeeRegistryManager;
class EmployeeWidget;
class Schedule;
class ScheduleAssignmentsWidget;
class ScheduleDemandsWidget;
class ScheduleDocumentController;
class ScheduleEntryRegistryController;
class ScheduleEntryRegistryManager;
class ScheduleEntryWidget;
class ScheduleHeaderWidget;
class ScheduleManager;

namespace Ui
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(
		ScheduleManager* scheduleManager,
		EmployeeRegistryManager* employeeRegistryManager,
		ScheduleEntryRegistryManager* scheduleEntryRegistryManager,
		QWidget* parent = nullptr);
	~MainWindow() override;

private:
	void setupModelsAndViews();
	void setupConnections();

	Ui::MainWindow* ui;

	ScheduleManager* _scheduleManager;
	EmployeeRegistryManager* _employeeRegistryManager;
	ScheduleEntryRegistryManager* _scheduleEntryRegistryManager;

	std::unique_ptr<ScheduleDocumentController> _documentController;
	std::unique_ptr<EmployeeRegistryController> _employeeRegistryController;
	std::unique_ptr<ScheduleEntryRegistryController> _scheduleEntryRegistryController;

	ScheduleHeaderWidget* _scheduleHeaderWidget = nullptr;
	EmployeeWidget* _employeeWidget = nullptr;
	ScheduleEntryWidget* _scheduleEntryWidget = nullptr;
	ScheduleDemandsWidget* _scheduleDemandsWidget = nullptr;
	ScheduleAssignmentsWidget* _scheduleAssignmentsWidget = nullptr;
};