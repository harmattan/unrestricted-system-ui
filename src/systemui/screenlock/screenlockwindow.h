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
#ifndef SCREENLOCKWINDOW_H
#define SCREENLOCKWINDOW_H

#include <MWindow>
#include <X11/Xdefs.h>

class MApplicationExtensionArea;

/*!
 * Screen lock window is a window for the screen lock.
 */
class ScreenLockWindow : public MWindow
{
    Q_OBJECT

public:
    /*!
     * Creates a screen lock window.
     *
     * \param extensionArea the application extension area containing the lock screen extension.
                            The window takes ownership of the extension area.
     * \param parent the parent widget for the window
     */
    ScreenLockWindow(MApplicationExtensionArea *extensionArea, QWidget *parent = NULL);

    /*!
     * Destroys the screen lock window.
     */
    virtual ~ScreenLockWindow();

    /*!
     * Sets the low power mode property of the window to enable or isable compositor
     * painting even in low power mode state
     * \param enable if true, enables the low power state
     */
    void setLowPowerMode(bool enable);

protected:
    /*!
     * Sets the _MEEGO_STACKING_LAYER window property to 5. For incoming call feature
     * this needs to be at least higher than call-ui's value (currently 2).
     */
    virtual void showEvent(QShowEvent *event);

    //! \reimp
    virtual void paintEvent(QPaintEvent *event);
    //! \reimp_end

private:
    //! Sets the _MEEGO_LOW_POWER_MODE window property if needed to make compositor paint the window even when the display state is off, so the low power mode view can update itself
    void setLowPowerModeProperty();

    //! Set the _MEEGO_STACKING_LAYER window property based on the current mode
    void setStackingLayerProperty();

    //! Set the _MEEGOTOUCH_ORIENTATION_ANGLE window property based on the current orientation (since it doesn't seem to get set properly for translucent windows, see bug #230352)
    void setOrientationAngleProperty();

    //! Set the _NET_WM_STATE window property to _NET_WM_STATE_SKIP_TASKBAR window to not show the window in the switcher
    void setSkipTaskbarProperty();

    void changeNetWmState(bool set, Atom one, Atom two = 0);

    //! Applies the lock screen orientation and locking from the style
    void applyStyle();

    //! Low power mode of the window
    bool lowPowerMode;

#ifdef UNIT_TEST
    friend class Ut_ScreenLockWindow;
#endif
};

#endif
