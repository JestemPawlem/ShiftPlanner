#include <QItemSelectionModel>

#include "Schedule.h"
#include "ScheduleEntryListDelegate.h"
#include "ScheduleEntryListView.h"



ScheduleEntryListView::ScheduleEntryListView(QWidget* parent) :
	QListView{ parent }
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setProperty("class", "item-list");
	setItemDelegate(new ScheduleEntryListDelegate(this));
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);
	setContextMenuPolicy(Qt::ActionsContextMenu);
}



void ScheduleEntryListView::setModel(QAbstractItemModel* model)
{
	QListView::setModel(model);

	connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &ScheduleEntryListView::onCurrentChanged);
}



qulonglong ScheduleEntryListView::selectedScheduleEntryId() const
{
	QModelIndex current = currentIndex();
	return current.isValid() ? current.data(Qt::UserRole).toULongLong() : 0;
}



void ScheduleEntryListView::onScheduleOpened(const Schedule& schedule)
{
	Q_UNUSED(schedule);

	selectionModel()->clear();
}



void ScheduleEntryListView::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	Q_UNUSED(previous);

	qulonglong id = current.isValid() ? current.data(Qt::UserRole).toULongLong() : 0;
	emit currentScheduleEntryChanged(id);
}