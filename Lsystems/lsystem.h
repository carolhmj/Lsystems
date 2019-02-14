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

using namespace std;

#define DEG2RAD(x) float(x * (M_PI / 180.0f))
typedef vector<glm::vec4> vecCol; //vector collection

class Lsystem
{
private:
    string axiom;
    string state;
    unordered_map<char, vector<string>> rules;
    float d, angle;
    /* Heading, Left, Up, vetores que representam a orientação do turtle. Heading é inicialmente o eixo y, Left o eixo z
     * e Up o eixo x
     */
    glm::vec4 H = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), L = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), U = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    //Vetor que marca a posição da turtle
    glm::vec4 O = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    //Para exportar em um modelo
    vecCol verticesList; //Vértices gerados pela turtle
    vector<int> edgesList; //Ligações entre esses vértices
public:
    Lsystem(string ruleDescriptorName);
    void printRules();
    string turtleString(float turtle[], int i);
    std::string trim(const std::string &s);
    void evolveState();
    void drawState(glm::mat4 mv);
    void generateModel();
    void writeModel(const string &verticesFileName, const string &edgesFileName);
    string getAxiom() const;
    void setAxiom(const string &value);
    string getState() const;
};

#endif // LSYSTEM_H
