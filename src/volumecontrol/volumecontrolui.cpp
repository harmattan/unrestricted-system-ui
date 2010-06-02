#include "volumecontrolui.h"
#include "volumeoverlay.h"
#include "volumebarlogic.h"

#undef DEBUG
#include "../debug.h"

VolumeControlUI::VolumeControlUI (QObject *parent) :
    QObject (parent),
    m_logic (new VolumeBarLogic),
    m_overlay (new VolumeOverlay),
    m_hwkeys (0)
{
    SYS_DEBUG ("");

    connect (m_overlay, SIGNAL (VolumeChanged (int)),
             this, SLOT (overlayChanged (int)));

    m_hwkeys = new QmKeys (this);

    m_hwkeyResource = new ResourceSet ("event");
    m_hwkeyResource->setAlwaysReply ();

    ScaleButtonResource *volumeKeys = new ScaleButtonResource;

    m_hwkeyResource->addResourceObject (volumeKeys);

    connect (m_hwkeyResource, SIGNAL (resourcesGranted (QList<ResourcePolicy::ResourceType>)),
             this, SLOT (hwKeyResourceAcquired ()));
    connect (m_hwkeyResource, SIGNAL (lostResources ()),
             this, SLOT (hwKeyResourceLost ()));

    m_hwkeyResource->acquire ();
}

VolumeControlUI::~VolumeControlUI ()
{
    //Free the resources here
    m_hwkeyResource->deleteResource (ScaleButtonType);

    delete m_hwkeys;
    m_hwkeys = 0;

    delete m_logic;
    m_logic = 0;

    delete m_overlay;
    m_overlay = 0;
}

void
VolumeControlUI::overlayChanged (int val)
{
    // Set the volume value
    m_logic->setVolume ((quint32) val);
}

void
VolumeControlUI::hwKeyEvent (QmKeys::Key key, QmKeys::State state)
{
    int change_val = 0;

    if (state == QmKeys::KeyUp)
        return;

    switch (key)
    {
        // TODO: check the orientation stuff from UI specs.
        case QmKeys::VolumeUp:
            change_val++;
            break;
        case QmKeys::VolumeDown:
            change_val--;
            break;
        default:
            // no-op for other hw-keys...
            return;
            break;
    }

    int current_volume = (int) m_logic->getVolume ();
    int max_volume = (int) m_logic->getMaxVolume ();

    current_volume += change_val;

    if (current_volume >= max_volume)
        current_volume = max_volume - 1;
    if (current_volume < 0)
        current_volume = 0;

    // This sets the volume and update the slider ...
    overlayChanged (current_volume);
    // ... and show the overlay
    m_overlay->UpdateVolume (current_volume, max_volume);
}

void
VolumeControlUI::hwKeyResourceAcquired ()
{
    SYS_DEBUG ("");

    // Disconnect from everything first
    m_hwkeys->disconnect ();

    connect (m_hwkeys,
             SIGNAL (keyEvent (QmKeys::Key, QmKeys::State)),
             this,
             SLOT (hwKeyEvent (QmKeys::Key, QmKeys::State)));
}

void
VolumeControlUI::hwKeyResourceLost ()
{
    SYS_DEBUG ("");
    m_hwkeys->disconnect ();
}

