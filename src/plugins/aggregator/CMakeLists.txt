IF (NOT QT_USE_FILE)
	CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
	IF (COMMAND cmake_policy)
		cmake_policy (SET CMP0003 NEW)
	ENDIF (COMMAND cmake_policy)

	PROJECT (leechcraft_aggregator)

	IF (NOT CMAKE_MODULE_PATH)
		SET (CMAKE_MODULE_PATH "/usr/local/share/leechcraft/cmake;/usr/share/leechcraft/cmake")
	ENDIF (NOT CMAKE_MODULE_PATH)

	FIND_PACKAGE (Boost REQUIRED COMPONENTS date_time filesystem system thread signals)
	FIND_PACKAGE (Qt4 REQUIRED)
	FIND_PACKAGE (LeechCraft REQUIRED)
ENDIF (NOT QT_USE_FILE)

OPTION (ENABLE_AGGREGATOR_BODYFETCH "Enable BodyFetch for fetching full bodies of news items" ON)

SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTNETWORK TRUE)

IF (ENABLE_AGGREGATOR_BODYFETCH)
	SET (QT_USE_QTWEBKIT TRUE)
ENDIF (ENABLE_AGGREGATOR_BODYFETCH)

INCLUDE (${QT_USE_FILE})
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${LEECHCRAFT_INCLUDE_DIR}
	)

SET (SRCS
    aggregator.cpp
    core.cpp
    addfeed.cpp
    parserfactory.cpp
	rssparser.cpp
    rss20parser.cpp
	rss10parser.cpp
	rss091parser.cpp
	atomparser.cpp
    atom10parser.cpp
    atom03parser.cpp
    parser.cpp
    item.cpp
    channel.cpp
    feed.cpp
    xmlsettingsmanager.cpp
    uistatepersist.cpp
    channelsmodel.cpp
    channelsfiltermodel.cpp
	regexpmatcherui.cpp
	singleregexp.cpp
	regexpmatchermanager.cpp
	itemsfiltermodel.cpp
	opmlparser.cpp
	opmlwriter.cpp
	importopml.cpp
	export.cpp
	sqlstoragebackend.cpp
	storagebackend.cpp
	importbinary.cpp
	feedsettings.cpp
	jobholderrepresentation.cpp
	itemswidget.cpp
	itemslistmodel.cpp
	wizardgenerator.cpp
	startupfirstpage.cpp
	startupsecondpage.cpp
	startupthirdpage.cpp
	export2fb2dialog.cpp
	actionsstructs.cpp
	sqlstoragebackend_mysql.cpp
	pluginmanager.cpp
	)
SET (HEADERS
    aggregator.h
    core.h
    addfeed.h
    item.h
    channel.h
    feed.h
    parserfactory.h
    parser.h
	rssparser.h
    rss20parser.h
	rss10parser.h
	rss091parser.h
	atomparser.h
    atom10parser.h
    atom03parser.h
    channelsmodel.h
    xmlsettingsmanager.h
    uistatepersist.h
    channelsfiltermodel.h
	regexpmatcherui.h
	singleregexp.h
	regexpmatchermanager.h
	itemsfiltermodel.h
	opmlparser.h
	opmlwriter.h
	importopml.h
	export.h
	sqlstoragebackend.h
	storagebackend.h
	opmlitem.h
	importbinary.h
	feedsettings.h
	jobholderrepresentation.h
	itemswidget.h
	itemslistmodel.h
	wizardgenerator.h
	startupfirstpage.h
	startupsecondpage.h
	startupthirdpage.h
	export2fb2dialog.h
	actionsstructs.h
	sqlstoragebackend_mysql.h
	pluginmanager.h
	)
SET (FORMS
    mainwidget.ui
    addfeed.ui
	itembucket.ui
	regexpmatcherui.ui
	singleregexp.ui
	importopml.ui
	export.ui
	importbinary.ui
	feedsettings.ui
	itemswidget.ui
	startupfirstpage.ui
	startupsecondpage.ui
	startupthirdpage.ui
	export2fb2dialog.ui
	)
SET (RESOURCES aggregatorresources.qrc)
CreateTrVars ("aggregator" "ar;de;en;eo;es;fr;it;oc;pl;ru_RU;uk_UA" TRANSLATIONS COMPILED_TRANSLATIONS)

ADD_DEFINITIONS(-DBOOST_DYN_LINK)
QT4_WRAP_CPP (MOC_SRCS ${HEADERS})
QT4_WRAP_UI (UIS_H ${FORMS})
QT4_ADD_RESOURCES (RCCS ${RESOURCES})

ADD_CUSTOM_COMMAND (OUTPUT
	${COMPILED_TRANSLATIONS}
	COMMAND "${QT_LRELEASE_EXECUTABLE}" ${TRANSLATIONS}
	DEPENDS ${TRANSLATIONS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
ADD_LIBRARY (leechcraft_aggregator SHARED
	${COMPILED_TRANSLATIONS}
	${SRCS}
	${MOC_SRCS}
	${UIS_H}
	${RCCS}
	)
LINK_DIRECTORIES (${Boost_LIBRARY_DIR})
TARGET_LINK_LIBRARIES (leechcraft_aggregator
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_aggregator DESTINATION ${LC_PLUGINS_DEST})
INSTALL (FILES ${COMPILED_TRANSLATIONS} DESTINATION ${LC_TRANSLATIONS_DEST})
INSTALL (FILES aggregatorsettings.xml DESTINATION ${LC_SETTINGS_DEST})

SET (AGGREGATOR_INCLUDE_DIR ${CURRENT_SOURCE_DIR})

IF (ENABLE_AGGREGATOR_BODYFETCH)
	ADD_SUBDIRECTORY (plugins/bodyfetch)
ENDIF (ENABLE_AGGREGATOR_BODYFETCH)
