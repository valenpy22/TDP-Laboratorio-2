#include <iostream>
#include <string>
#include <math.h>
#include "Node.h"

using namespace std;

//Constructor: Crea un nodo con el tipo INVALID
Node::Node(){
    type = INVALID;
    parent = nullptr;
    visited = false;
}

//Destructor: No se necesita
Node::~Node(){
    //No se necesita
}

//Método que imprime el nodo
void Node::print(){
    cout << "Invalid Node" << endl;
}