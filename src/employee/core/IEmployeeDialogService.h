#pragma once

#include <optional>
#include <QString>

#include "EmployeeData.h"


class IEmployeeDialogService
{
public:
	virtual ~IEmployeeDialogService() = default;

	virtual std::optional<EmployeeData> requestAddEmployee() = 0;
	virtual std::optional<EmployeeData> requestModifyEmployee(const EmployeeData& current) = 0;

	virtual bool confirmRemoval(const EmployeeData& data) = 0;
	virtual void showMessage(const QString& title, const QString& message) = 0;
};