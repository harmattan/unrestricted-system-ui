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

#define DEBUG
#include "../debug.h"

namespace ProfileApplet
{
    const int sliderMin = 0;
    const int sliderMax = 100;
}

ProfileContainer::ProfileContainer (
        int            id, 
        const QString &title, 
        int            level, 
        bool           vibra, 
        DuiWidget     *parent) :
    DuiContainer (title, parent),
    m_Slider (0),
    m_Img (0),
    m_Level (-2),
    m_ProfileId (id)
{
    qDebug() << Q_FUNC_INFO << title;

    if (0 <= level) {
        /*
         * Had to remove the 'continuous' for it ruined the theming of the
         * slider, the height become huge.
         */
        m_Slider = new DuiSlider (0/*, "continuous"*/);
        m_Slider->setOrientation (Qt::Horizontal);
        m_Slider->setRange (ProfileApplet::sliderMin, ProfileApplet::sliderMax);
        #if 0
        /*
         * Ok, this worth a try, but did not work. Maybe we should try it again,
         * with some other version of the libdui.
         */
        m_Slider->setMinLabelVisible (true);
        m_Slider->setMinLabelIconID ("icon-m-volume");
        #endif

        setLevel (level);
        connect (m_Slider, SIGNAL (valueChanged(int)), 
                this, SIGNAL (sliderValueChanged(int)));
        connect (m_Slider, SIGNAL (valueChanged(int)), 
                this, SLOT (slotSliderValueChanged(int)));
    }

    //% "Vibration"
    m_Button = new DuiButton (qtTrId ("qtn_prof_vibration"));
    m_Button->setCheckable (true);
    setVibration (vibra);
    connect (m_Button, SIGNAL(toggled(bool)), 
            this, SIGNAL(vibrationChanged(bool)));

    setLayout ();
}

ProfileContainer::~ProfileContainer()
{
    SYS_DEBUG ("");
}

void 
ProfileContainer::setLayout ()
{
    DuiLayout *layout = new DuiLayout();

    DuiGridLayoutPolicy *landscapePolicy = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(landscapePolicy); // ownership transferred

    DuiGridLayoutPolicy *portraitPolicy = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred


    QGraphicsLayoutItem* item = NULL;

    /*
     * FIXME: Ok, but what happens when we change the 'mute' state?
     */
    if (m_Slider) {
        m_Img = new DuiImage ("icon-m-volume");
        item = m_Slider;
    } else {
        m_Img = new DuiImage ("icon-m-volume-off");
        //% "No ringing"
        item = new DuiLabel (qtTrId ("qtn_prof_noringing"));
    }
    m_Img->setObjectName ("speakerIcon");

    /*
     * Adding the widgets to the proper places.
     */
    landscapePolicy->addItemAtPosition (m_Img, 0, 0);
    portraitPolicy->addItemAtPosition (m_Img, 0, 0);
    landscapePolicy->addItemAtPosition (item, 0, 1);
    portraitPolicy->addItemAtPosition (item, 0, 1);
    landscapePolicy->addItemAtPosition (m_Button, 1, 0, 1, 2);
    portraitPolicy->addItemAtPosition (m_Button, 1, 0, 1, 2);
    
    centralWidget()->setLayout(layout);
}

void 
ProfileContainer::setLevel (
        int value)
{
    qDebug() << Q_FUNC_INFO << "for" << title() << ":" << value << 
        "(old:" << m_Level << ")";

    if (!m_Slider || value == m_Level)
        return;

    m_Level = value;
    if (ProfileApplet::sliderMin < m_Level && 
            ProfileApplet::sliderMax >= m_Level) {
        m_Slider->setValue (m_Level);
        qDebug() << Q_FUNC_INFO << "done:" << m_Slider->value();
    }
    qDebug() << Q_FUNC_INFO << "done:" << m_Slider->value();
}

/*!
 * This slot is called when the volume slider value has been changed. The slot
 * can be changed to modify the icon that shows if the volume is muted. 
 */
void
ProfileContainer::slotSliderValueChanged (
        int newValue)
{
    SYS_DEBUG ("*** newValue = %d", newValue);
    SYS_DEBUG ("*** m_Level  = %d", m_Level);
    if (!m_Img)
        return;

    if (newValue == 0 && m_Level != 0) {
        SYS_DEBUG ("setIconID (icon-m-volume-off)");
        m_Img->setImage ("icon-m-volume-off");
    } else if (newValue != 0 && m_Level == 0) {
        SYS_DEBUG ("setIconID (icon-m-volume)");
        m_Img->setImage ("icon-m-volume");
    }
}

/*!
 * This function is called when the vibration enabled/disabled. It will only 
 * modify the button state.
 */
void 
ProfileContainer::setVibration (
        bool enabled)
{
    qDebug() << Q_FUNC_INFO << "for " << title() << ":" << enabled;
    m_Button->setChecked (enabled);
}

int 
ProfileContainer::id ()
{
    return m_ProfileId;
}
