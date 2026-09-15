#include <memory>

#include <QApplication>
#include <QCoreApplication>

#include "MainWindow.h"
#include "ScheduleManager.h"
#include "EmployeeRegistryManager.h"
#include "ScheduleEntryRegistryManager.h"
#include "FileJsonStorage.h"

#include <qdiriterator.h>

int main(int argc, char* argv[])
{
	Q_INIT_RESOURCE(resources);

	QApplication app(argc, argv);

	const QString employeesDir = QCoreApplication::applicationDirPath() + "/data/employees";
	const QString entriesDir = QCoreApplication::applicationDirPath() + "/data/entries";

	auto employeeRegistryManager = std::make_unique<EmployeeRegistryManager>(
		std::make_unique<FileJsonStorage>(),
		employeesDir
	);

	auto entryRegistryManager = std::make_unique<ScheduleEntryRegistryManager>(
		std::make_unique<FileJsonStorage>(),
		entriesDir
	);

	auto scheduleManager = std::make_unique<ScheduleManager>(
		std::make_unique<FileJsonStorage>()
	);

	result_t<void> result;

	result = employeeRegistryManager->loadAll();
	if (!result)
	{
		qCritical() << "Error:" << result.error();
		return 1;
	}

	result = entryRegistryManager->loadAll();
	if (!result)
	{
		qCritical() << "Error:" << result.error();
		return 1;
	}

	QFile file(":/resources/styles/style.qss");
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		app.setStyleSheet(QString::fromUtf8(file.readAll()));
		file.close();
	}

	MainWindow w(scheduleManager.get(), employeeRegistryManager.get(), entryRegistryManager.get());
	w.show();

	return app.exec();
}