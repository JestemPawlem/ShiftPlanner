#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "ScheduleDemandsWeeklyDialog.h"
#include "ScheduleDemandsWeeklyTableModel.h"
#include "ScheduleDemandsWeeklyTableView.h"
#include "ScheduleManager.h"
#include "Schedule.h"
#include "ui_ScheduleDemandsWeeklyDialog.h"


ScheduleDemandsWeeklyDialog::ScheduleDemandsWeeklyDialog(ScheduleManager& manager, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ScheduleDemandsWeeklyDialog)
    , _manager(manager)
    , _model(new ScheduleDemandsWeeklyTableModel(manager.schedule(), this))
{
    Q_ASSERT(manager.schedule() != nullptr);

    ui->setupUi(this);

    setWindowTitle("Set Weekly Demands");
    setProperty("class", "container");

    if (layout())
        layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->scheduleDemandsWeeklyTableView->setModel(_model);

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->okButton, &QPushButton::clicked, this, [this]() {
        applyDemands();
        accept();
        });

    adjustSize();
}


bool ScheduleDemandsWeeklyDialog::execute(ScheduleManager& manager, QWidget* parent)
{
    if (!manager.schedule())
        return false;

    ScheduleDemandsWeeklyDialog dialog(manager, parent);
    return dialog.exec() == QDialog::Accepted;
}


void ScheduleDemandsWeeklyDialog::applyDemands()
{
    const Schedule* schedule = _manager.schedule();
    if (!schedule)
        return;

    const auto& grid = _model->templateGrid();
    QDate current = schedule->startDate();
    const QDate end = schedule->endDate();

    while (current <= end)
    {
        int dayIndex = current.dayOfWeek() - 1;

        for (auto it = grid.cbegin(); it != grid.cend(); ++it)
        {
            qulonglong entryId = it.key();
            qsizetype count = it.value()[dayIndex];

            if (count > 0)
                _manager.setDemand(entryId, current, count);
            else
                _manager.removeDemand(entryId, current);
        }
        current = current.addDays(1);
    }
}