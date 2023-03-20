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

struct ParPerturbado
{
	int desde;
	int hasta;
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

int respAleatoria(vector<Vertice> todosVert, vector<vector<int>> conexiones, vector<int> &cubierta);

bool chequearIntercambio(vector<int> lados, vector<int> solucionInicial, vector<int> solucionFinal, int elemQuita, int elemAgre);

bool verificarSolucionBL(vector<int> solucion, vector<int> conexiones);

int busquedaLocal(vector<vector<int>> conexiones, vector<int> &cubierta);

bool verificarMemoriaTabu(vector<vector<ParPerturbado>> memoria, int desde, int hasta);

vector<int> busquedaLocalTabu(vector<vector<int>> conexiones, vector<int> &cubierta, vector<vector<ParPerturbado>> &memoria);

int busquedaTabu(vector<vector<int>> conexiones, vector<int> &cubierta);

vector<bool> codificar(vector<int> fenotipo, int size);

vector<int> decodificar(vector<bool> genotipo);

int aptitud(vector<bool> genotipo);

void seleccionarPadres(vector<bool> &padre1, vector<bool> &padre2, vector<vector<bool>> poblacion);

void generarPoblacion(vector<vector<bool>> &poblacion, vector<vector<int>> conexiones, vector<Vertice> todosVert, int tam);

void generarHijos(vector<bool> padre1, vector<bool> padre2, vector<bool> &hijo1, vector<bool> &hijo2);

void mutarHijo(vector<bool> &hijo);

vector<vector<bool>> nuevaPoblacion(vector<vector<bool>> poblacion, vector<bool> hijo1, vector<bool> hijo2, vector<vector<int>> conexiones, int tam);

int algoritmoGenetico(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta);

vector<int> indiceVertirceOrdenGrado(vector<int> cubierta, vector<Vertice> todosVert);

bool verificarEnMemoriaBLI(vector<int> cubierta, vector<vector<int>> memoria);

vector<int> perturbacionBLI(vector<int> cubierta, vector<vector<int>> &memoria, vector<vector<int>> conexiones, int distancia);

int busquedaLocalIterada(vector<Vertice> todosVert, vector<vector<int>> conexiones, vector<int> &cubierta);

int SeleccionMetodo(int mode, string dir, int numberTest);

#endif