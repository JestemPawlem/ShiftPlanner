#include <QFont>
#include <QPainter>
#include <QPen>

#include "ScheduleDemandsTableHeaderView.h"
#include "ScheduleRoles.h"



namespace
{
	static const QColor SectionBackgroundColor{ 28, 25, 66, 220 };
	static const QColor WeekendTextColor{ 207, 157, 74 };
	static const QColor WeekdayTextColor{ 240, 240, 250 };
	static const QPen GridPen{ QColor{ 255, 255, 255, 15 } };
	static const QFont SectionFont{ "Montserrat", 8, QFont::Bold };
}



ScheduleDemandsTableHeaderView::ScheduleDemandsTableHeaderView(Qt::Orientation orientation, QWidget* parent) :
	QHeaderView{ orientation, parent }
{
	setSectionsClickable(false);
	setHighlightSections(false);
	setSectionResizeMode(QHeaderView::Fixed);

	if (orientation == Qt::Horizontal)
	{
		setFixedHeight(35);
		setDefaultSectionSize(38);
	}
	else
	{
		setFixedWidth(160);
		setDefaultSectionSize(30);
	}
}



void ScheduleDemandsTableHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
	painter->save();

	painter->fillRect(rect, SectionBackgroundColor);

	if (orientation() == Qt::Horizontal)
	{
		bool isWeekend = model()->headerData(logicalIndex, Qt::Horizontal, ScheduleRoles::IsWeekend).toBool();
		QString text = model()->headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();

		painter->setPen(GridPen);
		painter->drawLine(rect.bottomLeft(), rect.bottomRight());
		if (logicalIndex < count() - 1)
			painter->drawLine(rect.topRight(), rect.bottomRight());

		painter->setFont(SectionFont);
		painter->setPen(isWeekend ? WeekendTextColor : WeekdayTextColor);
		painter->drawText(rect, Qt::AlignCenter, text);
	}
	else
	{
		QString text = model()->headerData(logicalIndex, Qt::Vertical, Qt::DisplayRole).toString();

		painter->setPen(GridPen);
		painter->drawLine(rect.topRight(), rect.bottomRight());
		if (logicalIndex < count() - 1)
			painter->drawLine(rect.bottomLeft(), rect.bottomRight());

		QVariant backgroundVar = model()->headerData(logicalIndex, Qt::Vertical, Qt::BackgroundRole);
		QVariant foregroundVar = model()->headerData(logicalIndex, Qt::Vertical, Qt::ForegroundRole);

		QColor tileBackgroundColor = backgroundVar.value<QColor>();
		QColor tileForegroundColor = foregroundVar.value<QColor>();

		painter->setRenderHint(QPainter::Antialiasing, true);
		QRect tileRect = rect.adjusted(3, 3, -3, -3);

		painter->setBrush(tileBackgroundColor);
		painter->setPen(Qt::NoPen);
		painter->drawRoundedRect(tileRect, 4, 4);

		painter->setFont(SectionFont);
		painter->setPen(tileForegroundColor);
		painter->drawText(tileRect.adjusted(8, 0, -8, 0), Qt::AlignLeft | Qt::AlignVCenter, text);
	}
}