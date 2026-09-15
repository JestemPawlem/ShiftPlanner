#include <QAbstractItemView>
#include <QScrollBar>
#include <QWheelEvent>

#include "ScrollSynchronizer.h"



ScrollSynchronizer::ScrollSynchronizer(QAbstractItemView* first, QAbstractItemView* second, QObject* parent) :
	QObject{ parent },
	_first{ first },
	_second{ second }
{
	connect(_first->horizontalScrollBar(), &QScrollBar::valueChanged,
		_second->horizontalScrollBar(), &QScrollBar::setValue);
	connect(_second->horizontalScrollBar(), &QScrollBar::valueChanged,
		_first->horizontalScrollBar(), &QScrollBar::setValue);

	_first->viewport()->installEventFilter(this);
	_second->viewport()->installEventFilter(this);
}



bool ScrollSynchronizer::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Wheel)
	{
		QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);

		if (wheelEvent->modifiers() & Qt::ShiftModifier || wheelEvent->angleDelta().x() != 0)
		{
			QPoint angleDelta = wheelEvent->angleDelta();
			QPoint pixelDelta = wheelEvent->pixelDelta();

			if (angleDelta.x() == 0 && angleDelta.y() != 0)
				angleDelta = QPoint{ angleDelta.y(), 0 };

			if (pixelDelta.x() == 0 && pixelDelta.y() != 0)
				pixelDelta = QPoint{ pixelDelta.y(), 0 };

			QWheelEvent horizontalEvent
			{
				wheelEvent->position(),
				wheelEvent->globalPosition(),
				pixelDelta,
				angleDelta,
				wheelEvent->buttons(),
				wheelEvent->modifiers() & ~Qt::ShiftModifier,
				wheelEvent->phase(),
				wheelEvent->inverted(),
				wheelEvent->source()
			};

			QCoreApplication::sendEvent(_first->horizontalScrollBar(), &horizontalEvent);
			wheelEvent->accept();
			return true;
		}
	}

	return QObject::eventFilter(watched, event);
}