#include "NodeVariable.h"

using namespace std;

//Constructor: Crea un nodo con el tipo VARIABLE y el caracter indicado
NodeVariable::NodeVariable(char variable){
    this->variable = variable;
    this->parent = nullptr;
    type = VARIABLE;
    visited = false;
}

//Destructor: No se necesita
NodeVariable::~NodeVariable(){
    
}

//Método que imprime lo que contiene el nodo
void NodeVariable::print(){
    cout << variable << endl;
}