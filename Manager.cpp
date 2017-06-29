#include "Manager.h"

Manager::Manager(ILong *iL, QObject *parent) : QObject(parent),iLong(iL),sqlExcute(&iL->sqlExcute)
{
    QSqlQuery * query = sqlExcute->initLayerManager();
    while(query->next())
    {
        QString id = query->value(0).toString();
        QString name = query->value(1).toString();
        int visible = query->value(2).toInt();
        int selectable = query->value(3).toInt();
        loadLayer(id,name,visible, selectable);
    }
    delete query;
    query = 0;
    connect(this, SIGNAL(addGeoToScene(Geometry*)), iLong, SLOT(addGeoToScene(Geometry*)));
}

QList<Layer *> Manager::getLayers()
{
    return list;
}

Layer *Manager::addLayer(QString name, QList<LayerFormat> *typeList)
{
    QString layerName = checkLayerName(name);
    if(name == "iLongio" && layerName != name)
        return getLayer(name);
    Layer * layer  = new Layer(iLong, layerName, typeList);
    list.append(layer);
    return layer;
}

Layer *Manager::getLayer(QString name)
{
    Layer * l = nullptr;
    for(int i=0; i<list.size(); i++)
    {
        if(list.at(i)->getLayerName() == name)
        {
            l = list.at(i);
            break;
        }
    }
    return l;
}

Layer *Manager::getLayerByID(QString id)
{
    Layer * l = nullptr;
    for(int i=0; i<list.size(); i++)
    {
        if(list.at(i)->getLayerID() == id)
        {
            l = list.at(i);
            break;
        }
    }
    return l;
}

void Manager::removeLayer(QString name)
{
     Layer * l = getLayer(name);
    if(name == "iLongio")
    {
        if(l != nullptr)
            sqlExcute->clearLayer(l->getLayerID());
        return;
    }
    if(l != nullptr)
    {
        if(name == "iLongio")
            sqlExcute->clearLayer(l->getLayerID());
        else
        {
            list.removeOne(l);
            delete l;
        }
        updatLayer();
    }
}

void Manager::stopUpdateLayer()
{
    isUpdate = false;
    iLong->scene()->clear();
}

void Manager::addTempItem(QPointF world, ILongGeoType type)
{
    qsrand(QDateTime::currentDateTime().time().second());
    tempGeoType = type;
    tempGeoWorldPos = world;
    QColor pen = QColor(qrand()%255,qrand()%255,qrand()%255);
    Geometry * g = nullptr;
    switch (tempGeoType) {
    case iGeoCircle:
        g = new GeoCircle(tempGeoWorldPos,40,pen,pen);
        break;
    case iGeoRect:
        g = new GeoRect(tempGeoWorldPos,40,pen,pen);
        break;
    case iGeoPie:
        g = new GeoPie(tempGeoWorldPos,80,0,pen,pen);
        break;
    case iGeoStar:
        g = new GeoStar(tempGeoWorldPos,40,pen,pen);
        break;
    case iGeoTri:
        g = new GeoTri(tempGeoWorldPos,40,pen,pen);
        break;
    default:
        break;
    }
    if(g)
    {
        g->setPos(iLong->worldToScene(tempGeoWorldPos));
        g->setScale(iLong->itemScale);
        //iLong->scene()->addItem(tempGeo);
        emit addGeoToScene(g);
    }
}

bool Manager::moveLayer(QString name, bool up)
{
    Layer * l = getLayer(name);
    if(!l)
        return false;
    int index = list.indexOf(l);
    if(up)
    {
        if(index == 0)
            return false;
        list.removeAt(index);
        list.insert(index - 1, l);
        return true;
    }
    if(index == list.size()+1)
        return false;
    list.removeAt(index);
    list.insert(index + 1, l);
    return true;
}

void Manager::updatLayer()
{
    isUpdate = true;
    //iLong->scene()->clear();
    for(int i=0; i<list.size() && isUpdate; i++)
    {
        if(list.at(i)->isVisible() && isUpdate)
        {
            list.at(i)->updatLayer(&isUpdate);
        }
    }
    addTempItem(tempGeoWorldPos,tempGeoType);
    this->thread()->exit();
}

QString Manager::checkLayerName(QString name)
{
    QString tmp = name ;
    for(int i=0; i<list.size(); i++)
    {
        if(list.at(i)->getLayerName() == tmp)
        {
            tmp.append("*");
            break;
        }
    }
    return name  == tmp ? tmp : checkLayerName(tmp);
}

void Manager::loadLayer(QString id, QString name, bool visible, bool selectable)
{
    Layer * layer = new Layer(iLong,id,name,visible,selectable);
    list.append(layer);
}
