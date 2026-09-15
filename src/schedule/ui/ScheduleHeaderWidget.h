#pragma once

#include <QWidget>

class QLabel;
class Schedule;
class ScheduleManager;


class ScheduleHeaderWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ScheduleHeaderWidget(ScheduleManager* scheduleManager, QWidget* parent = nullptr);

public slots:
	void onScheduleOpened(const Schedule& schedule);
	void onScheduleClosed();

private:
	void setupUi();
	void setupConnections();

	ScheduleManager* _scheduleManager;

	QLabel* _nameLabel = nullptr;
	QLabel* _periodLabel = nullptr;
};