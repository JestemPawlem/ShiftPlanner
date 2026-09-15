#include <QFontMetrics>
#include <QPainter>

#include "ScheduleEntryListDelegate.h"
#include "ScheduleRoles.h"



ScheduleEntryListDelegate::ScheduleEntryListDelegate(QObject* parent) :
	QStyledItemDelegate{ parent }
{}



void ScheduleEntryListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);

	QRect cardRect = option.rect.adjusted(4, 4, -4, 0);

	QColor bg = index.data(Qt::BackgroundRole).value<QColor>();
	QColor fg = index.data(Qt::ForegroundRole).value<QColor>();

	painter->setBrush(bg);
	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(cardRect, 6, 6);

	if (option.state & QStyle::State_Selected)
	{
		painter->setBrush(Qt::NoBrush);
		painter->setPen(QPen(Qt::white, 1.5));
		painter->drawRoundedRect(cardRect.adjusted(1, 1, -1, -1), 6, 6);
	}

	QString name = index.data(ScheduleRoles::ScheduleEntryName).toString();
	QString code = index.data(ScheduleRoles::ScheduleEntryCode).toString();
	QString details = index.data(ScheduleRoles::ScheduleEntryDetails).toString();

	painter->setPen(fg);

	QFont boldFont = painter->font();
	boldFont.setBold(true);
	boldFont.setPointSize(10);
	painter->setFont(boldFont);

	constexpr int codeWidth = 48;
	QRect codeRect = cardRect.adjusted(cardRect.width() - codeWidth - 10, 5, -10, 0);
	codeRect.setHeight(18);
	painter->drawText(codeRect, Qt::AlignRight | Qt::AlignVCenter, code);

	QFont regFont = painter->font();
	regFont.setBold(true);
	regFont.setPointSize(9);
	painter->setFont(regFont);

	QRect nameRect = cardRect.adjusted(10, 5, -(codeWidth + 20), 0);
	nameRect.setHeight(18);
	QFontMetrics fmReg(regFont);
	painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, name);

	QColor detailsColor = fg;
	detailsColor.setAlpha(200);
	painter->setPen(detailsColor);

	QFont smallFont = painter->font();
	smallFont.setPointSize(8);
	painter->setFont(smallFont);

	QRect detailsRect = cardRect.adjusted(10, 24, -10, -4);
	painter->drawText(detailsRect, Qt::AlignLeft | Qt::AlignVCenter, details);

	painter->restore();
}

QSize ScheduleEntryListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	return QSize(200, 46);
}