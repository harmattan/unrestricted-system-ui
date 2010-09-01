#ifndef _VOLUMEOVERLAY_H
#define _VOLUMEOVERLAY_H

#include <MWindow>
#include <MOverlay>
#include <QObject>

class QGraphicsLinearLayout;
class MSlider;
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
    /*
     * for window mask handling:
     */
    void updateMask ();
    void removeMask ();

  private:
    QTimer                  *m_timer;
    MSlider                 *m_slider;
    MWindow                 *m_window;
    QGraphicsLinearLayout   *m_layout;

    void constructUi ();

#ifdef UNIT_TEST
    friend class Ut_VolumeOverlay;
#endif
};

#endif
