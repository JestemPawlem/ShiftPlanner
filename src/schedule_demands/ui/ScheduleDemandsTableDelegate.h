#pragma once

#include <QStyledItemDelegate>

class QPainter;


class ScheduleDemandsTableDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit ScheduleDemandsTableDelegate(QObject* parent = nullptr);

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};