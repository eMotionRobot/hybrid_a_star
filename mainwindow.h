#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "item.h"
#include "Astar.h"
#include <QMainWindow>

#include "algorithm.h"

namespace Ui {
class MainWindow;
}

#define RECT_WIDTH      20
#define RECT_HEIGHT     20

#define START_X         100
#define START_Y         100


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitItems();                   //随机初始化雷点位置


    void NewGame();                     //新开一局游戏
    void GameFail();                    //点到雷,游戏失败
    bool FindAll();                     //是否找到所有雷

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private slots:
    void OnSearchPath();
    void OnSearchPath2();    
    void OnMenuLevel1();
    void OnMenuLevel2();
    void OnMenuLevel3();
    void OnClearPath();

private:

    void ReleaseItems();
    void ReleaseMazeMap();
    void ReleasePath();

    void DrawChessboard();
    void DrawItems();
    void DrawItem(QPainter& painter,Item* pItem);
    void DrawPath();
    void DrawPath2();

    void createMazeMap();			//create mazeMap from m_items
    void createMazeMap2();			//create mazeMap from m_items
    bool PointInGameArea(QPoint pt);		//判断坐标是否超过游戏区域

    void printMazeMap();
    void printPath();
    void markPathInMazeMap();
    
    void printPath2();
    void markPathInMazeMap2();

private:
    Ui::MainWindow *ui;
    QPixmap m_ObstacleImage;				//obstacle Image
    QPixmap m_RobotImage;                //Robot Image
    QPixmap m_GoalImage;                 //Goal Image

    Astar astar;
    std::vector<std::vector<int>> mazeMap;	//
    Point start;
    Point end;
    std::list<Point*> path;


	int m_nRows;						//行数
    int m_nColumes;                     //列数
    bool m_bIsHaveRobot=false;
    bool m_bIsHaveGoal=false;

	QVector<QVector<Item*>> m_items;	//所有元素
	bool m_bGameFail;					//是否是游戏失败，失败了需要显示雷

    Map map_;
    std::vector<PathPoint> h_path_;
};

#endif // MAINWINDOW_H
