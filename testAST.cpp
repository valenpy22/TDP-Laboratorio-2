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
    AST* ast2 = new AST("exp8.txt");
    AST* ast3 = new AST("exp2.txt");

    NodeOperation* op1 = new NodeOperation('*');
    NodeOperation* op3 = new NodeOperation('+');
    NodeVariable* x = new NodeVariable('x');
    NodeVariable* y = new NodeVariable('y');
    NodeNumber* m = new NodeNumber(5);

    ast->root = op1;

    op1->left = x;
    op1->right = y;

    //Link 
    ast->link(ast->root);

    //printAST
    ast->printAST(ast->root);

    //Clone
    cout << "Clonando... " << endl;
    Node* node = ast->clone(ast->root);

    cout << "Borrando raíz anterior... " << endl;
    delete ast->root;

    cout << "Asignando nueva raíz... " << endl;
    ast->root = node;

    cout << "Enlazando... " << endl;
    ast->link(ast->root);

    cout << "Imprimiendo nuevo árbol... " << endl;
    ast->printAST(ast->root);

    //hasVariableInTree
    cout << "¿Este árbol tiene variables? " << endl;
    ast->printAST(ast->root);
    determinate(ast->hasVariableInTree(ast->root));

    cout << "¿Este árbol tiene variables? " << endl;
    ast2->printAST(ast2->root);
    determinate(ast2->hasVariableInTree(ast2->root));

    //Equal
    cout << "¿Este árbol es igual al anterior? " << endl;
    determinate(ast->equal(ast->root, ast2->root));

    op3->left = y;
    op3->right = m;

    ast2->root = op3;

    ast2->link(ast2->root);

    cout << "Árbol nuevo: " << endl;
    ast2->printAST(ast2->root);

    //Sort
    cout << "Árbol ordenado: " << endl;
    ast2->root = ast2->sort(ast2->root);
    ast2->printAST(ast2->root);
    
    //Replace
    cout << "Reemplazando... " << endl;
    ast2->root = ast2->replace(ast2->root, "y", new int[1]{5});
    ast2->printAST(ast2->root);

    //Simplify
    cout << "Simplificando... " << endl;
    ast3->root = ast3->simplify(ast3->root);
    ast3->printAST(ast3->root);

    //Derivate
    cout << "Derivando... " << endl;
    ast2->root = ast2->derivate(ast2->root, 'x');
    ast2->printAST(ast2->root);

    //evalRecursive
    cout << "Evaluando... " << endl;
    ast->root = ast->replace(ast->root, "xy", new int[2]{5, 10});
    ast->root = ast->evalRecursive(ast->root);
    ast->printAST(ast->root);

    //evalInt
    cout << "Evaluando... " << endl;
    cout << "El resultado de evaluar el árbol es: " << ast->evalInt(ast->root) << endl;

    return 1;
}