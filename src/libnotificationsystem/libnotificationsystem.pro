MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
DESTDIR = ../../lib

QMAKE_LFLAGS_RPATH = -Wl

TEMPLATE = lib
TARGET = notificationsystem
CONFIG += meegotouch
INCLUDEPATH +=  .
DEPENDPATH += .
QT += dbus

include(../../localconfig.pri)

# For setting the coverage flag ON
contains(COV_OPTION, on) {
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -ftest-coverage \
        -fprofile-arcs
}

system(qdbusxml2cpp notificationsink.xml -a notificationsinkadaptor -c NotificationSinkAdaptor -l NotificationSink -i notificationsink.h -i metatypedeclarations.h)

HEADERS += \
    notificationsink.h \
    notificationsinkadaptor.h \
    notificationparameters.h \
    notificationparameter.h \
    notificationgroup.h \
    notification.h \
    genericnotificationparameterfactory.h \
    notificationwidgetparameterfactory.h \
    notificationmanagerinterface.h \
    metatypedeclarations.h

SOURCES += \
    notificationsink.cpp \
    notificationsinkadaptor.cpp \
    notificationparameters.cpp \
    notificationparameter.cpp \
    notificationgroup.cpp \
    notification.cpp


# Input
headers.path += /usr/include/$$TARGET
headers.files += $$HEADERS
target.path += /usr/lib
INSTALLS += target \
    headers

QMAKE_CXXFLAGS += \
    -Werror \
    -g
QMAKE_CLEAN += *.gcov \
    ./.obj/*.gcno
