#pragma once

#include "IEmployeeDialogService.h"


class QWidget;

class EmployeeDialogService : public IEmployeeDialogService
{
public:
	explicit EmployeeDialogService(QWidget* parent);

	std::optional<EmployeeData> requestAddEmployee() override;
	std::optional<EmployeeData> requestModifyEmployee(const EmployeeData& current) override;

	bool confirmRemoval(const EmployeeData& data) override;
	void showMessage(const QString& title, const QString& message) override;

private:
	QWidget* _parent;
};