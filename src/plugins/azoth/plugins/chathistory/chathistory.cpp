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

#include "chathistory.h"
#include <QDir>
#include <QIcon>
#include <QAction>
#include <QTranslator>
#include <util/util.h>
#include <interfaces/imessage.h>
#include <interfaces/iclentry.h>
#include <interfaces/azothcommon.h>
#include "core.h"
#include "chathistorywidget.h"

namespace LeechCraft
{
namespace Azoth
{
namespace ChatHistory
{
	void Plugin::Init (ICoreProxy_ptr)
	{
		Translator_.reset (Util::InstallTranslator ("azoth_chathistory"));

		ChatHistoryWidget::SetParentMultiTabs (this);

		Guard_.reset (new STGuard<Core> ());
		ActionHistory_ = new QAction (tr ("IM history"), this);
		connect (ActionHistory_,
				SIGNAL (triggered ()),
				this,
				SLOT (handleHistoryRequested ()));
	}

	void Plugin::SecondInit ()
	{
	}

	QByteArray Plugin::GetUniqueID () const
	{
		return "org.LeechCraft.Azoth.ChatHistory";
	}

	void Plugin::Release ()
	{
		Guard_.reset ();
	}

	QString Plugin::GetName () const
	{
		return "Azoth ChatHistory";
	}

	QString Plugin::GetInfo () const
	{
		return tr ("Stores message history in Azoth.");
	}

	QIcon Plugin::GetIcon () const
	{
		return QIcon ();
	}

	QSet<QByteArray> Plugin::GetPluginClasses () const
	{
		QSet<QByteArray> result;
		result << "org.LeechCraft.Plugins.Azoth.Plugins.IGeneralPlugin";
		return result;
	}
	
	QList<QAction*> Plugin::GetActions (ActionsEmbedPlace) const
	{
		return QList<QAction*> ();
	}
	
	QMap<QString, QList<QAction*> > Plugin::GetMenuActions () const
	{
		QMap<QString, QList<QAction*> > result;
		result ["Azoth"] << ActionHistory_;
		return result;
	}
	
	TabClasses_t Plugin::GetTabClasses () const
	{
		TabClasses_t result;
		result << Core::Instance ()->GetTabClass ();
		return result;
	}
	
	void Plugin::TabOpenRequested (const QByteArray& tabClass)
	{
		if (tabClass == "Chathistory")
			handleHistoryRequested ();
		else
			qWarning () << Q_FUNC_INFO
					<< "unknown tab class"
					<< tabClass;
	}
	
	void Plugin::initPlugin (QObject *proxy)
	{
		Core::Instance ()->SetPluginProxy (proxy);
	}
	
	void Plugin::hookEntryActionAreasRequested (IHookProxy_ptr proxy,
			QObject *action, QObject*)
	{
		if (!action->property ("Azoth/ChatHistory/IsGood").toBool ())
			return;
		
		QStringList ours;
		ours << "contactListContextMenu"
			<< "tabContextMenu";
		proxy->SetReturnValue (proxy->GetReturnValue ().toStringList () + ours);
	}
	
	void Plugin::hookEntryActionsRequested (IHookProxy_ptr proxy, QObject *entry)
	{
		if (!Entry2ActionHistory_.contains (entry))
		{
			connect (entry,
					SIGNAL (destroyed ()),
					this,
					SLOT (handleEntryDestroyed ()),
					Qt::UniqueConnection);

			QAction *action = new QAction (tr ("History..."), entry);
			action->setProperty ("Azoth/ChatHistory/IsGood", true);
			action->setProperty ("Azoth/ChatHistory/Entry",
					QVariant::fromValue<QObject*> (entry));
			connect (action,
					SIGNAL (triggered ()),
					this,
					SLOT (handleEntryHistoryRequested ()));
			Entry2ActionHistory_ [entry] = action;
		}
		if (!Entry2ActionEnableHistory_.contains (entry))
		{
			connect (entry,
					SIGNAL (destroyed ()),
					this,
					SLOT (handleEntryDestroyed ()),
					Qt::UniqueConnection);

			QAction *action = new QAction (tr ("Logging enabled"), entry);
			action->setCheckable (true);
			action->setChecked (Core::Instance ()->IsLoggingEnabled (entry));
			action->setProperty ("Azoth/ChatHistory/IsGood", true);
			action->setProperty ("Azoth/ChatHistory/Entry",
					QVariant::fromValue<QObject*> (entry));
			connect (action,
					SIGNAL (toggled (bool)),
					this,
					SLOT (handleEntryEnableHistoryRequested (bool)));
			Entry2ActionEnableHistory_ [entry] = action;
		}

		QList<QVariant> list = proxy->GetReturnValue ().toList ();
		list << QVariant::fromValue<QObject*> (Entry2ActionHistory_ [entry]);
		list << QVariant::fromValue<QObject*> (Entry2ActionEnableHistory_ [entry]);
		proxy->SetReturnValue (list);
	}

	void Plugin::hookGotMessage (LeechCraft::IHookProxy_ptr,
				QObject *message)
	{
		IMessage *msg = qobject_cast<IMessage*> (message);
		if (!msg)
		{
			qWarning () << Q_FUNC_INFO
					<< message
					<< "doesn't implement IMessage"
					<< sender ();
			return;
		}
		Core::Instance ()->Process (message);
	}
	
	void Plugin::handleHistoryRequested ()
	{
		ChatHistoryWidget *wh = new ChatHistoryWidget;
		connect (wh,
				SIGNAL (removeSelf (QWidget*)),
				this,
				SIGNAL (removeTab (QWidget*)));
		emit addNewTab (tr ("Chat history"), wh);
	}
	
	void Plugin::handleEntryHistoryRequested ()
	{
		if (!sender ())
		{
			qWarning () << Q_FUNC_INFO
					<< "null sender()";
			return;
		}

		QObject *obj = sender ()->property ("Azoth/ChatHistory/Entry")
				.value<QObject*> ();
		if (!obj)
		{
			qWarning () << Q_FUNC_INFO
					<< "null object for sender"
					<< sender ();
			return;
		}
		
		ICLEntry *entry = qobject_cast<ICLEntry*> (obj);
		if (!entry)
		{
			qWarning () << Q_FUNC_INFO
					<< "null entry for sender"
					<< sender ()
					<< "and object"
					<< obj;
			return;
		}

		ChatHistoryWidget *wh = new ChatHistoryWidget (entry);
		connect (wh,
				SIGNAL (removeSelf (QWidget*)),
				this,
				SIGNAL (removeTab (QWidget*)));
		emit addNewTab (tr ("Chat history"), wh);
		emit raiseTab (wh);
	}
	
	void Plugin::handleEntryEnableHistoryRequested (bool enable)
	{
		if (!sender ())
		{
			qWarning () << Q_FUNC_INFO
					<< "null sender()";
			return;
		}

		QObject *obj = sender ()->property ("Azoth/ChatHistory/Entry")
				.value<QObject*> ();
		if (!obj)
		{
			qWarning () << Q_FUNC_INFO
					<< "null object for sender"
					<< sender ();
			return;
		}
		
		Core::Instance ()->SetLoggingEnabled (obj, enable);
	}
	
	void Plugin::handleEntryDestroyed ()
	{
		Entry2ActionHistory_.remove (sender ());
	}
}
}
}

Q_EXPORT_PLUGIN2 (leechcraft_azoth_chathistory, LeechCraft::Azoth::ChatHistory::Plugin);
