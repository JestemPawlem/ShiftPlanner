#pragma once

#include <QDialog>
#include <QString>

#include "CommonTypes.h"


namespace Ui {
	class InputDialog;
}

class InputDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InputDialog(
		QWidget* parent,
		const QString& title,
		const QString& label,
		const QString& initialText = "");

	~InputDialog();

	static optional<QString> getText(
		QWidget* parent,
		const QString& title,
		const QString& label,
		const QString& initialText = "");

private slots:
	void validateAndAccept();
	void handleInputTextChanged(const QString& text);

private:
	void updateStyle(QWidget* widget);

	Ui::InputDialog* ui;
};