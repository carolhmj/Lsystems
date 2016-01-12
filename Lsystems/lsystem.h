#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"

#define DEG2RAD(x) float(x * (M_PI / 180.0f))

using namespace std;
class Lsystem
{
private:
    string axiom;
    string state;
    unordered_map<char, vector<string>> rules;
    float d, angle;
    /* Heading, Left, Up, vetores que representam a orientação do turtle. Heading é inicialmente o eixo x, Left o eixo z
     * e Up o eixo Y
     */
    glm::vec4 H = glm::vec4(1.0, 0.0, 0.0), L = glm::vec4(0.0, 0.0, 1.0), U = glm::vec4(0.0, 1.0, 0.0);
    //Vetor que marca a posição da turtle
    glm::vec4 O;

public:
    Lsystem(string ruleDescriptorName);
    void printRules();
    string turtleString(float turtle[], int i);
    void evolveState();
    void drawState(glm::mat4 mv);
    void drawLine(glm::mat4 m, float d);
    string getAxiom() const;
    void setAxiom(const string &value);
};

#endif // LSYSTEM_H
