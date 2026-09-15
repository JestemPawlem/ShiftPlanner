#include <QAbstractItemView>

#include "IScheduleEntryRegistryDialogService.h"
#include "IScheduleEntryRegistryManager.h"
#include "ScheduleEntryRegistryListModel.h"
#include "ScheduleEntryRegistryDialog.h"

#include "ui_ScheduleEntryRegistryDialog.h"



ScheduleEntryRegistryDialog::ScheduleEntryRegistryDialog(
	IScheduleEntryRegistryManager& manager,
	IScheduleEntryRegistryDialogService* dialogService,
	QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::ScheduleEntryRegistryDialog },
	_dialogService{ dialogService },
	_model{ new ScheduleEntryRegistryListModel{ manager, this } }
{
	Q_ASSERT(_dialogService != nullptr);

	ui->setupUi(this);
	setWindowTitle("Manage Definition Templates");

	ui->scheduleEntryList->setProperty("class", "item-list");
	ui->scheduleEntryList->setModel(_model);
	ui->scheduleEntryList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

	connect(ui->renameButton, &QPushButton::clicked, this, &ScheduleEntryRegistryDialog::onRenameClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &ScheduleEntryRegistryDialog::onDeleteClicked);
	connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);

	connect(_model, &ScheduleEntryRegistryListModel::operationFailed, this, &ScheduleEntryRegistryDialog::onOperationFailed);
}

ScheduleEntryRegistryDialog::~ScheduleEntryRegistryDialog()
{
	delete ui;
}



void ScheduleEntryRegistryDialog::onRenameClicked()
{
	QModelIndex index = ui->scheduleEntryList->currentIndex();
	if (!index.isValid())
	{
		_dialogService->showMessage("Edit Template", "Select a template to rename.");
		return;
	}

	ui->scheduleEntryList->edit(index);
}

void ScheduleEntryRegistryDialog::onDeleteClicked()
{
	QModelIndex index = ui->scheduleEntryList->currentIndex();
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

void ScheduleEntryRegistryDialog::onOperationFailed(const QString& message)
{
	_dialogService->showMessage("Template Operation Error", message);
}