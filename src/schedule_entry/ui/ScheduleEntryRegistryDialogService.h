#pragma once

#include "IScheduleEntryRegistryDialogService.h"

class QWidget;


class ScheduleEntryRegistryDialogService : public IScheduleEntryRegistryDialogService
{
public:
	explicit ScheduleEntryRegistryDialogService(QWidget* parent);

	void showDialog(IScheduleEntryRegistryManager& manager) override;
	std::optional<QString> requestTemplateName(const QString& currentName = {}) override;
	bool confirmRemoval(const QString& name) override;
	void showMessage(const QString& title, const QString& message) override;

private:
	QWidget* _parent;
};