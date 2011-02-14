/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (directui@nokia.com)
 **
 ** This file is part of systemui.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at directui@nokia.com.
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License version 2.1 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.LGPL included in the packaging
 ** of this file.
 **
 ****************************************************************************/
#ifndef VOLUMEBARWINDOW_H
#define VOLUMEBARWINDOW_H

#include <MWindow>

class VolumeBarLogic;
class VolumeBar;

/*!
 * The VolumeBarWindow allows to display a volume bar based on current volume
 * on hardware volume key presses and lets user to modify the volume by touching
 * the full screen VolumeBar widget.
 */
class VolumeBarWindow : public MWindow {
    Q_OBJECT

public:
    //! Constructs a new VolumeBarWindow instance
    VolumeBarWindow(VolumeBarLogic *logic, QWidget *parent = 0);

    //! Destructs a VolumeBarWindow instance
    ~VolumeBarWindow();

    //! Updates the volume representation
    void updateVolume();

private slots:
    /*!
     * Sets the volume to the logic
     *
     * \param percentage the volume percentage
     */
    void setVolume(qreal percentage);

private:
    Q_DISABLE_COPY(VolumeBarWindow)

    //! The volume logic
    VolumeBarLogic *logic;

    //! The VolumeBar widget
    VolumeBar *volumeBar;

#ifdef UNIT_TEST
    friend class Ut_VolumeBarWindow;
#endif
};

#endif
