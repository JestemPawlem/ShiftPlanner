#pragma once

#include <QDialog>

namespace Ui
{
	class EmployeeRegistryDialog;
}

class IEmployeeRegistryManager;
class IEmployeeRegistryDialogService;
class EmployeeRegistryListModel;


class EmployeeRegistryDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EmployeeRegistryDialog(
		IEmployeeRegistryManager& manager,
		IEmployeeRegistryDialogService* dialogService,
		QWidget* parent = nullptr);
	~EmployeeRegistryDialog() override;

private slots:
	void onRenameClicked();
	void onDeleteClicked();
	void onOperationFailed(const QString& message);

private:
	Ui::EmployeeRegistryDialog* ui;
	IEmployeeRegistryDialogService* _dialogService;
	EmployeeRegistryListModel* _model;
};