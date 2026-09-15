#pragma once

#include <QTableView>


class ScheduleDemandsTableModel;

class ScheduleDemandsTableView : public QTableView
{
	Q_OBJECT
public:
	explicit ScheduleDemandsTableView(QWidget* parent = nullptr);

	void setModel(ScheduleDemandsTableModel* model);

	QSize sizeHint() const override;

private slots:
	void updateTableSize();

private:
	ScheduleDemandsTableModel* _demandsModel = nullptr;
};