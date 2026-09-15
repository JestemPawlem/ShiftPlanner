#include <QStyle>

#include "InputDialog.h"
#include "ui_InputDialog.h"

InputDialog::InputDialog(
	QWidget* parent,
	const QString& title,
	const QString& label,
	const QString& initialText) :
	QDialog(parent),
	ui(new Ui::InputDialog)
{
	ui->setupUi(this);
	setWindowTitle(title);

	ui->promptLabel->setProperty("class", "form-label");
	ui->errorLabel->setProperty("class", "error-label");
	ui->container->setProperty("class", "container");

	QSizePolicy sizePolicy = ui->errorLabel->sizePolicy();
	sizePolicy.setRetainSizeWhenHidden(true);
	ui->errorLabel->setSizePolicy(sizePolicy);

	ui->errorLabel->hide();

	ui->promptLabel->setText(label);
	ui->inputLineEdit->setText(initialText);
	ui->inputLineEdit->selectAll();

	connect(ui->okButton, &QPushButton::clicked, this, &InputDialog::validateAndAccept);
	connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->inputLineEdit, &QLineEdit::textChanged, this, &InputDialog::handleInputTextChanged);
	connect(ui->inputLineEdit, &QLineEdit::returnPressed, this, &InputDialog::validateAndAccept);
}

InputDialog::~InputDialog()
{
	delete ui;
}

optional<QString> InputDialog::getText(
	QWidget* parent,
	const QString& title,
	const QString& label,
	const QString& initialText)
{
	InputDialog dialog(parent, title, label, initialText);

	if (dialog.exec() == QDialog::Accepted)
	{
		QString text = dialog.ui->inputLineEdit->text().trimmed();
		return text;
	}

	return std::nullopt;
}

void InputDialog::updateStyle(QWidget* widget)
{
	widget->style()->unpolish(widget);
	widget->style()->polish(widget);
	widget->update();
}

void InputDialog::validateAndAccept()
{
	bool isValid = !ui->inputLineEdit->text().trimmed().isEmpty();

	ui->errorLabel->setVisible(!isValid);
	ui->inputLineEdit->setProperty("error", !isValid);
	updateStyle(ui->inputLineEdit);

	if (isValid)
		accept();
}

void InputDialog::handleInputTextChanged(const QString& text)
{
	Q_UNUSED(text);

	ui->errorLabel->setVisible(false);
	ui->inputLineEdit->setProperty("error", false);
	updateStyle(ui->inputLineEdit);
}