/*
#include <string>
#include <iostream>
using namespace std;
*/

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
	if (sr < 0  ||  sr > nRows  ||  sc < 0  || sc > nCols  ||
        er < 0  ||  er > nRows  ||  ec < 0  || ec > nCols  ||
        maze[sr][sc] != '.'  ||  maze[er][ec] != '.')
       return false;
	if (sr == er && sc == ec)
		return true;
	maze[sr][sc] = 'o';
	if (maze[sr-1][sc] == '.') //north
		if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec) )
			return true;
	if (maze[sr][sc+1] == '.') //east
		if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec) )
			return true;
	if (maze[sr+1][sc] == '.') //south
		if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec) )
			return true;
	if (maze[sr][sc-1] == '.') //west
		if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec) )
			return true;
	return false;
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