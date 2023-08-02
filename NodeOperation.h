#ifndef NODEOPERATION_H
#define NODEOPERATION_H

#include "Node.h"

using namespace std;

//Clase NodeOperation: Clase que representa un nodo del árbol de sintaxis abstracta que contiene una operación.
//Hereda atributos y métodos de la clase Node.
class NodeOperation : public Node{
    public:

        //Atributos
        Node *left;
        Node *right;
        Node *parent;
        char operation;

        //Métodos
        //Constructor
        NodeOperation(char operation, Node *left, Node *right);
        NodeOperation(char operation); //Caso de no conocer hijos
        //Destructor
        ~NodeOperation();

        void print();
};

#endif