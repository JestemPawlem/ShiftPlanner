#pragma once

#include <optional>
#include <QDialog>

#include "ScheduleEntryData.h"

namespace Ui
{
	class ScheduleEntryDialog;
}


class ScheduleEntryDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ScheduleEntryDialog(QWidget* parent = nullptr, std::optional<ScheduleEntryData> data = std::nullopt);
	~ScheduleEntryDialog() override;

	static std::optional<ScheduleEntryData> getEntryData(QWidget* parent = nullptr, std::optional<ScheduleEntryData> data = std::nullopt);

private slots:
	void validateAndAccept();
	void onNameEditTextChanged(const QString& text);
	void onCodeEditTextChanged(const QString& text);
	void onPaidCheckboxCheckStateChanged(const Qt::CheckState& state);
	void onPaymentRateEditEditingFinished();
	void onCategoryComboCurrentIndexChanged(int index);

private:
	void updateStyle(QWidget* widget);

	Ui::ScheduleEntryDialog* ui = nullptr;
};