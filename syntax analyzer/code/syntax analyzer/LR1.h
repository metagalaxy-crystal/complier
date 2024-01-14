#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "state.h"
#include "action.h"
#include "whole production.h"
using namespace std;

class LR1 {
public:
    vector<state> st_set;
    vector<action> table;
    int st_num; //  the total number of the state;
    int action_num;

    LR1();
    ~LR1();
    void newstate();
    void addproduction(vector<string> p, int nxtread, set<string> symbol);
    void generate_production(whole_production whole_p);
    void generate_table(whole_production whole_p);
    void shift(int cur_st, int nxt_st, string input);
    int isinLR1(state st);
    void newaction(int cur_st, string input);
    void showtable();
    void state_output();
    void table_output();
    action getaction(int cur_st_num, string buf);
};

LR1::LR1() {
    st_set.clear();
    table.clear();
    st_num = -1;
    action_num = -1;
}

LR1::~LR1()
{
}

void LR1::newstate() {
    state temp;
    st_set.push_back(temp);
    st_num++;
    st_set[st_num].state_serial = st_num;
}

void LR1::addproduction(vector<string> p, int nxtread, set<string> symbol) {
    st_set[st_num].addproduction(p, nxtread, symbol);
}

void LR1::generate_production(whole_production whole_p) {
    st_set[st_num].generate_production(whole_p);
}

void LR1::generate_table(whole_production whole_p) {
    //st_set[0].show();
    int i = 0;
    while (i != st_set.size()) {
        state temp_st = st_set[i]; //cout << endl;
        set<string> possible_input;
        possible_input = temp_st.find_possible_input();
        set<string>::iterator pos = possible_input.begin();
        while (pos != possible_input.end()) {
            string input = *pos;
            newstate();
            shift(i, st_num, input);
            generate_production(whole_p);
            //st_set[st_num].show(); cout << endl;
            int st_num_return = isinLR1(st_set[st_num]);
            if (st_num_return != -1) {
                st_set.pop_back();
                st_num--;
                //cout << "equal to state " << st_num_return << endl << endl;
                table[action_num].shift_st = st_num_return;
            }
            else if(st_set[st_num].production_set.size() == 0) {
                st_set.pop_back();
                st_num--;
                //cout << "delete" << endl << endl;
            }
            *pos++;
        }
        i++;
    }
}

void LR1::shift(int cur_st, int nxt_st, string input) {
    newaction(cur_st, input);
    for (int i = 0; i < st_set[cur_st].production_set.size(); i++) {
        if (st_set[cur_st].production_set[i].nxtread == st_set[cur_st].production_set[i].main_production.size()) {
            //add reduction act
            if (st_set[cur_st].production_set[i].symbol.find(input) != st_set[cur_st].production_set[i].symbol.end()) {
                table[action_num].reduction_production = st_set[cur_st].production_set[i].main_production;
            }
        }
        else if (st_set[cur_st].production_set[i].main_production[st_set[cur_st].production_set[i].nxtread] == input) {
            addproduction(st_set[cur_st].production_set[i].main_production, st_set[cur_st].production_set[i].nxtread + 1, st_set[cur_st].production_set[i].symbol);
            //add shift act
            table[action_num].shift_st = st_num;
        }
    }
}

int LR1::isinLR1(state st) {
    int st_num_return = -1;
    for (int i = 0; i < st_set.size()-1; ) {
        state temp_st = st_set[i];
        if (temp_st.production_set.size() == st.production_set.size()){
            for (int k = 0; k < st.production_set.size(); k++) {
                int pairing_succ = 0;
                for (int j = 0; j < temp_st.production_set.size(); j++) {
                    if (((temp_st.production_set[j].main_production == st.production_set[k].main_production) &&
                        (temp_st.production_set[j].nxtread == st.production_set[k].nxtread) &&
                        (temp_st.production_set[j].symbol == st.production_set[k].symbol)))
                    {
                        pairing_succ = 1;
                        break;
                    }
                }
                if (!pairing_succ)
                {
                    goto choose_next_cur_st;
                }
            }
            st_num_return = i;
            return st_num_return;
        }
    choose_next_cur_st:
        i++;
    }
    return st_num_return;
}

void LR1::newaction(int cur_st, string input) {
    action temp;
    temp.cur_st = cur_st;
    temp.inputsymbol = input;
    table.push_back(temp);
    action_num++;
}

void LR1::showtable() {
    for (int i = 0; i < table.size(); i++)
    {
        table[i].show();
    }
}

void LR1::state_output() {
    ofstream LR1_state("LR1 state.txt"); LR1_state.clear();
    for (int i = 0; i < st_set.size(); i++)
    {
        state temp_st = st_set[i];
        LR1_state << "state " << temp_st.state_serial << '\n';
        for (int i = 0; i < temp_st.production_set.size(); i++)
        {
            production temp = temp_st.production_set[i];
            for (int j = 0; j < temp.main_production.size(); j++)
            {
                LR1_state << temp.main_production[j];
                if (j == 0)
                {
                    LR1_state << "->";
                }
                else
                {
                    LR1_state << " ";
                }
            }
            LR1_state << '\t' << temp.nxtread << '\t';
            set<string>::iterator pos = temp.symbol.begin();
            while (pos != temp.symbol.end()) {
                string input = *pos;
                LR1_state << input << ' ';
                *pos++;
            }
            LR1_state << '\n';
        }
        LR1_state << '\n' ;
    }
    
}

void LR1::table_output() {
    ofstream LR1_table("LR1 table.txt"); LR1_table.clear();
    for (int i = 0; i < table.size(); i++)
    {
        action temp_act = table[i];
        LR1_table << "state " << temp_act.cur_st << '\t' << "input " << temp_act.inputsymbol << '\n';
        if (temp_act.reduction_production.size() != 0) {
            LR1_table << "reduction ";
            for (int j = 0; j < temp_act.reduction_production.size(); j++)
            {
                LR1_table << temp_act.reduction_production[j];
                if (j == 0)
                {
                    LR1_table << "->";
                }
                else
                {
                    LR1_table << " ";
                }
            }
        }
        if (temp_act.shift_st != -1) {
            LR1_table << "shift state " << temp_act.shift_st;
        }
        LR1_table << '\n' << '\n';
    }
}

action LR1::getaction(int cur_st_num, string buf) {
    action temp; int i;
    for (i = 0; i < table.size(); i++)
    {
        if ((table[i].cur_st == cur_st_num) && (table[i].inputsymbol == buf)) {
            temp = table[i];
            break;
        }
    }
    if (i == table.size()){ // different num according to different error
        if (buf == "{" || buf == "}" || buf == "(" || buf == ")" || buf == "," || buf == ";" )
        { temp.error = 1; }  // error 1: unexpected ";"
        else if (buf == "$") { temp.error = 2; }    // error 2: incomplete code
        else if (buf == "<" || buf == ">" || buf == "<=" || buf == ">=" || buf == "==" || 
            buf == "!=" || buf == "+" || buf == "-" || buf == "*" || buf == "/") 
        { temp.error = 3; } // error 3: meaningless code 代表缺失了关键字或者是缺失了id，这些词常常表示意义
        else { temp.error = 4; }    // error 4: lossing oprator
    }
    return temp;
}