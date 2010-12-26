#include <QtGui/QApplication>
#include "KamiCmd.h"

int main(int argc, char *argv[])
{
	int res = -1;
	{
		QApplication a(argc, argv);
		QSettings::setDefaultFormat(QSettings::IniFormat);

		QCoreApplication::setApplicationName("KamiCmd");
		{
			QFileInfo settings(QCoreApplication::applicationDirPath() + "/Settings/KamiCmd.ini");
			if (settings.isFile() && settings.isWritable())
			{
				QCoreApplication::setOrganizationName("Settings");
				QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath());
			}
			else
				QCoreApplication::setOrganizationName("KamiCmd");
		}

		CoreFunctions core;
		if (!core.LoadModules())
			return 1;

		IUnitManager *unitManager;
		if (unitManager = dynamic_cast<IUnitManager *>(core.QueryModule("UnitManager", 1)))
			unitManager->Start();
		else
		{
			core.DebugWrite("Core", "UnitManager module not found", ICoreFunctions::Error);
			return 1;
		}
		res = a.exec();

		//delete unitManager; // dont know why is it wrong
	}

	return res;
}
