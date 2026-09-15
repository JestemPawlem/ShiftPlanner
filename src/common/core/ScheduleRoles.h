#pragma once

#include <Qt>

namespace ScheduleRoles
{
    enum
    {
        IsWeekend = Qt::UserRole + 100,
        AssignedCount,
        DemandCount,

        ScheduleEntryName,
        ScheduleEntryCode,
        ScheduleEntryDetails,
    };
}