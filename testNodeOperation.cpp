#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#include "AST.h"
#include "Node.h"
#include "NodeOperation.h"
#include "NodeVariable.h"
#include "NodeNumber.h"

using namespace std;

void determinate(bool condition){
    if(condition){
        cout << "Sí" << endl;
    }else{
        cout << "No" << endl;
    }
}

int main(){
    AST* ast = new AST("exp8.txt");

    NodeOperation* op = new NodeOperation('*');
    NodeOperation* op2 = new NodeOperation('+');
    NodeOperation* op3 = new NodeOperation('-');
    NodeOperation* op4 = new NodeOperation('^');

    NodeNumber* n = new NodeNumber(5);
    NodeNumber* m = new NodeNumber(10);

    op->left = n;
    op->right = m;

    cout << "El resultado de operar * entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate(op->operation, n->number, m->number) << endl;

    cout << "El resultado de operar + entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate(op2->operation, n->number, m->number) << endl;

    cout << "El resultado de operar - entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate(op3->operation, n->number, m->number) << endl;

    cout << "El resultado de operar ^ entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate(op4->operation, n->number, m->number) << endl;

    //isNodeOperation, isNodeNumber, isNodeVariable
    cout << "¿Este nodo es nodo operación? " << endl;
    determinate(ast->isNodeOperation(op));

    cout << "¿Este nodo es nodo número? " << endl;
    determinate(ast->isNodeNumber(op));

    cout << "¿Este nodo es nodo variable? " << endl;
    determinate(ast->isNodeVariable(op));

    cout << "La operación del nodo op es: " << op->operation << endl;
    cout << "La operación del nodo op2 es: " << op2->operation << endl;
    cout << "La operación del nodo op3 es: " << op3->operation << endl;
    cout << "La operación del nodo op4 es: " << op4->operation << endl;

    return 1;
}