#ifndef STATE_H
#define STATE_H

#include "utils.h"

struct PathPoint
{
  PathPoint():gx(0.0), gy(0.0), gtheta(0.0){}
  PathPoint(const int x, const int y, const int theta):gx(x), gy(y), gtheta(theta){}
  int gx;
  int gy;
  int gtheta;
};

class State {
public: // methods
  /**
   * Constructor
   */
  State();

  /**
   * Constructor
   */
  State(double x, double y, double theta);

  /**
   * get the states of the next move
   *
   */
  std::vector<State> getNextStates();
  static double resolution;
public: // variables
  // 3d state on the geometric map
  double x;
  double y;
  double theta;

  // gx, gy and gtheta are co-ordinates in the 80X80 grid
  int gx;
  int gy;
  int gtheta;

  // for 2d planning
  int dx;
  int dy;

  double cost2d;
  double cost3d;

  double change;
  double velocity;

  State *prev;
  State *next;
};

#endif // STATE_H
