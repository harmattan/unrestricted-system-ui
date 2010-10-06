#ifndef _VOLUMEOVERLAY_H
#define _VOLUMEOVERLAY_H

#include <MWindow>
#include <MOverlay>
#include <QObject>
#include <QPointer>

class QGraphicsLinearLayout;
class QPropertyAnimation;
class MStylableWidget;
class MImageWidget;
class QTimer;

class VolumeOverlay : public MOverlay
{
  Q_OBJECT

  public:
    VolumeOverlay (QGraphicsItem *parent = 0);
    ~VolumeOverlay ();

  signals:
    void VolumeChanged (int val);

  public slots:
    void UpdateVolume (int val, int max);

  private slots:
    void hideMe ();
    void updateContents ();

  protected:
    void mousePressEvent (QGraphicsSceneMouseEvent *event);

  private:
    QTimer                  *m_timer;
    MWindow                 *m_window;
    QGraphicsLinearLayout   *m_layout;
    MStylableWidget         *m_slider;
    MImageWidget            *m_icon;
    int                      m_value;
    int                      m_valueMax;
    QPointer<QPropertyAnimation>    m_anim;

    void constructUi ();

#ifdef UNIT_TEST
    friend class Ut_VolumeOverlay;
#endif
};

#endif
