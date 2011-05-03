/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2011  Georg Rudoy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#include "servicediscoverywidget.h"
#include <QToolBar>
#include <QComboBox>
#include <QLineEdit>

namespace LeechCraft
{
namespace Azoth
{
	QObject* ServiceDiscoveryWidget::S_ParentMultiTabs_ = 0;

	void ServiceDiscoveryWidget::SetParentMultiTabs (QObject *parent)
	{
		S_ParentMultiTabs_ = parent;
	}

	ServiceDiscoveryWidget::ServiceDiscoveryWidget (QWidget *parent)
	: QWidget (parent)
	, Toolbar_ (new QToolBar)
	{
		Ui_.setupUi (this);
		
		QComboBox *combo = new QComboBox;
		Toolbar_->addWidget (combo);
		
		QLineEdit *address = new QLineEdit;
		Toolbar_->addWidget (address);
	}
	
	TabClassInfo ServiceDiscoveryWidget::GetTabClassInfo () const
	{
		TabClassInfo sdTab =
		{
			"SD",
			tr ("Service discovery"),
			tr ("A service discovery tab that allows to discover "
				"capabilities of remote entries"),
			QIcon (),
			55,
			TFOpenableByRequest
		};
		return sdTab;
	}
	
	QObject* ServiceDiscoveryWidget::ParentMultiTabs ()
	{
		return S_ParentMultiTabs_;
	}
	
	void ServiceDiscoveryWidget::Remove ()
	{
		emit removeTab (this);
		deleteLater ();
	}
	
	QToolBar* ServiceDiscoveryWidget::GetToolBar () const
	{
		return Toolbar_;
	}
}
}