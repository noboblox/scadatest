QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connectionmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    telegramtablemodel.cpp \
    vrtu/datamodel/apdu.cpp \
    vrtuthread.cpp \
    vrtu/api/service.cpp \
    vrtu/datamodel/server.cpp \
    vrtu/datamodel/id.cpp \
    vrtu/datamodel/model.cpp

HEADERS += \
    uistate.hpp \
    connectionmodel.hpp \
    mainwindow.h \
    vrtu/datamodel/apdu.hpp \
    vrtuthread.hpp \
    telegramtablemodel.h \
    vrtu/api/apimessage.hpp \
    vrtu/api/events.hpp \
    vrtu/api/requests.hpp \
    vrtu/api/response.hpp \
    vrtu/api/service.hpp \
    vrtu/datamodel/server.hpp \
    vrtu/datamodel/enum.hpp \
    vrtu/datamodel/id.hpp \
    vrtu/datamodel/model.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += $$PWD/../lib60870/lib60870-C/b/src/liblib60870.a -lws2_32

INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/inc/api
INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/hal/inc

INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/inc/internal
INCLUDEPATH += $$PWD/../lib60870/lib60870-C/config/
