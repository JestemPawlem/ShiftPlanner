#pragma once

#include "IEmployeeRegistryDialogService.h"

class QWidget;


class EmployeeRegistryDialogService : public IEmployeeRegistryDialogService
{
public:
	explicit EmployeeRegistryDialogService(QWidget* parent);

	void showDialog(IEmployeeRegistryManager& manager) override;
	std::optional<QString> requestTemplateName(const QString& currentName = {}) override;
	bool confirmRemoval(const QString& name) override;
	void showMessage(const QString& title, const QString& message) override;

private:
	QWidget* _parent;
};