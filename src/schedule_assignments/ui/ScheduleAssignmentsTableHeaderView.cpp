#include <QFont>
#include <QPainter>
#include <QPen>

#include "ScheduleAssignmentsTableHeaderView.h"



namespace
{
	static const QColor SectionBackgroundColor{ 28, 25, 66, 220 };
	static const QColor EmployeeTextColor{ 220, 220, 240 };
	static const QPen GridPen{ QColor{ 255, 255, 255, 15 } };
	static const QFont EmployeeFont{ "Montserrat", 8, QFont::Bold };
}



ScheduleAssignmentsTableHeaderView::ScheduleAssignmentsTableHeaderView(QWidget* parent) :
	QHeaderView{ Qt::Vertical, parent }
{
	setSectionsClickable(false);
	setHighlightSections(false);
	setSectionResizeMode(QHeaderView::Fixed);
	setDefaultSectionSize(30);
	setFixedWidth(160);
}



void ScheduleAssignmentsTableHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
	painter->save();

	painter->fillRect(rect, SectionBackgroundColor);

	painter->setPen(GridPen);
	painter->drawLine(rect.topRight(), rect.bottomRight());
	if (logicalIndex < count() - 1)
		painter->drawLine(rect.bottomLeft(), rect.bottomRight());

	painter->setFont(EmployeeFont);
	painter->setPen(EmployeeTextColor);
	painter->drawText(rect.adjusted(12, 0, -12, 0), Qt::AlignLeft | Qt::AlignVCenter,
		model()->headerData(logicalIndex, Qt::Vertical, Qt::DisplayRole).toString());

	painter->restore();
}