#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class action {
public:
    int cur_st;
    string inputsymbol;
    vector<string> reduction_production;
    int shift_st;
    int error;

    action();
    ~action();
    void show();
};

action::action() {
    inputsymbol.clear();
    reduction_production.clear();
    cur_st = -1;
    shift_st = -1;
    error = 0;
}

action::~action()
{
}

void action::show() {
    cout << "state " << cur_st << '\t' << "input " << inputsymbol << endl;
    if (reduction_production.size() != 0) {
        cout << "reduction ";
        for (int j = 0; j < reduction_production.size(); j++)
        {
            cout << reduction_production[j];
            if (j == 0)
            {
                cout << "->";
            }
            else
            {
                cout << " ";
            }
        }
    }
    if (shift_st != -1) {
        cout << "shift state " << shift_st;
    }
    cout << endl;
}