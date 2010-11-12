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

#include <MApplicationPage>

class QTimer;
class MLabel;
class MFeedback;
class MStylableWidget;
class QGraphicsLinearLayout;

/*!
 * A full screen window that is shown when the device is shutting down. The
 * window first shows a two line text, then---after a time period---it shows a
 * logo image. After the second time period the touch screen is going to be
 * turned off.
 */
class ShutdownUI : public MWindow
{
    Q_OBJECT

public:
    ShutdownUI ();
    virtual ~ShutdownUI ();

    /*!
     * Shows a full screen window with two lines of text, then waits for the
     * specified amount of time and will hide the text lines and show an image.
     * After the image image shown for some time will dim the screen.
     *
     * \param text1 The text of the primary message line
     * \param text2 The text of the secondary message line
     * \param timeout After this timeout we will hide the texts and show the logo
     */
    void showWindow(const QString &text1, const QString &text2, int timeout);

protected:
    //! \reimp
    void showEvent (QShowEvent *event);
    //! \reimp_end

private slots:
    /*!
     * Hides the labels, shows the logo image and starts up a timer to turn off the
     * screen.
     */
    void showLogo ();

    /*!
     * We need to turn off the screen so that the user will not see the actual
     * shutdown on the GUI. We could show a black screen but we might be prematurely
     * killed, so this window will be removed early. Turning off the screen is an
     * excellent way to solve this issue... except that we might be killed even
     * sooner.
     */
    void turnOffScreen ();

    /*!
     * Here we create the widgets that we use, and we put them into the layout that
     * we use. The logo will not be shown yet.
     */
    void realize ();

private:
    bool realized;
    MSceneWindow *sceneWindow;
    QTimer *timer;
    MLabel *label1;
    MLabel *label2;
    MStylableWidget *logo;
    QGraphicsLinearLayout *layout;
    MFeedback *feedback;

#ifdef UNIT_TEST
    friend class Ft_ShutdownUI;
    friend class Ut_ShutdownUI;
#endif
};

#endif
