find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(ShiftPlannerTests
    unit/tst_Example.cpp
)

target_link_libraries(ShiftPlannerTests PRIVATE
    ShiftPlannerLib
    Qt6::Test
)

add_test(NAME ShiftPlannerTests COMMAND ShiftPlannerTests)