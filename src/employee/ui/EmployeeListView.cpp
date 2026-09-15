#include <QMouseEvent>

#include "EmployeeListView.h"



EmployeeListView::EmployeeListView(QWidget* parent) :
	QListView{ parent }
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setProperty("class", "item-list");
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setContextMenuPolicy(Qt::ActionsContextMenu);
}



void EmployeeListView::setModel(QAbstractItemModel* model)
{
	QListView::setModel(model);

	connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &EmployeeListView::onCurrentChanged);
}



void EmployeeListView::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	selectionModel()->clear();
}



void EmployeeListView::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous);

	qulonglong id = current.isValid() ? current.data(Qt::UserRole).toULongLong() : 0;
	emit currentEmployeeChanged(id);
}