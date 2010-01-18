/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "profilecontainer.h"
#include "profilebuttons.h"

#include <DuiButton>
#include <DuiSlider>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiImage>
#include <DuiGridLayoutPolicy>
#include <QDebug>

namespace ProfileApplet
{
const int sliderMin = 0;
const int sliderMax = 100;
}

ProfileContainer::ProfileContainer(int id, const QString &title, int level, bool vibra, DuiWidget *parent) :
        DuiContainer(title, parent),
        slider(NULL),
        level(-2),
        profileId(id)
{
    qDebug() << Q_FUNC_INFO << title;

    if (0 <= level) {
        slider = new DuiSlider(0, "continuous");
        slider->setOrientation(Qt::Horizontal);
        slider->setRange(ProfileApplet::sliderMin, ProfileApplet::sliderMax);
        slider->setThumbLabelVisible(false);
        setLevel(level);
        connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(sliderValueChanged(int)));
    }

    //% "Vibration"
    button = new DuiButton (qtTrId ("qtn_prof_vibration"));
    button->setCheckable(true);
    setVibration(vibra);
    connect(button, SIGNAL(toggled(bool)), this, SIGNAL(vibrationChanged(bool)));

    setLayout();
}

ProfileContainer::~ProfileContainer()
{
}

void ProfileContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(landscapePolicy); // ownership transferred

    DuiGridLayoutPolicy *portraitPolicy = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred


    DuiImage* img = NULL;
    QGraphicsLayoutItem* item = NULL;

    if (slider) {
        img = new DuiImage ("icon-m-volume");
        item = slider;
        qDebug() << Q_FUNC_INFO << "slider height" << slider->size().height();
        slider->setMaximumHeight(32);
    } else {
        img = new DuiImage("icon-m-volume-off");
        //% "No ringing"
        item = new DuiLabel (qtTrId ("qtn_prof_noringing"));
    }
    img->setObjectName("speakerIcon");

    landscapePolicy->setColumnFixedWidth(0, img->imageSize().width());
    portraitPolicy->setColumnFixedWidth(0, img->imageSize().width());
    landscapePolicy->setRowFixedHeight(0, img->imageSize().height());
    portraitPolicy->setRowFixedHeight(0, img->imageSize().height());

    // set objects
    landscapePolicy->addItemAtPosition(img, 0, 0);
    portraitPolicy->addItemAtPosition(img, 0, 0);
    landscapePolicy->addItemAtPosition(item, 0, 1);
    portraitPolicy->addItemAtPosition(item, 0, 1);
    landscapePolicy->addItemAtPosition(button, 1, 0, 1, 2);
    portraitPolicy->addItemAtPosition(button, 1, 0, 1, 2);

    landscapePolicy->setSpacing(5);
    portraitPolicy->setSpacing(5);

    centralWidget()->setLayout(layout);

}

void ProfileContainer::setLevel(int value)
{
    qDebug() << Q_FUNC_INFO << "for" << title() << ":" << value << "(old:" << level << ")";
    if (!slider || value == level)
        return;

    level = value;
    if (ProfileApplet::sliderMin < level && ProfileApplet::sliderMax >= level) {
        slider->setValue(level);
        qDebug() << Q_FUNC_INFO << "done:" << slider->value();
    }
    qDebug() << Q_FUNC_INFO << "done:" << slider->value();
}

void ProfileContainer::setVibration(bool enabled)
{
    qDebug() << Q_FUNC_INFO << "for " << title() << ":" << enabled;
    button->setChecked(enabled);
}

int 
ProfileContainer::id ()
{
    return profileId;
}
