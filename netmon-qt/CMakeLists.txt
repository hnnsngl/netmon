FIND_PACKAGE(Qt4 REQUIRED)

SET(netmon-qt_SOURCES netmon-qt.cpp
                      netmon-qt-window.cpp netmon-qt-models.cpp)
SET(netmon-qt_HEADERS netmon-qt-window.cpp netmon-qt-models.hpp)

## QT4_WRAP_CPP(netmon-qt_HEADERS_MOC ${netmon-qt_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(netmon-qt ${netmon-qt_SOURCES} ${netmon-qt_HEADERS_MOC})
SET_TARGET_PROPERTIES(netmon-qt PROPERTIES AUTOMOC True)
TARGET_LINK_LIBRARIES(netmon-qt ${QT_LIBRARIES})
