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

#include <QApplication>
#include <MOnDisplayChangeEvent>

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
    m_layout = new QGraphicsLinearLayout;

    m_slider = new MSlider;
    m_slider->setObjectName ("CommonSlider");

    m_slider->setOrientation (Qt::Vertical);
    m_slider->setMaxLabelIconID ("icon-m-common-volume");
    m_slider->setMaxLabelVisible (true);
    m_slider->setMinLabelIconID ("icon-m-common-volume-off");
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
    m_window->setAttribute (Qt::WA_X11NetWmWindowTypeNotification, true);
    m_window->setAttribute (Qt::WA_X11DoNotAcceptFocus, true);
    m_window->setObjectName ("VolumeOverlayWindow");

    connect (m_window->sceneManager (), SIGNAL (orientationAboutToChange(M::Orientation)),
             this, SLOT (removeMask ()));
    connect (m_window->sceneManager (), SIGNAL (orientationChangeFinished(M::Orientation)),
             this, SLOT (updateMask ()));

    m_layout->addItem (m_slider);
    m_layout->addStretch ();

    setLayout (m_layout);
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
        updateMask ();
        m_window->sceneManager ()->appearSceneWindow (this);
    }
    //TODO: Remove sending fake displaychange events when setTranslucentBackground bug is solved
    MOnDisplayChangeEvent* event =
        new MOnDisplayChangeEvent (true, QRectF (0,0,1,1));
    QApplication::sendEvent (m_window, event);

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
VolumeOverlay::updateMask ()
{
    /*
     * Set up window mask so that mouse events are passed on to lower widgets.
     */
    M::Orientation orientation =
        m_window->sceneManager ()->orientation ();

    setPreferredHeight (orientation == M::Landscape ?
                        m_window->height () : m_window->width ());
    m_layout->invalidate ();

    QSize  size = preferredSize().toSize();
    QPoint origin (0, 0);

    /*
     * Here scene window coordinates is in use...
     */
    if (orientation == M::Landscape)
        setPos (QPointF (0., 0.));
    else // Portrait
        setPos (QPointF (m_window->height () - size.width (), 0.));

    m_window->sceneManager ()->appearSceneWindow (this);

    /*
     * ...but here plain X coordinates is in use:
     */
    switch (m_window->sceneManager ()->orientationAngle ())
    {
    case M::Angle90:
        SYS_DEBUG ("Angle90");
        size.transpose ();
        origin.setX (m_window->width () - size.width ());
        origin.setY (m_window->height () - size.height ());
        break;

    case M::Angle270:
        SYS_DEBUG ("Angle270");
        size.transpose ();
        break;

    case M::Angle180:
        SYS_DEBUG ("Angle180");
        origin.setX (m_window->width () - size.width ());
        break;

    default:
        SYS_DEBUG ("Angle0");
        break;
    }

    m_window->setMask (QRegion (QRect (origin, size), QRegion::Rectangle));
}

void
VolumeOverlay::removeMask ()
{
    /*
     * Clear the window-mask during orientation changes
     */
    m_window->clearMask ();
    m_window->sceneManager ()->disappearSceneWindow (this);
}

