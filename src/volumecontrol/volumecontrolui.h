#ifndef VOLUMECONTROLUI_H
#define VOLUMECONTROLUI_H

#include <QObject>

class VolumeBarLogic;
class VolumeOverlay;

// For Hw-volume key handling
#include <qmkeys.h>
#include <policy/resource-set.h>
using namespace Maemo;
using namespace ResourcePolicy;

class VolumeControlUI : public QObject
{
    Q_OBJECT

public:
    VolumeControlUI (QObject *parent = 0);
    ~VolumeControlUI ();

private slots:
    void overlayChanged (int val);
    void hwKeyEvent (QmKeys::Key key, QmKeys::State state);
    void hwKeyResourceAcquired ();
    void hwKeyResourceLost ();

private:
    VolumeBarLogic  *m_logic;
    VolumeOverlay   *m_overlay;
    Maemo::QmKeys   *m_hwkeys;
    ResourceSet     *m_hwkeyResource;
};

#endif
