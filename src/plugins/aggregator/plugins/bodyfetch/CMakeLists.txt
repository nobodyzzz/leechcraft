INCLUDE_DIRECTORIES (${AGGREGATOR_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)
SET (BODYFETCH_SRCS
	bodyfetch.cpp
	workerthread.cpp
	workerobject.cpp
	)
SET (BODYFETCH_HEADERS
	bodyfetch.h
	workerthread.h
	workerobject.h
	)
QT4_WRAP_CPP (BODYFETCH_MOC_SRCS ${BODYFETCH_HEADERS})

ADD_LIBRARY (leechcraft_aggregator_bodyfetch SHARED
	${BODYFETCH_COMPILED_TRANSLATIONS}
	${BODYFETCH_SRCS}
	${BODYFETCH_MOC_SRCS}
	)
TARGET_LINK_LIBRARIES (leechcraft_aggregator_bodyfetch
	${QT_LIBRARIES}
	${LEECHCRAFT_LIBRARIES}
	)
INSTALL (TARGETS leechcraft_aggregator_bodyfetch DESTINATION ${LC_PLUGINS_DEST})
INSTALL (DIRECTORY share/scripts DESTINATION share/leechcraft)