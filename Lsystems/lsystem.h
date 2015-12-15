#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "glm/mat4x4.hpp"

#define DEG2RAD(x) float(x * (M_PI / 180.0f))

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
