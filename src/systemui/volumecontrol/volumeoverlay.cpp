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

/*
 * define this if you want volume-change animation...
 */
#undef WANT_ANIMATION

#define HIDE_TIMEOUT 1000

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

    m_window->sceneManager()->appearSceneWindow (this);
}

void
VolumeOverlay::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    /* tapping on screen left side should decrease the value */
    int step = -1;

    /* .. and tapping on right side should increase the value */
    if (event->pos ().x () > size ().width () / 2)
        step = 1;

    m_value += step;

    /* check for corner cases... */
    if (m_value < 0)
        m_value = 0;
    if (m_value >= m_valueMax)
        m_value = m_valueMax - 1;

    /* emit VolumeChanged signal... */
    emit VolumeChanged (m_value);
    /* and the update the UI */
    UpdateVolume (m_value, m_valueMax);
}

void
VolumeOverlay::updateContents ()
{
#ifdef WANT_ANIMATION
    bool orientationChanged = false;
#endif
    MSceneManager *sm = m_window->sceneManager ();
    QSize screen = sm->visibleSceneSize (sm->orientation ());

    /*
     * update the bar geometry based on current screen size,
     * and on actual volume-level
     */

    QSizeF newSize;
    newSize.setHeight ((screen.height () / (m_valueMax - 1)) * m_value);
    newSize.setWidth (screen.width ());

    QRectF barGeom;
    barGeom.setX (0.0);
    barGeom.setY (screen.height () - newSize.height ());
    barGeom.setSize (newSize);

#ifdef WANT_ANIMATION
    if (qAbs (m_slider->geometry ().width () - screen.width ()) > 10)
    {
        /* when orientation changed, we don't want animation */
        orientationChanged = true;
    }

    if (isOnDisplay () && ! orientationChanged)
    {
        if (! m_anim.isNull ())
            m_anim->stop ();
        // previous stop call also delete the previous animation obj.
        m_anim = new QPropertyAnimation (m_slider, "geometry");

        m_anim->setDuration (50);
        m_anim->setStartValue (m_slider->geometry ());
        m_anim->setEndValue (barGeom);

        m_anim->start (QAbstractAnimation::DeleteWhenStopped);
    }
    else
#endif
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

    updateContents ();

    if (m_window->isVisible () == false)
        m_window->show ();

    m_timer->start ();
}

void
VolumeOverlay::hideMe ()
{
    m_timer->stop ();

#ifdef WANT_ANIMATION
    if (! m_anim.isNull ())
        m_anim->stop ();
#endif

    m_window->hide ();
}

