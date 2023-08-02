#ifndef AST_H
#define AST_H

#include <string>
#include "Node.h"

using namespace std;

//Clase AST: Clase que representa el árbol de sintaxis abstracta de una expresión matemática.
class AST{
    public:
        //Atributos de la clase
        Node* root;
        string expression_text;
        
        //Métodos de la clase
        //Constructor
        AST(string filename);
        //Destructor
        ~AST();
        //Métodos
        void parse();
        Node* derivate(Node* node, char var); 
        Node* simplify(Node* node);
        Node* clone(Node* node); 
        Node* evalRecursive(Node* node); 
        Node* replace(Node* node, string vars, int* values);
        int evalInt(Node* node); 
        void link(Node* node); 
        bool equal(Node* left, Node* right); 
        Node* sort(Node* node); 
        bool hasVariableInTree(Node* node); 
        bool isOperator(string token);
        bool isNumber(string token);
        bool isVariable(string token); 
        bool isNodeOperation(Node* node); 
        bool isNodeNumber(Node* node); 
        bool isNodeVariable(Node* node); 
        int operate(char operacion, int val1, int val2); 
        void recursivePrint(string prefix, Node* node, bool isRight); 
        void printAST(Node* node); 
        bool hasOnlyAddition(Node* node);
        int addAllNodeNumber(Node* node);
        int countVariables(Node* node);
        bool hasOnlyOneTypeVariable(Node* node, char c);
        char findVariable(Node* node);
};

#endif