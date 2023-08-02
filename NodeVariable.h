#ifndef NODEVARIABLE_H
#define NODEVARIABLE_H

#include "Node.h"

using namespace std;

//Clase NodeVariable: Clase que representa un nodo del árbol de sintaxis abstracta que contiene una variable.
//Hereda atributos y métodos de la clase Node.
class NodeVariable : public Node{
    public:
        //Atributos
        char variable;

        //Métodos
        //Constructor
        NodeVariable(char variable);
        //Destructor
        ~NodeVariable();
        
        void print();
};

#endif