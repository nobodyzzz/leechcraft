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

#include "executecommanddialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include "glooxaccount.h"
#include "clientconnection.h"
#include "formbuilder.h"
#include "ui_commandslistpage.h"
#include "ui_commandresultpage.h"

namespace LeechCraft
{
namespace Azoth
{
namespace Xoox
{
	class WaitPage : public QWizardPage
	{
		bool DataFetched_;
	public:
		WaitPage (const QString& text, QWidget *parent = 0)
		: QWizardPage (parent)
		, DataFetched_ (false)
		{
			setTitle (tr ("Fetching data..."));
			setCommitPage (true);
			
			setLayout (new QVBoxLayout ());
			layout ()->addWidget (new QLabel (text));
		}
		
		bool isComplete () const
		{
			return DataFetched_;
		}
		
		void SetDataFetched ()
		{
			DataFetched_ = true;
			emit completeChanged ();
		}
	};
	
	class CommandsListPage : public QWizardPage
	{
		Ui::CommandsListPage Ui_;
		QList<AdHocCommand> Commands_;
	public:
		CommandsListPage (const QList<AdHocCommand>& commands, QWidget *parent = 0)
		: QWizardPage (parent)
		, Commands_ (commands)
		{
			Ui_.setupUi (this);
			setCommitPage (true);

			Q_FOREACH (const AdHocCommand& cmd, commands)
				Ui_.CommandsBox_->addItem (cmd.GetName ());
		}
		
		AdHocCommand GetSelectedCommand () const
		{
			const int idx = Ui_.CommandsBox_->currentIndex ();
			return Commands_.at (idx);
		}
	};
	
	class CommandResultPage : public QWizardPage
	{
		Ui::CommandResultPage Ui_;
		
		AdHocResult Result_;
		mutable FormBuilder FB_;
	public:
		CommandResultPage (const AdHocResult& result, QWidget *parent = 0)
		: QWizardPage (parent)
		, Result_ (result)
		{
			Ui_.setupUi (this);
			setCommitPage (true);
			
			Ui_.Actions_->addItems (result.GetActions ());
			
			const QXmppDataForm& form = result.GetDataForm ();
			if (!form.isNull ())
				Ui_.FormArea_->setWidget (FB_.CreateForm (form));
		}
		
		QString GetSelectedAction () const
		{
			return Ui_.Actions_->currentText ();
		}
		
		QXmppDataForm GetForm () const
		{
			return FB_.GetForm ();
		}
		
		AdHocResult GetResult () const
		{
			return Result_;
		}
	};

	ExecuteCommandDialog::ExecuteCommandDialog (const QString& jid,
			GlooxAccount *account, QWidget *parent)
	: QWizard (parent)
	, Account_ (account)
	, Manager_ (account->GetClientConnection ()->GetAdHocCommandManager ())
	, JID_ (jid)
	{
		Ui_.setupUi (this);
		
		connect (this,
				SIGNAL (currentIdChanged (int)),
				this,
				SLOT (handleCurrentChanged (int)));
		
		RequestCommands ();
	}
	
	void ExecuteCommandDialog::RequestCommands ()
	{
		const int idx = addPage (new WaitPage (tr ("Please wait while "
				"the list of commands is fetched.")));
		if (currentId () != idx)
			next ();

		connect (Manager_,
				SIGNAL (gotCommands (QString, QList<AdHocCommand>)),
				this,
				SLOT (handleGotCommands (QString, QList<AdHocCommand>)),
				Qt::UniqueConnection);
		Manager_->QueryCommands (JID_);
	}
	
	void ExecuteCommandDialog::ExecuteCommand (const AdHocCommand& command)
	{
		connect (Manager_,
				SIGNAL (gotResult (QString, AdHocResult)),
				this,
				SLOT (handleGotResult (QString, AdHocResult)),
				Qt::UniqueConnection);
		Manager_->ExecuteCommand (JID_, command);
	}
	
	void ExecuteCommandDialog::ProceedExecuting (const AdHocResult& result, const QString& action)
	{
		connect (Manager_,
				SIGNAL (gotResult (QString, AdHocResult)),
				this,
				SLOT (handleGotResult (QString, AdHocResult)),
				Qt::UniqueConnection);
		Manager_->ProceedExecuting (JID_, result, action);
	}

	void ExecuteCommandDialog::handleCurrentChanged (int id)
	{
		if (!dynamic_cast<WaitPage*> (currentPage ()))
			return;

		const QList<int>& ids = pageIds ();

		const int pos = ids.indexOf (id);
		if (pos <= 0)
			return;

		QWizardPage *prevPage = page (ids.at (pos - 1));
		if (dynamic_cast<CommandsListPage*> (prevPage))
			ExecuteCommand (dynamic_cast<CommandsListPage*> (prevPage)->GetSelectedCommand ());
		else if (dynamic_cast<CommandResultPage*> (prevPage))
		{
			CommandResultPage *crp = dynamic_cast<CommandResultPage*> (prevPage);
			const QString& action = crp->GetSelectedAction ();
			if (action.isEmpty ())
				return;
		
			AdHocResult result = crp->GetResult ();
			result.SetDataForm (crp->GetForm ());
			ProceedExecuting (result, action);
		}
	}
	
	void ExecuteCommandDialog::handleGotCommands (const QString& jid, const QList<AdHocCommand>& commands)
	{
		if (jid != JID_)
			return;

		disconnect (Manager_,
				SIGNAL (gotCommands (QString, QList<AdHocCommand>)),
				this,
				SLOT (handleGotCommands (QString, QList<AdHocCommand>)));

		addPage (new CommandsListPage (commands));
		addPage (new WaitPage (tr ("Please wait while command result "
					"is fetched.")));
		next ();
	}
	
	void ExecuteCommandDialog::handleGotResult (const QString& jid, const AdHocResult& result)
	{
		if (jid != JID_)
			return;

		disconnect (Manager_,
				SIGNAL (gotResult (QString, AdHocResult&)),
				this,
				SLOT (handleGotResult (QString, AdHocResult)));
		
		addPage (new CommandResultPage (result));
		if (!result.GetActions ().isEmpty ())
			addPage (new WaitPage (tr ("Please wait while action "
						"is performed")));
		next ();
	}
}
}
}
