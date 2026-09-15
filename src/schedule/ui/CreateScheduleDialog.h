#pragma once

#include <QDialog>
#include <QWidget>
#include <QDate>
#include <QString>

#include "CommonTypes.h"
#include "ScheduleData.h"
#include "EmployeeRegistryManager.h"
#include "ScheduleEntryRegistryManager.h"


namespace Ui {
	class CreateScheduleDialog;
}

class CreateScheduleDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CreateScheduleDialog(EmployeeRegistryManager* employeeRegistryManager,
		ScheduleEntryRegistryManager* entryRegistryManager,
		QWidget* parent = nullptr);
	~CreateScheduleDialog();

	static optional<ScheduleData> getScheduleData(EmployeeRegistryManager* employeeManager,
		ScheduleEntryRegistryManager* entryManager,
		QWidget* parent);

private:
	Ui::CreateScheduleDialog* ui;

	void updateStyle(QWidget* widget);

	EmployeeRegistryManager* _employeeRegistryManager;
	ScheduleEntryRegistryManager* _entryRegistryManager;

private slots:
	void validateAndAccept();
	void on_beginDateEdit_dateChanged(const QDate& date);
	void on_endDateEdit_dateChanged(const QDate& date);
	void on_scheduleNameEdit_textChanged(const QString& arg1);
};