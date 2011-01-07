TEMPLATE = subdirs
include(../shared.pri)
addSubDirs(libnotificationsystem)
addSubDirs(systemui, libnotificationsystem )
addSubDirs(extensions, libnotificationsystem )
addSubDirs(privatemodeapplet)
