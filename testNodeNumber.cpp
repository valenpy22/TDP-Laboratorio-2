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

    //Construct
    NodeNumber* n = new NodeNumber(5);
    NodeNumber* m = new NodeNumber(10);

    //Operate
    cout << "El resultado de operar * entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate('*', n->number, m->number) << endl;

    cout << "El resultado de operar + entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate('+', n->number, m->number) << endl;

    cout << "El resultado de operar - entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate('-', n->number, m->number) << endl;

    cout << "El resultado de operar ^ entre " << n->number << " y " << m->number << " es: ";
    cout << ast->operate('^', n->number, m->number) << endl;

    //isNodeOperation, isNodeNumber, isNodeVariable
    cout << "¿Este nodo es nodo operación? " << endl;
    determinate(ast->isNodeOperation(n));

    cout << "¿Este nodo es nodo número? " << endl;
    determinate(ast->isNodeNumber(n));

    cout << "¿Este nodo es nodo variable? " << endl;
    determinate(ast->isNodeVariable(n));

    cout << "Número del nodo n: " << n->number << endl;
    cout << "Número del nodo m: " << m->number << endl;

    return 1;
}