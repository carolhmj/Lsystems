#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
class Lsystem
{
private:
    string axiom;
    string state;
    unordered_map<char, vector<string>> rules;
    float d, angle, initialTurtle[3];

public:
    Lsystem(string ruleDescriptorName);
    void printRules();
    void evolveState();
    void drawState();
    string getAxiom() const;
    void setAxiom(const string &value);
};

#endif // LSYSTEM_H
