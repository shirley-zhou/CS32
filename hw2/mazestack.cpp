#include <string>
#include <stack>
#include <iostream>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
	stack<Coord> coordStack;
	coordStack.push(Coord(sr,sc));
	maze[sr][sc] = '*';
	while (!coordStack.empty())
	{
		Coord curr = coordStack.top();
		coordStack.pop();
		if (curr.r() == er && curr.c() == ec)
			return true;
		//if can move north
		if (maze[curr.r()-1][curr.c()] != 'X' && maze[curr.r()-1][curr.c()] != '*')
		{
			coordStack.push( Coord(curr.r()-1, curr.c() ));
			maze[curr.r()-1][curr.c()] = '*';
		}
		//if can move east
		if (maze[curr.r()][curr.c()+1] != 'X' && maze[curr.r()][curr.c()+1] != '*')
		{
			coordStack.push( Coord(curr.r(), curr.c()+1 ));
			maze[curr.r()][curr.c()+1] = '*';
		}
		//if can move south
		if (maze[curr.r()+1][curr.c()] != 'X' && maze[curr.r()+1][curr.c()] != '*')
		{
			coordStack.push( Coord(curr.r()+1, curr.c() ));
			maze[curr.r()+1][curr.c()] = '*';
		}
		//if can move west
		if (maze[curr.r()][curr.c()-1] != 'X' && maze[curr.r()][curr.c()-1] != '*')
		{
			coordStack.push( Coord(curr.r(), curr.c()-1 ));
			maze[curr.r()][curr.c()-1] = '*';
		}
	}
	return false; //no solution
}

/*
int main()
        {
            string maze[10] = {
                "XXXXXXXXXX",
                "X........X",
                "XX.X.XXXXX",
                "X..X.X...X",
                "X..X...X.X",
                "XXXX.XXX.X",
                "X.X....XXX",
                "X..XX.XX.X",
                "X...X....X",
                "XXXXXXXXXX"
            };
        
            if (pathExists(maze, 10,10, 6,4, 1,1))
                cout << "Solvable!" << endl;
            else
                cout << "Out of luck!" << endl;
        }
*/