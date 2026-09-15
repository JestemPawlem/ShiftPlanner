#pragma once

#include <QString>

#include "CommonTypes.h"
#include "IStorable.h"


class IStorage
{
public:
	virtual ~IStorage() = default;

    virtual result_t<void> save(
		const IStorable& data,
		const QString& identifier) = 0;
    virtual result_t<void> load(
		IStorable& data,
		const QString& identifier) = 0;
};
