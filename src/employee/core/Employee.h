#pragma once

#include <QString>
#include <QVariant>

#include "IStorable.h"
#include "EmployeeData.h"


class Employee : public IStorable
{
public:
    Employee(size_t id);
    Employee(size_t id, const EmployeeData& data);

    bool operator==(const Employee& other) const;
    bool operator!=(const Employee& other) const;

    EmployeeData data() const;
    void setData(const EmployeeData& data);

    qulonglong id() const;
    QString firstName() const;
    QString lastName() const;
    QString fullName() const;

    QVariant exportState() const override;
    result_t<void> importState(const QVariant& data) override;

private:
    qulonglong _id;

    QString _firstName;
    QString _lastName;
};
