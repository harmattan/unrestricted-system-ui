#include "volumeoverlay.h"

#include <QTimer>
#include <QRegion>
#include <QPointF>
#include <QSizeF>

#include <MSceneWindow>
#include <MImageWidget>
#include <MApplication>
#include <MSceneManager>
#include <QGraphicsLinearLayout>
#include <MSlider>

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

    m_slider = new MSlider;

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

    // maybe this call helps:
    m_window->sceneManager ()->appearSceneWindowNow (this);
    m_window->sceneManager ()->disappearSceneWindowNow (this);

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

    if (m_window->isVisible () == false)
    {
        m_window->show ();
        m_window->showFullScreen ();
        m_window->sceneManager ()->appearSceneWindow (this);

    }

    m_timer->start ();
}

void
VolumeOverlay::hideMe ()
{
    m_timer->stop ();
    m_window->sceneManager ()->disappearSceneWindow (this);
    m_window->hide ();
}

void
VolumeOverlay::orientationChanged (M::Orientation orientation)
{
    QSize viewport (m_window->visibleSceneSize (orientation));

    m_slider->setPreferredHeight (viewport.height () < viewport.width () ?
                                  viewport.height () : viewport.width ());

    if (orientation == M::Landscape)
        setPos (QPointF (0., 0.));
    else // Portrait
        setPos (QPointF (viewport.width () - preferredWidth (), 0.));

    // This breaks the rotation somehow... so disabled for a while...
#if 0
    // Set up window mask so that mouse events are passed on to lower widgets.
    if (m_window->orientation () == M::Landscape)
    {
        QRegion region(QRect(0, 0,
                             preferredSize ().width (),
                             preferredSize ().height ()),
                             QRegion::Rectangle);
        m_window->setMask(region);
    }
    else
    {
        QRegion region(QRect(0, 0,
                             preferredSize ().height (),
                             preferredSize ().width ()),
                             QRegion::Rectangle);
        m_window->setMask(region);
    }

    m_window->update ();
#endif
}

