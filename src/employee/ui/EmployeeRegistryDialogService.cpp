#include <QWidget>

#include "ConfirmDialog.h"
#include "EmployeeRegistryDialogService.h"
#include "EmployeeRegistryDialog.h"
#include "InfoDialog.h"
#include "InputDialog.h"



EmployeeRegistryDialogService::EmployeeRegistryDialogService(QWidget* parent) :
	_parent{ parent }
{}



void EmployeeRegistryDialogService::showDialog(IEmployeeRegistryManager& manager)
{
	EmployeeRegistryDialog dialog{ manager, this, _parent };
	dialog.exec();
}

std::optional<QString> EmployeeRegistryDialogService::requestTemplateName(const QString& currentName)
{
	return InputDialog::getText(
		_parent,
		"Save Employees As Template",
		"Template name:",
		currentName);
}

bool EmployeeRegistryDialogService::confirmRemoval(const QString& name)
{
	return ConfirmDialog::confirm(
		_parent,
		"Delete Template",
		QStringLiteral("Are you sure you want to delete template '%1'?").arg(name));
}

void EmployeeRegistryDialogService::showMessage(const QString& title, const QString& message)
{
	InfoDialog::showMessage(_parent, title, message);
}