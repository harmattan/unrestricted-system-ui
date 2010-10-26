#ifndef VOLUMEBARWINDOW_STUB_H
#define VOLUMEBARWINDOW_STUB_H

#include "volumebarwindow.h"

VolumeBarWindow::VolumeBarWindow (QWidget *)
{
}

VolumeBarWindow::~VolumeBarWindow()
{
}

void VolumeBarWindow::volumeBarChanged (int)
{
}

#ifdef HAVE_QMSYSTEM
void VolumeBarWindow::hwKeyEvent (Maemo::QmKeys::Key, Maemo::QmKeys::State)
{
}

void VolumeBarWindow::locksChanged (Maemo::QmLocks::Lock, Maemo::QmLocks::State)
{
}
#endif

void VolumeBarWindow::hwKeyResourceAcquired ()
{
}

void VolumeBarWindow::hwKeyResourceLost ()
{
}

#endif
