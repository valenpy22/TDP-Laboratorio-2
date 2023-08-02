#ifndef NODENUMBER_H
#define NODENUMBER_H

#include "Node.h"

using namespace std;

//Clase NodeNumber: Clase que representa un nodo del árbol de sintaxis abstracta que contiene un número.
//Hereda atributos y métodos de la clase Node.
class NodeNumber : public Node{

    public:
        //Atributos
        int number;

        //Métodos
        //Constructor
        NodeNumber(int number);
        //Destructor
        ~NodeNumber();

        void print();
};

#endif