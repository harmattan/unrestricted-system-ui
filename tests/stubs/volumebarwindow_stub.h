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
void VolumeBarWindow::hwKeyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)
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
