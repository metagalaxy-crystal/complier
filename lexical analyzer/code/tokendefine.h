#pragma once
#include <set>
#include <string>
using namespace std;

const set<string> reserved_words = {
		"int", "while", "if", "else", "return", "void", "cout"
};

const set<char> separators = {
		'(', ')', '{', '}', '[', ']', ',', ';', ':', '.', ' ', '\t'
};

const set<char> operators = {
		'+', '-', '=', '*', '/', '!', '<', '>'
};

const set<string> doubleoperators = {
		">=", "<=", "==", "!=", "<<", ">>"
};


bool isSeparator(char c) {
	if (separators.find(c) != separators.end()) { return true; }
	else { return false; }
}

bool isOperator(char c) {
	if (operators.find(c) != operators.end()) { return true; }
	else { return false; }
}

bool isReservedWord(string word) {
	auto result_it = reserved_words.find(word);
	if (result_it != reserved_words.end()) { return true; }
	else { return false; }
}

bool isdoubleoperate(string word) {
	auto result_it = doubleoperators.find(word);
	if (result_it != doubleoperators.end()) { return true; }
	else { return false; }
}