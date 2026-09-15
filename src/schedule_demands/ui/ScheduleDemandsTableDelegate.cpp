#include <QFont>
#include <QPainter>
#include <QPen>

#include "ScheduleDemandsTableDelegate.h"
#include "ScheduleRoles.h"



namespace
{
	static const QColor WeekendColor{ 36, 32, 80 };
	static const QColor WeekdayColor{ 20, 18, 48 };
	static const QPen GridPen{ QColor{ 255, 255, 255, 15 } };

	static const QColor TextZeroColor{ 130, 130, 160 };
	static const QColor TextMatchedColor{ 140, 235, 170 };
	static const QColor TextUnderflowColor{ 255, 160, 150 };
	static const QColor TextOverflowColor{ 165, 190, 255 };

	static const QFont DemandFont{ "Montserrat", 8, QFont::Bold };
}



ScheduleDemandsTableDelegate::ScheduleDemandsTableDelegate(QObject* parent) :
	QStyledItemDelegate{ parent }
{}



void ScheduleDemandsTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();

	bool isWeekend = index.data(ScheduleRoles::IsWeekend).toBool();
	painter->fillRect(option.rect, isWeekend ? WeekendColor : WeekdayColor);

	painter->setPen(GridPen);
	if (index.column() < index.model()->columnCount() - 1)
		painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
	if (index.row() < index.model()->rowCount() - 1)
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

	const QString text = index.data(Qt::DisplayRole).toString();
	if (!text.isEmpty())
	{
		const QVariant assignedVar = index.data(ScheduleRoles::AssignedCount);
		const QVariant demandVar = index.data(ScheduleRoles::DemandCount);

		const qsizetype assigned = assignedVar.value<qsizetype>();
		const qsizetype demand = demandVar.value<qsizetype>();

		QColor textColor;

		if (demand == 0 && assigned == 0)
			textColor = TextZeroColor;
		else if (demand > 0 && assigned == demand)
			textColor = TextMatchedColor;
		else if (assigned < demand)
			textColor = TextUnderflowColor;
		else
			textColor = TextOverflowColor;

		painter->setFont(DemandFont);
		painter->setPen(textColor);
		painter->drawText(option.rect, Qt::AlignCenter, text);
	}

	painter->restore();
}