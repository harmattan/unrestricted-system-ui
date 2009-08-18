#include "eventhandler.h"

EventHandler::EventHandler()
{
    keys = new QmKeys();

    //Monitor hardware key presses
    connect(keys, SIGNAL(keyPressed(QmKeys::Key key, QmKeys::HowPressed how)),
            this, SLOT(keyPressed(QmKeys::Key key, QmKeys::HowPressed how)));
}

EventHandler::~EventHandler()
{
}

void EventHandler::keyPressed(QmKeys::Keys key, QmKeys::HowPressed how)
{
    switch(key) {
        case QmKeys::PowerKey:
            switch(how) {
                case QmKeys::ShortPress:
                    emit shortPowerKeyPressOccured();
                break;
                case QmKeys::LongPress:
                break;
            }
        break;
        case QmKeys::HomeKey:
        break;
        case QmKeys::CameraShutterKey:
        break;
        default:
        break;
    }
}
