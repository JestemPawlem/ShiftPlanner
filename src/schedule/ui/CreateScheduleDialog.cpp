#include "CreateScheduleDialog.h"
#include "ui_CreateScheduleDialog.h"

#include <QStyle>
#include <QPushButton>
#include <QSizePolicy>
#include <QComboBox>


CreateScheduleDialog::CreateScheduleDialog(EmployeeRegistryManager* employeeRegistryManager,
	ScheduleEntryRegistryManager* entryRegistryManager,
	QWidget* parent) :
	QDialog(parent),
	ui(new Ui::CreateScheduleDialog),
	_employeeRegistryManager(employeeRegistryManager),
	_entryRegistryManager(entryRegistryManager)
{
	ui->setupUi(this);
	setWindowTitle("Create New Schedule");

	ui->scheduleNameLabel->setProperty("class", "form-label");
	ui->beginDateLabel->setProperty("class", "form-label");
	ui->endDateLabel->setProperty("class", "form-label");
	ui->employeesLabel->setProperty("class", "form-label");
	ui->scheduleEntriesLabel->setProperty("class", "form-label");

	ui->scheduleNameErrorLabel->setProperty("class", "error-label");
	ui->beginDateErrorLabel->setProperty("class", "error-label");
	ui->endDateErrorLabel->setProperty("class", "error-label");

	ui->nameContainer->setProperty("class", "container");
	ui->datesContainer->setProperty("class", "container");
	ui->definitionsContainer->setProperty("class", "container");

	QSizePolicy sizePolicy = ui->scheduleNameErrorLabel->sizePolicy();
	sizePolicy.setRetainSizeWhenHidden(true);
	ui->scheduleNameErrorLabel->setSizePolicy(sizePolicy);
	ui->beginDateErrorLabel->setSizePolicy(sizePolicy);
	ui->endDateErrorLabel->setSizePolicy(sizePolicy);

	ui->scheduleNameErrorLabel->hide();
	ui->beginDateErrorLabel->hide();
	ui->endDateErrorLabel->hide();

	connect(ui->okButton, &QPushButton::clicked, this, &CreateScheduleDialog::validateAndAccept);
	connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

	QDate today = QDate::currentDate();
	QDate nextMonthFirstDay = today.addMonths(1);
	nextMonthFirstDay.setDate(nextMonthFirstDay.year(), nextMonthFirstDay.month(), 1);
	QDate nextMonthLastDay;
	nextMonthLastDay.setDate(nextMonthFirstDay.year(), nextMonthFirstDay.month(), nextMonthFirstDay.daysInMonth());

	ui->beginDateEdit->setDate(nextMonthFirstDay);
	ui->endDateEdit->setDate(nextMonthLastDay);

	auto populateCombo = [](QComboBox* combo, const QList<QString>& names) {
		for (const QString& name: names)
			combo->addItem(name, name);
	};

	populateCombo(ui->employeesCombo, _employeeRegistryManager->registriesNames());
	populateCombo(ui->scheduleEntriesCombo, _entryRegistryManager->registriesNames());
}

CreateScheduleDialog::~CreateScheduleDialog()
{
	delete ui;
}

std::optional<ScheduleData> CreateScheduleDialog::getScheduleData(EmployeeRegistryManager* employeeRegistryManager,
	ScheduleEntryRegistryManager* entryRegistryManager,
	QWidget* parent)
{
	CreateScheduleDialog dialog(employeeRegistryManager,
		entryRegistryManager,
		parent);

	if (dialog.exec() == QDialog::Accepted)
	{
		ScheduleData data;
		data.name = dialog.ui->scheduleNameEdit->text().trimmed();
		data.startDate = dialog.ui->beginDateEdit->date();
		data.endDate = dialog.ui->endDateEdit->date();

		QString selectedEmployees = dialog.ui->employeesCombo->currentData().toString();
		if (!selectedEmployees.isEmpty())
			data.employeeRegistry = *employeeRegistryManager->registry(selectedEmployees);

		QString selectedEntries = dialog.ui->scheduleEntriesCombo->currentData().toString();
		if (!selectedEntries.isEmpty())
			data.scheduleEntryRegistry = *entryRegistryManager->registry(selectedEntries);

		return data;
	}

	return std::nullopt;
}

void CreateScheduleDialog::updateStyle(QWidget* widget)
{
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
	widget->update();
}

void CreateScheduleDialog::validateAndAccept()
{
	bool valid = true;

	if (ui->scheduleNameEdit->text().trimmed().isEmpty())
	{
		ui->scheduleNameErrorLabel->show();
		ui->scheduleNameEdit->setProperty("error", true);
		updateStyle(ui->scheduleNameEdit);
		valid = false;
	}

	QDate beginDate = ui->beginDateEdit->date();
	QDate endDate = ui->endDateEdit->date();

	if (beginDate >= endDate)
	{
		ui->beginDateErrorLabel->show();
		ui->endDateErrorLabel->show();
		ui->beginDateEdit->setProperty("error", true);
		ui->endDateEdit->setProperty("error", true);
		updateStyle(ui->beginDateEdit);
		updateStyle(ui->endDateEdit);
		valid = false;
	}

	if (valid)
		this->accept();
}

void CreateScheduleDialog::on_beginDateEdit_dateChanged(const QDate& date)
{
	Q_UNUSED(date);

	ui->beginDateErrorLabel->hide();
	ui->beginDateEdit->setProperty("error", false);
	updateStyle(ui->beginDateEdit);
}

void CreateScheduleDialog::on_endDateEdit_dateChanged(const QDate& date)
{
	Q_UNUSED(date);

	ui->endDateErrorLabel->hide();
	ui->endDateEdit->setProperty("error", false);
	updateStyle(ui->endDateEdit);
}

void CreateScheduleDialog::on_scheduleNameEdit_textChanged(const QString& arg1)
{
	Q_UNUSED(arg1);

	ui->scheduleNameErrorLabel->hide();
	ui->scheduleNameEdit->setProperty("error", false);
	updateStyle(ui->scheduleNameEdit);
}