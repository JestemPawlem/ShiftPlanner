#pragma once

#include <QDialog>


class ScheduleManager;
class ScheduleDemandsWeeklyTableModel;
class ScheduleDemandsWeeklyTableView;

namespace Ui {
    class ScheduleDemandsWeeklyDialog;
}

class ScheduleDemandsWeeklyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleDemandsWeeklyDialog(ScheduleManager& manager, QWidget* parent = nullptr);

    static bool execute(ScheduleManager& manager, QWidget* parent = nullptr);

private:
    void applyDemands();

    Ui::ScheduleDemandsWeeklyDialog* ui;

    ScheduleManager& _manager;
    ScheduleDemandsWeeklyTableModel* _model;
    ScheduleDemandsWeeklyTableView* _view;
};