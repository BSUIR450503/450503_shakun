#-------------------------------------------------
#
# Project created by QtCreator 2015-06-26T19:35:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTP_Client
TEMPLATE = app

win32:LIBS += -lws2_32
INCLUDEPATH += $$PWD

LIBS += "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\User32.lib"
LIBS += "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\Mswsock.lib"

SOURCES += main.cpp\
        mainwindow.cpp\
        form.cpp \
    client.cpp \
    dialogdeletefile.cpp \
    dialoggotodir.cpp \
    dialogdonwloadfile.cpp \
    dialoguploadfile.cpp

HEADERS  += mainwindow.h \
    form.h \
    client.h \
    dialogdeletefile.h \
    dialoggotodir.h \
    dialogdonwloadfile.h \
    dialoguploadfile.h

FORMS    += mainwindow.ui\
    dialog.ui \
    form.ui \
    dialogdeletefile.ui \
    dialoggotodir.ui \
    dialogdonwloadfile.ui \
    dialoguploadfile.ui
RESOURCES += ftp.qrc
OTHER_FILES += images/star.png \
    images/question.png \
    images/outbox.png \
    images/inbox.png \
    images/gear.png \
    images/button-synchronize.png \
    images/button-shuffle.png \
    images/button-play.png \
    images/button-pause.png \
    images/button-cross.png \
    images/web.png \
    images/folder.png

