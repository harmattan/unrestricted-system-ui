TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    silent \
    debug
LIBS += -lduicontrolpanel
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
HEADERS = profileapplet.h \
    profilewidget.h \
    profilebrief.h \
    profiledbusinterface.h \
    profilecontainer.h \
    profiletranslation.h
SOURCES = profileapplet.cpp \
    profilewidget.cpp \
    profilebrief.cpp \
    profiledbusinterface.cpp \
    profilecontainer.cpp
css.files = profileapplet.css
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
