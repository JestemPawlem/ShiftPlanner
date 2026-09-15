#pragma once

#include <QDialog>


namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(const QString& title, const QString& message, QWidget *parent = nullptr);
    ~ConfirmDialog();

    static bool confirm(QWidget* parent, const QString& title, const QString& message);

private:
    Ui::ConfirmDialog *ui;
};
