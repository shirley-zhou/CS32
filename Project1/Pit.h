#ifndef PIT_INCLUDED
#define PIT_INCLUDED

#include <string>
using namespace std;

#include "globals.h"
#include "History.h"

class Player;
class Snake;

class Pit
{
  public:
        // Constructor/destructor
    Pit(int nRows, int nCols);
    ~Pit();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
	History& history();
    int     snakeCount() const;
    int     numberOfSnakesAt(int r, int c) const;
    void    display(string msg) const;

        // Mutators
    bool   addSnake(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyOneSnake(int r, int c);
    bool   moveSnakes();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Snake*  m_snakes[MAXSNAKES];
	History m_history;
    int     m_nSnakes;
};
#endif //PIT_INCLUDED