FIND_PACKAGE(Qt4 REQUIRED)

SET(netmon-qt_SOURCES netmon-qt.cpp ../src/netmon-hosts.cpp
                      netmon-qt-window.cpp hostlistmodel.cpp processlistmodel.cpp)
SET(netmon-qt_HEADERS netmon-qt-window.cpp hostlistmodel.hpp processlistmodel.hpp)

SET(netmon-qt_RESOURCES images.qrc)

SET(LANG translation_en.ts translation_de.ts)
## obsoleted by AUTOMOC
## QT4_WRAP_CPP(netmon-qt_HEADERS_MOC ${netmon-qt_HEADERS})

QT4_ADD_RESOURCES(netmon-qt_RESOURCES_RCC ${netmon-qt_RESOURCES})
QT4_ADD_TRANSLATION(TRANSLATIONS ${LANG})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(netmon-qt
               ${netmon-qt_SOURCES}
	       ${netmon-qt_HEADERS_MOC}
	       ${netmon-qt_RESOURCES_RCC}
	       ${TRANSLATIONS})

SET_TARGET_PROPERTIES(netmon-qt PROPERTIES AUTOMOC True)
TARGET_LINK_LIBRARIES(netmon-qt ${QT_LIBRARIES})
