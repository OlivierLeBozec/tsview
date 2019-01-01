#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T21:27:33
#
#-------------------------------------------------

QT += core gui charts widgets

TARGET = viewTS
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    chart.cpp \
    chartview.cpp \
    workers.cpp \
    ../src/timestamp.cpp \
    ../src/pes.cpp \
    ../src/packet.cpp \
    ../src/pidmap.cpp

HEADERS  += mainwindow.h \
    chart.h \
    chartview.h \
    workers.h \
    ../src/timestamp.h \
    ../src/pes.h \
    ../src/packet.h \
    ../src/pidmap.h


RC_FILE = viewTS.rc

win32 {
  DESTDIR = $$PWD/../bin/windows/
  INCLUDEPATH += $$PWD/../bin/windows
  DEPENDPATH += $$PWD/../bin/windows
  CONFIG  += static
}

linux {
  DESTDIR = $$PWD/../bin/linux/
  INCLUDEPATH += $$PWD/../bin/linux
  DEPENDPATH += $$PWD/../bin/linux
}

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/windows/ -ltstoolcheck -ltstoolmap
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/windows/ -ltstoolcheck -ltstoolmap

#linux:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/linux/ -ltstoolcheck -ltstoolmap
#else:linux:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/linux/ -ltstoolcheck -ltstoolmap


#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/windows/libtstoolcheck.a $$PWD/../bin/windows/libtstoolmap.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/windows/libtstoolcheck.a $$PWD/../bin/windows/libtstoolmap.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/windows/tstoolcheck.lib $$PWD/../bin/windows/tstoolmap.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/windows/tstoolcheck.lib $$PWD/../bin/windows/tstoolmap.lib

#linux-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/linux/libtstoolcheck.a $$PWD/../bin/linux/libtstoolmap.a
#else:linux-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/linux/libtstoolcheck.a $$PWD/../bin/linux/libtstoolmap.a
#else:linux:!linux-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/linux/tstoolcheck.so $$PWD/../bin/linux/tstoolmap.so
#else:linux:!linux-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/linux/tstoolcheck.so $$PWD/../bin/linux/tstoolmap.so
