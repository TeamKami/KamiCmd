#include <QtWidgets/QApplication>
#include "CoreFunctions.h"

int main(int argc, char *argv[])
{
	int res = -1;
	{
		//QApplication::setStyle("plastique");
		QApplication a(argc, argv);
		QSettings::setDefaultFormat(QSettings::IniFormat);

		QCoreApplication::setApplicationName("KamiCmd");
		{
			QFileInfo settings(QCoreApplication::applicationDirPath() + "/Settings/KamiCmd.ini");
			if (1) //settings.isFile() && settings.isWritable())
			{ // For now - always locally stored settings
				QCoreApplication::setOrganizationName("Settings");
				QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath());
			}
			else
				QCoreApplication::setOrganizationName("KamiCmd");
		}

		CoreFunctions core;

#ifdef QT_DEBUG
		core.RedirectDebug();
#endif // QT_DEBUG

		if (!core.LoadModules())
			return 1;

        IUnitManager *unitManager;
        if ((unitManager = dynamic_cast<IUnitManager *>(core.QueryModule("UnitManager", 1))) != 0)
			unitManager->Start();
		else
		{
			QVector<const Module*> modulesInfo = core.GetModulesInfo();
			QString msg; 
			QSet<QString> set;
			foreach(const Module * module, modulesInfo)
				set.insert(module->parentLibName);
			foreach(QString module, set.toList())
				msg.append("\n" + module);
			
			core.DebugWrite("Core", QString("UnitManager module not found\nLoaded libraries:%1").arg(msg), ICoreFunctions::Error);
			return 1;
		}
		res = a.exec();

		//delete unitManager; // dont know why is it wrong
	}

	return res;
}
