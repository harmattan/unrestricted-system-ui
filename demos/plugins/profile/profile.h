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

#ifndef PROFILE_H
#define PROFILE_H

#include <MContentItem>

class MStatusIndicatorMenuInterface;
class ProfilePlugin;

/*!
 * The Profile widget makes it possible to select the currently
 * active profile.
 */
class Profile : public MContentItem
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
