#include <QHeaderView>

#include "ScheduleDemandsTableView.h"
#include "ScheduleDemandsTableModel.h"
#include "ScheduleDemandsTableDelegate.h"
#include "ScheduleDemandsTableHeaderView.h"



ScheduleDemandsTableView::ScheduleDemandsTableView(QWidget* parent)
	: QTableView(parent)
{
	setProperty("class", "schedule-grid");

	setShowGrid(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setItemDelegate(new ScheduleDemandsTableDelegate(this));

	setHorizontalHeader(new ScheduleDemandsTableHeaderView(Qt::Horizontal, this));
	setVerticalHeader(new ScheduleDemandsTableHeaderView(Qt::Vertical, this));

	setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
}



void ScheduleDemandsTableView::setModel(ScheduleDemandsTableModel* model)
{
	if (_demandsModel)
		_demandsModel->disconnect(this);

	_demandsModel = model;
	QTableView::setModel(model);

	if (model)
	{
		connect(model, &QAbstractItemModel::modelReset, this, &ScheduleDemandsTableView::updateTableSize);
		connect(model, &QAbstractItemModel::rowsInserted, this, &ScheduleDemandsTableView::updateTableSize);
		connect(model, &QAbstractItemModel::rowsRemoved, this, &ScheduleDemandsTableView::updateTableSize);
		connect(model, &QAbstractItemModel::columnsInserted, this, &ScheduleDemandsTableView::updateTableSize);
		connect(model, &QAbstractItemModel::columnsRemoved, this, &ScheduleDemandsTableView::updateTableSize);
	}

	updateTableSize();
}



QSize ScheduleDemandsTableView::sizeHint() const
{
	if (!model())
		return QTableView::sizeHint();

	int totalWidth = verticalHeader()->width() + horizontalHeader()->length() + frameWidth() * 2;
	int totalHeight = horizontalHeader()->height() + verticalHeader()->length() + frameWidth() * 2;

	return QSize(totalWidth, totalHeight);
}



void ScheduleDemandsTableView::updateTableSize()
{
	updateGeometry();
	setFixedHeight(sizeHint().height());
}