add_library(ShiftPlannerLib STATIC
    # common / core
    common/resources.qrc
    common/core/ColorScheme.h
    common/core/CommonTypes.h
    common/core/FileJsonStorage.cpp
    common/core/FileJsonStorage.h
    common/core/IRegistryManager.h
    common/core/IStorable.h
    common/core/IStorage.h
    common/core/ScheduleRoles.h

    # common / ui
    common/ui/ConfirmDialog.cpp
    common/ui/ConfirmDialog.h
    common/ui/ConfirmDialog.ui
    common/ui/InfoDialog.cpp
    common/ui/InfoDialog.h
    common/ui/InfoDialog.ui
    common/ui/InputDialog.cpp
    common/ui/InputDialog.h
    common/ui/InputDialog.ui
    common/ui/MainWindow.cpp
    common/ui/MainWindow.h
    common/ui/MainWindow.ui
    common/ui/ScrollSynchronizer.cpp
    common/ui/ScrollSynchronizer.h

    # employee / core
    employee/core/Employee.cpp
    employee/core/Employee.h
    employee/core/EmployeeData.h
    employee/core/EmployeeRegistry.cpp
    employee/core/EmployeeRegistry.h
    employee/core/EmployeeRegistryManager.cpp
    employee/core/EmployeeRegistryManager.h
    employee/core/IEmployeeDialogService.h
    employee/core/IEmployeeRegistryDialogService.h
    employee/core/IEmployeeRegistryManager.h

    # employee / ui
    employee/ui/EmployeeDialog.cpp
    employee/ui/EmployeeDialog.h
    employee/ui/EmployeeDialog.ui
    employee/ui/EmployeeDialogService.cpp
    employee/ui/EmployeeDialogService.h
    employee/ui/EmployeeListModel.cpp
    employee/ui/EmployeeListModel.h
    employee/ui/EmployeeListView.cpp
    employee/ui/EmployeeListView.h
    employee/ui/EmployeeRegistryController.cpp
    employee/ui/EmployeeRegistryController.h
    employee/ui/EmployeeRegistryDialog.cpp
    employee/ui/EmployeeRegistryDialog.h
    employee/ui/EmployeeRegistryDialog.ui
    employee/ui/EmployeeRegistryDialogService.cpp
    employee/ui/EmployeeRegistryDialogService.h
    employee/ui/EmployeeRegistryListModel.cpp
    employee/ui/EmployeeRegistryListModel.h
    employee/ui/EmployeeWidget.cpp
    employee/ui/EmployeeWidget.h

    # schedule / core
    schedule/core/IScheduleDocumentDialogService.h
    schedule/core/Schedule.cpp
    schedule/core/Schedule.h
    schedule/core/ScheduleData.h
    schedule/core/ScheduleManager.cpp
    schedule/core/ScheduleManager.h

    # schedule / ui
    schedule/ui/CreateScheduleDialog.cpp
    schedule/ui/CreateScheduleDialog.h
    schedule/ui/CreateScheduleDialog.ui
    schedule/ui/ScheduleDocumentController.cpp
    schedule/ui/ScheduleDocumentController.h
    schedule/ui/ScheduleDocumentDialogService.cpp
    schedule/ui/ScheduleDocumentDialogService.h
    schedule/ui/ScheduleHeaderWidget.cpp
    schedule/ui/ScheduleHeaderWidget.h

    # schedule_assignments / core
    schedule_assignments/core/ScheduleAssignments.cpp
    schedule_assignments/core/ScheduleAssignments.h

    # schedule_assignments / ui
    schedule_assignments/ui/ScheduleAssignmentsTableDelegate.cpp
    schedule_assignments/ui/ScheduleAssignmentsTableDelegate.h
    schedule_assignments/ui/ScheduleAssignmentsTableHeaderView.cpp
    schedule_assignments/ui/ScheduleAssignmentsTableHeaderView.h
    schedule_assignments/ui/ScheduleAssignmentsTableModel.cpp
    schedule_assignments/ui/ScheduleAssignmentsTableModel.h
    schedule_assignments/ui/ScheduleAssignmentsTableView.cpp
    schedule_assignments/ui/ScheduleAssignmentsTableView.h
    schedule_assignments/ui/ScheduleAssignmentsWidget.cpp
    schedule_assignments/ui/ScheduleAssignmentsWidget.h

    # schedule_demands / core
    schedule_demands/core/IScheduleDemandsDialogService.h
    schedule_demands/core/ScheduleDemands.cpp
    schedule_demands/core/ScheduleDemands.h

    # schedule_demands / ui
    schedule_demands/ui/ScheduleDemandsDialogService.cpp
    schedule_demands/ui/ScheduleDemandsDialogService.h
    schedule_demands/ui/ScheduleDemandsTableDelegate.cpp
    schedule_demands/ui/ScheduleDemandsTableDelegate.h
    schedule_demands/ui/ScheduleDemandsTableHeaderView.cpp
    schedule_demands/ui/ScheduleDemandsTableHeaderView.h
    schedule_demands/ui/ScheduleDemandsTableModel.cpp
    schedule_demands/ui/ScheduleDemandsTableModel.h
    schedule_demands/ui/ScheduleDemandsTableView.cpp
    schedule_demands/ui/ScheduleDemandsTableView.h
    schedule_demands/ui/ScheduleDemandsWeeklyDialog.cpp
    schedule_demands/ui/ScheduleDemandsWeeklyDialog.h
    schedule_demands/ui/ScheduleDemandsWeeklyDialog.ui
    schedule_demands/ui/ScheduleDemandsWeeklyTableModel.cpp
    schedule_demands/ui/ScheduleDemandsWeeklyTableModel.h
    schedule_demands/ui/ScheduleDemandsWeeklyTableView.cpp
    schedule_demands/ui/ScheduleDemandsWeeklyTableView.h
    schedule_demands/ui/ScheduleDemandsWidget.cpp
    schedule_demands/ui/ScheduleDemandsWidget.h

    # schedule_entry / core
    schedule_entry/core/IScheduleEntryDialogService.h
    schedule_entry/core/IScheduleEntryRegistryDialogService.h
    schedule_entry/core/IScheduleEntryRegistryManager.h
    schedule_entry/core/ScheduleEntry.cpp
    schedule_entry/core/ScheduleEntry.h
    schedule_entry/core/ScheduleEntryCategory.h
    schedule_entry/core/ScheduleEntryData.h
    schedule_entry/core/ScheduleEntryRegistry.cpp
    schedule_entry/core/ScheduleEntryRegistry.h
    schedule_entry/core/ScheduleEntryRegistryManager.cpp
    schedule_entry/core/ScheduleEntryRegistryManager.h

    # schedule_entry / ui
    schedule_entry/ui/ScheduleEntryDialog.cpp
    schedule_entry/ui/ScheduleEntryDialog.h
    schedule_entry/ui/ScheduleEntryDialog.ui
    schedule_entry/ui/ScheduleEntryDialogColorSchemeComboBox.cpp
    schedule_entry/ui/ScheduleEntryDialogColorSchemeComboBox.h
    schedule_entry/ui/ScheduleEntryDialogColorSchemeComboBoxDelegate.cpp
    schedule_entry/ui/ScheduleEntryDialogColorSchemeComboBoxDelegate.h
    schedule_entry/ui/ScheduleEntryDialogService.cpp
    schedule_entry/ui/ScheduleEntryDialogService.h
    schedule_entry/ui/ScheduleEntryListDelegate.cpp
    schedule_entry/ui/ScheduleEntryListDelegate.h
    schedule_entry/ui/ScheduleEntryListModel.cpp
    schedule_entry/ui/ScheduleEntryListModel.h
    schedule_entry/ui/ScheduleEntryListView.cpp
    schedule_entry/ui/ScheduleEntryListView.h
    schedule_entry/ui/ScheduleEntryRegistryController.cpp
    schedule_entry/ui/ScheduleEntryRegistryController.h
    schedule_entry/ui/ScheduleEntryRegistryDialog.cpp
    schedule_entry/ui/ScheduleEntryRegistryDialog.h
    schedule_entry/ui/ScheduleEntryRegistryDialog.ui
    schedule_entry/ui/ScheduleEntryRegistryDialogService.cpp
    schedule_entry/ui/ScheduleEntryRegistryDialogService.h
    schedule_entry/ui/ScheduleEntryRegistryListModel.cpp
    schedule_entry/ui/ScheduleEntryRegistryListModel.h
    schedule_entry/ui/ScheduleEntryWidget.cpp
    schedule_entry/ui/ScheduleEntryWidget.h
)

target_include_directories(ShiftPlannerLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common/core
    ${CMAKE_CURRENT_SOURCE_DIR}/common/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/employee/core
    ${CMAKE_CURRENT_SOURCE_DIR}/employee/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule/core
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule_assignments/core
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule_assignments/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule_demands/core
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule_demands/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule_entry/core
    ${CMAKE_CURRENT_SOURCE_DIR}/schedule_entry/ui
)

target_link_libraries(ShiftPlannerLib PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
)