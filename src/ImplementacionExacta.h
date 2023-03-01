#ifndef IMPLEMENTACIONEXACTA_H
#define IMPLEMENTACIONEXACTA_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#define max 3

using namespace std;

struct Vertice
{
	int nro;
	int grado;

	bool operator>(const Vertice &str) const
	{
		return (grado > str.grado);
	}
};

int len(string str);

void split(string str, char seperator);

int lecturaGrafo(string dir, vector<Vertice> &todosVert, vector<vector<int>> &conexiones, int &nroVer, int &nroAris);

void EliminarLado(int vert1, int vert2, vector<vector<int>> &conexiones, vector<Vertice> &todosVert, int &nroAris);

bool vecinosAdyacentes(int vecino1, int vecino2, vector<vector<int>> &conexiones);

void addVer(int vert, vector<int> &cubierta, vector<vector<int>> &conexiones, vector<Vertice> &todosVert, int &nroAris);

void checkVer(vector<int> &cubierta, vector<vector<int>> &conexiones, vector<Vertice> &todosVert, int &nroAris);

bool verificarSolucion(vector<int> solucion, vector<vector<int>> conexiones);

int respExacta(vector<int> todosVert, vector<vector<int>> conexiones, int cantidadVertices);

int respAproximada(vector<Vertice> &todosVert, vector<vector<int>> &conexiones, int &nroVer, int &nroAris, vector<int> &cubierta, int maximo);

bool chequearIntercambio(vector<int> lados, vector<int> solucionInicial, vector<int> solucionFinal, int elemQuita, int elemAgre);

int busquedaLocal(vector<vector<int>> conexiones, vector<int> &cubierta);

vector<int> indiceVertirceOrdenGrado(vector<int> cubierta, vector<Vertice> todosVert);

bool verificarEnMemoriaBLI(vector<int> cubierta, vector<vector<int>> memoria);

vector<int> perturbacionBLI(vector<int> cubierta, vector<vector<int>> &memoria, vector<vector<int>> conexiones, int distancia);

int busquedaLocalIterada(vector<Vertice> todosVert, vector<vector<int>> conexiones, vector<int> &cubierta);

int SeleccionMetodo(int mode, string dir, int numberTest);

#endif