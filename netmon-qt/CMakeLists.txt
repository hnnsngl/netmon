## for AUTOMOC we need at least
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.6)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

SET(netmon-qt_SOURCES netmon-qt.cpp
                      ../src/netgroup.cpp
                      ../src/netmon-hosts.cpp
                      ../src/netmon-lib.cpp
                      netmon-qt-window.cpp
		      netmon-updater.cpp
		      hostlistmodel.cpp hostlistproxy.cpp
		      processlistmodel.cpp processlistproxy.cpp)

SET(netmon-qt_HEADERS netmon-qt-window.cpp
                      netmon-updater.hpp
                      hostlistmodel.hpp hostlistproxy.hpp
		      processlistmodel.hpp processlistproxy.hpp)

SET(netmon-qt_RESOURCES images.qrc)

SET(LANG translation_en.ts translation_de.ts)

QT4_ADD_RESOURCES(netmon-qt_RESOURCES_RCC ${netmon-qt_RESOURCES})
QT4_ADD_TRANSLATION(TRANSLATIONS ${LANG})

ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(netmon-qt
               ${netmon-qt_SOURCES}
	       ${netmon-qt_RESOURCES_RCC}
	       ${TRANSLATIONS})

SET_TARGET_PROPERTIES(netmon-qt PROPERTIES AUTOMOC True)
TARGET_LINK_LIBRARIES(netmon-qt ${QT_LIBRARIES} ${CORELIBS})
