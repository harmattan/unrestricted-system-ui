#ifndef DEVICEMODESTUB_H
#define DEVICEMODESTUB_H

#include <QObject>

// STUB CLASS

namespace Maemo
{

class QmDeviceMode : public QObject
{
    Q_OBJECT
public:

    /** Possible power save modes for the device. */
    enum PSMState {
        PSMError = -1,    /**< Power save mode could not be figured out. */
        PSMStateOff = 0,  /**< Power save mode is off */
        PSMStateOn        /**< Power save mode is on */
    };

    QmDeviceMode(QObject *parent = 0);
    virtual ~QmDeviceMode();

    /**
     * Get current power save mode.
     * @return Currenet power save mode.
     */
    PSMState getPSMState() const;

    /**
     * Set device power save mode.
     * @param state Power state mode to set.
     * @return True on success, false otherwise.
     */
    bool setPSMState(PSMState state);


signals:
    /** Sent when device power save mode has changed.
     * @param state Current power save mode.
     */
    void devicePSMStateChanged(Maemo::QmDeviceMode::PSMState state);

private:
    PSMState state;
};
}

#endif // DEVICEMODESTUB_H
