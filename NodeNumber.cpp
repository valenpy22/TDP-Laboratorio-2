#include "NodeNumber.h"

using namespace std;

//Constructor: Crea un nodo con el tipo NUMBER y el número indicado
NodeNumber::NodeNumber(int number){
    this->number = number;
    type = NUMBER;
    parent = nullptr;
    visited = false;
}

//Destructor: No se necesita
NodeNumber::~NodeNumber(){
    
}

//Método que imprime lo que contiene el nodo
void NodeNumber::print(){
    cout << number << endl;
}