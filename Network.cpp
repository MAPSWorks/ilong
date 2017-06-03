#include "Network.h"

Network::Network(ILong *iL, QObject *parent) : QObject(parent),list(&iL->list),
    manager(new QNetworkAccessManager(this)),isDownloading(false),iLong(iL),sqlExcute(&iL->sqlExcute)
{
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
    connect(this,SIGNAL(startAgain()),this,SLOT(start()));
}

Network::~Network()
{
    if(manager)
    {
        delete manager;
        manager = 0;
    }
}

bool Network::getDownloadState()
{
    return isDownloading;
}

QString Network::getUrl(QString host, QString path)
{
    QString hostName = host;
    QString portNumber = QString("80");
    QRegExp r(".:.");

    if(r.indexIn(host) >= 0)
    {
        QStringList s = host.split(":");
        hostName = s.at(0);
        portNumber = s.at(1);
    }
    return QString("http://%1:%2%3").arg(hostName).arg(portNumber).arg(path);
}

TPoint Network::getXYZFromUrl(QString Url)
{
    /*
     * 感觉这么写很Low,但先这么用着吧
     * */
    int xoffset = Url.indexOf("x=");
    int yoffset = Url.indexOf("y=");
    int zoffset = Url.indexOf("z=");

    int x = Url.mid(xoffset+2,yoffset-(xoffset+3)).toInt();
    int y = Url.mid(yoffset+2,zoffset-(yoffset+3)).toInt();
    int z = Url.mid(zoffset+2,Url.length()).toInt();
    TPoint t;
    t.x = x;
    t.y = y;
    t.z = z;
    return t;
}

void Network::start()
{
    if(!list->isEmpty())
    {
        isDownloading = true;
        QString fullUrl = getUrl(iLong->map.getServer(),list->at(0));
        QNetworkRequest request = QNetworkRequest(QUrl(fullUrl));
        request.setRawHeader("User-Agent", "Mozilla/5.0 (PC; U; Intel; Linux; en) AppleWebKit/420+ (KHTML, like Gecko)");
        manager->get(request);
        list->removeFirst();
        emit sendTileCount(list->size());
        return;
    }
    isDownloading  = false;
    //this->thread()->exit();
}

void Network::requestFinished(QNetworkReply *reply)
{
    emit startAgain();
    if (!reply)
    {
        qDebug() << "MapNetwork::requestFinished - reply no longer valid";
        return;
    }
    if (reply->error() != QNetworkReply::NoError)
    {
        //qDebug() << "QNetworkReply Error: " << reply->errorString();
        return;
    }
    QByteArray ax;
    if (reply->bytesAvailable()>0)
    {
        QPixmap pm;
        ax = reply->readAll();

        if (pm.loadFromData(ax) && pm.size().width() > 1 && pm.size().height() > 1)
        {
            TPoint t = getXYZFromUrl(reply->url().toString());
            /*
             * 下载到的瓦片,看看是不是当前场景内,如果是就打印到场景背景图片里
             * */
            if(t.z == iLong->zoomLevel())
            {
                QPoint middle = iLong->middle;
                QPoint leftTop = iLong->leftTop;
                int x = (t.x+leftTop.x()-middle.x())*DEFAULTTILESIZE;
                int y = (t.y+leftTop.y()-middle.y())*DEFAULTTILESIZE;
                if(x>=0 && x<iLong->background.width() && y >=0 && y < iLong->background.height())
                {
                    QPainter p(&iLong->background);
                    p.drawPixmap(x,y,DEFAULTTILESIZE,DEFAULTTILESIZE,pm);
                    p.end();
                    emit newImage();
                }
            }
            /*
             * 不管是不是场景里的瓦片,只要有瓦片下完都保存到数据库里,方便下次直接调用嘛是吧
             * */
            sqlExcute->insertImage(t.x, t.y, t.z, ax);
        }
    }
}

