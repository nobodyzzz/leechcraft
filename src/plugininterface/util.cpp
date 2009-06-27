#include "util.h"
#include <stdexcept>
#include <QString>
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QtDebug>

QTranslator* LeechCraft::Util::InstallTranslator (const QString& baseName,
		const QString& prefix,
		const QString& appName)
{
	QTranslator *transl = new QTranslator;
	QString localeName = QString(::getenv ("LANG"));
	if (localeName.isEmpty () || localeName.size () != 5)
		localeName = QLocale::system ().name ();
	localeName = localeName.left (5);

	QString filename = prefix;
	filename.append ("_");
	if (!baseName.isEmpty ())
		filename.append (baseName).append ("_");
	filename.append (localeName);

#ifdef Q_WS_WIN
	if (transl->load (filename, ":/") ||
			transl->load (filename, ""))
#else
	if (transl->load (filename, ":/") ||
			transl->load (filename,
				QString ("/usr/local/share/%1/translations").arg (appName)) ||
			transl->load (filename,
				QString ("/usr/share/%1/translations").arg (appName)))
#endif
	{
		qApp->installTranslator (transl);
		return transl;
	}

	qWarning () << Q_FUNC_INFO
		<< "could not load translation file for locale"
		<< localeName
		<< filename;
	return 0;
}

QDir LeechCraft::Util::CreateIfNotExists (const QString& path)
{
	QDir home = QDir::home ();
	home.cd (".leechcraft");
	if (home.exists (path) && !home.cd (path))
	{
		throw std::runtime_error (qPrintable (QObject::tr ("Could not cd into %1")
					.arg (QDir::toNativeSeparators (home.filePath (path)))));
	}
	home = QDir::home ();
	home.cd (".leechcraft");
	if (!home.exists (path) && !home.mkpath (path))
	{
		throw std::runtime_error (qPrintable (QObject::tr ("Could not create %1")
					.arg (QDir::toNativeSeparators (home.filePath (path)))));
	}

	home = QDir::home ();
	home.cd (".leechcraft");
	home.cd (path);
	return home;
}

QString LeechCraft::Util::GetTemporaryName (const QString& pattern)
{
	QTemporaryFile file (QDir::tempPath () + "/" + pattern);
	file.open ();
	QString name = file.fileName ();
	file.close ();
	file.remove ();
	return name;
}

LeechCraft::DownloadEntity LeechCraft::Util::MakeEntity (const QByteArray& entity,
		const QString& location,
		LeechCraft::TaskParameters tp,
		const QString& mime)
{
	LeechCraft::DownloadEntity result;
	result.Entity_ = entity;
	result.Location_ = location;
	result.Parameters_ = tp;
	result.Mime_ = mime;
	return result;
}

