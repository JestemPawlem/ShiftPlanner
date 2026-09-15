#include <QPalette>

#include "ScheduleEntryDialogColorSchemeComboBox.h"
#include "ScheduleEntryDialogColorSchemeComboBoxDelegate.h"



namespace
{
	static const QList<ColorScheme> DefaultSchemes =
	{
		{ QColor(186, 153, 112), QColor(235, 233, 230) },
		{ QColor(153, 120, 80),  QColor(227, 225, 220) },
		{ QColor(115, 88,  54),  QColor(220, 218, 212) },
		{ QColor(128, 176, 204), QColor(230, 233, 235) },
		{ QColor(95,  147, 179), QColor(224, 228, 230) },
		{ QColor(68,  116, 145), QColor(218, 223, 226) },
		{ QColor(112, 194, 166), QColor(230, 235, 233) },
		{ QColor(82,  168, 138), QColor(223, 230, 228) },
		{ QColor(52,  130, 104), QColor(214, 222, 220) }
	};
}



ScheduleEntryDialogColorSchemeComboBox::ScheduleEntryDialogColorSchemeComboBox(QWidget* parent) :
	QComboBox{ parent },
	_delegate{ new ScheduleEntryDialogColorSchemeComboBoxDelegate{ this } }
{
	setEditable(false);
	setItemDelegate(_delegate);

	connect(this, &QComboBox::currentIndexChanged,
		this, &ScheduleEntryDialogColorSchemeComboBox::onCurrentIndexChanged);

	for (const ColorScheme& scheme : DefaultSchemes)
	{
		addItem(QString{});
		int index = count() - 1;
		setItemData(index, scheme.background, Qt::BackgroundRole);
		setItemData(index, scheme.foreground, Qt::ForegroundRole);
	}

	onCurrentIndexChanged(0);
}



ColorScheme ScheduleEntryDialogColorSchemeComboBox::selectedColorScheme() const
{
	int index = currentIndex();

	return
	{
		itemData(index, Qt::BackgroundRole).value<QColor>(),
		itemData(index, Qt::ForegroundRole).value<QColor>()
	};
}

void ScheduleEntryDialogColorSchemeComboBox::setSelectedColorScheme(const ColorScheme& scheme)
{
	for (int i = 0; i < count(); ++i)
	{
		QColor background = itemData(i, Qt::BackgroundRole).value<QColor>();
		QColor foreground = itemData(i, Qt::ForegroundRole).value<QColor>();

		if (background == scheme.background && foreground == scheme.foreground)
		{
			setCurrentIndex(i);
			return;
		}
	}
}

void ScheduleEntryDialogColorSchemeComboBox::onScheduleEntryCodeChanged(const QString& text)
{
	_delegate->setScheduleEntryCode(text);

	for (int i = 0; i < count(); ++i)
		setItemText(i, text);

	update();
}



void ScheduleEntryDialogColorSchemeComboBox::onCurrentIndexChanged(int index)
{
	QColor background = itemData(index, Qt::BackgroundRole).value<QColor>();
	QColor foreground = itemData(index, Qt::ForegroundRole).value<QColor>();

	setStyleSheet(QString("background-color: %1; color: %2;")
		.arg(background.name(), foreground.name()));
}