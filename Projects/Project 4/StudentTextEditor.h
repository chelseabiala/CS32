#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>

class Undo;

class StudentTextEditor : public TextEditor {
public:

    StudentTextEditor(Undo* undo);
    ~StudentTextEditor();
    bool load(std::string file);
    bool save(std::string file);
    void reset();
    void move(Dir dir);
    void del();
    void backspace();
    void insert(char ch);
    void enter();
    void getPos(int& row, int& col) const;
    int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
    void undo();

private:
    int m_editingPosRow;
    int m_editingPosCol;
    std::list<std::string> m_text;
    std::list<std::string>::iterator m_rowIter;
};

#endif // STUDENTTEXTEDITOR_H_
