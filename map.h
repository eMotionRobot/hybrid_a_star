#ifndef MAP_H
#define MAP_H

#include "state.h"
#include "utils.h"

struct Node {
  int x;
  int y;
  int nearest_obstacle;
};

class Map {
public:
  /**
   * Constructor
   *
   */
  Map() {}

  /**
   * Constructor
   *
   */
  Map(std::string map_file);
  bool initMap(std::vector<std::vector<int>>& maze_map);
  
  void initCollisionChecker();

  bool checkCollision(State pos);

  void findNearObs();

  int nearestObstacleDistance(State pos);

  bool isBoundaryObstacle(int i, int j);

public:
  int **obs_map;
  int **acc_obs_map;
  int **nearest_obstacle;
  int obs_dist_max;

  int map_x_ = 800;
  int map_y_ = 800;
  
  // define grid map size and resolution
  int grid_x_ = 80;
  int grid_y_ = 80;
  int grid_res_ = 10;
};

#endif // MAP_H
