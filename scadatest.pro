QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connectionmodel.cpp \
    decodedmessage.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp \
    telegramtablemodel.cpp

HEADERS += \
    connectionevent.h \
    connectionmodel.hpp \
    decodedmessage.hpp \
    mainwindow.h \
    server.h \
    telegramevent.h \
    telegramtablemodel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += $$PWD/../lib60870/lib60870-C/b/src/liblib60870.a -lws2_32

INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/inc/api
INCLUDEPATH += $$PWD/../lib60870/lib60870-C/src/hal/inc
