#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
    StudentSpellCheck() {}
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
    struct Trie
    {
        Trie() : m_data('\0'), is_end(false)    // Default constructor for root triee
        {
            for(int i = 0; i < 27; i++)
            {
                children[i] = nullptr;
            }
        };
        Trie(char d, bool b) : m_data(d), is_end(b)     // Constructor for each node trie
        {
            for(int i = 0; i < 27; i++)
            {
                children[i] = nullptr;
            }
        };
        ~Trie()
        {
            for(int i = 0; i < 27 && children[i] != nullptr; i++)
                delete children[i];
        }
        Trie* children[27];
        char m_data;
        bool is_end;
    };
    bool find(const std::string word)
    {
        Trie* temp = &m_trie; bool found = false;
        int i = 0;
            for(int j = 0; j < 27 && i < word.length();)
            {
                if(temp->children[j] == nullptr)    // If letter isn't found in children, word is not found
                {
                    found = false;
                    break;
                }
                if(temp->children[j]->m_data == tolower(word[i]))   // Letter is in dictionary
                {
                    if(i == word.length() - 1 && temp->children[j]->is_end) // Found if end of word is marked as such in dictionary
                        found = true;
                    temp = temp->children[j];
                    j = 0; i++;
                }
                else
                    j++;
            }
        return found;
    }
    Trie m_trie;
};

#endif  // STUDENTSPELLCHECK_H_
