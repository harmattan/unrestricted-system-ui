# ####################################################################
# Main projectfile
# ####################################################################


TEMPLATE = subdirs
SUBDIRS = src batteryapplet displayapplet networkapplet

QMAKE_CLEAN += configure-stamp build-stamp \

startup_script.path = /etc/X11/Xsession.post
startup_script.files = startup/16sysuid
startup_script.command = $$INSTALL_PROGRAM

INSTALLS += startup_script

