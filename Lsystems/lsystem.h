#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "glm/mat4x4.hpp"

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
    void drawLine(glm::mat4 m, float d);
    string getAxiom() const;
    void setAxiom(const string &value);
};

#endif // LSYSTEM_H
