#pragma once

#include <QHeaderView>


class ScheduleAssignmentsTableHeaderView : public QHeaderView
{
	Q_OBJECT

public:
	explicit ScheduleAssignmentsTableHeaderView(QWidget* parent = nullptr);

protected:
	void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
};