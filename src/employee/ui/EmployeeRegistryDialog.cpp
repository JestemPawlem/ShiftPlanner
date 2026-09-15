#include <QAbstractItemView>

#include "EmployeeRegistryListModel.h"
#include "EmployeeRegistryDialog.h"
#include "IEmployeeRegistryDialogService.h"
#include "IEmployeeRegistryManager.h"
#include "ui_EmployeeRegistryDialog.h"



EmployeeRegistryDialog::EmployeeRegistryDialog(
	IEmployeeRegistryManager& manager,
	IEmployeeRegistryDialogService* dialogService,
	QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::EmployeeRegistryDialog },
	_dialogService{ dialogService },
	_model{ new EmployeeRegistryListModel{ manager, this } }
{
	Q_ASSERT(_dialogService != nullptr);

	ui->setupUi(this);
	setWindowTitle("Manage Employee Templates");

	ui->employeeList->setProperty("class", "item-list");
	ui->employeeList->setModel(_model);
	ui->employeeList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

	connect(ui->renameButton, &QPushButton::clicked, this, &EmployeeRegistryDialog::onRenameClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &EmployeeRegistryDialog::onDeleteClicked);
	connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);

	connect(_model, &EmployeeRegistryListModel::operationFailed, this, &EmployeeRegistryDialog::onOperationFailed);
}

EmployeeRegistryDialog::~EmployeeRegistryDialog()
{
	delete ui;
}



void EmployeeRegistryDialog::onRenameClicked()
{
	QModelIndex index = ui->employeeList->currentIndex();
	if (!index.isValid())
	{
		_dialogService->showMessage("Edit Template", "Select a template to rename.");
		return;
	}

	ui->employeeList->edit(index);
}

void EmployeeRegistryDialog::onDeleteClicked()
{
	QModelIndex index = ui->employeeList->currentIndex();
	if (!index.isValid())
	{
		_dialogService->showMessage("Delete Template", "Select a template to delete.");
		return;
	}

	QString name = index.data(Qt::DisplayRole).toString();
	if (!_dialogService->confirmRemoval(name))
		return;

	_model->removeRow(index.row());
}

void EmployeeRegistryDialog::onOperationFailed(const QString& message)
{
	_dialogService->showMessage("Template Operation Error", message);
}