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

#ifndef CODEINITIALIZER_H
#define CODEINITIALIZER_H

#include "executer_global.h"

#include <QScriptValue>
#include <QScriptEngine>

class QScriptEngine;

namespace ActionTools
{
	class ActionFactory;
}

namespace LibExecuter
{
	class ScriptAgent;
	
	class EXECUTERSHARED_EXPORT CodeInitializer
	{
	public:
		static void initialize(QScriptEngine *scriptEngine, ScriptAgent *scriptAgent, ActionTools::ActionFactory *actionFactory);
		
		template<typename T>
		static QScriptValue addCodeClass(const QString &objectName, QScriptEngine *scriptEngine)
		{
			QScriptValue metaObject = scriptEngine->newQMetaObject(&T::staticMetaObject, scriptEngine->newFunction(&T::constructor));
			scriptEngine->globalObject().setProperty(objectName, metaObject);
			
			return metaObject;
		}
	};
}

#endif // CODEINITIALIZER_H