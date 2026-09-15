#pragma once

#include <QVariant>

#include "CommonTypes.h"


class IStorable
{
public:
	virtual ~IStorable() = default;

	virtual QVariant exportState() const = 0;
    virtual result_t<void> importState(const QVariant& data) = 0;
};
