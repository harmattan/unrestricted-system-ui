TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    cellular-qt \
    silent \
    debug
LIBS += -lduicontrolpanel \
    -lcellular-qt
INCLUDEPATH += /usr/include/qt4/dui \
    /usr/include/duicontrolpanel
QT += dbus
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = callandsimapplet.h \
    callandsimwidget.h \
    dcpcallandsim.h \
    callcontainer.h \
    simcontainer.h \
    callandsimtranslation.h \
    callandsimbrief.h \
    callandsimdbusinterface.h \
    forwardingwidget.h \
    forwardingcontainer.h
SOURCES = callandsimapplet.cpp \
    callandsimwidget.cpp \
    callcontainer.cpp \
    simcontainer.cpp \
    callandsimbrief.cpp \
    callandsimdbusinterface.cpp \
    forwardingwidget.cpp \
    forwardingcontainer.cpp
css.files = callandsimapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/duicontrolpanel/themes/style
message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
    css \
    desktop
