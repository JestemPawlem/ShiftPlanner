#pragma once

#include <optional>
#include <QList>
#include <QString>
#include <QtTypes>

#include "CommonTypes.h"
#include "Employee.h"
#include "IStorable.h"


struct EmployeeData;

class EmployeeRegistry : public IStorable
{
public:
	EmployeeRegistry() = default;
	explicit EmployeeRegistry(const QString& name);

	QString name() const;
	void setName(const QString& name);

	qulonglong lastId() const;
	QList<qulonglong> ids() const;

	const Employee* entity(qulonglong id) const;
	const QList<Employee>& entities() const;

	std::optional<qulonglong> add(const EmployeeData& data);
	bool add(qulonglong id, const EmployeeData& data);
	bool remove(qulonglong id);
	bool modify(qulonglong id, const EmployeeData& data);
	bool move(qsizetype from, qsizetype to);

	QVariant exportState() const override;
	result_t<void> importState(const QVariant& data) override;

private:
	QList<Employee>::const_iterator findById(qulonglong id) const;
	QList<Employee>::iterator findById(qulonglong id);

	QString _name;
	qulonglong _lastId = 0;
	QList<Employee> _entities;
};