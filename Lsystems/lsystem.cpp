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
#include <cctype>
#include <algorithm>
#include <utility>

string Lsystem::getAxiom() const
{
    return axiom;
}

void Lsystem::setAxiom(const string &value)
{
    axiom = value;
    state = axiom;
}

string Lsystem::getState() const
{
    return state;
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
        this->d = strtof(line.substr(0, pos).c_str(), NULL);
        this->angle = strtof(line.substr(pos+1, line.size()-1).c_str(), NULL);
        cout << "line dist: " << this->d << " angle: " << this->angle << endl;
    } else {
        return;
    }
    //Lê axioma inicial
    if (getline(ruleDescriptor, line)){
        setAxiom(trim(line));
    }

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

//Função de trim de http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
std::string Lsystem::trim(const std::string &s)
{
   auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
   auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
   return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
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
    deque<vecCol> mq;
    glColor3f(0,0,0);
    glLineWidth(2);

    //A turtle é formada por Heading-Up-Left-Origin
    vecCol turtle;
    turtle = {H, U, L, O};

    glm::vec4 drawState;
    for (const auto &c : state){
//        cout << "state: " << c << "\n";
//        flush(cout);
        switch (c) {
        case 'F':
            drawState = turtle[3];
            glBegin(GL_LINES);
            //Desenha o ponto de origem
            drawState = mv*turtle[3];
            glVertex3f(drawState[0], drawState[1], drawState[2]);

            //Translada o ponto de origem em direção à Heading
            turtle[3] = turtle[3] + d*turtle[0];

            //Desenha o novo ponto de origem
            drawState = mv*turtle[3];
            glVertex3f(drawState[0], drawState[1], drawState[2]);
            glEnd();
            break;
        case 'f':
            //Translada o ponto de origem em direção à Heading
            turtle[3] = turtle[3] + d*turtle[0];
            break;
        case '+':
            //Rotação positiva em torno do eixo up
            turtle[0] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[1])) * turtle[0];
            turtle[2] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[1])) * turtle[2];
            break;
        case '-':
            //Rotação negativa em torno do eixo up
            turtle[0] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[1])) * turtle[0];
            turtle[2] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[1])) * turtle[2];
            break;
        case '&':
            //Rotação positiva em torno do eixo left
            turtle[0] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[2])) * turtle[0];
            turtle[1] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[2])) * turtle[1];
            break;
        case '^':
            //Rotação negativa em torno do eixo left
            turtle[0] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[2])) * turtle[0];
            turtle[1] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[2])) * turtle[1];
            break;
        case '\\':
            //Rotação positiva em torno do eixo heading
            turtle[1] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[0])) * turtle[1];
            turtle[2] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[0])) * turtle[2];
            break;
        case '/':
            //Rotação negativa em torno do eixo heading
            turtle[1] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[0])) * turtle[1];
            turtle[2] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[0])) * turtle[2];
            break;
        case '!':
            //Turnaround em torno do eixo up
            turtle[0] = glm::rotate(glm::mat4(1.0f), 180.f, glm::vec3(turtle[1])) * turtle[0];
            turtle[2] = glm::rotate(glm::mat4(1.0f), 180.f, glm::vec3(turtle[1])) * turtle[2];
        case '[':
            mq.push_front(turtle);
            break;
        case ']':
            if (!mq.empty()) {
                turtle = mq.front();
                mq.pop_front();
            }
            break;
        default:
            break;
        }
    }
}

/*
 * Popula a lista de vértices e a lista de arestas com os valores apropriados
 */
void Lsystem::generateModel()
{
    int currentVerticeNumber = 0; //Conta quantos vértices foram gerados até agora
    int currentLineStart = 0; //Acompanha em qual vértice a linha atual começa
    bool writeVertice = true; //Se o vértice já foi escrito ou não
    verticesList.clear();
    edgesList.clear();

    deque<std::pair<vecCol, int>> mq; //Guarda o estado da turtle e o currentLineStart

    //A turtle é formada por Heading-Up-Left-Origin
    vecCol turtle;
    turtle = {H, U, L, O};

    for (const auto &c : state){
//        cout << "state: " << c << "\n";
//        flush(cout);
        switch (c) {
        case 'F':
            if (writeVertice) {
                verticesList.push_back(turtle[3]);
            }
            edgesList.push_back(currentLineStart);

            writeVertice = false;
            //Translada o ponto de origem em direção à Heading
            turtle[3] = turtle[3] + d*turtle[0];
            currentVerticeNumber++;

            verticesList.push_back(turtle[3]);
            edgesList.push_back(currentVerticeNumber);
            currentLineStart = currentVerticeNumber;

            break;
        case 'f':
            //Translada o ponto de origem em direção à Heading
            turtle[3] = turtle[3] + d*turtle[0];
            break;
        case '+':
            //Rotação positiva em torno do eixo up
            turtle[0] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[1])) * turtle[0];
            turtle[2] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[1])) * turtle[2];
            break;
        case '-':
            //Rotação negativa em torno do eixo up
            turtle[0] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[1])) * turtle[0];
            turtle[2] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[1])) * turtle[2];
            break;
        case '&':
            //Rotação positiva em torno do eixo left
            turtle[0] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[2])) * turtle[0];
            turtle[1] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[2])) * turtle[1];
            break;
        case '^':
            //Rotação negativa em torno do eixo left
            turtle[0] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[2])) * turtle[0];
            turtle[1] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[2])) * turtle[1];
            break;
        case '\\':
            //Rotação positiva em torno do eixo heading
            turtle[1] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[0])) * turtle[1];
            turtle[2] = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(turtle[0])) * turtle[2];
            break;
        case '/':
            //Rotação negativa em torno do eixo heading
            turtle[1] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[0])) * turtle[1];
            turtle[2] = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(turtle[0])) * turtle[2];
            break;
        case '!':
            //Turnaround em torno do eixo up
            turtle[0] = glm::rotate(glm::mat4(1.0f), 180.f, glm::vec3(turtle[1])) * turtle[0];
            turtle[2] = glm::rotate(glm::mat4(1.0f), 180.f, glm::vec3(turtle[1])) * turtle[2];
        case '[':
            mq.push_front(std::make_pair(turtle, currentLineStart));
            break;
        case ']':
            if (!mq.empty()) {
                turtle = mq.front().first;
                currentLineStart = mq.front().second;
                mq.pop_front();
            }
            break;
        default:
            break;
        }
    }
}

void Lsystem::writeModel(const string &verticesFileName, const string &edgesFileName)
{
    ofstream verticesFile, edgesFile;

    verticesFile.open(verticesFileName, ios::trunc);
    edgesFile.open(edgesFileName, ios::trunc);
    if (!verticesFile.is_open() || verticesList.empty() || edgesList.empty()){
        return;
    }
    //Salva os vértices no arquivo
    for (glm::vec4 &v : verticesList){
        verticesFile << v.x << " " << v.y << " " << v.z << std::endl;
    }
    verticesFile.close();
    //Salva as arestas no arquivo
    int count = 0;
    for (int &e : edgesList){
        if (count == 0) {
            edgesFile << e;
        } else {
            edgesFile << " " << e << std::endl;
        }
        count = (count + 1)%2;
    }
    edgesFile.close();
}
