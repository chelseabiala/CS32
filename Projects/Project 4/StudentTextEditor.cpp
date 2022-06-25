#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo), m_editingPosRow(0), m_editingPosCol(0)
{
    std::string s = ""; // Creates a line for non-loaded documents
    m_text.push_back(s);
    m_rowIter = m_text.begin();
}

StudentTextEditor::~StudentTextEditor()
{
    m_text.clear();
    getUndo()->clear();
}

bool StudentTextEditor::load(std::string file)
{
    std::ifstream infile(file);
    if(!infile)
        return false;
    reset();
    std::string s;
    while(std::getline(infile, s))
    {
        if(s[s.size() - 1] == '\r')     // Removes line characters
            s.pop_back();
        m_text.push_back(s);        // Puts each line into list
    }
    m_rowIter = m_text.begin();     // Initializes cursor to correct position
    return true;
}

bool StudentTextEditor::save(std::string file)
{
    std::ofstream outfile(file);
    if(!outfile)
        return false;
    for(std::list<std::string>::iterator it = m_text.begin(); it != m_text.end(); it++)
    {
        outfile << *it << '\n';     // Copies each line into file
    }
    return true;
}

void StudentTextEditor::reset()
{
    m_text.clear();
    m_editingPosRow = 0;
    m_editingPosCol = 0;
    getUndo()->clear();
}

void StudentTextEditor::move(Dir dir)
{
    switch(dir)
    {
        case UP:
            if(m_editingPosRow > 0)     // Moves cursor up if appropriate
            {
                m_editingPosRow--;
                int temp = (*m_rowIter).length();
                m_rowIter--;
                if(temp > (*m_rowIter).size() && m_editingPosCol > (*m_rowIter).size())
                    m_editingPosCol = (*m_rowIter).length();
            }
            break;
        case DOWN:      // Moves cursor down if appropriate
            if(m_editingPosRow < m_text.size() - 1)
            {
                m_editingPosRow++;
                int temp2 = (*m_rowIter).length();
                m_rowIter++;
                if(temp2 > (*m_rowIter).size() && m_editingPosCol > (*m_rowIter).size())
                    m_editingPosCol = (*m_rowIter).length();
            }
            break;
        case LEFT:      // Moves cursor left if appropriate
            if(m_editingPosCol == 0 && m_editingPosRow == 0)
                break;
            if(m_editingPosCol > 0)
                m_editingPosCol--;
            else if(m_editingPosRow > 0)
            {
                m_editingPosRow--;
                m_rowIter--;
                m_editingPosCol = (*m_rowIter).length();
            }
            break;
        case RIGHT:     // Moves cursor right if appropriate
            if(m_editingPosRow == m_text.size() && m_editingPosCol == (*m_rowIter).size())
                break;
            if(m_editingPosCol < (*m_rowIter).size())
                m_editingPosCol++;
            else if(m_editingPosRow < m_text.size())
            {
                m_editingPosRow++;
                m_rowIter++;
                m_editingPosCol = 0;
            }
            break;
        case HOME:      // Moves cursor to beginning of line
            m_editingPosCol = 0;
            break;
        case END:       // Moves cursor to end of line
            m_editingPosCol = (*m_rowIter).length();
            break;
    }
}

void StudentTextEditor::del()
{
    if(m_editingPosCol != (*m_rowIter).length())        // Deletes characters in line
    {
        char c = (*m_rowIter)[m_editingPosCol];
        (*m_rowIter).erase(m_editingPosCol, 1);
        getUndo()->submit(Undo::DELETE, m_editingPosRow, m_editingPosCol, c);
    }
    else if(m_editingPosRow < m_text.size())        // Joins two lines by deleting newline
    {
        (*m_rowIter).append(*(++m_rowIter));
        m_rowIter = m_text.erase(m_rowIter);
        m_rowIter--;
        getUndo()->submit(Undo::JOIN, m_editingPosRow, m_editingPosCol);
    }
}

void StudentTextEditor::backspace()
{
    if(m_editingPosCol != 0 && (*m_rowIter) != "")  // Removes character behind cursor
    {
        char c = (*m_rowIter)[m_editingPosCol - 1];
        (*m_rowIter).erase(m_editingPosCol - 1, 1);
        m_editingPosCol--;
        getUndo()->submit(Undo::DELETE, m_editingPosRow, m_editingPosCol, c);
    }
    else if(m_editingPosRow > 0)        // Joins two lines by backspacing newline
    {
        (*(--m_rowIter)).append(*(++m_rowIter));
        int temp = (*m_rowIter).length();
        m_rowIter = m_text.erase(m_rowIter);
        m_rowIter--; m_editingPosRow--; m_editingPosCol = (*m_rowIter).length() - temp;
        getUndo()->submit(Undo::JOIN, m_editingPosRow, m_editingPosCol);
    }
}

void StudentTextEditor::insert(char ch)
{
    std::string s = "";
    if(ch == '\t')
    {
        for(int i = 0; i < 4; i++)      // Loops 4 times for 4 spaces in tab character
        {
            s = " ";
            (*m_rowIter).insert(m_editingPosCol, s);
            m_editingPosCol++;
            getUndo()->submit(Undo::INSERT, m_editingPosRow, m_editingPosCol, (*m_rowIter)[m_editingPosRow - s.length()]);
        }
    }
    else
    {
        s += ch;        // Inserts a single character into list
        (*m_rowIter).insert(m_editingPosCol, s);
        m_editingPosCol += s.length();
        getUndo()->submit(Undo::INSERT, m_editingPosRow, m_editingPosCol, (*m_rowIter)[m_editingPosRow - s.length()]);
    }
}

void StudentTextEditor::enter()
{
    getUndo()->submit(Undo::SPLIT, m_editingPosRow, m_editingPosCol);
    if(m_editingPosCol == (*m_rowIter).length())    // Creates a newline with nothing on it
        m_text.insert(++m_rowIter, "");
    else    // Splits line into two and copies text from end of original to newline
    {
        std::string temp = (*m_rowIter).substr(m_editingPosCol, (*m_rowIter).length() - m_editingPosCol);
        (*m_rowIter).erase(m_editingPosCol, (*m_rowIter).length() - m_editingPosCol);
        m_text.insert(++m_rowIter, temp);
    }
    m_rowIter--;
    m_editingPosRow++;
    m_editingPosCol = 0;
}

void StudentTextEditor::getPos(int& row, int& col) const
{
    row = m_editingPosRow;
    col = m_editingPosCol;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const
{
    if(startRow < 0 || numRows < 0 || startRow < lines.size())
        return -1;
    int count = 0;
    std::list<std::string> temp = m_text; // So const list is not altered
    for(std::list<std::string>::iterator it = temp.begin(); it != temp.end(); it++)
    {
        if(count >= startRow)
            lines.push_back(*it);       //  Adds each line in list to given vector
        count++;
        if(count == numRows)
            break;
    }
    return count;
}

void StudentTextEditor::undo()
{
    int row; int col; int count; std::string text; Undo::Action a; int tempHolder; std::string st = "";
    bool helper = false;
    a = getUndo()->get(row, col, count, text);  // Get first (or only) action to undo
    m_rowIter = m_text.begin(); m_editingPosRow = 0;
    while(m_editingPosRow < row)
    {
        m_rowIter++; m_editingPosRow++;     // Puts cursor in correct spot to correct undo
    }
    m_editingPosCol = col;
    while(count > 0 && a != Undo::ERROR)    // Until batch is finished
    {
    switch (a)
    {
        case Undo::INSERT:
            backspace();        // Delete inserted characters
            getUndo()->get(row,col,tempHolder,text);    // Removes reversed action from stack of undo actions
            break;
        case Undo::DELETE:
            st += text[0];  // Creates string of characters to reinsert later
            break;
        case Undo::JOIN:
            enter();    // Enters to split two joined lines
            getUndo()->get(row,col,tempHolder,text);    // Removes reversed action from stack of undo actions
            break;
        case Undo::SPLIT:
            del();  // Deletes the newline character, rejoins split lines
            getUndo()->get(row,col,tempHolder,text);    // Removes reversed action from stack of undo actions
            break;
        case Undo::ERROR:
            break;
    }
        int r = row; int c = col;
        a = getUndo()->get(row,col,tempHolder,text);
        if(r == row && c == col)
            helper = true;      // Determines whether DELETE action was a backspace or a del
        count--;
    }
    for(int i = 0; i < st.length(); i++)    // Reinserts characters
    {
        if(!helper)
            insert(st[i]);
        else
            insert(st[st.length() - 1 - i]);       // Backwards if it was a del
        getUndo()->get(row,col,tempHolder,text);
    }
}
