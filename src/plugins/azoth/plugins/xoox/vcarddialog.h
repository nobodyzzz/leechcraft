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

#ifndef VCARDDIALOG_H
#define VCARDDIALOG_H
#include <QDialog>
#include <QXmppAnnotationsIq.h>
#include "ui_vcarddialog.h"

class QXmppVCardIq;

namespace LeechCraft
{
namespace Azoth
{
namespace Xoox
{
	class EntryBase;
	class GlooxAccount;

	class VCardDialog : public QDialog
	{
		Q_OBJECT

		Ui::VCardDialog Ui_;
		GlooxAccount *Account_;
		QString JID_;
		QXmppAnnotationsIq::NoteItem Note_;
	public:
		VCardDialog (QWidget* = 0);
		VCardDialog (EntryBase*, QWidget* = 0);

		void UpdateInfo (const QXmppVCardIq&);
	private slots:
		void setNote ();
	private:
		void UpdateNote (GlooxAccount*, const QString&);
	};
}
}
}

#endif
