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

#ifndef VOLUMEEXTENSIONINTERFACE_H
#define VOLUMEEXTENSIONINTERFACE_H

#include <QObject>
#include <mapplicationextensioninterface.h>

/**
 * VolumeExtensionInterface is the base class for volume extension classes.
 *
 * Extension developers need to implement this interface in their extension binary
 * and export their implementation from the binary. The host application will maintain
 * an inventory of available extensions and instantiate them using this interface
 * when needed.
 */
class VolumeExtensionInterface : public MApplicationExtensionInterface
{
public:
    /*!
     * Destructor.
     */
    virtual ~VolumeExtensionInterface() {}
};

Q_DECLARE_INTERFACE(VolumeExtensionInterface, "com.meego.core.VolumeExtensionInterface/0.20")

#endif // VOLUMEEXTENSIONINTERFACE_H
