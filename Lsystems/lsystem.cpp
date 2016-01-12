#include "lsystem.h"
#include <random>
#include <stdlib.h>
#include <deque>
#include <sstream>
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
    //Lê os parâmetros d, angleZ e angleY
    if (getline(ruleDescriptor, line)) {
        int pos1 = line.find(" ");
        this->d = strtof(line.substr(0,pos1).c_str(), NULL);
        int pos2 = line.find(" ", pos1+1);
        this->angleZ = strtof(line.substr(pos1+1, pos2).c_str(), NULL);
        this->angleY = strtof(line.substr(pos2+1,line.size()-1).c_str(), NULL);
        cout << "line dist: " << this->d << " angle Z: " << this->angleZ << " angle Y: " << this->angleY << endl;
    } else {
        return;
    }
    //Lê posição inicial do x, posição inicial do y, posição inicial do z, ângulo Z inicial e ângulo Y inicial
    if (getline(ruleDescriptor, line)){
        int posA = line.find(" ");
        int posB = line.find(" ", posA+1);
        int posC = line.find(" ", posB+1);
        int posD = line.find(" ", posC+1);

        this->initialPosition[0] = strtof(line.substr(0,posA).c_str(), NULL);
        this->initialPosition[1] = strtof(line.substr(posA+1, posB).c_str(), NULL);
        this->initialPosition[2] = strtof(line.substr(posB+1, posC).c_str(), NULL);
        this->initialAngles[0] = strtof(line.substr(posC+1, posD).c_str(), NULL);
        this->initialAngles[1] = strtof(line.substr(posD+1, line.size()-1).c_str(), NULL);
    }

    cout << "initial pos:" << turtleString(initialPosition, 3) << endl;
    cout << "initial ang:" << turtleString(initialAngles, 2) << endl;
    //Lê regras do sistema
    while (getline(ruleDescriptor, line)){
        int pos = line.find("|");
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

string Lsystem::turtleString(float turtle[], int i)
{
    ostringstream r;
    r << "[ ";
    for (int j = 0; j < i; j++){
        r << turtle[j] << " ";
    }
    r << "]";
    return r.str();

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
        //cout << state << endl;
    }

}

void Lsystem::drawState(glm::mat4 mv)
{
    deque<pair<glm::vec3, glm::vec2>> mq;
    glColor3f(0,0,0);
    glLineWidth(2);

    glm::vec3 turtlePosition(initialPosition[0], initialPosition[1], initialPosition[2]);
    glm::vec2 turtleAngles(initialAngles[0], initialAngles[1]);
    glm::vec4 drawState;
    for (const auto &c : state){
        //cout << "state: " << c << "\n";
        switch (c) {
        case 'F':
            glBegin(GL_LINES);
            drawState = mv*glm::vec4(turtlePosition[0], turtlePosition[1], turtlePosition[2], 1);
            glVertex3f(drawState[0], drawState[1], drawState[2]);
            turtlePosition = turtlePosition + glm::vec3(d*cos(DEG2RAD(turtleAngles[0])), d*sin(DEG2RAD(turtleAngles[0])), 0);
            turtlePosition = turtlePosition + glm::vec3(0,d*sin(DEG2RAD(turtleAngles[1])), d*cos(DEG2RAD(turtleAngles[1])));
            drawState = mv*glm::vec4(turtlePosition[0], turtlePosition[1], turtlePosition[2], 1);
            glVertex3f(drawState[0], drawState[1], drawState[2]);
            glEnd();
            //drawLine(mv, d);
            //mv = mv * glm::translate(glm::mat4(1.0f), glm::vec3(0,d,0));
            break;
        case 'f':
            //turtlePosition = turtleState + glm::vec3(d*cos(DEG2RAD(turtleState[2])), d*sin(DEG2RAD(turtleState[2])), 0);
            //mv = mv * glm::translate(glm::mat4(1.0f), glm::vec3(0,d,0));
        case '+':
            turtleAngles = turtleAngles + glm::vec2(angleZ, angleY);
            //mv = mv * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.f,0.f,1.f));
        case '-':
            turtleAngles = turtleAngles - glm::vec2(angleZ, angleY);
            //mv = mv * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.f,0.f,1.f));
        case '[':
            //cout << "pushing state: " << glm::to_string(turtleState) << endl;
            mq.push_front(make_pair(turtlePosition, turtleAngles));
        case ']':
            if (!mq.empty()) {
                auto turtles = mq.front();
                //cout << "popped state: " << glm::to_string(turtleState) << endl;
                mq.pop_front();
                turtlePosition = turtles.first;
                turtleAngles = turtles.second;
            }
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

