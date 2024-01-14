#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include "whole production.h"
using namespace std;

class production
{
public:
    vector<string> main_production; // represent the main production
    int nxtread;    // represent the symbol just behind the read point ( the expected symbol )
    set<string> symbol;  // represent the following symbol;

	production();
	~production();

private:

};

production::production()
{
    main_production.clear();
    nxtread = 1;
    symbol.clear();
}

production::~production()
{
}

class state {
public:
    vector<production> production_set;
    int state_serial;

    state();
    ~state();
    void addproduction(vector<string> p, int nxtread, set<string> symbol);
    void generate_production(whole_production whole_p);
    int production_in(vector<string> p);
    set<string> find_possible_input();
    void show();
};

state::state() {
    production_set.clear();
    state_serial = 0;
}

state::~state()
{
}

void state::addproduction(vector<string> p, int nxtread, set<string> symbol) {
    production temp;
    temp.main_production = p;
    temp.nxtread = nxtread;
    temp.symbol = symbol;
    production_set.push_back(temp);
}

void state::generate_production(whole_production whole_p) {
    int i = 0;
    while (i != production_set.size()) {
        production temp_p = production_set[i];  // get the production from the state
        if (temp_p.nxtread == temp_p.main_production.size()){
            i++;
            continue;
        }
        string next_read = temp_p.main_production[temp_p.nxtread];    // if the next symbol read is unterminal
        if (!isterminal(next_read)) {
            for (int j = 0; j < whole_p.pset.size(); j++) {
                if (whole_p.pset[j][0] == next_read) {
                    set<string> symbol;
                    if (temp_p.nxtread + 1 < temp_p.main_production.size()) {
                        string after_read_symbol = temp_p.main_production[temp_p.nxtread + 1];  //  read the symbol behind the symbol you want be read
                        if (isterminal(after_read_symbol)) {    // the symbol after the symbol will be read is terminal, then add
                            symbol.insert(after_read_symbol);
                        }
                        else {  // the symbol after the symbol will be read is unterminal, then add the first symbol of the unterminal
                            set<string> first = whole_p.first_symbol(after_read_symbol);
                            symbol.insert(first.begin(), first.end());
                        }
                    }
                    else if (temp_p.nxtread + 1 == temp_p.main_production.size()) { // if the symbol after the symbol will be read does not existing, use the symbol behind the production
                        symbol.insert(temp_p.symbol.begin(), temp_p.symbol.end());
                    }
                    if (production_in(whole_p.pset[j]) == -1) { // judge whether the production has exist in the state
                        addproduction(whole_p.pset[j], 1, symbol);
                        i = -1;  // 强制进行重新循环，因为不重新循环的话会造成已经被使用过的式子在后期会产生新的symbol，而该symbol无法再传递到后续的由该式子产生的生成式子中
                    }
                    else {
                        int pos = production_in(whole_p.pset[j]);
                        production_set[pos].symbol.insert(symbol.begin(), symbol.end());
                    }
                }
            }
        }
        i++;
    }
}

int state::production_in(vector<string> p) {
    for (int i = 0; i < production_set.size(); i++) {
        if (production_set[i].main_production == p && production_set[i].nxtread == 1) {
            return i;
        }
    }
    return -1;
}

set<string> state::find_possible_input() {
    set<string> temp;
    for (int i = 0; i < production_set.size(); i++) {
        if (production_set[i].nxtread < production_set[i].main_production.size()) {
            temp.insert(production_set[i].main_production[production_set[i].nxtread]);
        }
        else {
            temp.insert(production_set[i].symbol.begin(), production_set[i].symbol.end());
        }
    }
    return temp;
}

void state::show() {
    cout << "state " << state_serial << endl;
    for (int i = 0; i < production_set.size(); i++)
    {
        production temp = production_set[i];
        for (int j = 0; j < temp.main_production.size(); j++)
        {
            cout << temp.main_production[j];
            if (j==0)
            {
                cout << "->";
            }
            else
            {
                cout << " ";
            }
        }
        cout << '\t' << temp.nxtread << '\t';
        set<string>::iterator pos = temp.symbol.begin();
        while (pos != temp.symbol.end()) {
            string input = *pos;
            cout << input << ' ';
            *pos++;
        }
        cout << endl;
    }
}