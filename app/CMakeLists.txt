add_executable(ShiftPlanner
    main.cpp
)

target_link_libraries(ShiftPlanner PRIVATE
    ShiftPlannerLib
)