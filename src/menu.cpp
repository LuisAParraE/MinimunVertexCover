#include <iostream>
#include "ImplementacionExacta.h"

using namespace std;

int main()
{

	int modo;
	int cantidadItera;
	string archivo;

	cout << "Que Modo quieres Probar: ";
	cin >> modo;
	cout << "Cantidad de Iteraciones: ";
	cin >> cantidadItera;
	cout << "Archivo a Probar: ";
	cin >> archivo;

	SeleccionMetodo(modo, archivo, cantidadItera);

	return 0;
}