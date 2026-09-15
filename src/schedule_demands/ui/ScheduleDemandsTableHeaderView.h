#pragma once

#include <QHeaderView>


class ScheduleDemandsTableHeaderView : public QHeaderView
{
	Q_OBJECT

public:
	explicit ScheduleDemandsTableHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
	void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
};