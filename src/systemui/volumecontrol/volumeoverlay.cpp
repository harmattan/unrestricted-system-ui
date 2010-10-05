#include "volumeoverlay.h"

#include <QTimer>
#include <QRegion>
#include <QPointF>
#include <QSizeF>

#include <MStylableWidget>
#include <MSceneWindow>
#include <MImageWidget>
#include <MApplication>
#include <MSceneManager>
#include <QGraphicsLinearLayout>

#include <QApplication>
#include <MOnDisplayChangeEvent>
#include <QPropertyAnimation>

#undef DEBUG
#define WARNING
#include "../debug.h"

#define HIDE_TIMEOUT 3000

VolumeOverlay::VolumeOverlay (QGraphicsItem *parent) :
    MOverlay (parent),
    m_timer (0),
    m_window (0),
    m_slider (0),
    m_icon (0),
    m_value (0),
    m_valueMax (0)
{
    m_timer = new QTimer;
    m_timer->setInterval (HIDE_TIMEOUT);
    connect (m_timer, SIGNAL (timeout ()),
             this, SLOT (hideMe ()));

    setObjectName ("FSVolumeOverlay");

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
    m_layout = new QGraphicsLinearLayout (Qt::Vertical);

    m_slider = new MStylableWidget (this);
    m_slider->setObjectName ("FSVolumeBar");

    // TODO: Implement volume-level-changing via touch-screen..
    // [left side of screen decreases, right side increases the volume
#if 0
    // Stop the timeout when the slider is in pressed state ...
    connect (m_slider, SIGNAL (sliderPressed ()),
             m_timer, SLOT (stop ()));
    // ...and re-start it when slider released.
    connect (m_slider, SIGNAL (sliderReleased ()),
             m_timer, SLOT (start ()));

    //FIXME emit VolumeChanged on volume-changes...
#endif

    // create a new scene manager
    m_window = new MWindow (new MSceneManager);
    m_window->setTranslucentBackground (true);
    m_window->setAttribute (Qt::WA_X11NetWmWindowTypeNotification, true);
    m_window->setAttribute (Qt::WA_X11DoNotAcceptFocus, true);
    m_window->setObjectName ("VolumeOverlayWindow");

    connect (m_window->sceneManager (), SIGNAL (orientationChangeFinished(M::Orientation)),
             this, SLOT (updateContents ()));

    m_icon = new MImageWidget;
    m_icon->setImage ("icon-m-common-volume", QSize (128, 128));
    m_icon->setZoomFactor (1.0);
    m_icon->setOpacity (0.8);

    m_layout->addStretch (2);
    m_layout->addItem (m_icon);
    m_layout->addStretch (1);

    setLayout (m_layout);
}

void
VolumeOverlay::updateContents ()
{
    MSceneManager *sm = m_window->sceneManager ();
    QSize screen = sm->visibleSceneSize (sm->orientation ());

    /* 
     * update the bar geometry based on current screen size,
     * and on actual volume-level
     */

    QSizeF newSize;
    newSize.setHeight ((screen.height () / m_valueMax) * m_value);
    newSize.setWidth (screen.width ());

    QRectF barGeom;
    barGeom.setX (0.0);
    barGeom.setY (screen.height () - newSize.height ());
    barGeom.setSize (newSize);

    if (isOnDisplay ())
    {
        QPropertyAnimation *anim = new QPropertyAnimation (m_slider, "geometry");
        anim->setDuration (300);
        anim->setStartValue (m_slider->geometry ());
        anim->setEndValue (barGeom);

        anim->start (QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        // overlay is not visible on the display, just set the new geom.:
        m_slider->setGeometry (barGeom);
    }
}

void
VolumeOverlay::UpdateVolume (int val, int max)
{
    m_timer->stop ();

    m_value = val;
    m_valueMax = max;

#if 0
    if (m_window->orientation () == M::Landscape)
        m_window->setPortraitOrientation ();
    else
        m_window->setLandscapeOrientation ();
#endif

    updateContents ();

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

