#include <QFont>
#include <QHash>
#include <QPainter>
#include <QPen>
#include <QPixmap>

#include "ScheduleAssignmentsTableDelegate.h"
#include "ScheduleRoles.h"



namespace
{
	static const QColor WeekendColor{ 36, 32, 80 };
	static const QColor WeekdayColor{ 20, 18, 48 };
	static const QPen GridPen{ QColor{ 255, 255, 255, 15 } };
	static const QFont CellFont{ "Montserrat", 8, QFont::Bold };

	struct TileKey
	{
		QString text;
		QRgb background;
		QRgb foreground;

		bool operator==(const TileKey& other) const = default;
	};

	inline size_t qHash(const TileKey& key, size_t seed = 0) noexcept
	{
		return qHashMulti(seed, key.text, key.background, key.foreground);
	}
}

struct ScheduleAssignmentsTableDelegate::Impl
{
	const QPixmap& getTile(
		const QString& text,
		const QColor& background,
		const QColor& foreground,
		const QSize& size,
		qreal dpr)
	{
		TileKey key{ text, background.rgba(), foreground.rgba() };

		auto it = cache.find(key);
		if (it != cache.end())
			return it.value();

		QPixmap pixmap(size * dpr);
		pixmap.setDevicePixelRatio(dpr);
		pixmap.fill(Qt::transparent);

		QPainter painter(&pixmap);
		painter.setRenderHint(QPainter::Antialiasing, true);

		QRect rect(0, 0, size.width(), size.height());
		painter.setBrush(background);
		painter.setPen(Qt::NoPen);
		painter.drawRoundedRect(rect, 4, 4);

		painter.setFont(CellFont);
		painter.setPen(foreground);
		painter.drawText(rect, Qt::AlignCenter, text);

		return *cache.insert(key, std::move(pixmap));
	}

	QHash<TileKey, QPixmap> cache;
};



ScheduleAssignmentsTableDelegate::ScheduleAssignmentsTableDelegate(QObject* parent) :
	QStyledItemDelegate{ parent },
	_impl{ std::make_unique<Impl>() }
{}

ScheduleAssignmentsTableDelegate::~ScheduleAssignmentsTableDelegate() = default;



void ScheduleAssignmentsTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	bool isWeekend = index.data(ScheduleRoles::IsWeekend).toBool();
	painter->fillRect(option.rect, isWeekend ? WeekendColor : WeekdayColor);

	painter->setPen(GridPen);
	if (index.column() < index.model()->columnCount() - 1)
		painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
	if (index.row() < index.model()->rowCount() - 1)
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

	QString text = index.data(Qt::DisplayRole).toString();
	if (!text.isEmpty())
	{
		QRect tileRect = option.rect.adjusted(2, 2, -3, -3);
		QColor background = index.data(Qt::BackgroundRole).value<QColor>();
		QColor foreground = index.data(Qt::ForegroundRole).value<QColor>();

		qreal dpr = painter->device()->devicePixelRatio();
		const QPixmap& tile = _impl->getTile(text, background, foreground, tileRect.size(), dpr);
		painter->drawPixmap(tileRect.topLeft(), tile);
	}
}