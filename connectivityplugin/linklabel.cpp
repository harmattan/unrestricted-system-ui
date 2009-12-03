#include "linklabel.h"

#include <DuiControlPanelIf>

LinkLabel::LinkLabel(LinkLabel::Link link, const QString &text, QGraphicsItem *parent) :
        DuiLabel(text, parent),
        link(link)
{
    setObjectName("connectivityLinkLabel");
}

LinkLabel::~LinkLabel()
{
}

bool LinkLabel::event(QEvent *event)
{
    bool result = DuiLabel::event(event);
    if(event->type() == QEvent::GraphicsSceneMousePress)
        showNetworkModificationPage();
    return result;
}

void LinkLabel::showNetworkModificationPage()
{
    // instantiate the interface
    DuiControlPanelIf cpIf;
    // check the interface is valid
    if (!cpIf.isValid())
        return;

    switch(link) {
        case LinkLabel::Network:
            cpIf.appletPage("Phone network");
            break;
        case LinkLabel::Internet:
            //TODO: add link
            break;
        case LinkLabel::Bluetooth:
            //TODO: add link
            break;
        case LinkLabel::Location:
            //TODO: add link
            break;
        default:
            break;
    }
}
