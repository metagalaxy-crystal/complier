#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

const set<string> terminal = {
		//"if", "else", ";", "a"
	"Identifier", "(", ")", ";", ",", "if", "else", 
	"{", "}", "while", "return", "continue", "break", "int", "void", 
	"<", ">", "<=", ">=", "==", "!=", "+", "-", "*", "/", "Digits"
};

bool isterminal(string word) {
	auto result_it = terminal.find(word);
	if (result_it != terminal.end()) { return true; }
	else { return false; }
}

class whole_production {
public:
	vector<vector<string>> pset;

	whole_production();
	~whole_production();
	void add(vector<string> p);
	set<string> first_symbol(string s);
	int size() { return pset.size(); }
};

whole_production::whole_production() {
	pset.clear();
}

whole_production::~whole_production()
{
}

void whole_production::add(vector<string> p) {
	pset.push_back(p);
}

set<string> whole_production::first_symbol(string s) {
	set<string> first;
	first.clear();
	for (int i = 0; i < pset.size(); i++)
	{
		vector<string> temp = pset[i];	// get a production
		if (temp[0] == s) {
			if (isterminal(temp[1])) {	// isterminal add
				first.insert(temp[1]);
			}
			else if(s != temp[1]) {	// notterminal add unterminal's first symbol
				set<string> unterminalfirst = first_symbol(temp[1]);
				first.insert(unterminalfirst.begin(), unterminalfirst.end());
			}
		}
	}
	return first;
}