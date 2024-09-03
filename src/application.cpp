#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

/// Display how to use the program from the command line
void displayUsage(const char* executablePath)
{
  try {
    fs::path ep(executablePath);
    
    std::cout
      << "Usage:\n\t"
      << ep.filename()
      << " <dictionary> <filter> <text>"
      << std::endl;
  }
  catch (...) {
    std::cout << "Cannot parse path from argv[0]";
  }
}

int main(int argc, char* argv[])
{
  if(argc < 4) {
    displayUsage(argv[0]);
    return 1;
  }

  Dictionary dictionary;

  int correctDictWords = 0;
  int incorrectDictWords = 0;
  loadDictFile(argv[1], dictionary, correctDictWords, incorrectDictWords);
  std::cout << std::endl;

  int correctFiltWords = 0;
  int incorrectFiltWords = 0;
  loadFiltFile(argv[2], dictionary, correctFiltWords, incorrectFiltWords);
  std::cout << std::endl;

  int correctTextWords = 0;
  int incorrectTextWords = 0;
  int allTextWords = 0;
  loadTextFile(argv[3], dictionary, correctTextWords, incorrectTextWords, allTextWords);
  std::cout << std::endl;

  std::cout << "Statistics:" << std::endl;
  std::cout << "    Dictionary entries: " << correctDictWords << " correct, " << incorrectDictWords << " incorrect" << std::endl;
  std::cout << "        Filter entries: " << correctFiltWords << " correct, " << incorrectFiltWords << " incorrect" << std::endl;
  std::cout << "  Resultant dictionary: " << allTextWords << std::endl;
  std::cout << "         Words in text: " << correctTextWords << " correct, " << incorrectTextWords << " incorrect" << std::endl;

  //TODO implement the program

  return 0;
}