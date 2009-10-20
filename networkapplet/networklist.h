#ifndef NETWORKLIST_H
#define NETWORKLIST_H

#include <DuiList>

class DuiWidgetListModel;
class QModelIndex;

class NetworkList : public DuiList
{
    Q_OBJECT

public:
    NetworkList(QGraphicsItem *parent = 0);
    virtual ~NetworkList();

    bool insertNetworks(int selected, const QStringList &networks);    

signals:
    void availableNetworkSelected(QString);

private slots:
    void availableNetworkClicked(const QModelIndex &index);

private:    
    QGraphicsItem *parent;
    DuiWidgetListModel *listModel;
};

#endif // NETWORKLIST_H
