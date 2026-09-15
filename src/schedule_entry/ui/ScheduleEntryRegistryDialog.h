#pragma once

#include <QDialog>

namespace Ui
{
	class ScheduleEntryRegistryDialog;
}

class IScheduleEntryRegistryManager;
class IScheduleEntryRegistryDialogService;
class ScheduleEntryRegistryListModel;


class ScheduleEntryRegistryDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ScheduleEntryRegistryDialog(
		IScheduleEntryRegistryManager& manager,
		IScheduleEntryRegistryDialogService* dialogService,
		QWidget* parent = nullptr);
	~ScheduleEntryRegistryDialog() override;

private slots:
	void onRenameClicked();
	void onDeleteClicked();
	void onOperationFailed(const QString& message);

private:
	Ui::ScheduleEntryRegistryDialog* ui;
	IScheduleEntryRegistryDialogService* _dialogService;
	ScheduleEntryRegistryListModel* _model;
};