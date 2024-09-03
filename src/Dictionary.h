#pragma once
#include <stdexcept>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>

const int ALPHABET_SIZE = 'Z' - 'A' + 1;

class incorrect_word_exception : public std::runtime_error {
public:
    incorrect_word_exception()
        : runtime_error("incorrect word")
    {
        // Nothing to do here
    }
};

class Node {
public:
    Node();
    Node(bool endOfWord);
    ~Node();
    Node(const Node& other);
    Node& operator=(const Node& other);
    void clean();
private:
    
    void copy(const Node& other);
public:
    Node* successors[ALPHABET_SIZE];
    bool isEndOfWord;
};
class Dictionary {
public:
    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary& other) = delete;
    Dictionary& operator=(const Dictionary& other) = delete;
public:
    void insert(const char* word);
    void erase(const char* word) noexcept;
    bool contains(const char* word) const noexcept;
    size_t size() const noexcept;
    static bool isCorrectWord(const char* word) noexcept;

    static bool isCorrectWord(const std::string& word) noexcept;
    bool isEmpty()const;

private:
    Node* root;
    size_t count;

    //TODO: Add your implementation details
};
char* toLowerCase(const std::string& s);

void loadDictFile(const std::string& fileName, Dictionary& dictionary,
    int& correctDictWords, int& incorrectDictWords);

void loadFiltFile(const std::string& fileName, Dictionary& dictionary, int& correctFiltWords,
    int& incorrectFiltWords);

void loadTextFile(const std::string& fileName, Dictionary& dictionary, int& correctTextWords,
    int& incorrectTextWords, int& allTextWords);