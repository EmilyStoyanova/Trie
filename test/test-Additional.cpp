#include "catch2/catch_all.hpp"
#include "Dictionary.h"
#include <sstream>

//TODO Add your own unit tests to this file,
// or create additional files in this directory.

// TEST_CASE("Sample test")
// {
//     CHECK(true);
// }


TEST_CASE("Node class: constructor, copy constructor, assignment operator")
{
    Node original(true);
    REQUIRE(original.isEndOfWord == true);

    Node copy = original; 
    REQUIRE(copy.isEndOfWord == true);

    Node assigned;
    assigned = original; 
    REQUIRE(assigned.isEndOfWord == true);
}

TEST_CASE("Node class: clean() and copy() functions")
{
    Node original(true);
    original.successors[0] = new Node(false); 
    original.successors[1] = new Node(true);  

    Node copy = original;
    REQUIRE(copy.successors[0] != nullptr);
    REQUIRE(copy.successors[1] != nullptr);

    original.clean();
    REQUIRE(original.successors[0] == nullptr);
    REQUIRE(original.successors[1] == nullptr);
    REQUIRE(copy.successors[0] != nullptr); 
    REQUIRE(copy.successors[1] != nullptr);
}

TEST_CASE("toLowerCase() converts string to lowercase")
{
    std::string original = "HeLLoWorLD";
    char* lowercased = toLowerCase(original);
    REQUIRE(std::string(lowercased) == "helloworld");
    delete[] lowercased;
}

TEST_CASE("Dictionary::isEmpty() returns false for a non-empty dictionary")
{
    Dictionary dict;
    dict.insert("test");
    REQUIRE(!dict.isEmpty());
}

TEST_CASE("Dictionary::isEmpty() returns true for an empty dictionary")
{
    Dictionary dict;
    REQUIRE(dict.isEmpty());
}

TEST_CASE("loadDictFile() tracks correct and incorrect words")
{
    Dictionary dict;
    int correctDictWords = 0;
    int incorrectDictWords = 0;
    std::stringstream ss("# Comment\nhello\n123world\nworld!\nabc\n");
    std::ofstream outFile("test_dict.txt");
    outFile << ss.str();
    outFile.close();

    loadDictFile("test_dict.txt", dict, correctDictWords, incorrectDictWords);

    REQUIRE(correctDictWords == 2);
    REQUIRE(incorrectDictWords == 2);
    REQUIRE(dict.size() == 2);

    remove("test_dict.txt");
}
TEST_CASE("loadDictFile() handles empty dictionary file")
{
    Dictionary dict;
    int correctDictWords = 0;
    int incorrectDictWords = 0;

    std::ofstream outFile("empty_dict.txt");
    outFile.close();

    loadDictFile("empty_dict.txt", dict, correctDictWords, incorrectDictWords);

    REQUIRE(correctDictWords == 0);
    REQUIRE(incorrectDictWords == 0);
    REQUIRE(dict.size() == 0);

    remove("empty_dict.txt");
}

TEST_CASE("loadFiltFile() tracks correct and incorrect words")
{
    Dictionary dict;
    dict.insert("hello");
    dict.insert("world");

    int correctFiltWords = 0;
    int incorrectFiltWords = 0;
    std::stringstream ss("# Comment\nworld\ninvalid#word\n");
    std::ofstream outFile("test_filter.txt");
    outFile << ss.str();
    outFile.close();

    loadFiltFile("test_filter.txt", dict, correctFiltWords, incorrectFiltWords);

    REQUIRE(correctFiltWords == 1);
    REQUIRE(incorrectFiltWords == 1);
    REQUIRE(dict.size() == 1); 

    remove("test_filter.txt");
}
TEST_CASE("loadFiltFile() handles empty filter file")
{
    Dictionary dict;
    dict.insert("hello");

    int correctFiltWords = 0;
    int incorrectFiltWords = 0;

    std::ofstream outFile("empty_filter.txt");
    outFile.close();

    loadFiltFile("empty_filter.txt", dict, correctFiltWords, incorrectFiltWords);

    REQUIRE(correctFiltWords == 0);
    REQUIRE(incorrectFiltWords == 0);
    REQUIRE(dict.size() == 1); 

    remove("empty_filter.txt");
}


TEST_CASE("loadTextFile() tracks correct, incorrect, and all words in text")
{
    Dictionary dict;
    dict.insert("hello");
    dict.insert("world");

    int correctTextWords = 0;
    int incorrectTextWords = 0;
    int allTextWords = 0;

    std::stringstream ss("hello there\nworld this is a test\n");
    std::ofstream outFile("test_text.txt");
    outFile << ss.str();
    outFile.close();

    loadTextFile("test_text.txt", dict, correctTextWords, incorrectTextWords, allTextWords);

    REQUIRE(correctTextWords == 2);   
    REQUIRE(incorrectTextWords == 5); 
    REQUIRE(allTextWords == 7);

    remove("test_text.txt");
}
TEST_CASE("loadTextFile() handles empty text file")
{
    Dictionary dict;
    dict.insert("hello");
    dict.insert("world");

    int correctTextWords = 0;
    int incorrectTextWords = 0;
    int allTextWords = 0;

    std::ofstream outFile("empty_text.txt");
    outFile.close();

    loadTextFile("empty_text.txt", dict, correctTextWords, incorrectTextWords, allTextWords);

    REQUIRE(correctTextWords == 0);
    REQUIRE(incorrectTextWords == 0);
    REQUIRE(allTextWords == 0);

    remove("empty_text.txt");
}

TEST_CASE("Program behavior: Load dictionary, apply filter, process text file with unique 'beach'")
{
    
    Dictionary dict;
    int correctDictWords = 0;
    int incorrectDictWords = 0;

    std::stringstream dictStream("ball\nbeach\nsun\n123fruit\n");
    std::ofstream dictFile("test_dict.txt");
    dictFile << dictStream.str();
    dictFile.close();

    loadDictFile("test_dict.txt", dict, correctDictWords, incorrectDictWords);

    REQUIRE(correctDictWords == 3);  
    REQUIRE(incorrectDictWords == 1); 
    REQUIRE(dict.size() == 3);

    
    int correctFiltWords = 0;
    int incorrectFiltWords = 0;

    std::stringstream filtStream("ball\n123\nsun\n");
    std::ofstream filtFile("test_filter.txt");
    filtFile << filtStream.str();
    filtFile.close();

    loadFiltFile("test_filter.txt", dict, correctFiltWords, incorrectFiltWords);

    REQUIRE(correctFiltWords == 2);   
    REQUIRE(incorrectFiltWords == 1); 
    REQUIRE(dict.size() == 1);        

    
    int correctTextWords = 0;
    int incorrectTextWords = 0;
    int allTextWords = 0;

    std::stringstream textStream("apple\nbeach\nchair\norange\n");
    std::ofstream textFile("test_text.txt");
    textFile << textStream.str();
    textFile.close();

    loadTextFile("test_text.txt", dict, correctTextWords, incorrectTextWords, allTextWords);

    REQUIRE(correctTextWords == 1);   
    REQUIRE(incorrectTextWords == 3); 
    REQUIRE(allTextWords == 4);       

   
    remove("test_dict.txt");
    remove("test_filter.txt");
    remove("test_text.txt");
}
