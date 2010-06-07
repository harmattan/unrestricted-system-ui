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
class MImageWidget;

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


    void showWindow (
		    const QString  &text1, 
		    const QString  &text2, 
		    int             timeout);

private slots:
    void showLogo ();
    void turnOffScreen ();
    void realize ();

private:
    bool             m_Realized;
    MSceneWindow    *m_SceneWindow;
    QTimer          *m_Timer;
    MLabel          *m_Label1;
    MLabel          *m_Label2;
    MImageWidget    *m_Image;
    MFeedback       *m_Feedback;
#ifdef UNIT_TEST
    friend class Ft_ShutdownUI;
    friend class Ut_ShutdownUI;
#endif
};

#endif
