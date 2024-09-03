#include "Dictionary.h"

#include <cctype> // Contains std::isalpha()

//TODO Implement the Dictionary class

Node::Node() :isEndOfWord(false)
{
	for (size_t i = 0; i < ALPHABET_SIZE; i++)
	{
		successors[i] = nullptr;
	}
}
Node::Node(bool endOfWord) : isEndOfWord(endOfWord) {
	for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
		successors[i] = nullptr;
	}
}
Node::~Node()
{
	clean();
}

Node::Node(const Node& other)
{
	copy(other);
}

Node& Node::operator=(const Node& other)
{
	if (this != &other) {
		clean();
		copy(other);
	}
	return *this;
}

void Node::clean()
{
	for (size_t i = 0; i < ALPHABET_SIZE; i++)
	{
		if (successors[i] != nullptr)
		{
			delete successors[i];
			successors[i] = nullptr;  
		}
	}
}

void Node::copy(const Node& other)
{
	isEndOfWord = other.isEndOfWord;
	for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
		if (other.successors[i]) {
			successors[i] = new Node(*other.successors[i]);
		}
		else {
			successors[i] = nullptr;
		}
	}
}

Dictionary::Dictionary()
{
	root = new Node(); 
	count = 0;
}

Dictionary::~Dictionary()
{
	delete root;
}

void Dictionary::insert(const char* word)
{
	if (word == nullptr || !isCorrectWord(word)) {
		throw incorrect_word_exception();
	}
	Node* current = root;
	while (*word) {
		int index = *word - 'a';
		if (!current->successors[index]) {
			current->successors[index] = new Node();
		}
		current = current->successors[index];
		++word;
	}
	if (!current->isEndOfWord) {
		current->isEndOfWord = true;
		++count;
	}
}

void Dictionary::erase(const char* word) noexcept
{
	if (word == nullptr || !isCorrectWord(word) || !contains(word)) {
		return; 
	}
	Node* current = root;
	while (*word) {
		int index = *word - 'a';
		if (!current->successors[index]) {
			return; 
		}
		current = current->successors[index];
		++word;
	}

	if (current->isEndOfWord) {
		current->isEndOfWord = false;
		--count;
	}
}

bool Dictionary::contains(const char* word) const noexcept
{
	if (word == nullptr || !isCorrectWord(word)) {
		return false;
	}

	Node* current = root;
	while (*word) {
		int index = *word - 'a';
		if (!current->successors[index]) {
			return false;
		}
		current = current->successors[index];
		++word;
	}

	return current != nullptr && current->isEndOfWord;
}

size_t Dictionary::size() const noexcept
{
	return count;
}

bool Dictionary::isCorrectWord(const char* word) noexcept
{
	if (word == nullptr || *word == '\0') {
		return false;
	}
	while (*word) {
		if (!isalpha(*word)) {
			return false;
		}
		++word;
	}
	return true;
}

bool Dictionary::isCorrectWord(const std::string& word) noexcept
{
	return isCorrectWord(word.c_str());
}

bool Dictionary::isEmpty()const
{
	return size() == 0;
}
char* toLowerCase(const std::string& s) {
	char* result = new char[s.size() + 1];
	std::strcpy(result, s.c_str());
	for (size_t i = 0; i < s.size(); ++i) {
		if (result[i] >= 'A' && result[i] <= 'Z') {
			result[i] = result[i] + ('a' - 'A');
		}
	}
	result[s.size()] = '\0';
	return result;
}

void loadDictFile(const std::string& fileName, Dictionary& dictionary, int& correctDictWords, int& incorrectDictWords)
{

	std::cout << "Loading dictionary from " << fileName << std::endl;
	std::ifstream file(fileName);
	if (!file.is_open()) {
		return; 
	}
	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	if (fileSize == 0) {
		std::cout << "The dictionary is empty." << std::endl;
		return;
	}
	file.seekg(0, std::ios::beg);
	std::string line;
	int numLine = 0;
	while (getline(file, line)) {
		if (line.empty() || line[0] == '#')
		{
			continue;
		}
		numLine++;
		if (line.find(' ') != std::string::npos) {
			incorrectDictWords++;
			std::cout << "ERROR: incorrect entry " << line << " on line " << numLine << std::endl;
			continue;
		}
		std::istringstream iss(line);
		std::string word;
		while (iss >> word) {
			char* lowerWord = toLowerCase(word);
			if (Dictionary::isCorrectWord(lowerWord)) {
				correctDictWords++;
				dictionary.insert(lowerWord);
			}
			else {
				std::cout << "ERROR: incorrect entry " << line << " on line " << numLine << std::endl;
				incorrectDictWords++;
			}
			delete[] lowerWord;
		}
	}
	file.close();
}

void loadFiltFile(const std::string& fileName, Dictionary& dictionary, int& correctFiltWords,
	int& incorrectFiltWords)
{
	std::cout << "Removing the words listed at " << fileName << std::endl;
	if (dictionary.isEmpty()) {
		std::cout << "The requested operation could not be performed because the dictionary is empty." << std::endl;
		return;
	}
	std::ifstream file(fileName);
	if (!file.is_open()) {
		return; 
	}
	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	if (fileSize == 0) {
		std::cout << "The filter is empty." << std::endl;
		return;
	}
	file.seekg(0, std::ios::beg);
	std::string line;
	int numLine = 0;
	while (getline(file, line)) {

		if (line.empty() || line[0] == '#')
		{
			continue;
		}
		numLine++;
		if (line.find(' ') != std::string::npos) {
			incorrectFiltWords++;
			continue;
		}
		std::istringstream iss(line);
		std::string word;
		while (iss >> word) {
			char* lowerWord = toLowerCase(word);
			if (Dictionary::isCorrectWord(lowerWord))
			{
				correctFiltWords++;
				if (dictionary.contains(lowerWord) && !dictionary.isEmpty())
					dictionary.erase(lowerWord);
			}
			else {
				incorrectFiltWords++;
				std::cout << "ERROR: incorrect entry " << line << " on line " << numLine << std::endl;
			}
			delete[] lowerWord;
		}
	}
	file.close();
}

void loadTextFile(const std::string& fileName, Dictionary& dictionary, int& correctTextWords,
	int& incorrectTextWords, int& allTextWords)
{

	std::cout << "Verifying the contents of " << fileName << std::endl;
	if (dictionary.isEmpty()) {
		std::cout << "The requested operation could not be performed because the dictionary is empty." << std::endl;
		return;
	}
	std::ifstream file(fileName);
	if (!file.is_open()) {
		return; 
	}
	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	if (fileSize == 0) {
		std::cout << "The text is empty." << std::endl;
		return;
	}
	file.seekg(0, std::ios::beg);
	std::string line;
	int numLine = 0;
	while (getline(file, line)) {
		numLine++;
		std::istringstream iss(line);
		std::string word;
		while (iss >> word) {
			allTextWords++;
			char* lowerWord = toLowerCase(word);
			if (Dictionary::isCorrectWord(lowerWord) && dictionary.contains(lowerWord)) {
				correctTextWords++;
			}
			else {
				incorrectTextWords++;
				std::cout << "SPELLING ERROR: " << word << " on line " << numLine << std::endl;
			}
			delete[] lowerWord;
		}
	}
	file.close();
}



