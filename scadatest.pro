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
    vrtu/server.cpp \
    vrtu/service.cpp \
    vrtu/id.cpp \
    vrtu/model.cpp \
    vrtuthread.cpp

HEADERS += \
    uistate.hpp \
    vrtu/api/apimessage.hpp \
    vrtu/api/events.hpp \
    vrtu/api/requests.hpp \
    vrtu/api/response.hpp \
    connectionmodel.hpp \
    mainwindow.h \
    telegramtablemodel.h \
    vrtu/server.hpp \
    vrtu/service.hpp \
    vrtu/id.hpp \
    vrtu/model.hpp \
    vrtuthread.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += $$PWD/../lib60870/lib60870-C/b/src/liblib60870.a -lws2_32

INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/inc/api
INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/hal/inc
