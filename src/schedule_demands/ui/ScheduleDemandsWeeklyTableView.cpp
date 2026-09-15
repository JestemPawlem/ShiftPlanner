#include "ScheduleDemandsWeeklyTableView.h"
#include "ScheduleDemandsWeeklyTableModel.h"
#include "ScheduleDemandsTableDelegate.h"
#include "ScheduleDemandsTableHeaderView.h"


ScheduleDemandsWeeklyTableView::ScheduleDemandsWeeklyTableView(QWidget* parent)
    : QTableView(parent)
{
    setProperty("class", "schedule-grid");

    setItemDelegate(new ScheduleDemandsTableDelegate(this));
    setShowGrid(false);

    setHorizontalHeader(new ScheduleDemandsTableHeaderView(Qt::Horizontal, this));
    setVerticalHeader(new ScheduleDemandsTableHeaderView(Qt::Vertical, this));

    setSelectionMode(QAbstractItemView::NoSelection);
    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    setSortingEnabled(false);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void ScheduleDemandsWeeklyTableView::setModel(ScheduleDemandsWeeklyTableModel* model)
{
    if (this->model())
        this->model()->disconnect(this);

    QTableView::setModel(model);

    if (model)
    {
        connect(model, &QAbstractItemModel::modelReset, this, &ScheduleDemandsWeeklyTableView::updateTableSize);
        connect(model, &QAbstractItemModel::rowsInserted, this, &ScheduleDemandsWeeklyTableView::updateTableSize);
        connect(model, &QAbstractItemModel::rowsRemoved, this, &ScheduleDemandsWeeklyTableView::updateTableSize);
    }

    updateTableSize();
}


QSize ScheduleDemandsWeeklyTableView::sizeHint() const
{
    if (!model())
        return QTableView::sizeHint();

    int totalWidth = verticalHeader()->width() + horizontalHeader()->length() + frameWidth() * 2;
    int totalHeight = horizontalHeader()->height() + verticalHeader()->length() + frameWidth() * 2;

    return QSize(totalWidth, totalHeight);
}

QSize ScheduleDemandsWeeklyTableView::minimumSizeHint() const
{
    return sizeHint();
}

void ScheduleDemandsWeeklyTableView::updateTableSize()
{
    updateGeometry();
}