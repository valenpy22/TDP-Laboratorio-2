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
    NodeNumber* num = new NodeNumber(5);
    NodeVariable* var = new NodeVariable('z');

    NodeVariable* x = new NodeVariable('x');
    NodeVariable* y = new NodeVariable('y');

    op->left = num;
    op->right = var;

    ast->root = ast->replace(ast->root, "x", new int[1]{2});

    cout << "El resultado de reemplazar x por 2 es: " << endl;
    ast->printAST(ast->root);

    //isNodeOperation, isNodeNumber, isNodeVariable
    cout << "¿Este nodo es nodo operación? " << endl;
    determinate(ast->isNodeOperation(var));

    cout << "¿Este nodo es nodo número? " << endl;
    determinate(ast->isNodeNumber(var));

    cout << "¿Este nodo es nodo variable? " << endl;
    determinate(ast->isNodeVariable(var));

    cout << "La variable del nodo var es: " << var->variable << endl;
    cout << "La variable del nodo x es: " << x->variable << endl;
    cout << "La variable del nodo y es: " << y->variable << endl;

    return 1;
}