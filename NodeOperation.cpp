#include "NodeOperation.h"

using namespace std;

//Constructor: Crea un nodo con el tipo OPERATOR y asigna los nodos hijos
NodeOperation::NodeOperation(char operation, Node *left, Node *right){
    this->operation = operation;
    this->left = left;
    this->right = right;
    this->parent = nullptr;
    type = OPERATOR;
    visited = false;
}

//Constructor: Crea un nodo con el tipo OPERATOR (caso en el que no se conocen los hijos)
NodeOperation::NodeOperation(char operation){
    this->operation = operation;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    type = OPERATOR;
    visited = false;
}

//Destructor: Elimina los nodos hijos
NodeOperation::~NodeOperation(){
    if(left != nullptr){
        delete left;
    }
        
    if(right != nullptr){
        delete right;
    }
}

//Método que imprime lo que contiene el nodo y sus nodos hijos
void NodeOperation::print(){
    cout << operation << endl;
    left->print();
    right->print();
}