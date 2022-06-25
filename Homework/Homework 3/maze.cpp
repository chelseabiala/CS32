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
    if(sr == er && sc == ec)
        return true;
    maze[sr][sc] = 'C';
    Coord b(sr, sc);
    if(maze[b.r()-1][b.c()] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
           return true;
    }
    if(maze[b.r()][b.c()+1] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    }
    if(maze[b.r()+1][b.c()] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    }
    if(maze[b.r()][b.c()-1] == '.')
    {
        if(pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    }
    return false;
}
