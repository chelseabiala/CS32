#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include "stack"

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    struct ActionHelper     // To put in stack and store all needed information
    {
        ActionHelper(Action a, int r, int c, char cha = 0)
        : action(a), row(r), col(c), ch(cha) {};
        Action action;
        int row;
        int col;
        char ch;
    };
    std::stack<ActionHelper*> m_tasks;
    int m_row;
    int m_col;
    int m_count;
};

#endif // STUDENTUNDO_H_
