#pragma once

#include <QObject>

class QAbstractItemView;


class ScrollSynchronizer : public QObject
{
	Q_OBJECT

public:
	explicit ScrollSynchronizer(QAbstractItemView* first, QAbstractItemView* second, QObject* parent = nullptr);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	QAbstractItemView* _first = nullptr;
	QAbstractItemView* _second = nullptr;
};