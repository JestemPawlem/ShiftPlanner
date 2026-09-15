#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(const QString& title,
                             const QString& message,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);

    ui->messageLabel->setProperty("class", "message-label");

    ui->messageLabel->setText(message);

    connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}

bool ConfirmDialog::confirm(QWidget* parent, const QString& title, const QString& message)
{
    ConfirmDialog dialog(title, message, parent);
    return dialog.exec() == QDialog::Accepted;
}
