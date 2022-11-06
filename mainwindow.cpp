#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->hide();

    connect(ui->menu,SIGNAL(aboutToShow()),this,SLOT(OnSearchPath2()));
    connect(ui->menu_2,SIGNAL(aboutToShow()),this,SLOT(OnMenu_Settings()));
    connect(ui->menu_3,SIGNAL(aboutToShow()),this,SLOT(OnClearPath()));
    connect(ui->action9_9_10, SIGNAL(triggered()), this, SLOT(OnMenuLevel1()));
    connect(ui->action16_16_40, SIGNAL(triggered()), this, SLOT(OnMenuLevel2()));
    connect(ui->action_16_30_99, SIGNAL(triggered()), this, SLOT(OnMenuLevel3()));
	
    m_ObstacleImage = QPixmap(":/new/image/wall.jpg");	//障碍墙
    m_RobotImage = QPixmap(":/new/image/robot.png");	//机器人
    m_GoalImage = QPixmap(":/new/image/goal.png");   //目标
	
    m_nRows = 30;
    m_nColumes = 30;

    start.x=0;
    start.y=0;
    end.x=0;
    end.y=0;

	NewGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitItems()
{
	//随机初始化雷
    /*
    m_Mines.clear();
    for(int i = 0; i<m_nMineCount; i++)
    {
        qsrand(QTime::currentTime().msec());
        int x = qrand()%m_nColumes;
        int y = qrand()%m_nRows;
        while(m_Mines.contains(QPoint(x,y)))
        {
            x = qrand()%m_nColumes;
            y = qrand()%m_nRows;
        }
        m_Mines.append(QPoint(x,y));
    }
    */
	//建立2维数组保存所有元素位置，方便索引
	for(int i=0; i<m_nColumes; i++)
	{
		QVector<Item*> rowItems;
		for(int j=0; j<m_nRows; j++)
		{
			QPoint pos = QPoint(i,j);
			Item* pItem = new Item(pos);
			rowItems.append(pItem);		
		}
		m_items.append(rowItems);
	}
	//计算雷附近格子的数字
    /*

    for(int i=0; i<m_nColumes; i++)
    {
        for(int j=0; j<m_nRows; j++)
        {
            if (m_items[i][j]->m_bIsMine)
            {
                continue;
            }
            int nCountMines = 0;
            //求每个点附近的8个点的是雷的总数
            for (int m=-1;m<=1;m++)
            {
                for (int n=-1; n<=1;n++)
                {
                    if (m==0 && n==0)
                    {
                        continue;
                    }
                    QPoint ptNew = QPoint(i+m,j+n);
                    if (!PointInGameArea(ptNew))
                    {
                        continue;
                    }

                    if (m_items[i+m][j+n]->m_bIsMine)
                    {
                        nCountMines++;
                    }
                }
            }
            m_items[i][j]->m_nNumber = nCountMines;
        }
    }
  */
}

void MainWindow::ReleaseItems()
{
    for (int i=0; i<m_items.size(); i++)
	{
        for (int j=0;j<m_items[i].size(); j++)
        {
			if (m_items[i][j] != NULL)
			{
				delete m_items[i][j];
				m_items[i][j] = NULL;
			}
		}
	}
	m_items.clear();


}


void MainWindow::ReleaseMazeMap()
{
    mazeMap.clear();
    m_bIsHaveRobot=false;
    m_bIsHaveGoal=false;
}

void MainWindow::ReleasePath()
{
    for(auto &p:path)
    {
        if(p!=nullptr)
            p=nullptr;
    }
    std::cout<<__FUNCTION__<<" line: "<<__LINE__<<": clear path"<<std::endl;
    path.clear();

    h_path_.clear();
}



void MainWindow::NewGame()
{
	resize(START_X*2 + m_nColumes*RECT_WIDTH  ,START_Y*2 + m_nRows*RECT_HEIGHT);

	ReleaseItems();
    ReleasePath();
    InitItems();
}



void MainWindow::paintEvent(QPaintEvent *e)
{
    //std::cout<<" paintEvent "<<std::endl;
    DrawChessboard();
    DrawItems();
    DrawPath2();

    update();
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
	//得到鼠标处的格子坐标
	QPoint pt;
	pt.setX( (e->pos().x() - START_X ) / RECT_WIDTH);
	pt.setY( (e->pos().y() - START_X ) / RECT_HEIGHT);
	//是否点在游戏区域内
	if (!PointInGameArea(pt))
	{
		return;
	}
	//获取所点击矩形元素
	Item* pItem = m_items[pt.x()][pt.y()];
    //leftbutton set object tpye,rightbutton set obstacle
    if(e->button()==Qt::LeftButton) //left button ,set Robot or goal
	{
        //
        pItem->m_nObjectType++;
        pItem->m_nObjectType%=3;

        pItem->m_bIsObstacle=false;  //clear obstacle
	}
    else if(e->button()==Qt::RightButton)  //set obstacle or not obstacle
	{
        pItem->m_nObjectType=0;  //clear object type

        if (pItem->m_bIsObstacle)
		{
            pItem->m_bIsObstacle = false;
		}
        else
		{			
            pItem->m_bIsObstacle = true;
		}
	}
}

void MainWindow::OnSearchPath()
{
    ReleasePath();
    createMazeMap();  //create Maze map and start end point
    if(!m_bIsHaveRobot || !m_bIsHaveGoal)
    {
        std::cout<<"have not roobot or goal"<<std::endl;
        return;
    }


    astar.InitAstar(mazeMap);

    path=astar.GetPath(start, end, false);

    markPathInMazeMap();
    printMazeMap();
    printPath();

    if(!path.empty())
        path.pop_back();
    if(!path.empty())
        path.pop_front();

    update();

}

void MainWindow::OnSearchPath2()
{
    ReleasePath();
    createMazeMap2();  //create Maze map and start end point
    if(!m_bIsHaveRobot || !m_bIsHaveGoal)
    {
        std::cout<<"have not roobot or goal"<<std::endl;
        return;
    }
    std::cout<<"startPos: ("<<start.x<<","<<start.y<<")"<<std::endl;
    std::cout<<"endPose: ("<<end.x<<","<<end.y<<")"<<std::endl;

    State initial = State(start.x*map_.grid_res_, start.y*map_.grid_res_, 0);
    State goal = State(end.x*map_.grid_res_, end.y*map_.grid_res_, 0);
    std::cout<<"initial: ("<<initial.x<<","<<initial.y<<")"<<std::endl;
    std::cout<<"goal: ("<<goal.x<<","<<goal.y<<")"<<std::endl;

    std::cout<<"Start algorithm"<<std::endl; 
    Algorithm algorithm(map_);
    std::cout<<"updateInitial"<<std::endl; 
    algorithm.updateInitial(initial);
    std::cout<<"updateGoal"<<std::endl; 
    algorithm.updateGoal(goal);
    std::cout<<"s_hybridAstarPlanning"<<std::endl; 
    std::vector<PathPoint> h_path = algorithm.hybridAstarPlanning();
    
    h_path_.clear();
    for(int i=h_path.size()-1; i>=0; i--)
    {
      h_path_.push_back(h_path[i]);
    }
    std::cout<<"h_path_:"<<h_path_.size()<<std::endl; 
    markPathInMazeMap2();
    printMazeMap();
    printPath2();

//    if(!path.empty())
//        path.pop_back();
//    if(!path.empty())
//        path.pop_front();

    update();

}


void MainWindow::OnMenuLevel1()
{
    std::cout<<"OnMenuLevel1"<<std::endl;
    m_nRows = 8;
    m_nColumes = 12;

    NewGame();
}

void MainWindow::OnMenuLevel2()
{
    std::cout<<"OnMenuLevel2"<<std::endl;
    m_nRows = 16;
    m_nColumes = 16;

    NewGame();
}

void MainWindow::OnMenuLevel3()
{
    std::cout<<"OnMenuLevel3"<<std::endl;
    m_nRows = 40;
    m_nColumes = 80;

    NewGame();
}

void MainWindow::OnClearPath()
{
    ReleasePath();
}

//
void MainWindow::DrawChessboard()
{
    QPainter painter(this);
    painter.setBrush(Qt::lightGray);
    painter.drawRect( 0,0,width(),height());
}

void MainWindow::DrawItems()
{
    QPainter painter(this);
    painter.setBrush(Qt::lightGray);
    painter.setPen(QPen(QColor(Qt::black),1));

    for(int i=0; i<m_nColumes; i++)
    {
        for(int j=0; j<m_nRows; j++)
        {
            DrawItem(painter,m_items[i][j]);
        }
    }
}
void MainWindow::DrawItem(QPainter& painter,Item* pItem)
{
    if(pItem->m_bIsObstacle) //show obstacle
    {
        QRect rcSrc(0,0,m_ObstacleImage.width(),m_ObstacleImage.height());
        QRect rcTarget(START_X + pItem->m_pos.x()*RECT_WIDTH + 2,START_Y + pItem->m_pos.y()*RECT_HEIGHT + 2,RECT_WIDTH-4,RECT_HEIGHT-4);
        painter.drawPixmap(rcTarget,m_ObstacleImage,rcSrc);

        painter.setBrush(Qt::transparent);
        painter.drawRect( START_X + pItem->m_pos.x()*RECT_WIDTH,START_Y + pItem->m_pos.y()*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
		return;
	}
    else if (pItem->m_nObjectType!=0)  //show Robot item or goal item
	{
        if(pItem->m_nObjectType == 1)  //show Robot
		{
            QRect rcSrc(0,0,m_RobotImage.width(),m_RobotImage.height());
            QRect rcTarget(START_X + pItem->m_pos.x()*RECT_WIDTH + 2,START_Y + pItem->m_pos.y()*RECT_HEIGHT + 2,RECT_WIDTH-4,RECT_HEIGHT-4);
            painter.drawPixmap(rcTarget,m_RobotImage,rcSrc);

            painter.setBrush(Qt::transparent);
            painter.drawRect( START_X + pItem->m_pos.x()*RECT_WIDTH,START_Y + pItem->m_pos.y()*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
            return ;
		}
        else
		{
            QRect rcSrc(0,0,m_GoalImage.width(),m_GoalImage.height());
            QRect rcTarget(START_X + pItem->m_pos.x()*RECT_WIDTH + 2,START_Y + pItem->m_pos.y()*RECT_HEIGHT + 2,RECT_WIDTH-4,RECT_HEIGHT-4);
            painter.drawPixmap(rcTarget,m_GoalImage,rcSrc);

            painter.setBrush(Qt::transparent);
            painter.drawRect( START_X + pItem->m_pos.x()*RECT_WIDTH,START_Y + pItem->m_pos.y()*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
            return;
		}
	}
	else
	{
        painter.setBrush(Qt::yellow);

	}
    painter.drawRect( START_X + pItem->m_pos.x()*RECT_WIDTH,START_Y + pItem->m_pos.y()*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
}

void MainWindow::DrawPath()
{
    //std::cout<<" DrawPath "<<std::endl;
    QPainter painter(this);

    painter.setBrush(Qt::red);

    if(!path.empty())
    {
        std::cout<<"DrawPath path.size(): "<<path.size()<<std::endl;
        for(auto &p:path)
        {
          std::cout<<" DrawPath:"<< p->x<<" "<< p->y<<std::endl;
          painter.drawRect( START_X + p->x*RECT_WIDTH,START_Y + p->y*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
        }
    }
}

void MainWindow::DrawPath2()
{
    //std::cout<<" DrawPath "<<std::endl;
    QPainter painter(this);

    painter.setBrush(Qt::red);

    if(!h_path_.empty())
    {
        std::cout<<"DrawPath h_path_.size(): "<<h_path_.size()<<std::endl;
        for(auto &p:h_path_)
        {
          std::cout<<" DrawPath:"<< p.gx<<" "<< p.gy<<std::endl;
          painter.drawRect( START_X + p.gx*RECT_WIDTH,START_Y + p.gy*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
        }
    }
}



void MainWindow::createMazeMap()
{
    ReleaseMazeMap();

    for(int i=0;i<m_nColumes    ;++i)
    {
        std::vector<int> rowMazeMap;
        for(int j=0;j<m_nRows  ;++j)
        {
            rowMazeMap.push_back((int)m_items[i][j]->m_bIsObstacle);
            std::cout<<rowMazeMap.back();
            //find start and end point
            if(m_items[i][j]->m_nObjectType==1)
            {
                start.x=m_items[i][j]->m_pos.x();
                start.y=m_items[i][j]->m_pos.y();
                m_bIsHaveRobot=true;

            }
            else if(m_items[i][j]->m_nObjectType==2)
            {
                end.x=m_items[i][j]->m_pos.x();
                end.y=m_items[i][j]->m_pos.y();
                m_bIsHaveGoal=true;

            }
        }

        std::cout<<std::endl;
        mazeMap.push_back(rowMazeMap);

    }

    printMazeMap();

    std::cout<<"startPos: ("<<start.x<<","<<start.y<<")"<<std::endl;
    std::cout<<"endPose: ("<<end.x<<","<<end.y<<")"<<std::endl;
}

void MainWindow::createMazeMap2()
{
    ReleaseMazeMap();

    for(int i=0;i<m_nColumes    ;++i)
    {
        std::vector<int> rowMazeMap;
        for(int j=0;j<m_nRows  ;++j)
        {
            rowMazeMap.push_back((int)m_items[i][j]->m_bIsObstacle);
            std::cout<<rowMazeMap.back();
            //find start and end point
            if(m_items[i][j]->m_nObjectType==1)
            {
                start.x=m_items[i][j]->m_pos.x();
                start.y=m_items[i][j]->m_pos.y();
                m_bIsHaveRobot=true;

            }
            else if(m_items[i][j]->m_nObjectType==2)
            {
                end.x=m_items[i][j]->m_pos.x();
                end.y=m_items[i][j]->m_pos.y();
                m_bIsHaveGoal=true;

            }
        }

        std::cout<<std::endl;
        mazeMap.push_back(rowMazeMap);
    }

    //printMazeMap();
//    start.x=1;//m_items[i][j]->m_pos.x();
//    start.y=1;//m_items[i][j]->m_pos.y();
//    m_bIsHaveRobot=true;
//    end.x=28;//m_items[i][j]->m_pos.x();
//    end.y=28;//m_items[i][j]->m_pos.y();
//    m_bIsHaveGoal=true;

    std::cout<<"startPos: ("<<start.x<<","<<start.y<<")"<<std::endl;
    std::cout<<"endPose: ("<<end.x<<","<<end.y<<")"<<std::endl;
    map_.grid_x_ = m_nColumes;
    map_.grid_y_ = m_nRows;
    map_.grid_res_ = 10;
    map_.map_x_ = m_nColumes*map_.grid_res_;
    map_.map_y_ = m_nRows * map_.grid_res_;

    std::vector<std::vector<int>> full_map;
    for(int i=0;i<map_.map_x_;++i)
    {
        std::vector<int> row_map;
        for(int j=0;j<map_.map_y_;++j)
        {
          row_map.push_back(mazeMap[i/map_.grid_res_][j/map_.grid_res_]);
        }
        full_map.push_back(row_map);
    }
    std::cout<<"full_map:"<<full_map.size()<<" row:"<<full_map[0].size()<<std::endl;
    map_.initMap(full_map);
}

bool MainWindow::PointInGameArea(QPoint pt)
{
	if(pt.x()>=0 && pt.x()< m_nColumes && pt.y()>=0 && pt.y()< m_nRows)
	{
		return true;
	}
	return false;
}

void MainWindow::printMazeMap()
{
    std::cout<<"paint mazeMap:"<<std::endl;
//    for(int i=0;i<m_nRows;++i)
//    {
//       for(int j=0;j<m_nColumes;++j)
//           std::cout<<" "<<mazeMap<<" ";
//       std::cout<<std::endl;
//    }
    std::vector <std::vector <int> >::iterator iter=mazeMap.begin();
    for(iter=mazeMap.begin();iter!=mazeMap.end();iter++)
    {
        std::vector<int>::iterator it;
        for(it=(*iter).begin();it!=(*iter).end();it++)
        {
            std::cout<<" "<<*it<<" ";
        }
        std::cout<<std::endl;

    }
    std::cout<<"mazeMap.size(): "<<mazeMap.size()<<std::endl;
    std::cout<<"mazeMap[0].size(): "<<mazeMap[0].size()<<std::endl;
    std::cout<<std::endl;
}

void MainWindow::printPath()
{
    std::cout<<"print path:"<<path.size()<<std::endl;
    for (auto &p : path)
        std::cout << '(' << p->x << ',' << p->y << ')' << std::endl;
}

void MainWindow::markPathInMazeMap()
{
    for (auto &q : path)
    {
        mazeMap[q->x][q->y] = 7;
    }
}

void MainWindow::printPath2()
{
    std::cout<<"print h_path_:"<<h_path_.size()<<std::endl;
    for (auto &p : h_path_)
        std::cout << '(' << p.gx << ',' << p.gy << ')' << std::endl;
}

void MainWindow::markPathInMazeMap2()
{
    for (auto &q : h_path_)
    {
        mazeMap[q.gx][q.gy] = 7;
    }
}

