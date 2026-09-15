#include <QLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSizePolicy>
#include <QStyle>

#include "ScheduleEntryDialog.h"
#include "ui_ScheduleEntryDialog.h"



ScheduleEntryDialog::ScheduleEntryDialog(QWidget* parent, std::optional<ScheduleEntryData> data) :
	QDialog{ parent },
	ui{ new Ui::ScheduleEntryDialog }
{
	ui->setupUi(this);

	ui->definitionContainer->setProperty("class", "container");
	ui->timeContainer->setProperty("class", "container");
	ui->propertiesContainer->setProperty("class", "container");

	ui->codeLabel->setProperty("class", "form-label");
	ui->nameLabel->setProperty("class", "form-label");
	ui->startTimeLabel->setProperty("class", "form-label");
	ui->endTimeLabel->setProperty("class", "form-label");
	ui->colorLabel->setProperty("class", "form-label");
	ui->percentLabel->setProperty("class", "form-label");

	ui->codeErrorLabel->setProperty("class", "error-label");
	ui->nameErrorLabel->setProperty("class", "error-label");

	if (layout())
		layout()->setSizeConstraint(QLayout::SetFixedSize);

	QSizePolicy sizePolicy = ui->nameErrorLabel->sizePolicy();
	sizePolicy.setRetainSizeWhenHidden(true);
	ui->nameErrorLabel->setSizePolicy(sizePolicy);
	ui->codeErrorLabel->setSizePolicy(sizePolicy);

	ui->nameErrorLabel->hide();
	ui->codeErrorLabel->hide();

	static const QRegularExpression regex{ "^(?:(100|[1-9]?[0-9])?)$" };
	auto* validator = new QRegularExpressionValidator{ regex, this };
	ui->paymentRateEdit->setValidator(validator);

	ui->categoryCombo->addItem("Work", static_cast<int>(ScheduleEntryCategory::Work));
	ui->categoryCombo->addItem("Leave", static_cast<int>(ScheduleEntryCategory::Leave));

	connect(ui->okButton, &QPushButton::clicked, this, &ScheduleEntryDialog::validateAndAccept);
	connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

	connect(ui->nameEdit, &QLineEdit::textChanged, this, &ScheduleEntryDialog::onNameEditTextChanged);
	connect(ui->codeEdit, &QLineEdit::textChanged, this, &ScheduleEntryDialog::onCodeEditTextChanged);
	connect(ui->codeEdit, &QLineEdit::textChanged,
		ui->colorCombo, &ScheduleEntryDialogColorSchemeComboBox::onScheduleEntryCodeChanged);

	connect(ui->paidCheckbox, &QCheckBox::checkStateChanged, this, &ScheduleEntryDialog::onPaidCheckboxCheckStateChanged);
	connect(ui->paymentRateEdit, &QLineEdit::editingFinished, this, &ScheduleEntryDialog::onPaymentRateEditEditingFinished);
	connect(ui->categoryCombo, &QComboBox::currentIndexChanged, this, &ScheduleEntryDialog::onCategoryComboCurrentIndexChanged);

	ui->percentLabel->setEnabled(false);
	ui->paymentRateEdit->setEnabled(false);

	if (data)
	{
		ui->nameEdit->setText(data->name);
		ui->codeEdit->setText(data->code);

		ui->colorCombo->setSelectedColorScheme(data->colorScheme);

		int catIndex = ui->categoryCombo->findData(static_cast<int>(data->category));
		ui->categoryCombo->setCurrentIndex(catIndex);

		if (data->startTime)
			ui->startTimeEdit->setTime(*data->startTime);
		if (data->endTime)
			ui->endTimeEdit->setTime(*data->endTime);

		ui->paidCheckbox->setChecked(data->paymentRate > 0.0);
		ui->paymentRateEdit->setText(QString::number(data->paymentRate * 100));
		ui->countedAsWorkCheckbox->setChecked(data->isCountedAsWork);

		setWindowTitle("Edit Entry");
	}
	else
	{
		ui->categoryCombo->setCurrentIndex(0);
		onCategoryComboCurrentIndexChanged(0);
		setWindowTitle("Add New Entry");
	}
}

ScheduleEntryDialog::~ScheduleEntryDialog()
{
	delete ui;
}



std::optional<ScheduleEntryData> ScheduleEntryDialog::getEntryData(QWidget* parent, std::optional<ScheduleEntryData> data)
{
	ScheduleEntryDialog dialog{ parent, data };

	if (dialog.exec() == QDialog::Accepted)
	{
		ScheduleEntryData resultData;

		resultData.name = dialog.ui->nameEdit->text().trimmed();
		resultData.code = dialog.ui->codeEdit->text().trimmed();
		resultData.colorScheme = dialog.ui->colorCombo->selectedColorScheme();
		resultData.category = static_cast<ScheduleEntryCategory>(dialog.ui->categoryCombo->currentData().toInt());

		if (resultData.category == ScheduleEntryCategory::Work)
		{
			resultData.startTime = dialog.ui->startTimeEdit->time();
			resultData.endTime = dialog.ui->endTimeEdit->time();
			resultData.paymentRate = 1.0;
			resultData.isCountedAsWork = true;
		}
		else
		{
			resultData.startTime = std::nullopt;
			resultData.endTime = std::nullopt;
			resultData.paymentRate = dialog.ui->paidCheckbox->isChecked()
				? dialog.ui->paymentRateEdit->text().toDouble() / 100.0
				: 0.0;
			resultData.isCountedAsWork = dialog.ui->countedAsWorkCheckbox->isChecked();
		}

		return resultData;
	}

	return std::nullopt;
}



void ScheduleEntryDialog::updateStyle(QWidget* widget)
{
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
	widget->update();
}



void ScheduleEntryDialog::validateAndAccept()
{
	bool nameValid = !ui->nameEdit->text().trimmed().isEmpty();
	bool codeValid = !ui->codeEdit->text().trimmed().isEmpty();

	ui->nameErrorLabel->setVisible(!nameValid);
	ui->nameEdit->setProperty("error", !nameValid);
	updateStyle(ui->nameEdit);

	ui->codeErrorLabel->setVisible(!codeValid);
	ui->codeEdit->setProperty("error", !codeValid);
	updateStyle(ui->codeEdit);

	if (nameValid && codeValid)
		accept();
}



void ScheduleEntryDialog::onNameEditTextChanged(const QString& text)
{
	Q_UNUSED(text);
	ui->nameErrorLabel->setVisible(false);
	ui->nameEdit->setProperty("error", false);
	updateStyle(ui->nameEdit);
}



void ScheduleEntryDialog::onCodeEditTextChanged(const QString& text)
{
	Q_UNUSED(text);
	ui->codeErrorLabel->setVisible(false);
	ui->codeEdit->setProperty("error", false);
	updateStyle(ui->codeEdit);
}



void ScheduleEntryDialog::onPaidCheckboxCheckStateChanged(const Qt::CheckState& state)
{
	bool isActive = (state == Qt::Checked);
	ui->percentLabel->setEnabled(isActive);
	ui->paymentRateEdit->setEnabled(isActive);

	updateStyle(ui->percentLabel);
	updateStyle(ui->paymentRateEdit);
}



void ScheduleEntryDialog::onPaymentRateEditEditingFinished()
{
	if (ui->paymentRateEdit->text().trimmed().isEmpty())
		ui->paymentRateEdit->setText("0");
}



void ScheduleEntryDialog::onCategoryComboCurrentIndexChanged(int index)
{
	auto category = static_cast<ScheduleEntryCategory>(ui->categoryCombo->itemData(index).toInt());
	bool isWork = (category == ScheduleEntryCategory::Work);

	ui->timeContainer->setVisible(isWork);
	ui->propertiesContainer->setVisible(!isWork);

	adjustSize();
}