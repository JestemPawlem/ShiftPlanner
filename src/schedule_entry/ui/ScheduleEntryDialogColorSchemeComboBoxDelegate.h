#pragma once

#include <QStyledItemDelegate>


class ScheduleEntryDialogColorSchemeComboBoxDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit ScheduleEntryDialogColorSchemeComboBoxDelegate(QObject* parent = nullptr);

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	void setScheduleEntryCode(const QString& code);

private:
	QString _scheduleEntryCode;
};