#pragma once

#include <memory>
#include <QStyledItemDelegate>


class ScheduleAssignmentsTableDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit ScheduleAssignmentsTableDelegate(QObject* parent = nullptr);
	~ScheduleAssignmentsTableDelegate() override;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	struct Impl;
	std::unique_ptr<Impl> _impl;
};