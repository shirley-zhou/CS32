#include <stack>
using namespace std;
        
int main()
{
            stack<Coord> coordStack;     // declare a stack of Coords
        
            Coord a(5,6);
            coordStack.push(a);          // push the coordinate (5,6)
            coordStack.push(Coord(3,4)); // push the coordinate (3,4)
            ...
            Coord b = coordStack.top();  // look at top item in the stack
            coordStack.pop();            // remove the top item from stack
            if (coordStack.empty())      // Is the stack empty?
                cout << "empty!" << endl;
            cout << coordStack.size() << endl;  // num of elements
}