/**********************************************************************
 * LeechCraft - modular cross-platform feature rich internet client.
 * Copyright (C) 2006-2009  Georg Rudoy
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

#include "pageformsdata.h"
#include <QtDebug>

namespace LeechCraft
{
	namespace Plugins
	{
		namespace Poshuku
		{
			QDataStream& operator<< (QDataStream& out, const ElementData& ed)
			{
				out << static_cast<quint8> (1)
					<< ed.PageURL_
					<< ed.FormID_
					<< ed.Name_
					<< ed.Type_
					<< ed.Value_;
				return out;
			}

			QDataStream& operator>> (QDataStream& in, ElementData& ed)
			{
				quint8 version = 0;
				in >> version;
				if (version == 1)
					in >> ed.PageURL_
						>> ed.FormID_
						>> ed.Name_
						>> ed.Type_
						>> ed.Value_;
				else
					qWarning () << Q_FUNC_INFO
						<< "unable to deserialize ElementType of version"
						<< version;

				return in;
			}

			QDebug& operator<< (QDebug& dbg, const ElementData& ed)
			{
				dbg << "Element: {"
					<< ed.PageURL_
					<< ed.FormID_
					<< ed.Name_
					<< ed.Type_
					<< ed.Value_
					<< "}";
				return dbg;
			}
		};
	};
};

