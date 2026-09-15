#include <QHeaderView>

#include "ScheduleAssignmentsTableView.h"
#include "ScheduleAssignmentsTableModel.h"
#include "ScheduleAssignmentsTableDelegate.h"
#include "ScheduleAssignmentsTableHeaderView.h"



ScheduleAssignmentsTableView::ScheduleAssignmentsTableView(QWidget* parent)
    : QTableView(parent)
{
    setProperty("class", "schedule-grid");

    setShowGrid(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setItemDelegate(new ScheduleAssignmentsTableDelegate(this));

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::NoSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);

    horizontalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setDefaultSectionSize(38);

    setVerticalHeader(new ScheduleAssignmentsTableHeaderView(this));

    connect(this, &QTableView::clicked, this, &ScheduleAssignmentsTableView::onCellClicked);
    connect(this, &QTableView::customContextMenuRequested, this, &ScheduleAssignmentsTableView::onContextMenu);
}



void ScheduleAssignmentsTableView::setModel(ScheduleAssignmentsTableModel* model)
{
    _model = model;
    QTableView::setModel(model);
}



void ScheduleAssignmentsTableView::onCellClicked(const QModelIndex& index)
{
    if (!index.isValid() || !_model)
        return;

    qulonglong id = _model->getEmployeeIdFromRow(index.row());
    QDate date = _model->getDateFromColumn(index.column());

    emit assignmentChangeRequested(id, date);
}

void ScheduleAssignmentsTableView::onContextMenu(const QPoint& pos)
{
    QModelIndex index = indexAt(pos);
    if (!index.isValid() || !_model)
        return;

    if (index.data(Qt::UserRole).toULongLong() == 0)
        return;

    qulonglong id = _model->getEmployeeIdFromRow(index.row());
    QDate date = _model->getDateFromColumn(index.column());

    emit assignmentRemovalRequested(id, date);
}