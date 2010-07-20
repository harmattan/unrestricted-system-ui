#ifndef _QMUSBMODE_STUB_H
#define _QMUSBMODE_STUB_H

#include <QObject>

namespace Maemo
{

    class QmUSBMode : public QObject
    {
        Q_OBJECT;

    public:

        enum Mode {
            Connected = 0,
            DataInUse,
            Disconnected,
            MassStorage,
            ChargingOnly,
            OviSuite,
            ModeRequest,
            Ask,
            Undefined
        };

        QmUSBMode (QObject *parent = 0)
        {
            Q_UNUSED (parent);
            testMode = Undefined;
            testDefaultMode = Undefined;
        }
        ~QmUSBMode ()
        {
        }

        Mode getMode ()
        {
            return testMode;
        }
        bool setMode (Mode mode)
        {
            testMode = mode;
            return true;
        }
        bool setDefaultMode (Mode mode)
        {
            testDefaultMode = mode;
            return true;
        }
        Mode getDefaultMode ()
        {
            return testDefaultMode;
        }

    signals:
        void modeChanged (Maemo::QmUSBMode::Mode mode);

    /*
     * Tese are only available in this stub, not in real life
     */
    public slots:
        void test_emitModeChanged (Mode mode)
        {
            testMode = mode;
            emit modeChanged (mode);
        }

    private:
        Mode testMode;
        Mode testDefaultMode;
    };
}

#endif
