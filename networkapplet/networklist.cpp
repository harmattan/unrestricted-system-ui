#include "networklist.h"

#include <QModelIndex>
#include <QDebug>
#include <QStringListModel>


// Class list cannot inherit DuiList because the signals are not inherited along

NetworkList::NetworkList(QGraphicsItem *parent) :
        DuiList(parent),        
        listModel(NULL),        
        selected(false)
{
    setObjectName("availableNetworksList");
    enableItemSelection(true);
    listModel = new QStringListModel;    
    connect(this, SIGNAL(itemClicked(const QModelIndex &)), this, SLOT(availableOperatorClicked(const QModelIndex &)));
}

NetworkList::~NetworkList()
{
}

bool NetworkList::insertOperators(int selected, const QStringList &operators)
{
    if(operators.size() <= 0)
        return false;

    listModel->setStringList(operators);
    this->setItemModel(listModel);

    if(selected != -1) {
        selectItem(listModel->index(selected));
        this->selected = true;
    }

    return true;
}

void NetworkList::availableOperatorClicked(const QModelIndex &index)
{
    selected = true;    
    emit availableOperatorSelected(index.row());
}

bool NetworkList::itemSelected()
{
    return selected;
}
