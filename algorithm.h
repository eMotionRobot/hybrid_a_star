#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "utils.h"
#include "map.h"
#include "state.h"
//#include "gui.h"

class Algorithm {
public:
  static State initial;
  static State goal;
  static int **obs_map;
  static int **grid_obs_map;
  static double **shortest_2d;

  static int scale_map_x_;
  static int scale_map_y_;

  static int DX_;
  static int DY_;
  /**
   * Constructor
   */
  Algorithm(Map map);

  void updateInitial(State initial);

  void updateGoal(State goal);

  /*****************************************************************************
   * heuristic functions
   ****************************************************************************/

  // Based on Dijkstra or A* planning
  static double holonomicWithObs(State src);

  // Use Dubin's path length ignoring obstacles
  static double nonHolonomicWithoutObs(State src);

  /*****************************************************************************
   * Hybrid A* Planning
   *   - on the 3d map
   *   - state: [x, y, theta]
   ****************************************************************************/
  std::vector<PathPoint>  hybridAstarPlanning();

private:
  Map m_map;

  /*****************************************************************************
   * A* Planning
   *   - on the 2d grid map
   *   - state: [x, y]
   ****************************************************************************/
  void astarPlanning();
  State* previous_;
};

#endif // ALGORITHM_H
