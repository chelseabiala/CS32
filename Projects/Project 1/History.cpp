
#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
: m_rows(nRows), m_cols(nCols)
{
    for(int i = 0; i < nRows; i++)
    {
        for(int j = 0; j < nCols; j++)
        {
            m_display[i][j] = 0;
        }
    }
}

bool History::record(int r, int c)
{
    if(r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
        return false;
    m_display[r-1][c-1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    for(int i = 0; i < m_rows; i++)
    {
        for(int j = 0; j < m_cols; j++)
        {
            if(m_display[i][j] == 0)
                cout << '.';
            else if(m_display[i][j] <= 26)
            {
                char ch = m_display[i][j] + 64;
                cout << ch;
            }
            else if(m_display[i][j] > 26)
                cout << 'Z';
        }
        cout << endl;
    }
    cout << endl;
}
 
