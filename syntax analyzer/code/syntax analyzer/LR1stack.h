#pragma once
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class node
{
public:
	string symbol;
	int st_num;

	node();
	~node();
};

node::node()
{
	int state_num = -1;
}

node::~node()
{
}

class LR1_stack {
public:
	vector<node> stack;
	int stack_top;
	void pop();

	LR1_stack();
	~LR1_stack();
	void add(node n);
	int getcur_st();
	void show();
	void to_file(fstream& file);
};

LR1_stack::LR1_stack() {
	stack.clear();
	stack_top = -1;
}

LR1_stack::~LR1_stack()
{
}

void LR1_stack::add(node n) {
	stack.push_back(n);
	stack_top++;
}

int LR1_stack::getcur_st() {
	return stack[stack_top].st_num;
}

void LR1_stack::pop() {
	stack_top--;
	stack.pop_back();
}

void LR1_stack::show() {
	for (int i = 0; i < stack.size(); i++)
	{
		cout << stack[i].symbol << " ";
	}
	cout << endl << endl;
}

void LR1_stack::to_file(fstream& file) {
	for (int i = 0; i < stack.size(); i++)
	{
		file << stack[i].symbol << " ";
	}
	file << '\n' << '\n';
}