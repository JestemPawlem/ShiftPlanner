#pragma once

#include <QString>

#include "IStorage.h"
#include "IStorable.h"


class FileJsonStorage : public IStorage
{
public:
    result_t<void> save(
		const IStorable& data,
		const QString& identifier) override;
    result_t<void> load(
		IStorable& data,
		const QString& identifier) override;
};
