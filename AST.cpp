#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#include <stack>

#include "AST.h"
#include "Node.h"
#include "NodeOperation.h"
#include "NodeNumber.h"
#include "NodeVariable.h"

using namespace std;

//Constructor de la clase AST: Recibe un string con el nombre del archivo
AST::AST(string filename){

    ifstream file(filename);

    //Si el archivo no está abierto, se le indica que
    //no se puede abrir.
    if(!file.is_open()){
        cout << "No se pudo abrir el archivo" << endl;
        exit(1);
    }

    //Se lee la línea del archivo
    if(getline(file, expression_text, '\n')){
        parse();
    }else{
        cout << "No se puede leer la línea" << endl;
        exit(1);
    }

    cout << "Se creó el árbol" << endl;
}

//Destructor de la clase AST
AST::~AST(){
    if(root != nullptr){
        delete root;
    }
}

//Método que crea el árbol a partir de un string
void AST::parse(){

    //Si el string es vacío, se indica que hay un error
    if(expression_text.empty()){
        cout << "Error, el string es vacío" << endl;
        exit(1);
    
    //Si el string no es vacío, se crea el árbol
    }else{
        cout << "Expresión: " << expression_text << endl;

        //Se crean nodos vacíos.
        Node* node = nullptr;
        root = nullptr;
        NodeOperation *op;
        NodeNumber *num;
        NodeVariable *var;

        string line, token;
        stringstream ss;

        stack<NodeOperation*> pila;

        ss << expression_text;
        while(getline(ss, token, ' ')){

            //Se crea un nodo dependiendo del token
            if(isOperator(token)) {
                node = new NodeOperation(token[0]);
                cout << "Operador: " << token << endl;

                //Se le asigna el padre al nodo
                if(!pila.empty()){
                    ((NodeOperation*)node)->parent = pila.top();
                }else{
                    ((NodeOperation*)node)->parent = nullptr;
                }

            }else if(isNumber(token)) {
                int valor = stoi(token);
                node = new NodeNumber(valor);
                cout << "Número: " << valor << endl;

            }else if(isVariable(token)) {
                char name = token[0];
                node = new NodeVariable(name);
                cout << "Variable: " << name << endl;
            }

            //Si la raíz es nula, se le asigna el nodo
            if(root == nullptr){
                root = node;
            }

            //Dependiendo del top del stack se agrega a la izquierda o derecha
            if(!pila.empty()){
                //Asignación del padre
                node->parent = pila.top();
                if(pila.top()->left == nullptr){
                    pila.top()->left = node;
                }else if(pila.top()->right == nullptr){
                    pila.top()->right = node;
                    pila.pop();
                }else{
                    cout << "Error" << endl;
                    exit(1);
                }
            }

            //Si el nodo es un operador se agrega a la pila
            if(isNodeOperation(node)){
                pila.push((NodeOperation*)node);
            }
        }
    }

    //Se ordena el árbol
    root = sort(root);

    //Se linkea el árbol
    link(root);
}

//Método que linkea los nodos del árbol
void AST::link(Node* node){
    if(node != nullptr){
        if(isNodeOperation(node)){
            NodeOperation* op = (NodeOperation*)node;

            //Se le asigna el padre a los hijos
            op->left->parent = op;
            op->right->parent = op;
            link(op->left);
            link(op->right);
        }
    }
}

//Método que imprime el árbol de forma recursiva, con el fin de que se vea como un árbol.
void AST::recursivePrint(string prefix, Node* node, bool isRight){
    if(node != nullptr){
        if(node->parent == nullptr){
            cout << "El padre es vacío" << endl;
        }

        if(node->parent != nullptr){
            cout << prefix;
            cout << (isRight ? "R├──" : "L└──" );
        }else{
            cout << " ";
        }
        if(!isNodeOperation(node)){
            node->print();
        }else{
            NodeOperation* op = (NodeOperation*)node;
            cout << op->operation << endl;

            if(node->parent != nullptr){
                recursivePrint(prefix + (isRight ? "│   " : "    "), op->right, true);
                recursivePrint(prefix + (isRight ? "│   " : "    "), op->left, false);
            }else{
                recursivePrint(prefix, op->right, true);
                recursivePrint(prefix, op->left, false);
            }
        }
    }
}

//Método que imprime el árbol
void AST::printAST(Node* root){
    recursivePrint("", root, false);
}

//Método que permite determinar si un nodo es de tipo OPERATOR
bool AST::isNodeOperation(Node* node){
    return node->type == OPERATOR;
}

//Método que permite determinar si un nodo es de tipo NUMBER
bool AST::isNodeNumber(Node* node){
    return node->type == NUMBER;
}

//Método que permite determinar si un nodo es de tipo VARIABLE
bool AST::isNodeVariable(Node* node){
    return node->type == VARIABLE;
}

//Método que permite determinar si un caracter es un operador
bool AST::isOperator(string token){
    return token == "+" || token == "-" || token == "*" || token == "^";
}

//Método que permite determinar si un caracter es un número
bool AST::isNumber(string token){
    return token[0] >= '0' && token[0] <= '9';
}

//Método que permite determinar si un caracter es una variable
bool AST::isVariable(string token){
    return token[0] >= 'a' && token[0] <= 'z' || token[0] >= 'A' && token[0] <= 'Z';
}

//Método que opera dos números
int AST::operate(char operation, int l, int r){
    switch(operation){
        case '+':
            return l + r;
        case '-':
            return l - r;
        case '*':
            return l * r;
        case '^':
            return pow(l, r);
        default:
            cout << "Error" << endl;
            exit(1);
    }
}

//Método que evalúa el árbol de forma recursiva, solo si no hay variables.
Node* AST::evalRecursive(Node* node){
    //Si es nodo de tipo operación
    if(isNodeOperation(node)){

        //Se crea un nuevo nodo de tipo operación, cASTeando el
        //nodo ingresado. 
        //Luego, se evalúa recursivamente cada uno de sus hijos.
        NodeOperation *op = (NodeOperation*)node;
        Node *l = evalRecursive(op->left);
        Node *r = evalRecursive(op->right);

        //Si ambos nodos hijos son números
        if(isNodeNumber(l) && isNodeNumber(r)){
            //Se crea un nodo de tipo número donde almacene el resultado
            //de aplicar la operación.
            NodeNumber* num = new NodeNumber(operate(op->operation, ((NodeNumber*)l)->number, ((NodeNumber*)r)->number));

            //Luego, se dice que el padre de este nodo será el padre del
            //nodo ingresado cASTeado a nodo operación.
            num->parent = op->parent;

            //Además, se crea un nuevo nodo operación, el cuál será el padre
            //del nodo ingresado cASTeado a nodo operación.
            NodeOperation* parent = (NodeOperation*)op->parent;

            //Se actualizan los hijos izquierdo y derecho del padre
            //Si el padre del nodo ingresado no es nulo y si
            //el hijo izquierdo del padre es igual a la operación:
            if(op->parent != nullptr && parent->left == op){
                //Se actualiza el hijo izquierdo del padre,
                //y se reemplaza por el resultado de operar.
                parent->left = num;

            //Si el padre del nodo ingresado no es nulo y si
            //el hijo derecho del padre es igual a la operación:
            }else if(op->parent != nullptr && parent->right == op){
                //Se actualiza el hijo derecho del padre,
                //y se reemplaza por el resultado de operar.
                parent->right = num;
            }


            cout << "Valor operación: " << num->number << endl;

            return num;

        //Si al menos uno no es número, se deja como esta.
        }else{
            op->left = l;
            op->right = r;
        }
    }else if(isNodeNumber(node) || isNodeVariable(node)){
        return node;

    }else{
        cout << "Error: Se encontró algo distinto a operador, número o variable." << endl;
        exit(1);
    }
    return nullptr;
}

//Método que retorna un árbol con las variables reemplazadas por números.
Node* AST::replace(Node* n, string vars, int* values){
    Node* node = sort(n);

    //Si el nodo es de tipo operación, se reemplaza recursivamente
    //los hijos izquierdo y derecho.
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)clone(node);
        Node* l = replace(op->left, vars, values);
        Node* r = replace(op->right, vars, values);

        op->left = l;
        op->right = r;
        
        link(op);

        return op;

    //Si el nodo es de tipo variable, se reemplaza por el número correspondiente.
    }else if(isNodeVariable(node)){
        NodeVariable* var = (NodeVariable*)clone(node);
        int index = vars.find(var->variable);
        if(index != string::npos){
            NodeNumber* num = new NodeNumber(values[index]);
            num->parent = var->parent;

            link(num);

            return sort(num);
        }
    //Si el nodo es de tipo número, se deja como está.
    }else if(isNodeNumber(node)){
        return node;
    }
    return sort(node);
}

//Método que deriva el árbol dada una variable.
Node* AST::derivate(Node* node, char var){
    if(node == nullptr){
        return nullptr;
    }

    //Si el nodo es de tipo operación, se deriva recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)clone(node);
        NodeOperation* derivedNode; 

        //Se deriva dependiendo de la operación

        //Si la operación es suma, se deriva cada uno de los hijos.
        if(op->operation == '+'){
            derivedNode = new NodeOperation('+');

            Node* left = derivate(op->left, var);
            Node* right = derivate(op->right, var);

            derivedNode->left = left;
            derivedNode->right = right;

            right->parent = derivedNode;
            left->parent = derivedNode;

            link(derivedNode);

            return sort(derivedNode);

        //Si la operación es resta, se deriva cada uno de los hijos.
        }else if(op->operation == '-'){
            derivedNode = new NodeOperation('-');

            Node* left = derivate(op->left, var);
            Node* right = derivate(op->right, var);

            right->parent = derivedNode;
            left->parent = derivedNode;

            derivedNode->left = left;
            derivedNode->right = right;

            link(derivedNode);

            return sort(derivedNode);

        //Si la operación es multiplicación, se deriva cada uno de los hijos por regla de la cadena.
        }else if(op->operation == '*'){
            derivedNode = new NodeOperation('+');
            derivedNode->left = (NodeOperation*)clone(op);
            derivedNode->left->parent = derivedNode;
            derivedNode->right = (NodeOperation*)clone(op);
            derivedNode->right->parent = derivedNode;

            NodeOperation* derivedNodeLeft = (NodeOperation*)derivedNode->left;
            NodeOperation* derivedNodeRight = (NodeOperation*)derivedNode->right;

            derivedNodeLeft->left = derivate(op->left, var);
            derivedNodeLeft->left->parent = derivedNodeLeft;

            derivedNodeLeft->right = clone(op->right);
            derivedNodeLeft->right->parent = derivedNodeLeft;

            derivedNodeRight->right = derivate(op->right, var);
            derivedNodeRight->right->parent = derivedNodeRight;

            derivedNodeRight->left = clone(op->left);
            derivedNodeRight->left->parent = derivedNodeRight;

            link(derivedNode);

            return sort(derivedNode);

        //Si la operación es elevación, se deriva por regla de la cadena.
        }else if(op->operation == '^'){
            derivedNode = new NodeOperation('*');
            derivedNode->left = clone(op->right);
            derivedNode->right = new NodeOperation('*');

            NodeOperation* derivedNodeLeft = (NodeOperation*)derivedNode->left;
            NodeOperation* derivedNodeRight = (NodeOperation*)derivedNode->right;

            derivedNodeRight->left = new NodeOperation('^');
            derivedNodeRight->right = derivate(clone(op->left), var);

            NodeOperation* derivedNodeRightLeft = (NodeOperation*)derivedNodeRight->left;

            derivedNodeRightLeft->left = clone(op->left);
            derivedNodeRightLeft->right = new NodeNumber(((NodeNumber*)op->right)->number - 1);

            link(derivedNode);
            
            return sort(derivedNode);

        }else{
            cout << "Este operador no está permitido por este programa." << endl;
            exit(1);
        }

    //Si el nodo es de tipo variable, se deriva dependiendo de si es la variable
    }else if(isNodeVariable(node) && ((NodeVariable*)node)->variable == var){
        NodeNumber* derivedNode = new NodeNumber(1);
        return derivedNode;

    //Si el nodo es de tipo variable, pero no es la variable que se busca, se retorna 0.
    }else if(isNodeVariable(node) && ((NodeVariable*)node)->variable != var){
        NodeNumber* derivedNode = new NodeNumber(0);
        return derivedNode;

    //Si el nodo es de tipo número, se retorna 0.
    }else if(isNodeNumber(node)){
        NodeNumber* derivedNode = new NodeNumber(0);
        return derivedNode;
    }else{
        cout << "Error" << endl;
        exit(1);
    }

    return nullptr;
}

//Método que determina si dos árboles son iguales.
bool AST::equal(Node* left, Node* right){
    //Si ambos son nulos, son iguales.
    if(left == nullptr && right == nullptr){
        return true;
    //Si uno es nulo y el otro no, no son iguales.
    }else if(left == nullptr || right == nullptr){
        return false;
    }

    //Si ambos son operaciones, se comparan.
    if(isNodeOperation(left) && isNodeOperation(right)){
        NodeOperation* leftOp = (NodeOperation*)left;
        NodeOperation* rightOp = (NodeOperation*)right;

        if(leftOp->operation == rightOp->operation){
            return equal(leftOp->left, rightOp->left) && equal(leftOp->right, rightOp->right);
        }else{
            return false;
        }
    //Si ambos son números, se comparan.
    }else if(isNodeNumber(left) && isNodeNumber(right)){
        NodeNumber* leftNum = (NodeNumber*)left;
        NodeNumber* rightNum = (NodeNumber*)right;

        return leftNum->number == rightNum->number;

    //Si ambos son variables, se comparan.
    }else if(isNodeVariable(left) && isNodeVariable(right)){
        NodeVariable* leftVar = (NodeVariable*)left;
        NodeVariable* rightVar = (NodeVariable*)right;

        return leftVar->variable == rightVar->variable;
    //Si no son iguales.
    }else{
        return false;
    }
}

//Método que opera un árbol y retorna un entero.
int AST::evalInt(Node* node){

    //Si el nodo es de tipo operación, se evalúa recursivamente
    if(isNodeOperation(node)){
        int left = evalInt(((NodeOperation*)node)->left);
        int right = evalInt(((NodeOperation*)node)->right);

        return operate(((NodeOperation*)node)->operation, left, right);

    //Si el nodo es de tipo número, se retorna el número.
    }else if(isNodeNumber(node)){
        return ((NodeNumber*)node)->number;

    //Si el nodo es de tipo variable, se retorna un error.
    }else{
        cout << "Error: No es operador ni número." << endl;
        exit(1);
    }
}

//Método que clona un árbol.
Node* AST::clone(Node* node){
    if(node == nullptr){
        return nullptr;
    }

    //Se clona el nodo dependiendo de su tipo.
    if(isNodeOperation(node)){
        NodeOperation* cloneNode = new NodeOperation(((NodeOperation*)node)->operation);
        cloneNode->left = clone(((NodeOperation*)node)->left);
        cloneNode->right = clone(((NodeOperation*)node)->right);
        return cloneNode;
        
    }else if(isNodeVariable(node)){
        NodeVariable* cloneNode = new NodeVariable(((NodeVariable*)node)->variable);
        return cloneNode;

    }else if(isNodeNumber(node)){
        NodeNumber* cloneNode = new NodeNumber(((NodeNumber*)node)->number);
        return cloneNode;

    }else{
        cout << "Error" << endl;
        return nullptr;
    }

}

//Método que simplifica un árbol.
Node* AST::simplify(Node* node){
    if(node == nullptr){
        return nullptr;
    }

    link(node);

    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)clone(node);
        NodeOperation* rightOp = (NodeOperation*)op->right;
        NodeOperation* leftOp = (NodeOperation*)op->left;
        char c = findVariable(op);

        //Caso 1: Si ambos hijos son números
        if(isNodeNumber(op->left) && isNodeNumber(op->right) && op->operation != '^'){
            NodeNumber* num = new NodeNumber(operate(op->operation, ((NodeNumber*)op->left)->number, ((NodeNumber*)op->right)->number));
            num->parent = op->parent;

            link(num);

            return num;

        //Caso 2: Si la operación es más y el hijo izquierdo es 0
        }else if(op->operation == '+' && isNodeNumber(op->left) && ((NodeNumber*)op->left)->number == 0){
            Node* right = simplify(op->right);
            right->parent = op->parent;

            link(right);

            return right;

        //Caso 3: Si la operación es menos y el hijo derecho es 0
        }else if(op->operation == '-' && isNodeNumber(op->right) && ((NodeNumber*)op->right)->number == 0){
            Node* left = simplify(op->left);
            left->parent = op->parent;

            link(left);

            return left;
        //Caso 4: Si la operación es elevación y el hijo izquierdo es 0
        }else if(op->operation == '^' && isNodeNumber(op->left) && ((NodeNumber*)op->left)->number == 0){
            if(isNodeNumber(op->right) && ((NodeNumber*)op->right)->number == 0){
                cout << "Error: 0^0 no está definido." << endl;
                exit(1);
            }else{
                NodeNumber* num = new NodeNumber(0);
                num->parent = op->parent;

                link(num);

                return num;
            }
        //Caso 5: Si la operación es elevación y el hijo derecho es 0
        }else if(op->operation == '^' && isNodeNumber(op->right) && ((NodeNumber*)op->right)->number == 0){
            if(isNodeNumber(op->left) && ((NodeNumber*)op->left)->number == 0){
                cout << "Error: 0^0 no está definido." << endl;
                exit(1);

            }else{
                NodeNumber* num = new NodeNumber(1);
                num->parent = op->parent;

                link(num);

                return num;
            }
        //Caso 6: Si la operación es elevación y el hijo derecho es 1
        }else if(op->operation == '^' && isNodeNumber(op->right) && ((NodeNumber*)op->right)->number == 1){
            return op->left;

        //Caso 7: Si la operación es elevación y el hijo izquierdo es 1
        }else if(op->operation == '^' && isNodeNumber(op->left) && ((NodeNumber*)op->left)->number == 1){
            return op->right;

        //Caso 8: Si la operación es multiplicación y el hijo izquierdo es 0
        }else if(op->operation == '*' && isNodeNumber(op->left) && ((NodeNumber*)op->left)->number == 0){
            NodeNumber* num = new NodeNumber(0);
            num->parent = op->parent;

            link(num);

            return num;

        //Caso 9: Si la operación es multiplicación y el hijo izquierdo es 1
        }else if(op->operation == '*' && isNodeNumber(op->left) && ((NodeNumber*)op->left)->number == 1){
            Node* right = simplify(op->right);
            right->parent = op->parent;

            link(right);

            return right;
        //Caso 10: Si la operación es multiplicación y ambos hijos son iguales
        }else if(op->operation == '*' && equal(op->left, op->right)){
            NodeOperation* newOp = new NodeOperation('^');
            newOp->left = clone(op->left);
            newOp->right = new NodeNumber(2);
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 11: Si la operación es suma, su hijo izquierdo es una variable y su hijo derecho es 
        //una multiplicación, con su hijo izquierdo siendo un número y su hijo derecho siendo una variable.
        }else if(op->operation == '+' && isNodeVariable(op->left) && isNodeOperation(op->right) && rightOp->operation == '*' && isNodeVariable(rightOp->right) && isNodeNumber(rightOp->left)){

            //Si ambas variables son iguales, se suma el número.
            if(((NodeVariable*)op->left)->variable == ((NodeVariable*)rightOp->right)->variable){
                NodeOperation* newOp = new NodeOperation('*');
                NodeNumber* num = new NodeNumber(((NodeNumber*)rightOp->left)->number + 1);
                NodeVariable* var = new NodeVariable(((NodeVariable*)rightOp->right)->variable);

                newOp->left = num;
                newOp->right = var;
                newOp->parent = op->parent;

                link(newOp);

                return newOp;
            }
        //Caso 12: Si hay sumas entre multiplicaciones de variables, se suman los coeficientes.
        }else if(op->operation == '+' && isNodeOperation(op->left) && isNodeOperation(op->right) && leftOp->operation == '*' && rightOp->operation == '*' && isNodeVariable(leftOp->right) && isNodeVariable(rightOp->right) && ((NodeVariable*)leftOp->right)->variable == ((NodeVariable*)rightOp->right)->variable){
            NodeOperation* newOp = new NodeOperation('*');
            NodeNumber* num = new NodeNumber(((NodeNumber*)leftOp->left)->number + ((NodeNumber*)rightOp->left)->number);
            NodeVariable* var = new NodeVariable(((NodeVariable*)leftOp->right)->variable);

            newOp->left = num;
            newOp->right = var;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;  

        //Caso 13: Si hay multiplicaciones entre multiplicaciones de variables, se suman los exponentes.
        }else if(op->operation == '*' && isNodeOperation(op->left) && isNodeOperation(op->right) && leftOp->operation == '*' && rightOp->operation == '*' && isNodeVariable(leftOp->right) && isNodeVariable(rightOp->right) && ((NodeVariable*)leftOp->right)->variable == ((NodeVariable*)rightOp->right)->variable){
            NodeOperation* newOp = new NodeOperation('*');
            NodeNumber* num = new NodeNumber(((NodeNumber*)leftOp->left)->number + ((NodeNumber*)rightOp->left)->number);
            NodeVariable* var = new NodeVariable(((NodeVariable*)leftOp->right)->variable);

            newOp->left = num;
            newOp->right = var;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 14: Si la operación es suma, el hijo izquierdo es número, y el hijo derecho es una suma, el hijo izquierdo del derecho
        //es otro número y el hijo derecho del derecho es una variable, se suman los números.
        }else if(op->operation == '+' && isNodeNumber(op->left) && isNodeOperation(op->right) && rightOp->operation == '+' && isNodeNumber(rightOp->left) && isNodeVariable(rightOp->right)){
            NodeOperation* newOp = new NodeOperation('+');
            NodeNumber* num = new NodeNumber(((NodeNumber*)op->left)->number + ((NodeNumber*)rightOp->left)->number);
            NodeVariable* var = new NodeVariable(((NodeVariable*)rightOp->right)->variable);

            newOp->left = num;
            newOp->right = var;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;
            

        //Caso 15: Si la operación es multiplicación, el hijo izquierdo es una variable, y el hijo derecho es una elevación, el hijo izquierdo del derecho
        //es la misma variable y el hijo derecho del derecho es un número, se suman los números.
        }else if(op->operation == '*' && isNodeVariable(op->left) && isNodeOperation(op->right) && rightOp->operation == '^' && isNodeVariable(rightOp->left) && isNodeNumber(rightOp->right) && ((NodeVariable*)op->left)->variable == ((NodeVariable*)rightOp->left)->variable){
            NodeOperation* newOp = new NodeOperation('^');
            NodeVariable* var = new NodeVariable(((NodeVariable*)op->left)->variable);
            NodeNumber* num = new NodeNumber(((NodeNumber*)rightOp->right)->number + 1);

            newOp->left = var;
            newOp->right = num;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 16: Si la operación es multiplicación entre dos subárboles que elevan a las variables a números, se suman los exponentes.
        }else if(op->operation == '*' && isNodeOperation(op->left) && isNodeOperation(op->right) && leftOp->operation == '^' && rightOp->operation == '^' && isNodeVariable(leftOp->left) && isNodeVariable(rightOp->left) && ((NodeVariable*)leftOp->left)->variable == ((NodeVariable*)rightOp->left)->variable){
            NodeOperation* newOp = new NodeOperation('^');
            NodeVariable* var = new NodeVariable(((NodeVariable*)leftOp->left)->variable);
            NodeNumber* num = new NodeNumber(((NodeNumber*)leftOp->right)->number + ((NodeNumber*)rightOp->right)->number);

            newOp->left = var;
            newOp->right = num;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 17: Si la operación es suma, el hijo izquierdo es suma, el derecho es suma, el hijo izquierdo del hijo izquierdo
        //es número, el hijo derecho del hijo izquierdo es variable, el hijo izquierdo del hijo derecho es número y el hijo derecho
        //del hijo derecho es variable, se suman los números.
        }else if(op->operation == '+' && isNodeOperation(op->left) && isNodeOperation(op->right) && leftOp->operation == '+' && rightOp->operation == '+' && isNodeNumber(leftOp->left) && isNodeVariable(leftOp->right) && isNodeNumber(rightOp->left) && isNodeVariable(rightOp->right)){
            NodeOperation* newOp = new NodeOperation('+');
            NodeNumber* num = new NodeNumber(((NodeNumber*)leftOp->left)->number + ((NodeNumber*)rightOp->left)->number);
            NodeOperation* newOpRight = new NodeOperation('*');
            NodeNumber* numLeft = new NodeNumber(2);
            NodeVariable* varLeft = new NodeVariable(((NodeVariable*)leftOp->right)->variable);

            newOp->left = num;
            newOp->right = newOpRight;
            newOpRight->left = numLeft;
            newOpRight->right = varLeft;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 18: Si la operación es suma, el hijo izquierdo es suma, el derecho es suma, el hijo izquierdo del hijo izquierdo
        //es número, el hijo derecho del hijo izquierdo es igual al hijo derecho del hijo derecho, el hijo izquierdo del hijo derecho
        //es número, se suman los números y se multiplica.
        }else if(op->operation == '+' && isNodeOperation(op->left) && isNodeOperation(op->right) && leftOp->operation == '+' && rightOp->operation == '+' && isNodeNumber(leftOp->left) && equal(leftOp->right, rightOp->right) && isNodeNumber(rightOp->left)){
            NodeOperation* newOp = new NodeOperation('+');
            NodeNumber* num = new NodeNumber(((NodeNumber*)leftOp->left)->number + ((NodeNumber*)rightOp->left)->number);
            NodeOperation* newOpRight = new NodeOperation('*');
            NodeNumber* numLeft = new NodeNumber(2);
            NodeOperation* newOpRightRight = (NodeOperation*)clone(rightOp->right);

            newOp->left = num;
            newOp->right = newOpRight;
            newOpRight->left = numLeft;
            newOpRight->right = newOpRightRight;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 19: Si la operación es multiplicación, el hijo izquierdo es número, el hijo derecho otra multiplicación,
        //el hijo izquierdo del derecho es un número y el hijo derecho del derecho es una variable, se multiplican
        //los números y se multiplica por la variable.
        }else if(op->operation == '*' && isNodeNumber(op->left) && isNodeOperation(op->right) && rightOp->operation == '*' && isNodeNumber(rightOp->left) && isNodeVariable(rightOp->right)){
            NodeOperation* newOp = new NodeOperation('*');
            NodeNumber* num = new NodeNumber(((NodeNumber*)op->left)->number * ((NodeNumber*)rightOp->left)->number);
            NodeVariable* var = new NodeVariable(((NodeVariable*)rightOp->right)->variable);

            newOp->left = num;
            newOp->right = var;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 20: Si la operación es más, el hijo izquierdo es un número, el hijo derecho es una suma y
        //el hijo izquierdo del derecho es un número, se suman y se suma el subárbol derecho.
        }else if(op->operation == '+' && isNodeNumber(op->left) && isNodeOperation(op->right) && rightOp->operation == '+' && isNodeNumber(rightOp->left)){
            NodeOperation* newOp = new NodeOperation('+');
            NodeNumber* num = new NodeNumber(((NodeNumber*)op->left)->number + ((NodeNumber*)rightOp->left)->number);
            NodeOperation* newOpRight = (NodeOperation*)clone(rightOp->right);

            newOp->left = num;
            newOp->right = newOpRight;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;
        
        //Caso 21: Si la operación es más, el hijo izquierdo es variable, el hijo derecho es suma, el hijo derecho del derecho
        //es variable, se suman.
        }else if(op->operation == '+' && isNodeVariable(op->left) && isNodeOperation(op->right) && rightOp->operation == '+' && isNodeVariable(rightOp->right) && ((NodeVariable*)op->left)->variable == ((NodeVariable*)rightOp->right)->variable){
            NodeOperation* newOp = new NodeOperation('+');
            NodeNumber* num = new NodeNumber(2);
            NodeOperation* mult = new NodeOperation('*');
            NodeVariable* var = new NodeVariable(((NodeVariable*)op->left)->variable);

            newOp->left = clone(rightOp->left);
            newOp->right = mult;
            mult->left = num;
            mult->right = var;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 22: Si la operación es multiplicación, su hijo izquierdo es número, su hijo derecho es multiplicación y
        //el hijo izquierdo del derecho es número, se multiplican los números.
        }else if(op->operation == '*' && isNodeNumber(op->left) && isNodeOperation(op->right) && rightOp->operation == '*' && isNodeNumber(rightOp->left)){
            NodeOperation* newOp = new NodeOperation('*');
            NodeNumber* num = new NodeNumber(((NodeNumber*)op->left)->number * ((NodeNumber*)rightOp->left)->number);

            newOp->left = num;
            newOp->right = clone(rightOp->right);
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 23: Si la operación es elevado, su hijo derecho es número, su hijo izquierdo es elevado y
        //el hijo derecho del hijo izquierdo es número, se multiplican los números.
        }else if(op->operation == '^' && isNodeNumber(op->right) && isNodeOperation(op->left) && leftOp->operation == '^' && isNodeNumber(leftOp->right)){
            NodeOperation* newOp = new NodeOperation('^');
            NodeNumber* num = new NodeNumber(((NodeNumber*)op->right)->number * ((NodeNumber*)leftOp->right)->number);

            newOp->left = clone(leftOp->left);
            newOp->right = num;
            newOp->parent = op->parent;

            link(newOp);

            return newOp;

        //Caso 24: Si la operación es más y ambos hijos son iguales (último caso), se multiplica por 2.
        }else if(op->operation == '+' && equal(op->left, op->right)){
            NodeOperation* newOp = new NodeOperation('*');
            newOp->parent = op->parent;
            newOp->left = new NodeNumber(2);
            newOp->right = clone(op->left);

            link(newOp);

            return newOp;

        //Caso 25: Si la operación es menos y ambos hijos son iguales (último caso), se retorna 0.
        }else if(op->operation == '-' && equal(op->left, op->right)){
            NodeNumber* num = new NodeNumber(0);
            num->parent = op->parent;

            link(num);

            return num;

        //Caso 26: Si la operación es suma y solo tiene un tipo de variable, se suma el coeficiente.
        }else if(hasOnlyAddition(op) && op->operation == '+' && hasOnlyOneTypeVariable(op, c)){
            NodeOperation* plus = new NodeOperation('+');
            NodeOperation* mult = new NodeOperation('*');

            NodeNumber* num = new NodeNumber(addAllNodeNumber(op));
            NodeVariable* var = new NodeVariable(c);
            NodeNumber* coef = new NodeNumber(countVariables(op));

            plus->left = num;
            plus->right = mult;
            mult->left = coef;
            mult->right = var;

            link(plus);
            link(mult);

            return plus;

        //Caso 27: Se simplifica por ambos lados.
        }else{
            op->left = simplify(op->left);
            op->right = simplify(op->right);

            link(op);

            return op;
        }
    }
    return node;
}

//Método que retorna el árbol ordenado, es decir, priorizará a los números para que vayan a la izquierda
//y lo demás a la derecha.
Node* AST::sort(Node* node){
    if(node == nullptr){
        return nullptr;
    }

    //Si el nodo es de tipo operación, se ordena recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)clone(node);
        Node* aux = nullptr;

        //Caso 1: Si el número está a la derecha y una variable a la izquierda, mientras no sea
        //elevación o resta, se cambian de lugar.
        if(isNodeNumber(op->right) && isNodeVariable(op->left) && op->operation != '^' && op->operation != '-'){
            aux = op->left;
            op->left = op->right;
            op->right = aux;

            link(op);

            return op;
        
        //Caso 2: Si el número está a la derecha y una operación a la izquierda, mientras no sea
        //elevar o restar, se cambian de lugar y se ordena el subárbol derecho.
        }else if(isNodeNumber(op->right) && isNodeOperation(op->left) && op->operation != '^' && op->operation != '-'){
            aux = op->left;
            op->left = op->right;
            op->right = aux;

            op->right = sort(op->right);

            link(op);

            return op;
        //Caso 3: Si es operación o multiplicación, se ordenan los subárboles.
        }else if(op->operation == '+' || op->operation == '*'){
            //Si el derecho es una variable y la izquierda es operación, se cambian los hijos.
            if(isNodeVariable(op->right) && isNodeOperation(op->left)){
                aux = op->left;
                op->left = op->right;
                op->right = aux;

                op->right = sort(op->right);

                link(op);

                return op;
            //Si el derecho es una variable y la izquierda es número, se cambian los hijos.
            }else{
                op->left = sort(op->left);
                op->right = sort(op->right);

                link(op);

                return op;
            }
        
        }else{
            link(op);

            return op;
        }
    //Si el nodo es de tipo número o variable, se deja como está.
    }else if(isNodeNumber(node) || isNodeVariable(node)){
        return node;
    }else{
        cout << "Error" << endl;
        exit(1);
    }
}

//Método que determina si un árbol contiene variables.
bool AST::hasVariableInTree(Node* node){
    if(node == nullptr){
        return false;
    }

    //Si el nodo es de tipo operación, se determina recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)node;

        return hasVariableInTree(op->left) || hasVariableInTree(op->right);

    //Si el nodo es de tipo variable, se retorna true.
    }else if(isNodeVariable(node)){
        return true;

    //Si el nodo es de tipo número, se retorna false.
    }else if(isNodeNumber(node)){
        return false;

    }else{
        cout << "Error" << endl;
        exit(1);
    }
}

//Método que determina si un árbol contiene solo sumas.
bool AST::hasOnlyAddition(Node* node){
    if(node == nullptr){
        return false;
    }

    //Si el nodo es de tipo operación, se determina recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)node;

        if(op->operation == '+'){
            return hasOnlyAddition(op->left) && hasOnlyAddition(op->right);
        }else{
            return false;
        }

    //Si el nodo es de tipo variable, se retorna true.
    }else if(isNodeVariable(node)){
        return true;

    //Si el nodo es de tipo número, se retorna false.
    }else if(isNodeNumber(node)){
        return true;

    }else{
        cout << "Error" << endl;
        exit(1);
    }
}

//Método que permite sumar todos los números de un árbol.
int AST::addAllNodeNumber(Node* node){
    if(node == nullptr){
        return 0;
    }

    //Si el nodo es de tipo operación, se determina recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)node;

        return addAllNodeNumber(op->left) + addAllNodeNumber(op->right);
    }else if(isNodeNumber(node)){
        return ((NodeNumber*)node)->number;
    }else{
        return 0;
    }
}

//Método que permite contar la cantidad de variables en un árbol.
int AST::countVariables(Node* node){
    if(node == nullptr){
        return 0;
    }

    //Si el nodo es de tipo operación, se determina recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)node;

        return countVariables(op->left) + countVariables(op->right);
    }else if(isNodeVariable(node)){
        return 1;

    }else{
        return 0;
    }
}

bool AST::hasOnlyOneTypeVariable(Node* node, char c){
    if(node == nullptr){
        return false;
    }

    //Si el nodo es de tipo operación, se determina recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)node;
        return hasOnlyOneTypeVariable(op->left, c) && hasOnlyOneTypeVariable(op->right, c);

    }else if(isNodeVariable(node)){
        return ((NodeVariable*)node)->variable == c;

    }else{
        return true;
    }
}

char AST::findVariable(Node* node){
    if(node == nullptr){
        return ' ';
    }

    //Si el nodo es de tipo operación, se determina recursivamente
    if(isNodeOperation(node)){
        NodeOperation* op = (NodeOperation*)node;
        char left = findVariable(op->left);
        char right = findVariable(op->right);

        if(left != ' '){
            return left;
        }else if(right != ' '){
            return right;
        }else{
            return ' ';
        }

    }else if(isNodeVariable(node)){
        return ((NodeVariable*)node)->variable;

    }else{
        return ' ';
    }
}
