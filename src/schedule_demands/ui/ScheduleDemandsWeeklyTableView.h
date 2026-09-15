#pragma once

#include <QTableView>


class ScheduleDemandsWeeklyTableModel;

class ScheduleDemandsWeeklyTableView : public QTableView
{
    Q_OBJECT

public:
    explicit ScheduleDemandsWeeklyTableView(QWidget* parent = nullptr);

    void setModel(ScheduleDemandsWeeklyTableModel* model);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    void updateTableSize();
};