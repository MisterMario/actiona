/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include <QtGlobal>

#ifdef Q_WS_X11
#include <libnotify/notify.h>
#endif

#include "mainwindow.h"
#include "actioninstance.h"
#include "parameter.h"
#include "subparameter.h"
#include "actioninstancebuffer.h"
#include "global.h"
#include "version.h"
#include "globalshortcutmanager.h"

#include <QxtApplication>
#include <QxtCommandOptions>
#include <QDir>
#include <QSplashScreen>
#include <QDebug>
#include <QTextStream>

#ifdef QT_WS_WIN
#include <windows.h>
#endif

void cleanup()
{
	GlobalShortcutManager::clear();
}

int main(int argc, char **argv)
{
	QxtApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);

	qAddPostRoutine(cleanup);

#ifdef Q_WS_X11
	notify_init("Actionaz");
#endif

	//QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

	QxtCommandOptions options;
	options.setFlagStyle(QxtCommandOptions::DoubleDash);
	options.setScreenWidth(0);
	options.add("nosplash", QObject::tr("disable the splash screen"));
	options.alias("nosplash", "s");
	options.add("notrayicon", QObject::tr("disable the tray icon"));
	options.alias("notrayicon", "t");
	options.add("noexecutionwindow", QObject::tr("do not show the execution window"));
	options.alias("noexecutionwindow", "E");
	options.add("noconsolewindow", QObject::tr("do not show the console window"));
	options.alias("noconsolewindow", "C");
	options.add("execute", QObject::tr("execute the current script"));
	options.alias("execute", "e");
	options.add("exitatend", QObject::tr("close Actionaz after execution - requires execute"));
	options.alias("exitatend", "x");
	options.add("help", QObject::tr("show this help text"));
	options.alias("help", "h");
	options.parse(QCoreApplication::arguments());
	if(options.count("help") || options.showUnrecognizedWarning() || (options.count("exitatend") && !options.count("execute")))
	{
		QTextStream stream(stdout);
		stream << QObject::tr("usage : ") << QCoreApplication::arguments().at(0) << " " << QObject::tr("[parameters]") << " " << QObject::tr("[filename]") << "\n";
		stream << QObject::tr("Parameters are :") << "\n";
		stream << options.getUsage();
		stream.flush();
		return -1;
	}

	QString startScript;
	const QStringList &positionalParameters = options.positional();
	if(positionalParameters.count() > 0)
		startScript = positionalParameters.at(0);

#ifdef QT_WS_WIN
	AllowSetForegroundWindow(ASFW_ANY);
	// This is needed so that relative paths will work on Windows regardless of where the app is launched from.
	QDir::setCurrent(app.applicationDirPath());
#endif

	app.addLibraryPath(QDir::currentPath() + "/actions");

	qRegisterMetaType<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaType<ActionTools::ActionInstance::ExecutionException>("ActionInstance::ExecutionException");
	qRegisterMetaType<ActionTools::Parameter>("Parameter");
	qRegisterMetaType<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaType<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaType<Tools::Version>("Tools::Version");

	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstance>("ActionInstance");
	qRegisterMetaTypeStreamOperators<ActionTools::Parameter>("Parameter");
	qRegisterMetaTypeStreamOperators<ActionTools::SubParameter>("SubParameter");
	qRegisterMetaTypeStreamOperators<ActionTools::ActionInstanceBuffer>("ActionInstanceBuffer");
	qRegisterMetaTypeStreamOperators<Tools::Version>("Tools::Version");

	app.setOrganizationName("Actionaz");
	app.setOrganizationDomain("actionaz.eu");
	app.setApplicationName("Actionaz");
	app.setApplicationVersion(Global::ACTIONAZ_VERSION.toString());

	QSplashScreen *splash = 0;
	if(!options.count("nosplash") && !options.count("execute"))
	{
		splash = new QSplashScreen(QPixmap(":/images/start.png"), Qt::WindowStaysOnTopHint);
		splash->show();
		app.processEvents();
	}

	MainWindow mainWindow(&options, splash, startScript);
	mainWindow.setWindowOpacity(0.0);

	if(!options.count("execute"))
		mainWindow.show();

	try
	{
		return app.exec();
	}
	catch(...)
	{
		return -1;
	}
}