#include <QLabel>
#include <QVBoxLayout>

#include "Schedule.h"
#include "ScheduleHeaderWidget.h"
#include "ScheduleManager.h"



ScheduleHeaderWidget::ScheduleHeaderWidget(ScheduleManager* scheduleManager, QWidget* parent) :
	QWidget{ parent },
	_scheduleManager{ scheduleManager }
{
	Q_ASSERT(scheduleManager != nullptr);

	setupUi();
	setupConnections();
}



void ScheduleHeaderWidget::onScheduleOpened(const Schedule& schedule)
{
	_nameLabel->setText(schedule.name());

	QString subtitle = schedule.startDate().toString("dd.MM.yyyy") + " - " + schedule.endDate().toString("dd.MM.yyyy");
	_periodLabel->setText(subtitle);
}

void ScheduleHeaderWidget::onScheduleClosed()
{
	_nameLabel->clear();
	_periodLabel->clear();
}



void ScheduleHeaderWidget::setupUi()
{
	_nameLabel = new QLabel(this);
	_nameLabel->setProperty("class", "schedule-title");
	_nameLabel->setAlignment(Qt::AlignRight);

	_periodLabel = new QLabel(this);
	_periodLabel->setProperty("class", "schedule-subtitle");
	_periodLabel->setAlignment(Qt::AlignRight);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 10);
	layout->setSpacing(5);
	layout->addWidget(_nameLabel);
	layout->addWidget(_periodLabel);
}

void ScheduleHeaderWidget::setupConnections()
{
	connect(_scheduleManager, &ScheduleManager::scheduleOpened,
		this, &ScheduleHeaderWidget::onScheduleOpened);
	connect(_scheduleManager, &ScheduleManager::scheduleClosed,
		this, &ScheduleHeaderWidget::onScheduleClosed);
}