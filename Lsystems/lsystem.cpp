#include "lsystem.h"
#include <random>
#include <stdlib.h>
#include <deque>
#include "GL/gl.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"

string Lsystem::getAxiom() const
{
    return axiom;
}

void Lsystem::setAxiom(const string &value)
{
    axiom = value;
    state = axiom;
}
Lsystem::Lsystem(string ruleDescriptorName)
{
    ifstream ruleDescriptor;
    ruleDescriptor.open(ruleDescriptorName);
    if (!ruleDescriptor.is_open()) {
        return;
    }
    string line;
    //Lê os parâmetros d e angle
    if (getline(ruleDescriptor, line)) {
        int pos = line.find(" ");
        cout << "line d: " << line.substr(0,pos).c_str() << " angle line: " << line.substr(pos+1,line.size()-1).c_str() << endl;
        this->d = strtof(line.substr(0,pos).c_str(), NULL);
        this->angle = strtof(line.substr(pos+1,line.size()-1).c_str(), NULL);
    } else {
        return;
    }
    cout << "d: " << d << " angle: " << angle << endl;
    //Lê posição inicial do x, posição inicial do y e ângulo inicial
    if (getline(ruleDescriptor, line)){
        int posA = line.find(" ");
        int posB = line.find(" ", posA+1);

        this->initialTurtle[0] = strtof(line.substr(0,posA).c_str(), NULL);
        this->initialTurtle[1] = strtof(line.substr(posA+1, posB).c_str(), NULL);
        this->initialTurtle[2] = strtof(line.substr(posB+1, line.size()-1).c_str(), NULL);
    }
    //Lê regras do sistema
    while (getline(ruleDescriptor, line)){
        int pos = line.find("|");
        //vector<string> *r = &(this->rules[line[pos-1]]);
        //r->append(line.substr(pos+1,line.size()));
        this->rules[line[pos-1]].push_back(line.substr(pos+1,line.size()));
    }
}

/* Imprime o conjunto de regras do sistema
 */
void Lsystem::printRules()
{
    for (const auto &rule : this->rules){
        cout << rule.first << " -> ";
        for (const auto &s : rule.second){
            cout << s << " ";
        }
        cout << endl;
    }
}

void Lsystem::evolveState()
{
    if (!state.empty()) {
        string out;
        for (const auto &c : state){
            if (rules.count(c) == 0){
                out.push_back(c); //Regra de substituição por ele mesmo
            } else {
                vector<string> v = rules[c];
                if (v.size() == 1){
                    out.append(v[0]);
                } else {
                    //Escolhe um valor aleatório para substituir a regra
                    random_device rd;
                    default_random_engine el(rd());
                    uniform_int_distribution<int> uni_dist(0, v.size()-1);
                    int sorted = uni_dist(el);

                    //Coloca o número aleatóriamente escolhido
                    out.append(v[sorted]);
                }
            }
        }
        state = out;
        cout << state << endl;
    }

}

void Lsystem::drawState()
{
    glm::mat4 mv;
    deque<glm::vec3> mq;
    mv = glm::ortho(-10.f,10.f,-10.f, 10.f, -10.f, 10.f);
    glColor3f(0,0,0);
    glLineWidth(2);
    //float[3] turtleState = initialTurtle;
    glm::vec3 turtleState(initialTurtle[0], initialTurtle[1], initialTurtle[2]);
    glm::vec4 drawState;
    for (const auto &c : state){
        switch (c) {
        case 'F':
            glBegin(GL_LINES);
            drawState = mv*glm::vec4(turtleState[0], turtleState[1], 0, 1);
            glVertex2f(drawState[0], drawState[1]);
            turtleState = turtleState + glm::vec3(d*cos(DEG2RAD(turtleState[2])), d*sin(DEG2RAD(turtleState[2])), 0);
            drawState = mv*glm::vec4(turtleState[0], turtleState[1], 0, 1);
            glVertex2f(drawState[0], drawState[1]);
            glEnd();
            //drawLine(mv, d);
            //mv = mv * glm::translate(glm::mat4(1.0f), glm::vec3(0,d,0));
            break;
        case 'f':
            turtleState = turtleState + glm::vec3(d*cos(DEG2RAD(turtleState[2])), d*sin(DEG2RAD(turtleState[2])), 0);
            //mv = mv * glm::translate(glm::mat4(1.0f), glm::vec3(0,d,0));
        case '+':
            turtleState = turtleState + glm::vec3(0, 0, angle);
            //mv = mv * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.f,0.f,1.f));
        case '-':
            turtleState = turtleState - glm::vec3(0, 0, angle);
            //mv = mv * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.f,0.f,1.f));
        case '[':
            cout << "pushing state: " << glm::to_string(turtleState) << endl;
            mq.push_front(turtleState);
        case ']':
            turtleState = mq.front();
            cout << "popped state: " << glm::to_string(turtleState) << endl;
            mq.pop_front();
        default:
            break;
        }
    }
}

void Lsystem::drawLine(glm::mat4 m, float d)
{
    //cout << "drawing line " << d << "\n";
    glm::vec4 v1(0,0,0,1);
    glm::vec4 v2(0,d,0,0);
    v1 = m * v1;
    v2 = m * v2;
    //cout << "v1: " << v1[0] << " " << v1[1] << " " << v1[2] << endl;
    //cout << "v2: " << v2[0] << " " << v2[1] << " " << v2[2] << endl;
    glBegin(GL_LINES);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
    glEnd();
}

