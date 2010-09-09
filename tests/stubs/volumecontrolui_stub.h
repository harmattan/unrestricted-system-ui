#ifndef VOLUMECONTORLUI_STUB_H
#define VOLUMECONTORLUI_STUB_H

#include "volumecontrolui.h"

VolumeControlUI::VolumeControlUI (QObject *)
{
}

VolumeControlUI::~VolumeControlUI()
{
}

void VolumeControlUI::overlayChanged (int)
{
}

#ifdef HAVE_QMSYSTEM
void VolumeControlUI::hwKeyEvent (Maemo::QmKeys::Key, Maemo::QmKeys::State)
{
}

void VolumeControlUI::locksChanged (Maemo::QmLocks::Lock, Maemo::QmLocks::State)
{
}
#endif

void VolumeControlUI::hwKeyResourceAcquired ()
{
}

void VolumeControlUI::hwKeyResourceLost ()
{
}

#endif
