#pragma once

#include <QComboBox>
#include <QList>

#include "ColorScheme.h"

class ScheduleEntryDialogColorSchemeComboBoxDelegate;


class ScheduleEntryDialogColorSchemeComboBox : public QComboBox
{
	Q_OBJECT

public:
	explicit ScheduleEntryDialogColorSchemeComboBox(QWidget* parent = nullptr);

	ColorScheme selectedColorScheme() const;
	void setSelectedColorScheme(const ColorScheme& scheme);

public slots:
	void onScheduleEntryCodeChanged(const QString& text);

private slots:
	void onCurrentIndexChanged(int index);

private:
	ScheduleEntryDialogColorSchemeComboBoxDelegate* _delegate = nullptr;
};