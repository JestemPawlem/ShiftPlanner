#pragma once

#include <QString>


struct EmployeeData
{
	QString firstName;
	QString lastName;

	inline QString fullName() const
	{
		return lastName + " " + firstName;
	}
};