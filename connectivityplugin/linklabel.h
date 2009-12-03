#ifndef LINKLABEL_H
#define LINKLABEL_H

#include <DuiLabel>

class LinkLabel : public DuiLabel
{
    Q_OBJECT

public:
    enum Link {
        Network = 0,
        Internet,
        Bluetooth,
        Location
    };

public:
    LinkLabel(LinkLabel::Link link, const QString &text, QGraphicsItem *parent = 0);
    virtual ~LinkLabel();

private slots:
    bool event(QEvent *event);

private:
    void showNetworkModificationPage();

private:
    LinkLabel::Link link;

};

#endif // LINKLABEL_H
