#include "Employee.h"


Employee::Employee(size_t id) :
	_id(id)
{}

Employee::Employee(size_t id, const EmployeeData& data) :
	_id{ id },
	_firstName{ data.firstName },
	_lastName{ data.lastName }
{}


bool Employee::operator==(const Employee& other) const { return _id == other._id; }
bool Employee::operator!=(const Employee& other) const { return _id != other._id; }


EmployeeData Employee::data() const
{
	return { _firstName, _lastName };
}

void Employee::setData(const EmployeeData& data)
{
	_firstName = data.firstName;
	_lastName = data.lastName;
}


qulonglong Employee::id() const { return _id; }
QString Employee::firstName() const { return _firstName; }
QString Employee::lastName() const { return _lastName; }
QString Employee::fullName() const { return _lastName + " " + _firstName; }


QVariant Employee::exportState() const
{
	QVariantMap result;

	result["firstName"] = _firstName;
	result["lastName"] = _lastName;

	return result;
}

result_t<void> Employee::importState(const QVariant& data)
{
	if (!data.canConvert<QVariantMap>())
		return std::unexpected("Illegal employee data format");

	QVariantMap map = data.toMap();

	_firstName = map["firstName"].toString();
	_lastName = map["lastName"].toString();

	return {};
}
