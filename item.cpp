#include "item.h"

Item::Item()
{
    m_pos = QPoint(-1,-1);

    m_nNumber = 0;
    m_bIsMine = false;


	m_bOpen = false;
    m_bIsObstacle=false;
    m_nObjectType=0;
}


Item::Item(QPoint pos)
{
    m_pos = pos;

    m_nNumber = 0;
    m_bIsMine = false;

	m_bOpen = false;
    m_bIsObstacle=false;
    m_nObjectType=0;
}

