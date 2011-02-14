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
#include <QDebug>
#include <MLibrary>
#include "volumeextension.h"
#include "volumebarlogic.h"

Q_EXPORT_PLUGIN2(sysuid-volume, VolumeExtension)

VolumeExtension::VolumeExtension() :
    volumeBarLogic(NULL)
{
    qDebug() << "XX VOLUME EXTENSION INSTANTIATION";
}

VolumeExtension::~VolumeExtension()
{
    delete volumeBarLogic;
}

bool VolumeExtension::initialize(const QString &)
{
    qDebug() << "XX VOLUME EXTENSION INITIALIZATION";
    volumeBarLogic = new VolumeBarLogic(this);
    return true;
}

QGraphicsWidget *VolumeExtension::widget()
{
    return NULL;
}

M_LIBRARY
