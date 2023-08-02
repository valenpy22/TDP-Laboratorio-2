#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

//Clase Node: Clase que representa un nodo del árbol de sintaxis abstracta.
class Node{

    #define NUMBER 0
    #define OPERATOR 1
    #define VARIABLE 2
    #define INVALID 3

    public:
        //Atributos
        int type;
        bool visited;
        Node *parent;

        //Métodos
        Node(); //Constructor
        ~Node(); //Destructor
        virtual void print() = 0;

};

#endif