#pragma once

#include <optional>
#include <QString>

class IEmployeeRegistryManager;


class IEmployeeRegistryDialogService
{
public:
	virtual ~IEmployeeRegistryDialogService() = default;

	virtual void showDialog(IEmployeeRegistryManager& manager) = 0;
	virtual std::optional<QString> requestTemplateName(const QString& currentName = {}) = 0;
	virtual bool confirmRemoval(const QString& name) = 0;
	virtual void showMessage(const QString& title, const QString& message) = 0;
};