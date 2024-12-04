#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Nodo {
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead;
    bool was_king;
    bool is_king;
    Nodo* left;
    Nodo* right;

    Nodo(int id, string name, string last_name, char gender, int age, int id_father, bool is_dead, bool was_king, bool is_king)
        : id(id), name(name), last_name(last_name), gender(gender), age(age), id_father(id_father), is_dead(is_dead), was_king(was_king), is_king(is_king), left(nullptr), right(nullptr) {}
};

class ArbolBinario {
private:
    Nodo* raiz;

    Nodo* insertarNodo(Nodo* raiz, Nodo* nuevoNodo) {
        if (raiz == nullptr) {
            return nuevoNodo;
        }
        if (nuevoNodo->id_father == raiz->id) {
            if (raiz->left == nullptr) {
                raiz->left = nuevoNodo;
            } else {
                raiz->right = nuevoNodo;
            }
        } else {
            raiz->left = insertarNodo(raiz->left, nuevoNodo);
            raiz->right = insertarNodo(raiz->right, nuevoNodo);
        }
        return raiz;
    }

    void mostrarSucesion(Nodo* nodo) {
        if (nodo == nullptr) {
            return;
        }
        if (!nodo->is_dead) {
            cout << nodo->name << " " << nodo->last_name << endl;
        }
        mostrarSucesion(nodo->left);
        mostrarSucesion(nodo->right);
    }

    Nodo* buscarRey(Nodo* nodo) {
        if (nodo == nullptr) {
            return nullptr;
        }
        if (nodo->is_king) {
            return nodo;
        }
        Nodo* rey = buscarRey(nodo->left);
        if (rey == nullptr) {
            rey = buscarRey(nodo->right);
        }
        return rey;
    }

    Nodo* buscarPrimogenitoVivo(Nodo* nodo) {
        if (nodo == nullptr) {
            return nullptr;
        }
        if (!nodo->is_dead) {
            return nodo;
        }
        Nodo* primogenito = buscarPrimogenitoVivo(nodo->left);
        if (primogenito == nullptr) {
            primogenito = buscarPrimogenitoVivo(nodo->right);
        }
        return primogenito;
    }

    void asignarNuevoRey(Nodo* nodo) {
        Nodo* rey = buscarRey(nodo);
        if (rey == nullptr) {
            return;
        }
        if (rey->is_dead || rey->age > 70) {
            Nodo* nuevoRey = buscarPrimogenitoVivo(rey->left);
            if (nuevoRey == nullptr) {
                nuevoRey = buscarPrimogenitoVivo(rey->right);
            }
            if (nuevoRey != nullptr) {
                nuevoRey->is_king = true;
                rey->is_king = false;
            }
        }
    }

    Nodo* buscarNodo(Nodo* nodo, int id) {
        if (nodo == nullptr) {
            return nullptr;
        }
        if (nodo->id == id) {
            return nodo;
        }
        Nodo* encontrado = buscarNodo(nodo->left, id);
        if (encontrado == nullptr) {
            encontrado = buscarNodo(nodo->right, id);
        }
        return encontrado;
    }

public:
    ArbolBinario() : raiz(nullptr) {}

    void insertar(int id, string name, string last_name, char gender, int age, int id_father, bool is_dead, bool was_king, bool is_king) {
        Nodo* nuevoNodo = new Nodo(id, name, last_name, gender, age, id_father, is_dead, was_king, is_king);
        if (raiz == nullptr) {
            raiz = nuevoNodo;
        } else {
            insertarNodo(raiz, nuevoNodo);
        }
    }

    void mostrarSucesion() {
        mostrarSucesion(raiz);
    }

    void asignarNuevoRey() {
        asignarNuevoRey(raiz);
    }

    void cambiarDatos(int id, string name, string last_name, char gender, int age, bool is_dead, bool was_king, bool is_king) {
        Nodo* nodo = buscarNodo(raiz, id);
        if (nodo != nullptr) {
            nodo->name = name;
            nodo->last_name = last_name;
            nodo->gender = gender;
            nodo->age = age;
            nodo->is_dead = is_dead;
            nodo->was_king = was_king;
            nodo->is_king = is_king;
        }
    }
};

void leerCSV(string nombreArchivo, ArbolBinario& arbol) {
    ifstream archivo(nombreArchivo);
    string linea;
    getline(archivo, linea); // Leer la cabecera
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string id, name, last_name, gender, age, id_father, is_dead, was_king, is_king;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, last_name, ',');
        getline(ss, gender, ',');
        getline(ss, age, ',');
        getline(ss, id_father, ',');
        getline(ss, is_dead, ',');
        getline(ss, was_king, ',');
        getline(ss, is_king, ',');

        arbol.insertar(stoi(id), name, last_name, gender[0], stoi(age), stoi(id_father), stoi(is_dead), stoi(was_king), stoi(is_king));
    }
}

void mostrarMenu() {
    cout << "Seleccione una opcion:" << endl;
    cout << "1. Mostrar línea de sucesion" << endl;
    cout << "2. Asignar nuevo rey" << endl;
    cout << "3. Cambiar datos de un nodo" << endl;
    cout << "4. Salir" << endl;
}

int main() {
    ArbolBinario arbol;
    leerCSV("arbol_familiar.csv", arbol);

    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Línea de sucesion actual:" << endl;
                arbol.mostrarSucesion();
                break;
            case 2:
                arbol.asignarNuevoRey();
                cout << "Nuevo rey asignado." << endl;
                break;
            case 3: {
                int id, age;
                string name, last_name;
                char gender;
                bool is_dead, was_king, is_king;
                cout << "Ingrese el ID del nodo a cambiar: ";
                cin >> id;
                cout << "Ingrese el nuevo nombre: ";
                cin >> name;
                cout << "Ingrese el nuevo apellido: ";
                cin >> last_name;
                cout << "Ingrese el nuevo genero (H/M): ";
                cin >> gender;
                cout << "Ingrese la nueva edad: ";
                cin >> age;
                cout << "¿Esta muerto? (1: sí, 0: no): ";
                cin >> is_dead;
                cout << "¿Fue rey? (1: sí, 0: no): ";
                cin >> was_king;
                cout << "¿Es rey? (1: sí, 0: no): ";
                cin >> is_king;
                arbol.cambiarDatos(id, name, last_name, gender, age, is_dead, was_king, is_king);
                cout << "Datos cambiados." << endl;
                break;
            }
            case 4:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    } while (opcion != 4);

    return 0;
}