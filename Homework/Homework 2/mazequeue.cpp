
#include <queue>
#include <string>
#include <iostream>

using namespace std;

class Coord
{
    public:
        Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
        int r() const { return m_row; }
        int c() const { return m_col; }
    private:
        int m_row;
        int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> coordq;
    Coord a(sr,sc);
    coordq.push(a);
    maze[sr][sc] = 'C';
    while(!coordq.empty())
    {
        Coord b = coordq.front();
        coordq.pop();
        if(b.r() == er && b.c() == ec)
            return true;
        if(maze[b.r()-1][b.c()] == '.')
        {
            Coord c(b.r()-1,b.c());
            coordq.push(c);
            maze[b.r()-1][b.c()] = 'C';
        }
        if(maze[b.r()][b.c()+1] == '.')
        {
            Coord d(b.r(),b.c()+1);
            coordq.push(d);
            maze[b.r()][b.c()+1] = 'C';
        }
        if(maze[b.r()+1][b.c()] == '.')
        {
            Coord e(b.r()+1,b.c());
            coordq.push(e);
            maze[b.r()+1][b.c()] = 'C';
        }
        if(maze[b.r()][b.c()-1] == '.')
        {
            Coord f(b.r(),b.c()-1);
            coordq.push(f);
            maze[b.r()][b.c()-1] = 'C';
        }
    }
    return false;
}
