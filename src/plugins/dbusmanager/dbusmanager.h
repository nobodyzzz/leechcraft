#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H
#include <interfaces/iinfo.h>

class DBusManager : public QObject
				  , public IInfo
{
	Q_OBJECT

	Q_INTERFACES (IInfo);
public:
	void Init (ICoreProxy_ptr);
	void Release ();
	QString GetName () const;
	QString GetInfo () const;
	QStringList Provides () const;
	QStringList Uses () const;
	QStringList Needs () const;
	void SetProvider (QObject*, const QString&);
	QIcon GetIcon () const;
};

#endif

