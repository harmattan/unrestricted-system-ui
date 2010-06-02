#include "volumeoverlay.h"
#include <MSceneWindow>
#include <MImageWidget>
#include <MApplication>
#include <MSceneManager>
#include <QGraphicsLinearLayout>
#include <MSlider>
#include <QTimer>

#undef DEBUG
#include "../debug.h"

#define HIDE_TIMEOUT 3000

VolumeOverlay::VolumeOverlay (QGraphicsItem *parent) :
    MOverlay (parent),
    m_timer (0),
    m_slider (0),
    m_window (0)
{
    m_timer = new QTimer;
    m_timer->setInterval (HIDE_TIMEOUT);
    connect (m_timer, SIGNAL (timeout ()),
             this, SLOT (hideMe ()));

    setObjectName ("VolumeOverlay");
    setManagedManually (true);

    constructUi ();
}

VolumeOverlay::~VolumeOverlay ()
{
    delete m_timer;
    m_timer = 0;
}

void
VolumeOverlay::constructUi ()
{
    QGraphicsLinearLayout *layout =
        new QGraphicsLinearLayout;

    m_slider = new MSlider (this);

    m_slider->setOrientation (Qt::Vertical);
    m_slider->setMaxLabelIconID (QString ("icon-m-common-volume"));
    m_slider->setMaxLabelVisible (true);
    m_slider->setMinLabelIconID (QString ("icon-m-common-volume-off"));
    m_slider->setMinLabelVisible (true);

    connect (m_slider, SIGNAL (valueChanged (int)),
             this, SIGNAL (VolumeChanged (int)));

    // Stop the timeout when the slider is in pressed state ...
    connect (m_slider, SIGNAL (sliderPressed ()),
             m_timer, SLOT (stop ()));
    // ...and re-start it when slider released.
    connect (m_slider, SIGNAL (sliderReleased ()),
             m_timer, SLOT (start ()));

    // create a new scene manager
    m_window = new MWindow (new MSceneManager);
    m_window->setTranslucentBackground (true);
    m_window->setAttribute (Qt::WA_X11NetWmWindowTypeNotification);

    connect (m_window,
             SIGNAL (orientationChanged (M::Orientation)),
             this,
             SLOT (orientationChanged (M::Orientation)));

    layout->addItem (m_slider);

    setLayout (layout);

    orientationChanged (m_window->orientation ());
}

void
VolumeOverlay::UpdateVolume (int val, int max)
{
    m_timer->stop ();

    m_slider->blockSignals (true);

    m_slider->setValue (0);
    m_slider->setRange (0, max - 1);
    m_slider->setValue (val);

    m_slider->blockSignals (false);

    setOpacity (1.0);

    m_window->show ();
    m_window->showFullScreen ();
    appear (m_window, MSceneWindow::KeepWhenDone);

    m_timer->start ();
}

void
VolumeOverlay::hideMe ()
{
    m_timer->stop ();
    disappear ();
    m_window->hide ();
}

void
VolumeOverlay::orientationChanged (M::Orientation orientation)
{
    QSize viewport (m_window->visibleSceneSize (orientation));

    m_slider->setPreferredHeight (viewport.height ());

    if (orientation == M::Landscape)
        m_window->setAlignment (Qt::AlignLeft);
    else // Portrait
        m_window->setAlignment (Qt::AlignRight);
}

