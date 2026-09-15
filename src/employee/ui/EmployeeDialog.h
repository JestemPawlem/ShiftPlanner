#pragma once

#include <optional>
#include <QDialog>

#include "EmployeeData.h"

namespace Ui
{
    class EmployeeDialog;
}


class EmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeDialog(QWidget* parent = nullptr, std::optional<EmployeeData> data = {});
    ~EmployeeDialog();

    static std::optional<EmployeeData> getEmployeeData(QWidget* parent = nullptr, std::optional<EmployeeData> data = {});

private slots:
    void validateAndAccept();
    void on_firstNameEdit_textChanged(const QString& arg1);
    void on_lastNameEdit_textChanged(const QString& arg1);

private:
    void updateStyle(QWidget* widget);

    Ui::EmployeeDialog* ui;
};