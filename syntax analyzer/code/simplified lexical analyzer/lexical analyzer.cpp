#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include "tokendefine.h"
using namespace std;

enum state {
	START, NEXTLINE, ID, NUM, SLASH, OP, find_end, ReservedWord
};

int main() {
	ifstream code("testcode.txt");
	ofstream lex("code lex.txt"); lex.clear();
	string line; int line_num = 1; int isfind = 1;
	while (getline(code, line)) {
		state st = START; int pos = 0;
		string buf; char readchar;
		if (isfind == 0) {
			st = find_end;
		}
		while (pos != line.length()) {
			readchar = line.at(pos);
			if (st == START) {
				if (isupper(readchar) || islower(readchar) || readchar == '_') {
					st = ID;
					buf = buf + readchar;
					pos++;
				}
				else if (isdigit(readchar)) {
					st = NUM;
					buf = buf + readchar;
					pos++;
				}
				else if (isOperator(readchar)) {
					st = OP;
					buf = buf + readchar;
					pos++;
				}
				else if (isSeparator(readchar)) {
					if (readchar != '\t' && readchar != ' ') {
						lex << readchar << " ";
					}
					pos++;
				}
				else {
					cout << "line" << line_num << '\t' << "error:unrecognized symbol" << endl;
					exit(0);
				}
			}
			else if (st == ID) {
				if (isupper(readchar) || islower(readchar) || readchar == '_') {
					buf = buf + readchar;
					pos++;
				}
				else if (isReservedWord(buf)) {
					st = ReservedWord;
				}
				else {
					lex << "Identifier ";
					st = START; buf.clear();
				}
			}
			else if (st == ReservedWord) {
				lex << buf << ' ';
				st = START; buf.clear();
			}
			else if (st == NUM) {
				if (isdigit(readchar)) {
					buf = buf + readchar;
					pos++;
				}
				else if (isupper(readchar) || islower(readchar) || readchar == '_') {
					cout << "line" << line_num << '\t' << "error:unacceptable id name" << endl;
					exit(0);
				}
				else {
					lex << "Digits ";
					st = START; buf.clear();
				}
			}
			else if (st == OP) {
				string temp = buf + readchar;
				if (isdoubleoperate(temp)) {
					lex << temp << ' ';
					st = START; buf.clear(); pos++;
				}
				else if (temp == "//") {
					st = NEXTLINE;
				}
				else if (temp == "/*") {
					st = find_end;
					pos++;
					isfind = 0;
				}
				else {
					lex << buf << ' ';
					st = START; buf.clear();
				}
			}
			else if (st == NEXTLINE) {
				pos = line.length();
				buf.clear();
			}
			else if (st == find_end) {
				int a = line.find("*/");
				if (a >= pos) {
					pos = a + 2;
					st = START;
					isfind = 1;
					buf.clear();
				}
				else {
					st = NEXTLINE;
				}
			}
		}
		if (buf.length() > 0) {
			if (st == ID) { lex << "Identifier "; }
			else if (st == NUM) { lex << "Digits "; }
			else if (st == OP) { lex << buf << ' '; }
		}
		line_num++; //cout << line_num << '\t'; cout << line << endl;
		lex << '\n';
	}
	return 0;
}