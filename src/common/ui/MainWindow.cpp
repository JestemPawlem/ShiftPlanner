#include <QStyle>

#include "EmployeeDialogService.h"
#include "EmployeeRegistryController.h"
#include "EmployeeRegistryDialogService.h"
#include "EmployeeRegistryManager.h"
#include "EmployeeWidget.h"
#include "MainWindow.h"
#include "ScheduleAssignmentsTableView.h"
#include "ScheduleAssignmentsWidget.h"
#include "ScheduleDemandsDialogService.h"
#include "ScheduleDemandsTableView.h"
#include "ScheduleDemandsWidget.h"
#include "ScheduleDocumentDialogService.h"
#include "ScheduleDocumentController.h"
#include "ScheduleEntryDialogService.h"
#include "ScheduleEntryRegistryController.h"
#include "ScheduleEntryRegistryDialogService.h"
#include "ScheduleEntryRegistryManager.h"
#include "ScheduleEntryWidget.h"
#include "ScheduleHeaderWidget.h"
#include "ScheduleManager.h"
#include "ScrollSynchronizer.h"
#include "ui_MainWindow.h"



MainWindow::MainWindow(ScheduleManager* scheduleManager,
	EmployeeRegistryManager* employeeRegistryManager,
	ScheduleEntryRegistryManager* scheduleEntryRegistryManager,
	QWidget* parent) :
	QMainWindow{ parent },
	ui{ new Ui::MainWindow },
	_scheduleManager{ scheduleManager },
	_employeeRegistryManager{ employeeRegistryManager },
	_scheduleEntryRegistryManager{ scheduleEntryRegistryManager }
{
	ui->setupUi(this);
	setWindowTitle("Shift Planner");

	ui->employeesButton->setProperty("class", "tab-button");
	ui->scheduleEntriesButton->setProperty("class", "tab-button");

	setupModelsAndViews();
	setupConnections();
}

MainWindow::~MainWindow()
{
	delete ui;
}



void MainWindow::setupModelsAndViews()
{
	_documentController = std::make_unique<ScheduleDocumentController>(
		_scheduleManager,
		_employeeRegistryManager,
		_scheduleEntryRegistryManager,
		std::make_unique<ScheduleDocumentDialogService>(this),
		this);

	_employeeRegistryController = std::make_unique<EmployeeRegistryController>(
		_scheduleManager,
		_employeeRegistryManager,
		std::make_unique<EmployeeRegistryDialogService>(this),
		this);

	_scheduleEntryRegistryController = std::make_unique<ScheduleEntryRegistryController>(
		_scheduleManager,
		_scheduleEntryRegistryManager,
		std::make_unique<ScheduleEntryRegistryDialogService>(this),
		this);

	_scheduleHeaderWidget = new ScheduleHeaderWidget(_scheduleManager, this);
	ui->rightPanel->layout()->addWidget(_scheduleHeaderWidget);

	_employeeWidget = new EmployeeWidget(
		_scheduleManager,
		std::make_unique<EmployeeDialogService>(this),
		ui->employeesPage);
	ui->employeesPage->layout()->addWidget(_employeeWidget);

	_scheduleEntryWidget = new ScheduleEntryWidget(
		_scheduleManager,
		std::make_unique<ScheduleEntryDialogService>(this),
		ui->definitionsPage);
	ui->definitionsPage->layout()->addWidget(_scheduleEntryWidget);

	_scheduleDemandsWidget = new ScheduleDemandsWidget(
		_scheduleManager,
		std::make_unique<ScheduleDemandsDialogService>(this),
		ui->rightPanel);
	ui->rightPanel->layout()->addWidget(_scheduleDemandsWidget);

	_scheduleAssignmentsWidget = new ScheduleAssignmentsWidget(_scheduleManager);
	ui->rightPanel->layout()->addWidget(_scheduleAssignmentsWidget);

	ui->menuFile->addAction(_documentController->newAction());
	ui->menuFile->addSeparator();
	ui->menuFile->addAction(_documentController->openAction());
	ui->menuFile->addAction(_documentController->saveAction());
	ui->menuFile->addAction(_documentController->saveAsAction());
	ui->menuFile->addSeparator();
	ui->menuFile->addAction(ui->actionExit);

	ui->menuEdit->addAction(_employeeWidget->addAction());
	ui->menuEdit->addAction(_employeeWidget->modifyAction());
	ui->menuEdit->addAction(_employeeWidget->removeAction());
	ui->menuEdit->addSeparator();
	ui->menuEdit->addAction(_scheduleEntryWidget->addAction());
	ui->menuEdit->addAction(_scheduleEntryWidget->modifyAction());
	ui->menuEdit->addAction(_scheduleEntryWidget->removeAction());
	ui->menuEdit->addSeparator();
	ui->menuEdit->addAction(_scheduleDemandsWidget->setWeeklyDemandsAction());

	ui->menuEmployeeRegistry->addAction(_employeeRegistryController->saveAction());
	ui->menuEmployeeRegistry->addAction(_employeeRegistryController->saveAsAction());
	ui->menuEmployeeRegistry->addSeparator();
	ui->menuEmployeeRegistry->addAction(_employeeRegistryController->manageAction());

	ui->menuScheduleEntryRegistry->addAction(_scheduleEntryRegistryController->saveAction());
	ui->menuScheduleEntryRegistry->addAction(_scheduleEntryRegistryController->saveAsAction());
	ui->menuScheduleEntryRegistry->addSeparator();
	ui->menuScheduleEntryRegistry->addAction(_scheduleEntryRegistryController->manageAction());

	QButtonGroup* pageGroup = new QButtonGroup(this);
	pageGroup->addButton(ui->employeesButton, 0);
	pageGroup->addButton(ui->scheduleEntriesButton, 1);
	connect(pageGroup, &QButtonGroup::idClicked,
		ui->leftPanelStackedWidget, &QStackedWidget::setCurrentIndex);
}

void MainWindow::setupConnections()
{
	connect(_scheduleEntryWidget, &ScheduleEntryWidget::activeScheduleEntryChanged,
		_scheduleAssignmentsWidget, &ScheduleAssignmentsWidget::onActiveScheduleEntryIdChanged);

	connect(ui->actionExit, &QAction::triggered,
		this, &QWidget::close);

	new ScrollSynchronizer(_scheduleAssignmentsWidget->view(), _scheduleDemandsWidget->view(), this);
}