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

#include "numberparameterdefinition.h"
#include "subparameter.h"
#include "codespinbox.h"
#include "actioninstance.h"

namespace ActionTools
{
	NumberParameterDefinition::NumberParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent)
		: ParameterDefinition(category, name, translatedName, parent),
		mSpinBox(0),
		mMaximum(99),
		mMinimum(0),
		mSingleStep(1)
	{
	}

	void NumberParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mSpinBox = new CodeSpinBox(parent);
		mSpinBox->setObjectName("value");

		mSpinBox->setPrefix(mPrefix);
		mSpinBox->setSuffix(mSuffix);
		mSpinBox->setMaximum(mMaximum);
		mSpinBox->setMinimum(mMinimum);
		mSpinBox->setSingleStep(mSingleStep);

		addEditor(mSpinBox);
	}

	void NumberParameterDefinition::load(const ActionInstance *actionInstance)
	{
		mSpinBox->setFromSubParameter(actionInstance->subParameter(name(), "value"));
	}

	void NumberParameterDefinition::save(ActionInstance *actionInstance)
	{
		actionInstance->setSubParameter(name(), "value", mSpinBox->isCode(), QString::number(mSpinBox->value()));
	}

	void NumberParameterDefinition::setDefaultValues(Parameter &data)
	{
		data.subParameters()["value"].setValue(option("default"));
	}
}