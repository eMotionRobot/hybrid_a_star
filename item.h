#ifndef ITEM_H
#define ITEM_H

#include <QPoint>


class Item
{
public:
    Item();
    Item(QPoint pos);

    QPoint m_pos;		//位置

	bool m_bIsMine;	//是否是雷
    bool m_bIsObstacle;	//whether is obstacle
    int m_nObjectType;  //the object type , Nothing,Robot or goal

    int m_nNumber;	//数字
	bool m_bOpen;		//是否已打开，且非雷


};

#endif // ITEM_H
