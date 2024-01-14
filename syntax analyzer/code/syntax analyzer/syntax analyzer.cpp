#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

#include "state.h"
#include "action.h"
#include "whole production.h"
#include "LR1.h"
#include "LR1stack.h"


int main()
{
	whole_production whole_p;
//	whole_p.add({ "start","S" });
//	whole_p.add({ "S","if","S","else","S" });
//	whole_p.add({ "S","if","S" });
//	whole_p.add({ "S","S",";","S" });
//	whole_p.add({ "S","a" });
//  set<string> s_first = pset.first_symbol("S");

	whole_p.add({ "start",      "sentence" });
	whole_p.add({ "sentence",   "Func" });
	whole_p.add({ "sentence",   "Func", "sentence" });
	whole_p.add({ "Func",       "FuncDecl" });
	whole_p.add({ "Func",       "FuncDef" });
	whole_p.add({ "FuncDecl",   "Type", "Identifier", "(", "FuncParam", ")", ";" });
	whole_p.add({ "FuncDecl",   "Type", "Identifier", "(", ")", ";" });
	whole_p.add({ "FuncDef",    "Type", "Identifier", "(", "FuncParam", ")", "Block" });
	whole_p.add({ "FuncDef",    "Type", "Identifier", "(", ")", "Block" });
	whole_p.add({ "FuncParam",  "Type", "Identifier" });
	whole_p.add({ "FuncParam",  "Type", "Identifier", ",", "FuncParam" });
	whole_p.add({ "Block",      "{", "BlockItem", "}" });
	whole_p.add({ "BlockItem",  "Stmt" });
	whole_p.add({ "BlockItem",  "VarDef" });
	whole_p.add({ "BlockItem",  "VarAssign" });
	whole_p.add({ "Stmt",       "VarStmt" });
	whole_p.add({ "Stmt",       "VarStmt", "Stmt" });
	whole_p.add({ "Stmt",       "Block" });
	whole_p.add({ "Stmt",       "Block", "Stmt" });
	whole_p.add({ "Stmt",       "if", "(", "RelationExpr", ")", "Stmt" });
	whole_p.add({ "Stmt",       "if", "(", "RelationExpr", ")", "Stmt", "else", "Stmt" });
	whole_p.add({ "Stmt",       "while", "(", "RelationExpr", ")", "Stmt" });
	whole_p.add({ "Stmt",       "return", "Expr", ";" });
	whole_p.add({ "Stmt",       "return", ";" });
	whole_p.add({ "Stmt",       "continue", ";" });
	whole_p.add({ "Stmt",       "break", ";" });
	whole_p.add({ "Stmt",       ";" });
	whole_p.add({ "VarStmt",    "VarDecl" });
	whole_p.add({ "VarStmt",    "VarDef" });
	whole_p.add({ "VarStmt",    "VarAssign" });
	whole_p.add({ "VarDecl",    "Type", "Identifier", ";" });
	whole_p.add({ "VarDef",     "Type", "Identifier", "=", "Expr", ";" });
	whole_p.add({ "VarAssign",  "Identifier", "=", "Expr", ";" });
	whole_p.add({ "Expr",       "RelationExpr" });
	whole_p.add({ "Expr",       "OperationExpr" });
	whole_p.add({ "Expr",       "Identifier", "(", ")" });
	whole_p.add({ "Expr",       "Identifier", "(", "IdentList", ")" });
	whole_p.add({ "IdentList",  "Identifier" });
	whole_p.add({ "IdentList",  "Identifier", ",", "IdentList" });
	whole_p.add({ "Type",       "int" });
	whole_p.add({ "Type",       "void" });
	whole_p.add({ "RelationExpr", "UnequalExpr" });
	whole_p.add({ "RelationExpr", "EqualExpr" });
	whole_p.add({ "UnequalExpr",  "OperationExpr" });
	whole_p.add({ "UnequalExpr",  "UnequalExpr", "<", "OperationExpr" });
	whole_p.add({ "UnequalExpr",  "UnequalExpr", ">", "OperationExpr" });
	whole_p.add({ "UnequalExpr",  "UnequalExpr", "<=", "OperationExpr" });
	whole_p.add({ "UnequalExpr",  "UnequalExpr", ">=", "OperationExpr" });
	whole_p.add({ "EqualExpr",    "UnequalExpr" });
	whole_p.add({ "EqualExpr",    "EqualExpr", "==", "UnequalExpr" });
	whole_p.add({ "EqualExpr",    "EqualExpr", "!=", "UnequalExpr" });
	whole_p.add({ "OperationExpr","OperationExpr", "+", "Term" });
	whole_p.add({ "OperationExpr","OperationExpr", "-", "Term" });
	whole_p.add({ "OperationExpr","Term" });
	whole_p.add({ "Term",    "Term", "*", "Factor" });
	whole_p.add({ "Term",    "Term", "/", "Factor" });
	whole_p.add({ "Term",    "Factor" });
	whole_p.add({ "Factor",  "(", "OperationExpr", ")" });
	whole_p.add({ "Factor",  "Digits" });
	whole_p.add({ "Factor",  "Identifier" });

	
	LR1 LR1_table;
	LR1_table.newstate();	// add a new state
	vector<string> temp_p = whole_p.pset[0];	// get the start production
	int nxtread = 1;	// initial the production with next symbol
	set<string> symbol;
	symbol.clear();
	symbol.insert("$");
	LR1_table.addproduction(temp_p, nxtread, symbol);	//	add the production to the state
	LR1_table.generate_production(whole_p);	// generate the following state according to the produntion have got into the state
	LR1_table.generate_table(whole_p);
	//LR1_table.showtable();
	LR1_table.state_output();
	LR1_table.table_output();

	LR1_stack stack;
	node firstnode;
	firstnode.st_num = 0;
	firstnode.symbol = "$";
	stack.add(firstnode);
	
	fstream process("process.txt"); process.clear();
	
	fstream code("code lex.txt");
	//fstream code("error 1.txt");
	//fstream code("error 2.txt");
	//fstream code("error 3.txt");
	//fstream code("error 4.txt");

	string line; int line_num = 1; int isfind = 1;
	while (getline(code, line)) {
		string buf; char readchar; int pos = 0;
		while (pos != line.length()) {
			readchar = line.at(pos);
			if (readchar != ' ') {
				buf = buf + readchar;
			}
			else {
				getaction:
				// buf as the input
				int cur_st_num = stack.getcur_st();
				action act;
				act = LR1_table.getaction(cur_st_num, buf);
				if (act.error > 0) {	// error handling
					if (act.error == 1) { cout << endl << "line " << line_num << '\t' << "error 1: unexpected " << buf << endl; }
					else if (act.error == 2) { cout << endl << "line " << line_num << '\t' << "error 2: incomplete code " << endl; }
					else if (act.error == 3) { cout << endl << "line " << line_num << '\t' << "error 3: lossing id or num before " << buf << endl; }
					else { cout << endl << "line " << line_num << '\t' << "error 4: meaningless code " << endl; }
					exit(0);
				}
				if (act.reduction_production.size() != 0 && act.reduction_production[0] != "start") {
					for (int i = 1; i < act.reduction_production.size(); i++){
						stack.pop();
					}
					node temp_n;
					temp_n.symbol = act.reduction_production[0];
					cur_st_num = stack.getcur_st();
					temp_n.st_num = LR1_table.getaction(cur_st_num, temp_n.symbol).shift_st;
					int error = LR1_table.getaction(cur_st_num, temp_n.symbol).error;
					if (error > 0) {	// error handling
						if (error == 1) { cout << endl << "line " << line_num << '\t' << "error 1: unexpected " << buf << endl; }
						else if (error == 2) { cout << endl << "line " << line_num << '\t' << "error 2: incomplete code " << endl; }
						else if (error == 3) { cout << endl << "line " << line_num << '\t' << "error 3: lossing id or num before " << buf << endl; }
						else { cout << endl << "line " << line_num << '\t' << "error 4: meaningless code " << endl; }
						exit(0);
					}
					stack.add(temp_n);
					stack.show();
					stack.to_file(process);
					goto getaction;
				}
				else {
					node temp_n;
					temp_n.symbol = buf;
					temp_n.st_num = LR1_table.getaction(cur_st_num, buf).shift_st;
					int error = LR1_table.getaction(cur_st_num, buf).error;
					if (error > 0) {	// error handling
						if (error == 1) { cout << endl << "line " << line_num << '\t' << "error 1: unexpected " << buf << endl; }
						else if (error == 2) { cout << endl << "line " << line_num << '\t' << "error 2: incomplete code " << endl; }
						else if (error == 3) { cout << endl << "line " << line_num << '\t' << "error 3: lossing id or num before " << buf << endl; }
						else { cout << endl << "line " << line_num << '\t' << "error 4: meaningless code " << endl; }
						exit(0);
					}
					stack.add(temp_n);
					stack.show();
					stack.to_file(process);
				}

				buf.clear();
			}
			pos++;
		}
		line_num++;
	}

	// 读到头了，但是还没有分析完
	string buf = "$";
	int stack_change = 0;
	do
	{
		stack_change = 0;
		int cur_st_num = stack.getcur_st();
		action act;
		act = LR1_table.getaction(cur_st_num, buf);
		if (act.error > 0) {	// error handling
			if (act.error == 1) { cout << endl << "line " << line_num << '\t' << "error 1: unexpected " << buf << endl; }
			else if (act.error == 2) { cout << endl << "line " << line_num << '\t' << "error 2: incomplete code " << endl; }
			else if (act.error == 3) { cout << endl << "line " << line_num << '\t' << "error 3: lossing id or num before " << buf << endl; }
			else { cout << endl << "line " << line_num << '\t' << "error 4: meaningless code " << endl; }
			exit(0);
		}
		if (act.reduction_production.size() != 0 && act.reduction_production[0] != "start") {
			for (int i = 1; i < act.reduction_production.size(); i++) {
				stack.pop();
				stack_change = 1;
			}
			node temp_n;
			temp_n.symbol = act.reduction_production[0];
			cur_st_num = stack.getcur_st();
			temp_n.st_num = LR1_table.getaction(cur_st_num, temp_n.symbol).shift_st;
			int error = LR1_table.getaction(cur_st_num, temp_n.symbol).error;
			if (error > 0) {	// error handling
				if (error == 1) { cout << endl << "line " << line_num << '\t' << "error 1: unexpected " << buf << endl; }
				else if (error == 2) { cout << endl << "line " << line_num << '\t' << "error 2: incomplete code " << endl; }
				else if (error == 3) { cout << endl << "line " << line_num << '\t' << "error 3: lossing id or num before " << buf << endl; }
				else { cout << endl << "line " << line_num << '\t' << "error 4: meaningless code " << endl; }
				exit(0);
			}
			stack.add(temp_n);
			stack.show();
			stack.to_file(process);
		}
		else {
			node temp_n;
			temp_n.symbol = buf;
			temp_n.st_num = LR1_table.getaction(cur_st_num, buf).shift_st;
			int error = LR1_table.getaction(cur_st_num, buf).error;
			if (error > 0) {	// error handling
				if (error == 1) { cout << endl << "line " << line_num << '\t' << "error 1: unexpected " << buf << endl; }
				else if (error == 2) { cout << endl << "line " << line_num << '\t' << "error 2: incomplete code " << endl; }
				else if (error == 3) { cout << endl << "line " << line_num << '\t' << "error 3: lossing id or num before " << buf << endl; }
				else { cout << endl << "line " << line_num << '\t' << "error 4: meaningless code " << endl; }
				exit(0);
			}
			stack.add(temp_n);
			stack.show();
			stack.to_file(process);
		}
	} while (stack_change == 1);

    return 0;
}