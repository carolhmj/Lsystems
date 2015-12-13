#include "lsystem.h"
#include <random>

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
        this->d = strtof(line.substr(0,pos).c_str(), NULL);
        this->angle = strtof(line.substr(pos+1,line.size()-1).c_str(), NULL);
    } else {
        return;
    }
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

