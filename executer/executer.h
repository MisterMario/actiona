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

#ifndef EXECUTER_H
#define EXECUTER_H

#include "executer_global.h"
#include "actioninstance.h"
#include "scriptagent.h"

#include <QObject>
#include <QScriptEngine>
#include <QPoint>
#include <QKeySequence>

namespace ActionTools
{
	class Script;
	class ActionFactory;
	class ConsoleWidget;
}

class ExecutionWindow;
class QStandardItemModel;

class EXECUTERSHARED_EXPORT Executer : public QObject
{
	Q_OBJECT

public:
	Executer(ActionTools::Script *script,
			 ActionTools::ActionFactory *actionFactory,
			 bool showExecutionWindow,
			 int executionWindowPosition,
			 int executionWindowScreen,
			 bool showConsoleWindow,
			 int consoleWindowPosition,
			 int consoleWindowScreen,
			 const QKeySequence &stopExecutionHotkey,
			 QStandardItemModel *consoleModel,
			 QObject *parent = 0);
	~Executer();

	ExecutionWindow *executionWindow() const			{ return mExecutionWindow; }
	ActionTools::ConsoleWidget *consoleWidget() const	{ return mConsoleWidget; }
	ScriptAgent *scriptAgent() const					{ return mScriptAgent; }

	int currentActionIndex() const						{ return mCurrentActionIndex; }
	ActionTools::Script *script() const					{ return mScript; }

public slots:
	bool startExecution(bool onlySelection);
	void stopExecution();

signals:
	void executionStopped();
	void scriptError(int actionIndex, const QString &parameter, const QString &error);
	void actionStarted(int actionIndex);
	void actionEnded(int actionIndex);

private slots:
	void startFirstAction();
	void executionException(ActionTools::ActionInstance::ExecutionException exceptionType,
							const QString &message);
	void actionExecutionEnded();
	void disableAction(bool disable);
	void startNextAction();

private:
	enum ExecuteActionResult
	{
		CanExecute,
		IncorrectLine,
		InvalidAction,
		DisabledAction,
		UnselectedAction
	};

	ExecuteActionResult canExecuteAction(int index) const;
	void executeCurrentAction();
	void addClassToScript(QObject *classPointer, const QString &name);

	ActionTools::Script *mScript;
	ActionTools::ActionFactory *mActionFactory;
	bool mShowExecutionWindow;
	int mExecutionWindowPosition;
	int mExecutionWindowScreen;
	bool mShowConsoleWindow;
	int mConsoleWindowPosition;
	int mConsoleWindowScreen;
	QKeySequence mStopExecutionShortcut;
	ExecutionWindow *mExecutionWindow;
	ActionTools::ConsoleWidget *mConsoleWidget;
	int mCurrentActionIndex;
	bool mExecutionStarted;
	QScriptEngine mScriptEngine;
	bool mExecuteOnlySelection;
	ScriptAgent *mScriptAgent;
	QList<bool> mActionEnabled;

	Q_DISABLE_COPY(Executer)
};

#endif // EXECUTER_H