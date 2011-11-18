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
#ifndef SHUTDOWNUI_H
#define SHUTDOWNUI_H

#include <MWindow>

class MSceneWindow;
class MLabel;
class MFeedback;
class MImageWidget;
class QGraphicsLinearLayout;
class QTimer;

/*!
 * A full screen window that is shown when the device is shutting down. The
 * window shows two lines of text if defined and then a logo after the defined
 * time period. After the second time period the touch screen is turned off.
 */
class ShutdownUI : public MWindow
{
    Q_OBJECT

public:
    /*!
     * Creates a shutdown window.
     */
    ShutdownUI();

    /*!
     * Destroys the shutdown window.
     */
    virtual ~ShutdownUI();

    /*!
     * Shows the shutdown window. If text1 and text2 are given, these texts
     * are shown for the specified amount of time. After the texts have been
     * visible for the given period of time (or immediately if there are no
     * texts to be shown), a logo image is shown.
     *
     * \param text1 The text of the primary message line
     * \param text2 The text of the secondary message line
     * \param timeout the number of milliseconds to show the texts for
     */
    void showWindow(const QString &text1, const QString &text2, int timeout);

protected:
    //! \reimp
    void showEvent(QShowEvent *event);
    //! \reimp_end

private slots:
    //! Hides the labels, shows the logo image and starts up a timer to turn off the screen
    void showLogo();

    //! Switches the display off and fills the window with black if that failed
    void turnOffScreen();

    //! Creates the widgets and the layout for the shutdown screen
    void realize();

private:
    //! Applies the window orientation and locking from the style
    void applyStyle();

    bool realized;
    MSceneWindow *sceneWindow;
    QTimer *timer;
    MLabel *label1;
    MLabel *label2;
    MImageWidget *logo;
    QGraphicsLinearLayout *layout;
    MFeedback *feedback;

#ifdef UNIT_TEST
    friend class Ft_ShutdownUI;
    friend class Ut_ShutdownUI;
#endif
};

#endif
