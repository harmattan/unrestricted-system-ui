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
/*
 * profile.h
 *
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#ifndef PROFILE_H
#define PROFILE_H

#include <MButton>

class MStatusIndicatorMenuInterface;
class MButton;
class ProfilePlugin;

/*!
 * The Profile widget makes it possible to select the currently
 * active profile.
 */
class Profile : public MButton
{
    Q_OBJECT

public:
    Profile(ProfilePlugin *profilePlugin, QGraphicsItem *parent = NULL);
    virtual ~Profile();

private slots:
    /*!
     * \brief Shows the profile dialog
     */
    void showProfileDialog();

private:
    //! The profile plugin for accessing the status indicator menu
    ProfilePlugin *plugin;
};

#endif // PROFILE_H
