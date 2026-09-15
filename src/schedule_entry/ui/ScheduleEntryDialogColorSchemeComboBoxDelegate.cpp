#include <QColor>
#include <QPainter>

#include "ScheduleEntryDialogColorSchemeComboBoxDelegate.h"



ScheduleEntryDialogColorSchemeComboBoxDelegate::ScheduleEntryDialogColorSchemeComboBoxDelegate(QObject* parent) :
	QStyledItemDelegate{ parent }
{}



void ScheduleEntryDialogColorSchemeComboBoxDelegate::setScheduleEntryCode(const QString& code)
{
	_scheduleEntryCode = code;
}



void ScheduleEntryDialogColorSchemeComboBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();

	QColor background = index.data(Qt::BackgroundRole).value<QColor>();
	QColor foreground = index.data(Qt::ForegroundRole).value<QColor>();

	if (option.state & (QStyle::State_MouseOver | QStyle::State_Selected))
		background = background.lighter(108);

	painter->fillRect(option.rect, background);

	painter->setFont(option.font);
	painter->setPen(foreground);
	painter->drawText(option.rect.adjusted(10, 0, -10, 0), Qt::AlignLeft | Qt::AlignVCenter, _scheduleEntryCode);

	painter->restore();
}