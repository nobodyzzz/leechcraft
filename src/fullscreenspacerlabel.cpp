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

#include "fullscreenspacerlabel.h"
#include <QMouseEvent>
#include <QtDebug>
#include "core.h"
#include "xmlsettingsmanager.h"
#include "mainwindow.h"
#include "tabwidget.h"

using namespace LeechCraft;

FullscreenSpacerLabel::FullscreenSpacerLabel (QWidget *parent)
: QLabel (parent)
{
	this->hide ();

	if (!hasMouseTracking ())
		setMouseTracking (true);
}

void FullscreenSpacerLabel::mouseMoveEvent (QMouseEvent *event)
{
	MainWindow *wnd = Core::Instance ().GetReallyMainWindow ();
	if (wnd && (wnd->windowState () == Qt::WindowFullScreen))
	{
		QToolBar *toolbar = wnd->findChild<QToolBar*> ("MainToolbar_");
		if (!toolbar)
		{
			qWarning () << Q_FUNC_INFO
						<< "toolbar is null";
			return;
		}
		LeechCraft::TabWidget *tabwidget = wnd->GetTabWidget ();
		if (!tabwidget)
		{
			qWarning () << Q_FUNC_INFO
						<< "tabwidget is null";
			return;
		}
		QToolBar *bar = Core::Instance ().GetToolBar (tabwidget->currentIndex ());
		if (!bar)
		{
			qWarning () << Q_FUNC_INFO
						<< "bar is null";
			return;
		}

		if (event->y () < 5)
		{
			if (toolbar->isHidden ())
				toolbar->show ();
			if (bar && bar->isHidden ())
				bar->show ();
		}
		else
		{
			if (!toolbar->isHidden ())
				toolbar->hide ();
			if (bar && !bar->isHidden ())
				bar->hide ();
		}
	}
}
