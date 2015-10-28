#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"
class Pit;

class History
{
	public:
		History(int nRows, int nCols);
		bool record(int r, int c);
		void display() const;
	private:
		int m_rows; //necessary?
		int m_cols; //necessary?
		//Pit* m_pit;
		int m_grid[MAXROWS][MAXCOLS];
};
#endif //HISTORY_INCLUDED