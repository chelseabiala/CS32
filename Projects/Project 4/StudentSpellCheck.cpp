#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <fstream>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::~StudentSpellCheck()
{
    m_trie.~Trie();
}

bool StudentSpellCheck::load(std::string dictionaryFile)
{
    std::ifstream infile(dictionaryFile);
    if(!infile)
        return false;
    std::string s;
    while(std::getline(infile, s))
    {
        Trie* temp = &m_trie;
        for(int i = 0; i < s.length(); i++) // Iterate through each word
        {
            bool found = false; int j = 0;
            for(; j < 27;)
            {
                if(temp->children[j] == nullptr)    // There is not yet a spot for the current letter
                {
                    found = false;
                    break;
                }
                if(temp->children[j]->m_data == s[i])   // Current letter already in dictionary, move to next
                {
                    temp = temp->children[j];
                    j = 0; i++;
                    found = true;
                }
                else
                    j++;
            }
            if(!found)
            {
                bool b = false;
                if(i == s.length() - 1)     // If is end of a word, mark as such
                    b = true;
                temp->children[j] = new Trie(s[i], b);  // Add current letter to the dictionary
                temp = temp->children[j];
            }
        }
    }
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
    bool b = find(word);    // Checks if word is in the dictionary
    if(!b)
    {
    int c = 0; int suggestionsCount = 0;
    while(c < word.length() && suggestionsCount < max_suggestions)
    {
        for(int i = 0; i < 26; i++)     // Replace each position of word with each letter of alphabet
        {
            std::string s = word;
            s[c] = 'a' + i;
            if(find(s) && s != word)    // If the replaced word is in the dictionary, add it to suggestions
            {
                suggestions.push_back(s);
                suggestionsCount++;
            }
            if(suggestionsCount == max_suggestions)
                break;
        }
        std::string s = word;
        s[c] = '\'';        // Same as above but with apostrophes
        if(find(s) && s != word)
        {
            suggestions.push_back(s);
            suggestionsCount++;
        }
        c++;
    }
    }
    return b;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems)
{
    int start; int end;
    for(int i = 0; i < line.length();)
    {
        std::string word; start = i;
        while(isalpha(line[i]) || line[i] == '\'')      // Separates each word
        {
            word += tolower(line[i]);       // Lowercase for comparisons
            i++;
        }
        end = i - 1;
        if(!find(word))     // If word isn't in dictionary, push onto problems
        {
            Position p; p.start = start; p.end = end;
            problems.push_back(p);
        }
        i++;
    }
}
