#pragma once

#include <QDialog>

#include "ui_InfoDialog.h"


namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    InfoDialog(
        QWidget* parent,
        const QString& title,
        const QString& message);

    ~InfoDialog();

static void showMessage(
    QWidget* parent,
    const QString& title,
    const QString& message);

private:
    Ui::InfoDialog *ui;
};
