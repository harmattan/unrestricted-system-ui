MOC_DIR = .moc
M_MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
DESTDIR = ../../lib

TEMPLATE = lib
TARGET = notificationsystem
CONFIG += meegotouch \
    no_check_exist
INCLUDEPATH +=  . \
DEPENDPATH += .

include(../../localconfig.pri)

# For setting the coverage flag ON
contains(COV_OPTION, on) {
    LIBS += -lgcov
    QMAKE_CXXFLAGS += -ftest-coverage \
        -fprofile-arcs
}

HEADERS += \
    notificationsource.h \
    notificationsink.h \
    notificationparameters.h \
    notificationparameter.h \
    notificationgroup.h \
    notification.h \
    genericnotificationparameterfactory.h \
    notificationwidgetparameterfactory.h \
    notificationmanagerinterface.h

SOURCES += \
    notificationsource.cpp \
    notificationsink.cpp \
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
