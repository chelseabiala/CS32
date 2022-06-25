#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch)
{
    if(m_tasks.empty() || (m_tasks.top()->action == action && abs(row - m_tasks.top()->row) == 0 && abs(col - m_tasks.top()->col) <= 1))
        m_count++;      // If is a batching action, increases count
    else
        m_count = 1;    // If action shouldn't be batched, resets count
    m_tasks.push(new ActionHelper(action, row, col, ch));   // Pushes action onto stack
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text)
{
    if(!m_tasks.empty())
    {
        ActionHelper* a = m_tasks.top();    // Inspects last inserted action
        row = a->row;
        col = a->col;
        count = m_count;
        std::string s = ""; s += a->ch;
        text = s;
        m_tasks.pop();
        return a->action;
    }
    else return ERROR;
}

void StudentUndo::clear()
{
	while(!m_tasks.empty())     // Empties stack
        m_tasks.pop();
    m_count = 0;
}
