#include <QWidget>

#include "EmployeeDialogService.h"
#include "EmployeeDialog.h"
#include "ConfirmDialog.h"
#include "InfoDialog.h"



EmployeeDialogService::EmployeeDialogService(QWidget* parent) :
	_parent{ parent }
{}



std::optional<EmployeeData> EmployeeDialogService::requestAddEmployee()
{
	return EmployeeDialog::getEmployeeData(_parent);
}

std::optional<EmployeeData> EmployeeDialogService::requestModifyEmployee(const EmployeeData& current)
{
	return EmployeeDialog::getEmployeeData(_parent, current);
}



bool EmployeeDialogService::confirmRemoval(const EmployeeData& data)
{
	return ConfirmDialog::confirm(_parent, "Employee Removal",
		QStringLiteral("Are you sure you want to remove %1?").arg(data.fullName()));
}

void EmployeeDialogService::showMessage(const QString& title, const QString& message)
{
	InfoDialog::showMessage(_parent, title, message);
}