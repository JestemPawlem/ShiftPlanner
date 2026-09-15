#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QWidget* parent,
                       const QString& title,
                       const QString& message)
    : QDialog(parent)
    , ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);

    ui->messageLabel->setProperty("class", "message-label");

    connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);

    ui->messageLabel->setText(message);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::showMessage(QWidget* parent,
                             const QString& title,
                             const QString& message)
{
    InfoDialog dialog(parent, title, message);
    dialog.exec();
}
