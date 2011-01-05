TEMPLATE      = lib
CONFIG       += plugin gui meegotouch duicontrolpanel

MOC_DIR	      = .moc
OBJECTS_DIR   = .objects

HEADERS       = privatemodeapplet.h \
                privatemodebrief.h

SOURCES       = privatemodeapplet.cpp \
                privatemodebrief.cpp

TARGET        = $$qtLibraryTarget(privatemodeapplet)
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

INSTALLS += target \
            desktop

