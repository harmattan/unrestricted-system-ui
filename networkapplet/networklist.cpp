#include "networklist.h"

#include <DuiLabel>
#include <DuiWidgetFactory>
#include <DuiWidgetListModel>
#include <QModelIndex>

NetworkList::NetworkList(QGraphicsItem *parent) :
        DuiList(parent),
        listModel(new DuiWidgetListModel(this))
{
    setObjectName("availableNetworksList");
    enableItemSelection(true);
    setItemModel(listModel);
    connect(this, SIGNAL(itemClicked(const QModelIndex &)), this, SLOT(availableNetworkSelected(const QModelIndex &)));
}

NetworkList::~NetworkList()
{
}

bool NetworkList::insertNetworks(int selected, const QStringList &networks)
{
    if(networks.size() <= 0)
        return false;

    for(int i=0; i<networks.size(); ++i)
        listModel->appendWidget(new DuiLabel(networks.at(i), this));

    if(selected != -1)
        selectItem(listModel->index(selected));

    return true;
}

void NetworkList::availableNetworkSelected(const QModelIndex &index)
{
    qDebug() << "NetworkList::availableNetworkSelected(" << index << ")";
    DuiLabel *label = static_cast<DuiLabel*>(DuiWidgetFactory::instance()->create(listModel, index.row()));
    emit availableNetworkSelected(label->text());    
}
