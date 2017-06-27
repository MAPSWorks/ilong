##### 轻量级跨平台瓦片地图库，练手项目希望有人喜欢。

###### 简单用法:

```cpp
#include <ilong.h>

ILong * iLong = new ILong(this);
//设置默认加载位置和地图等级
iLong->setDefaultLocation(QPointF(99.70875,27.82188),16);
//创建图层
QList <LayerFormat> format;
format << LayerFormat{"VALUE1",ILongNUMBER} << LayerFormat{"VALUE2",ILongTEXT};
Layer * layer = iLong->addLayer(layerName, &format);
//添加图元
Geometry::ILongDataType t;
t.geometry = new GeoCircle(QPointF(99.70875,27.82188) /＊ ,80,pen,brush ＊/ );
t.data << value1 << value2;
layer->addGeo(t);
delete t.geometry;
//也可以用layer->addGeos批量添加图元
//现在只有GeoCircle，GeoRect，GeoPie，GeoStar，GeoTri，GeoPolygon
//GeoPolygon图元用来支持多边形和线条，现在基本用不到，暂时这样设计
//文字图元的话并不想加进去，如果真需要，可以放一个图元显示图元标就可以
```
##### 截图

![image](https://raw.githubusercontent.com/ilongio/ilong/e9c7bd65b4c838f4aea08cf3db7d562dc8ddc87c/Screenshot.png)
