#include <QStyle>
#include <QSizePolicy>

#include "EmployeeDialog.h"
#include "ui_EmployeeDialog.h"



EmployeeDialog::EmployeeDialog(QWidget* parent, std::optional<EmployeeData> data)
    : QDialog(parent)
    , ui(new Ui::EmployeeDialog)
{
    ui->setupUi(this);
    setWindowTitle(data ? "Edit Employee" : "Add New Employee");

    ui->firstNameLabel->setProperty("class", "form-label");
    ui->lastNameLabel->setProperty("class", "form-label");

    ui->firstNameErrorLabel->setProperty("class", "error-label");
    ui->lastNameErrorLabel->setProperty("class", "error-label");

    ui->nameContainer->setProperty("class", "container");

    QSizePolicy sizePolicy = ui->firstNameErrorLabel->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    ui->firstNameErrorLabel->setSizePolicy(sizePolicy);
    ui->lastNameErrorLabel->setSizePolicy(sizePolicy);

    ui->firstNameErrorLabel->hide();
    ui->lastNameErrorLabel->hide();

    connect(ui->okButton, &QPushButton::clicked, this, &EmployeeDialog::validateAndAccept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    if (data)
    {
        ui->firstNameEdit->setText(data->firstName);
        ui->lastNameEdit->setText(data->lastName);
    }
}

EmployeeDialog::~EmployeeDialog()
{
    delete ui;
}

std::optional<EmployeeData> EmployeeDialog::getEmployeeData(QWidget* parent, std::optional<EmployeeData> data)
{
    EmployeeDialog dialog(parent, data);

    if (dialog.exec() == QDialog::Accepted)
    {
        EmployeeData resultData;
        resultData.firstName = dialog.ui->firstNameEdit->text().trimmed();
        resultData.lastName = dialog.ui->lastNameEdit->text().trimmed();
        return resultData;
    }

    return std::nullopt;
}

void EmployeeDialog::updateStyle(QWidget* widget)
{
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
    widget->update();
}

void EmployeeDialog::validateAndAccept()
{
    bool firstValid = !ui->firstNameEdit->text().trimmed().isEmpty();
    bool lastValid = !ui->lastNameEdit->text().trimmed().isEmpty();

    ui->firstNameErrorLabel->setVisible(!firstValid);
    ui->firstNameEdit->setProperty("error", !firstValid);
    updateStyle(ui->firstNameEdit);

    ui->lastNameErrorLabel->setVisible(!lastValid);
    ui->lastNameEdit->setProperty("error", !lastValid);
    updateStyle(ui->lastNameEdit);

    if (firstValid && lastValid)
        accept();
}

void EmployeeDialog::on_firstNameEdit_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    ui->firstNameErrorLabel->setVisible(false);
    ui->firstNameEdit->setProperty("error", false);
    updateStyle(ui->firstNameEdit);
}

void EmployeeDialog::on_lastNameEdit_textChanged(const QString& arg1)
{
    Q_UNUSED(arg1);
    ui->lastNameErrorLabel->setVisible(false);
    ui->lastNameEdit->setProperty("error", false);
    updateStyle(ui->lastNameEdit);
}