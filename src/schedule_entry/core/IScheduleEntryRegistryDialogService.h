#pragma once

#include <optional>
#include <QString>

class IScheduleEntryRegistryManager;


class IScheduleEntryRegistryDialogService
{
public:
	virtual ~IScheduleEntryRegistryDialogService() = default;

	virtual void showDialog(IScheduleEntryRegistryManager& manager) = 0;
	virtual std::optional<QString> requestTemplateName(const QString& currentName = {}) = 0;
	virtual bool confirmRemoval(const QString& name) = 0;
	virtual void showMessage(const QString& title, const QString& message) = 0;
};