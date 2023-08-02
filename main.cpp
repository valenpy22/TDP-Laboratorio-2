#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <stack>
#include <cmath>

#include "AST.h"
#include "NodeOperation.h"
#include "NodeNumber.h"
#include "NodeVariable.h"

using namespace std;

/*
ATENCIÓN: AL REALIZAR OPERACIONES SOBRE EL ÁRBOL ESTE CAMBIA.
PARA VOLVER AL ÁRBOL ORIGINAL, SE DEBE INGRESAR LA OPCIÓN 7.
*/

//Método principal
int main(){
    string filename;
    string opcion;

    cout << "Ingrese el nombre del archivo: ";
    cin >> filename;

    //Se crea el árbol
    AST* tree = new AST(filename);

    bool option = true;

    //Se imprime el árbol
    tree->printAST(tree->root);

    //Se crea un menú para que el usuario pueda interactuar con el árbol
    while(option){
        cout << "\nIngrese la opción deseada: " << endl;
        cout << "1. Imprimir" << endl;
        cout << "2. Evaluar árbol" << endl;
        cout << "3. Reemplazar con variables" << endl;
        cout << "4. Reemplazar con variables y evaluar" << endl;
        cout << "5. Derivar" << endl;
        cout << "6. Simplificar" << endl;
        cout << "7. Volver al árbol original" << endl;
        cout << "8. Salir" << endl;

        cin >> opcion;

        //Imprime el árbol
        if(opcion == "1"){
            tree->printAST(tree->root);
            cout << endl;
            
        //Evalúa el árbol
        }else if(opcion == "2"){
            if(tree->hasVariableInTree(tree->root)){
                cout << "No se puede evaluar el árbol ya que contiene VARIABLES" << endl;
            }else{
                cout << "El resultado es: " << tree->evalInt(tree->root) << endl;
            }
            
        //Reemplaza las variables
        }else if(opcion == "3"){
            cout << "Ingrese las variables a reemplazar SIN espacios: ";
            string vars;
            cin >> vars;

            int* values = new int[vars.length()];

            for(int i = 0; i < vars.length(); i++){
                cout << "Ingrese el valor de " << vars[i] << ": ";
                cin >> values[i];
            }

            //Se reemplazan las variables
            tree->root = tree->replace(tree->root, vars, values);
            tree->printAST(tree->root);

        //Reemplaza las variables y evalúa el árbol
        }else if(opcion == "4"){
            cout << "Ingrese las variables a reemplazar SIN espacios: ";
            string vars;
            cin >> vars;

            int* values = new int[vars.length()];

            for(int i = 0; i < vars.length(); i++){
                cout << "Ingrese el valor de " << vars[i] << ": ";
                cin >> values[i];
            }

            //Se reemplazan las variables
            tree->root = tree->replace(tree->root, vars, values);

            //Si tiene variables, no se puede evaluar
            if(tree->hasVariableInTree(tree->root)){
                cout << endl;
                cout << "No se puede evaluar el árbol ya que contiene VARIABLES" << endl;
                cout << "Debe ingresar TODOS los VALORES de las VARIABLES" << endl;
            //Si no tiene variables, se evalúa
            }else{
                cout << "El resultado es: " << tree->evalInt(tree->root) << endl;
            }

        //Deriva el árbol
        }else if(opcion == "5"){
            cout << "Ingrese la variable a derivar: ";
            char var;
            cin >> var;

            tree->root = tree->derivate(tree->root, var);
            tree->printAST(tree->root);
            cout << endl;

        //Simplifica el árbol
        }else if(opcion == "6"){
            string times;
            tree->root = tree->simplify(tree->root);

            //Se pregunta cuántas veces se desea simplificar el árbol
            cout << "¿Cuántas veces desea realizar la simplificación sobre el árbol?: " << endl;
            cout << "PD: Mientras más veces, más simplificado." << endl;
            cout << "PD2: Si es un árbol de sumas, basta con 1 simplificación." << endl;
            cin >> times;

            //Se convierte el string a int
            int t = stoi(times);

            //Se simplifica el árbol la cantidad de veces ingresada
            for(int i = 0; i < t; i++){
                tree->root = tree->simplify(tree->root);
            }

            //Se imprime el árbol
            tree->printAST(tree->root);
            cout << endl;

        //Vuelve al árbol original
        }else if(opcion == "7"){
            tree->parse();
            tree->printAST(tree->root);
            cout << endl;

        //Sale del programa
        }else if(opcion == "8"){
            option = false;
        } 
    }
    return 1;
}

