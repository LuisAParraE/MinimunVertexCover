#include "ImplementacionExacta.h"

using namespace std;

string subCadenas[max];

// FUCIONES

// length of the string
int len(string str)
{
	int length = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		length++;
	}
	return length;
}

// create custom split() function
void split(string str, char seperator)
{
	int currIndex = 0, i = 0;
	int startIndex = 0, endIndex = 0;
	while (i <= len(str))
	{
		if (str[i] == seperator || i == len(str))
		{
			endIndex = i;
			string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);
			subCadenas[currIndex] = subStr;
			currIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
}

// Función que lee los archivos de Prueba. Inicializa tanto los lados como los vertices.
int lecturaGrafo(string dir, vector<Vertice> &todosVert, vector<vector<int>> &conexiones, int &nroVer, int &nroAris)
{
	int maxVerCuenta = -1;
	int maxVerIndi = 0;
	int lado1, lado2;
	string line = "";
	ifstream file;
	file.open(dir);

	getline(file, line, ' ');
	getline(file, line, ' ');
	getline(file, line, ' ');

	// Sacamos el nro de Vertices
	nroVer = stoi(line);

	getline(file, line, ' ');

	nroAris = stoi(line);

	for (int i = 0; i < nroVer; i++)
	{
		Vertice nuevoVertice;
		nuevoVertice.nro = i + 1;
		nuevoVertice.grado = 0;
		todosVert.push_back(nuevoVertice);
		vector<int> nuevoVector;
		conexiones.push_back(nuevoVector);
	}

	getline(file, line);
	while (getline(file, line))
	{
		split(line, ' ');
		lado1 = stoi(subCadenas[1]);
		// cout << "lado1: "<<lado1<<" ";

		lado2 = stoi(subCadenas[2]);
		// cout << "lado2: " << lado2 << "\n";

		// Aumentamos el Grado de los Vertices
		todosVert[lado1 - 1].grado = todosVert[lado1 - 1].grado + 1;
		todosVert[lado2 - 1].grado = todosVert[lado2 - 1].grado + 1;

		// Agregamos el Lado a la Lista de Conexiones.
		conexiones[lado1 - 1].push_back(lado2);
		conexiones[lado2 - 1].push_back(lado1);

		if (todosVert[lado1 - 1].grado >= maxVerCuenta)
		{
			maxVerIndi = lado1;
			maxVerCuenta = todosVert[lado1 - 1].grado;
		}

		if (todosVert[lado2 - 1].grado >= maxVerCuenta)
		{
			maxVerIndi = lado2;
			maxVerCuenta = todosVert[lado2 - 1].grado;
		}
	}

	file.close();

	return maxVerIndi;
}

void EliminarLado(int vert1, int vert2, vector<vector<int>> &conexiones, vector<Vertice> &todosVert, int &nroAris)
{

	vector<int>::iterator it = conexiones[vert1 - 1].begin();

	for (unsigned int i = 0; i < conexiones[vert1 - 1].size(); i++)
	{
		if (conexiones[vert1 - 1][i] == vert2)
		{
			conexiones[vert1 - 1].erase(it);
			break;
		}
		it++;
	}

	nroAris = nroAris - 1;
	todosVert[vert1 - 1].grado = todosVert[vert1 - 1].grado - 1;
}

bool vecinosAdyacentes(int vecino1, int vecino2, vector<vector<int>> &conexiones)
{

	for (unsigned int i = 0; i < conexiones[vecino1 - 1].size(); i++)
	{
		if (conexiones[vecino1 - 1][i] == vecino2)
		{
			return true;
		}
	}

	return false;
}

void addVer(int vert, vector<int> &cubierta, vector<vector<int>> &conexiones, vector<Vertice> &todosVert, int &nroAris)
{
	// cout << "Anadimos: " << vert <<"\n";
	cubierta.push_back(vert);
	// cout << "Aristas antes: " << nroAris << "\n";
	for (unsigned int i = 0; i < conexiones[vert - 1].size(); i++)
	{
		EliminarLado(conexiones[vert - 1][i], vert, conexiones, todosVert, nroAris);
	}
	// cout << "Aristas Despues: " << nroAris << "\n";

	conexiones[vert - 1].clear();
	todosVert[vert - 1].grado = 0;
	/*
	for (unsigned int j = 0; j < todosVert.size(); j++)
	{
		cout << "Vertice: " << todosVert[j].nro << "\n";

		for (unsigned int i = 0; i < conexiones[j].size(); i++)
		{
			cout << conexiones[j][i] << " ";
		}
		cout << "\n";
	}
	*/
}

void checkVer(vector<int> &cubierta, vector<vector<int>> &conexiones, vector<Vertice> &todosVert, int &nroAris)
{

	int maxVerCuenta = -1;
	int maxVerIndi = 0;

	for (unsigned int i = 0; i < todosVert.size(); i++)
	{
		if (todosVert[i].grado == 1)
		{

			addVer(conexiones[i][0], cubierta, conexiones, todosVert, nroAris);
			return;
		}
		else if (todosVert[i].grado == 2)
		{
			if (vecinosAdyacentes(conexiones[i][0], conexiones[i][1], conexiones))
			{

				int auxVer1, auxVer2;
				auxVer1 = conexiones[i][0];
				auxVer2 = conexiones[i][1];

				addVer(auxVer1, cubierta, conexiones, todosVert, nroAris);
				addVer(auxVer2, cubierta, conexiones, todosVert, nroAris);

				return;
			}
		}

		if (todosVert[i].grado >= maxVerCuenta)
		{
			maxVerIndi = todosVert[i].nro;
			maxVerCuenta = todosVert[i].grado;
		}
	}

	if (maxVerCuenta > 0)
	{
		addVer(maxVerIndi, cubierta, conexiones, todosVert, nroAris);
	}
}

bool verificarSolucion(vector<int> solucion, vector<vector<int>> conexiones)
{
	// cout << "Entre a Verificar" << endl;
	vector<vector<int>> todasConexiones = conexiones;
	unsigned int verticeIndex;
	unsigned int aux;
	vector<int>::iterator inicio;
	string casa;

	for (unsigned int i = 0; i < solucion.size(); i++)
	{
		verticeIndex = solucion[i] - 1;

		for (unsigned int j = 0; j < todasConexiones[verticeIndex].size(); j++)
		{

			aux = todasConexiones[verticeIndex][j] - 1;
			inicio = todasConexiones[aux].begin();

			// Eliminamos todas las Conexiones HACIA el vertice solucion
			for (unsigned int k = 0; k < todasConexiones[aux].size(); k++)
			{
				if ((int)(verticeIndex + 1) == todasConexiones[aux][k])
				{

					todasConexiones[aux].erase(inicio + k);

					break;
				}
			}

			// Eliminamos Todas Las Conexiones Desde El vertice solucion
		}
		todasConexiones[verticeIndex].clear();
	}

	for (unsigned int i = 0; i < conexiones.size(); i++)
	{
		if (!todasConexiones[i].empty())
		{
			// cout << "Verifique Falso" << endl;
			return false;
		}
	}
	// cout << "Verifique Verdadero" << endl;
	return true;
}

int respExacta(vector<int> todosVert, vector<vector<int>> conexiones, int cantidadVertices)
{
	int cantidadMin = cantidadVertices;
	int posibleMin;
	vector<int> solucion;
	vector<int> OriginaltodosVert = todosVert;
	vector<int>::iterator inicio;

	for (unsigned int i = 0; i < todosVert.size(); i++)
	{
		// cout << cantidadMin << endl;
		solucion = OriginaltodosVert;
		inicio = solucion.begin();
		solucion.erase(inicio + i);
		/*
		cout << "Posible Solucion-> ";
		for (unsigned int w = 0; w < solucion.size(); w++)
		{
			cout << solucion[w] << " ";
		}
		cout << endl;
		*/
		if (verificarSolucion(solucion, conexiones))
		{
			posibleMin = respExacta(solucion, conexiones, cantidadVertices - 1);

			if (posibleMin < cantidadMin)
			{
				// cout << "Menor Vertice" << endl;
				cantidadMin = posibleMin;
				// cout << cantidadMin << endl;
			}
		}
	}

	return cantidadMin;
}

// FUNCION DE APROXIMACIÓN
int respAproximada(vector<Vertice> &todosVert, vector<vector<int>> &conexiones, int &nroVer, int &nroAris, vector<int> &cubierta, int maximo)
{

	/*
	for (unsigned int j = 0; j < todosVert.size(); j++)
	{
		cout <<"Vertice: " << todosVert[j].nro << "\n";

		for (unsigned int i = 0; i < conexiones[j].size(); i++)
		{
			cout << conexiones[j][i] << " ";
		}
		cout << "\n";
	}


	cout << "Agregue el primer vertice\n";
	*/
	addVer(maximo, cubierta, conexiones, todosVert, nroAris);

	while (nroAris > 0)
	{
		checkVer(cubierta, conexiones, todosVert, nroAris);
	}

	// cout << "Sali\n";
	// cout << "Aristas: " << nroAris << "\n";
	// cout << cubierta.size() << "\n";
	/*
	for (unsigned int j = 0; j < cubierta.size(); j++)
	{
		cout << cubierta[j] << "\n";
	}
	*/
	return cubierta.size();
}

// FUNCION DE SOLUCION ALEATORIA
int respAleatoria(vector<vector<int>> conexiones, vector<int> &cubierta)
{
	int nroAleatorio1, nroAleatorio2, vertice1, vertice2;
	vector<int> conexionesVertice1;
	bool encontrado;

	while (!verificarSolucion(cubierta, conexiones))
	{
		nroAleatorio1 = rand() % conexiones.size();
		vertice1 = nroAleatorio1 + 1;
		conexionesVertice1 = conexiones[nroAleatorio1];

		nroAleatorio2 = rand() % conexionesVertice1.size();
		vertice2 = conexionesVertice1[nroAleatorio2];

		encontrado = false;
		for (unsigned int i = 0; i < cubierta.size(); i++)
		{
			if (vertice1 == cubierta[i])
			{
				encontrado = true;
				break;
			}
		}

		if (!encontrado)
		{
			cubierta.push_back(vertice1);
		}

		encontrado = false;
		for (unsigned int i = 0; i < cubierta.size(); i++)
		{
			if (vertice2 == cubierta[i])
			{
				encontrado = true;
				break;
			}
		}

		if (!encontrado)
		{
			cubierta.push_back(vertice2);
		}
	}

	return cubierta.size();
}

bool chequearIntercambio(vector<int> lados, vector<int> solucionInicial, vector<int> solucionFinal, int elemQuita, int elemAgre)
{

	bool found;

	for (unsigned int i = 0; i < lados.size(); i++)
	{
		int auxVertice = lados[i];
		found = false;
		for (unsigned int j = 0; j < solucionInicial.size(); j++)
		{
			if (auxVertice == solucionInicial[j])
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			for (unsigned int j = 0; j < solucionFinal.size(); j++)
			{

				if (auxVertice == solucionFinal[j])
				{
					found = true;
					break;
				}
			}
		}

		if (!found)
		{

			return false;
		}
	}

	return true;
}

bool verificarSolucionBL(vector<int> solucion, vector<int> conexiones)
{

	bool esSolucion;

	for (unsigned int i = 0; i < conexiones.size(); i++)
	{
		esSolucion = false;

		for (unsigned int j = 0; j < solucion.size(); j++)
		{
			if (conexiones[i] == solucion[j])
			{
				esSolucion = true;
				break;
			}
		}

		if (!esSolucion)
		{
			break;
		}
	}

	return esSolucion;
}

// FUNCION PRINCIPAL BUSQUEDA LOCAL
int busquedaLocal(vector<vector<int>> conexiones, vector<int> &cubierta)
{
	int elementoAleatorio1, elementoAleatorio2;
	int quitado, agregar;
	bool encontrado;
	vector<int> ladosDeX, ladosDeXTotales;
	vector<vector<int>> listaLados = conexiones;

	vector<int> solucionInicial = cubierta;
	vector<int> posibleSolucion;
	vector<int> solucionFinal = cubierta;

	while (solucionInicial.size() > 0)
	{
		posibleSolucion = solucionFinal;
		elementoAleatorio1 = rand() % solucionInicial.size();
		// cout << "Aleaorio 1:" << elementoAleatorio1 << endl;
		quitado = solucionInicial[elementoAleatorio1];
		ladosDeX = listaLados[quitado - 1];
		ladosDeXTotales = ladosDeX;
		encontrado = false;

		for (unsigned int w = 0; w < posibleSolucion.size(); w++)
		{
			if (posibleSolucion[w] == quitado)
			{
				posibleSolucion.erase(posibleSolucion.begin() + w);
				break;
			}
		}

		while (ladosDeX.size() > 0)
		{
			elementoAleatorio2 = rand() % ladosDeX.size();
			// cout << "Aleaorio 2:" << elementoAleatorio2 << endl;
			agregar = ladosDeX[elementoAleatorio2];
			ladosDeX.erase(ladosDeX.begin() + elementoAleatorio2);
			encontrado = false;

			// Verificamos si el nodo a agregar ya esta en la solución
			for (unsigned int z = 0; z < posibleSolucion.size(); z++)
			{
				if (posibleSolucion[z] == agregar)
				{
					encontrado = true;
					break;
				}
			}

			// Si no se encuentra, lo agregamos.
			if (!encontrado)
			{
				posibleSolucion.push_back(agregar);
			}

			// Verificamos que el Cambio sea una solucion
			if (verificarSolucionBL(posibleSolucion, ladosDeXTotales))
			{

				solucionFinal = posibleSolucion;
				encontrado = false;
				break;
			}

			if (!encontrado)
			{
				posibleSolucion.pop_back();
				encontrado = false;
			}

			if (ladosDeX.size() == 0)
			{
				posibleSolucion.push_back(quitado);
			}
		}

		// Quitamos el elemento que acabamos de probar
		solucionInicial.erase(solucionInicial.begin() + elementoAleatorio1);
	}

	cubierta = solucionFinal;
	return solucionFinal.size();
}

vector<int> indiceVertirceOrdenGrado(vector<int> cubierta, vector<Vertice> todosVert)
{

	vector<int> cubiertaOrdenada;
	vector<Vertice> verticesOrdenados;

	for (unsigned int i = 0; i < cubierta.size(); i++)
	{
		// cout << cubierta[i] - 1 << endl;
		verticesOrdenados.push_back(todosVert[cubierta[i] - 1]);
	}
	// cout << "Aqui" << endl;
	sort(verticesOrdenados.begin(), verticesOrdenados.end(), greater<Vertice>());

	for (unsigned int i = 0; i < verticesOrdenados.size(); i++)
	{
		cubiertaOrdenada.push_back(verticesOrdenados[i].nro);
	}
	// cout << "Aqui2" << endl;
	return cubiertaOrdenada;
}

bool verificarEnMemoriaBLI(vector<int> cubierta, vector<vector<int>> memoria)
{
	vector<int> posibleCubierta = cubierta;
	bool enMemoria;

	sort(posibleCubierta.begin(), posibleCubierta.end());

	for (unsigned int i = 0; i < memoria.size(); i++)
	{
		// cout << "ciclo 3" << endl;
		enMemoria = true;

		for (unsigned int j = 0; j < memoria[i].size(); j++)
		{
			// cout << "ciclo 4" << endl;
			// cout << "Mismo Tamano?" << endl;
			if (posibleCubierta.size() != memoria[i].size())
			{
				enMemoria = false;
				break;
			}
			// cout << "mismo elemento?" << endl;
			/*
			cout << "a: ";
			for (unsigned int w = 0; w < posibleCubierta.size(); w++)
			{
				cout << posibleCubierta[w] << " ";
			}
			cout << endl;
			cout << "b: ";
			for (unsigned int w = 0; w < memoria[i].size(); w++)
			{
				cout << memoria[i][w] << " ";
			}
			cout << endl;
			*/
			// cout << posibleCubierta[j] << endl;
			// cout << memoria[i][j] << endl;
			if (posibleCubierta[j] != memoria[i][j])
			{
				// cout << "entre" << endl;
				enMemoria = false;
				break;
			}
		}

		if (enMemoria)
		{
			// cout << "retorne" << endl;
			return true;
		}
	}

	// cout << "retorne" << endl;
	return false;
}

vector<int> perturbacionBLI(vector<int> cubierta, vector<vector<int>> &memoria, vector<vector<int>> conexiones, int distancia)
{

	vector<int> solucionPerturbada = cubierta;
	vector<int> indicesPerturbacion, indicesAux;
	int mitad = cubierta.size() / 2;
	int elementoPerturbar;
	int elementoAgregar;
	bool encontrado;

	for (int i = 1; i <= distancia; i++)
	{
		indicesPerturbacion.push_back(mitad - i);
		indicesPerturbacion.push_back(mitad + i);
	}

	// cout << "Seleccionados Elementos a Perturbar" << endl;
	//  Este ciclo se ejecuta hasta que se consiga una perturbacion que no esté en la memoria
	while (true)
	{
		// cout << "ciclo1" << endl;
		solucionPerturbada = cubierta;
		indicesAux = indicesPerturbacion;
		// Se le hace perturbacion a varios elementos
		while (indicesAux.size() > 0)
		{
			// cout << "ciclo2" << endl;
			encontrado = false;
			elementoPerturbar = solucionPerturbada[indicesAux.back()] - 1;

			solucionPerturbada.erase(solucionPerturbada.begin() + indicesAux.back());
			indicesAux.pop_back();

			vector<int> ladosDeX = conexiones[elementoPerturbar];
			elementoAgregar = ladosDeX[rand() % ladosDeX.size()];

			// Verificamos que no este ya el nodo en la posible solucion
			for (unsigned int z = 0; z < solucionPerturbada.size(); z++)
			{
				if (solucionPerturbada[z] == elementoAgregar)
				{
					encontrado = true;
					break;
				}
			}

			if (!encontrado)
			{
				solucionPerturbada.push_back(elementoAgregar);
			}
		}

		// cout << "Verficamos la perturbacion" << endl;
		if (!verificarEnMemoriaBLI(solucionPerturbada, memoria))
		{

			memoria.push_back(solucionPerturbada);

			sort(memoria[memoria.size() - 1].begin(), memoria[memoria.size() - 1].end());

			break;
		}
	}

	return solucionPerturbada;
}

// FUNCION DE BUSQUEDA LOCAL ITERADA
int busquedaLocalIterada(vector<Vertice> todosVert, vector<vector<int>> conexiones, vector<int> &cubierta, int iteraciones)
{
	vector<vector<int>> memoria;
	vector<int> posibleSolucion;
	int distancia;
	unsigned int menorCubierta;
	// cout << "Comienza Busqueda Local" << endl;
	menorCubierta = busquedaLocal(conexiones, cubierta);
	// cout << "Comienza a Order Vertices" << endl;
	cubierta = indiceVertirceOrdenGrado(cubierta, todosVert);
	// cout << "Ordene los vertices" << endl;
	distancia = 1;

	for (int i = 0; i < iteraciones; i++)
	{
		// cout << "Comenzo Perturbacion" << endl;
		vector<int> solucionPerturbada = perturbacionBLI(cubierta, memoria, conexiones, distancia);
		// cout << "Perturbacion Hecha" << endl;
		busquedaLocal(conexiones, solucionPerturbada);
		// cout << "BusquedaLocalHecha" << endl;
		if (verificarSolucion(solucionPerturbada, conexiones) && (solucionPerturbada.size() <= menorCubierta))
		{

			cubierta = indiceVertirceOrdenGrado(solucionPerturbada, todosVert);
			menorCubierta = cubierta.size();
		}
		// cout << "Pase el if" << endl;
		distancia++;
		if (2 * distancia >= cubierta.size())
		{
			distancia = cubierta.size() / 4;
		}
		// cout << "Sume distancia" << endl;
	}

	return menorCubierta;
}

bool verificarMemoriaTabu(vector<vector<ParPerturbado>> memoria, int desde, int hasta)
{
	for (unsigned int i = 0; i < memoria.size(); i++)
	{
		for (unsigned int j = 0; j < memoria[i].size(); j++)
		{
			if (memoria[i][j].desde == hasta && memoria[i][j].hasta == desde)
			{
				return true;
			}
		}
	}

	return false;
}

vector<int> busquedaLocalTabu(vector<vector<int>> conexiones, vector<int> &cubierta, vector<vector<ParPerturbado>> &memoria)
{
	int elementoAleatorio1, elementoAleatorio2;
	int quitado, agregar, nroCambios;
	unsigned int tamMemoria = conexiones.size();
	bool encontrado;
	vector<int> ladosDeX;
	vector<vector<int>> listaLados = conexiones;
	vector<ParPerturbado> cambiosMemoria;
	nroCambios = 0;
	vector<int> solucionInicial = cubierta;
	vector<int> posibleSolucion;
	vector<int> solucionFinal = cubierta;

	while (solucionInicial.size() > 0 && nroCambios < 1)
	{
		// cout << "entre" << endl;
		posibleSolucion = solucionFinal;
		// Elegimos el vertice a modificar
		elementoAleatorio1 = rand() % solucionInicial.size();
		// cout << "Aleaorio 1:" << elementoAleatorio1 << endl;
		quitado = solucionInicial[elementoAleatorio1];
		ladosDeX = listaLados[quitado - 1];
		encontrado = false;

		// Busca el vertice a cambiar de la posible solucion y lo quitamos
		for (unsigned int w = 0; w < posibleSolucion.size(); w++)
		{
			if (posibleSolucion[w] == quitado)
			{
				posibleSolucion.erase(posibleSolucion.begin() + w);
				break;
			}
		}

		// Probamos cambiar el vertice por algunos de sus vecinos
		while (ladosDeX.size() > 0)
		{
			elementoAleatorio2 = rand() % ladosDeX.size();
			// cout << "Aleaorio 2:" << elementoAleatorio2 << endl;
			agregar = ladosDeX[elementoAleatorio2];
			ladosDeX.erase(ladosDeX.begin() + elementoAleatorio2);

			// Verificamos si el nuevo nodo a agregar ya esta en la solucion
			for (unsigned int z = 0; z < posibleSolucion.size(); z++)
			{
				if (posibleSolucion[z] == agregar)
				{
					encontrado = true;
					break;
				}
			}

			// Si el nodo a agregar no esta en la solución. Se agrega
			if (!encontrado)
			{
				posibleSolucion.push_back(agregar);
			}

			// Verificamos que el Cambio sea una solucion
			if (!verificarMemoriaTabu(memoria, quitado, agregar) || (verificarSolucion(posibleSolucion, conexiones) && posibleSolucion.size() < solucionFinal.size()))
			{
				solucionFinal = posibleSolucion;
				encontrado = false;
				break;
			}
			else
			{
				posibleSolucion.pop_back();
				encontrado = false;
			}
		}

		// Quitamos el elemento que acabamos de probar
		solucionInicial.erase(solucionInicial.begin() + elementoAleatorio1);
		// Agregamos los cambios a los nodos al vector de pares perturbados
		ParPerturbado nuevoPar;
		nuevoPar.hasta = quitado;
		nuevoPar.desde = agregar;
		cambiosMemoria.push_back(nuevoPar);
		nroCambios++;
	}

	// Si la memoria temporal llego a su limite, la limpiamos
	if (memoria.size() == tamMemoria)
	{
		memoria.erase(memoria.begin());
	}

	memoria.push_back(cambiosMemoria);

	cubierta = solucionFinal;
	return solucionFinal;
}

// Busqueda Tabu
int busquedaTabu(vector<vector<int>> conexiones, vector<int> &cubierta, int iteraciones)
{
	vector<vector<ParPerturbado>> memoria;
	vector<int> posibleSolucion;
	vector<int> solucionConTabu = cubierta;
	unsigned int menorCubierta;

	menorCubierta = cubierta.size();

	for (int i = 0; i < iteraciones; i++)
	{
		busquedaLocalTabu(conexiones, solucionConTabu, memoria);

		if (verificarSolucion(solucionConTabu, conexiones))
		{
			cubierta = solucionConTabu;
			menorCubierta = cubierta.size();
		}

		if (i % 50 == 0 && i != 0)
		{
			solucionConTabu = cubierta;
		}
	}

	return menorCubierta;
}

vector<bool> codificar(vector<int> fenotipo, int size)
{
	vector<bool> genotipo;

	for (int i = 0; i < size; i++)
	{
		genotipo.push_back(false);
	}

	for (unsigned int i = 0; i < fenotipo.size(); i++)
	{
		genotipo[fenotipo[i] - 1] = true;
	}

	return genotipo;
}

vector<int> decodificar(vector<bool> genotipo)
{
	vector<int> fenotipo;

	for (unsigned int i = 0; i < genotipo.size(); i++)
	{
		if (genotipo[i])
		{
			fenotipo.push_back(i + 1);
		}
	}

	return fenotipo;
}

int aptitud(vector<bool> genotipo)
{
	vector<int> fenotipo = decodificar(genotipo);

	return fenotipo.size();
}

void seleccionarPadres(vector<vector<bool>> &padre1, vector<vector<bool>> &padre2, vector<vector<bool>> poblacion)
{

	int aleatorio;
	bool repetido;
	int cantPadres = 5 * ((rand() % 3) + 2);
	vector<int> numPadre1, numPadre2;

	// Escogemos los primeros padres
	while (padre1.size() < cantPadres)
	{
		aleatorio = rand() % poblacion.size();
		repetido = false;
		for (unsigned int i = 0; i < numPadre1.size(); i++)
		{
			if (aleatorio == numPadre1[i])
			{
				repetido = true;
				break;
			}
		}

		if (!repetido)
		{
			padre1.push_back(poblacion[aleatorio]);
			numPadre1.push_back(aleatorio);
		}
	}

	// Escogemos los segundos padres
	while (padre2.size() < cantPadres)
	{
		aleatorio = rand() % poblacion.size();
		repetido = false;
		for (unsigned int i = 0; i < numPadre1.size(); i++)
		{
			if (aleatorio == numPadre1[i])
			{
				repetido = true;
				break;
			}
		}

		for (unsigned int i = 0; i < numPadre2.size() && !repetido; i++)
		{
			if (aleatorio == numPadre2[i])
			{
				repetido = true;
				break;
			}
		}

		if (!repetido)
		{
			padre2.push_back(poblacion[aleatorio]);
			numPadre2.push_back(aleatorio);
		}
	}
}

void generarPoblacion(vector<vector<bool>> &poblacion, vector<vector<int>> conexiones, int tam)
{
	for (int i = 0; i < tam; i++)
	{
		vector<int> nuevaSolucion;
		respAleatoria(conexiones, nuevaSolucion);
		poblacion.push_back(codificar(nuevaSolucion, conexiones.size()));
	}

	return;
}

void generarHijos(vector<vector<bool>> padre1, vector<vector<bool>> padre2, vector<vector<bool>> &hijo1, vector<vector<bool>> &hijo2)
{
	int probabilidad;
	for (unsigned int j = 0; j < padre1.size(); j++)
	{
		vector<bool> nuevohijo1;
		vector<bool> nuevohijo2;
		hijo1.push_back(nuevohijo1);
		hijo2.push_back(nuevohijo2);

		for (unsigned int i = 0; i < padre1[j].size(); i++)
		{
			probabilidad = rand() % 100;

			if (probabilidad > 49)
			{
				hijo1[j].push_back(padre2[j][i]);
				hijo2[j].push_back(padre1[j][i]);
			}
			else
			{
				hijo1[j].push_back(padre1[j][i]);
				hijo2[j].push_back(padre2[j][i]);
			}
		}
	}
}

void mutarHijo(vector<vector<bool>> &hijo)
{
	int probabilidadDeMutacion, bitMutado;
	for (unsigned int j = 0; j < hijo.size(); j++)
	{

		probabilidadDeMutacion = rand() % 100;
		if (probabilidadDeMutacion < 40)
		{

			for (int i = 0; i < 1; i++)
			{
				bitMutado = rand() % hijo.size();

				hijo[j][bitMutado] = !hijo[j][bitMutado];
			}
		}
	}
}

vector<vector<bool>> nuevaPoblacion(vector<vector<bool>> poblacion, vector<vector<bool>> hijo1, vector<vector<bool>> hijo2, vector<vector<int>> conexiones, int tam)
{
	int probabilidad, indicePeor;
	unsigned int valorAptitudMinimo;
	bool agregado;
	for (unsigned int i = 0; i < hijo1.size(); i++)
	{
		poblacion.push_back(hijo1[i]);
		poblacion.push_back(hijo2[i]);
	}

	vector<vector<bool>> mejores;
	vector<vector<bool>> peores;
	vector<vector<bool>> poblacionNueva;
	vector<int> poblaDecodificada;

	for (unsigned int i = 0; i < poblacion.size(); i++)
	{
		agregado = false;
		poblaDecodificada = decodificar(poblacion[i]);
		if (verificarSolucion(poblaDecodificada, conexiones))
		{
			// Si esta vacio lo inicializamos
			if (mejores.size() == 0)
			{

				mejores.push_back(poblacion[i]);
				valorAptitudMinimo = poblaDecodificada.size();
				agregado = true;
			}
			else
			{ // Si la solucion encontrada es menor que la mejor obtenida se agrega de primera
				if (valorAptitudMinimo > poblaDecodificada.size())
				{
					mejores.insert(mejores.begin(), poblacion[i]);
					valorAptitudMinimo = poblaDecodificada.size();
					agregado = true;
				}
				else
				{ // Sino se agrega por orden de aptitud
					for (unsigned int j = 0; j < mejores.size(); j++)
					{
						if (aptitud(mejores[j]) == poblaDecodificada.size())
						{
							mejores.insert(mejores.begin() + j, poblacion[i]);
							agregado = true;
							break;
						}
					}
				}

				if (!agregado)
				{
					mejores.push_back(poblacion[i]);
				}
			}
		}
		else
		{ // Si no es solucion, se agrega en las peores
			peores.push_back(poblacion[i]);
		}
	}

	// Seleccionamos la segunda generación
	for (int i = 0; i < tam; i++)
	{
		probabilidad = rand() % 100;
		if (probabilidad < 70 && mejores.size() > 0)
		{
			poblacionNueva.push_back(mejores[0]);
			mejores.erase(mejores.begin());
		}
		else
		{
			if (peores.size() <= 0 && mejores.size() > 0)
			{
				poblacionNueva.push_back(mejores[0]);
				mejores.erase(mejores.begin());
			}
			else if (peores.size() > 0)
			{
				indicePeor = rand() % peores.size();
				poblacionNueva.push_back(peores[indicePeor]);
				peores.erase(peores.begin() + indicePeor);
			}
		}
	}

	return poblacionNueva;
}

// Algoritmo Genetico
int algoritmoGenetico(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta, int iteraciones)
{
	vector<vector<bool>> poblacion;
	vector<vector<bool>> padre1;
	vector<vector<bool>> padre2;
	vector<vector<bool>> hijo1;
	vector<vector<bool>> hijo2;
	vector<int> posibleSolucion;
	int tamano = 90;

	generarPoblacion(poblacion, conexiones, tamano);

	for (int i = 0; i < iteraciones; i++)
	{

		seleccionarPadres(padre1, padre2, poblacion);
		generarHijos(padre1, padre2, hijo1, hijo2);
		mutarHijo(hijo1);
		mutarHijo(hijo2);
		poblacion = nuevaPoblacion(poblacion, hijo1, hijo2, conexiones, tamano);
		hijo1.clear();
		hijo2.clear();
		padre1.clear();
		padre2.clear();
		// cout << i << endl;
	}

	for (unsigned int i = 0; i < poblacion.size(); i++)
	{
		posibleSolucion = decodificar(poblacion[i]);
		if (verificarSolucion(posibleSolucion, conexiones) && posibleSolucion.size() < cubierta.size())
		{
			cubierta = posibleSolucion;
		}
	}

	return cubierta.size();
}

vector<bool> codificarMeme(vector<int> fenotipo, int size)
{
	vector<bool> genotipo;

	for (int i = 0; i < size + 1; i++)
	{
		genotipo.push_back(false);
	}

	for (unsigned int i = 0; i < fenotipo.size(); i++)
	{
		genotipo[fenotipo[i] - 1] = true;
	}

	return genotipo;
}

vector<int> decodificarMeme(vector<bool> genotipo)
{
	vector<int> fenotipo;

	for (unsigned int i = 0; i < genotipo.size() - 1; i++)
	{
		if (genotipo[i])
		{
			fenotipo.push_back(i + 1);
		}
	}

	return fenotipo;
}

void generarPoblacionMeme(vector<vector<bool>> &poblacion, vector<vector<int>> conexiones, int tam)
{
	vector<bool> pobladorCodificado;
	for (int i = 0; i < tam; i++)
	{
		vector<int> nuevaSolucion;
		respAleatoria(conexiones, nuevaSolucion);
		pobladorCodificado = codificarMeme(nuevaSolucion, conexiones.size());
		pobladorCodificado[conexiones.size()] = true;
		poblacion.push_back(pobladorCodificado);
	}

	return;
}

vector<vector<bool>> nuevaPoblacionMeme(vector<vector<bool>> poblacion, vector<vector<bool>> hijos, vector<vector<int>> conexiones, int tam)
{
	vector<int> hijoDecodificado;
	vector<bool> hijoCodificado;
	vector<vector<bool>> nuevaPoblacion;

	for (unsigned int i = 0; i < hijos.size(); i++)
	{
		hijoDecodificado = decodificar(hijos[i]);
		busquedaLocal(conexiones, hijoDecodificado);
		hijoCodificado = codificarMeme(hijoDecodificado, hijos[0].size());

		if (verificarSolucion(hijoDecodificado, conexiones))
		{
			hijoCodificado[hijos[0].size()] = true;
		}

		nuevaPoblacion.push_back(hijoCodificado);
	}

	return nuevaPoblacion;
}

void seleccionarPadresMeme(vector<vector<int>> &padres, vector<vector<bool>> poblacion, int tam)
{
	int aleatorio;
	int cantPadres;
	int total = 0;
	int indice = 0;

	while (total < tam)
	{
		cantPadres = 4;

		if (total + cantPadres >= tam)
		{
			cantPadres = tam - total;
		}

		vector<int> nuevosPadres;
		padres.push_back(nuevosPadres);

		for (int i = 0; i < cantPadres; i++)
		{
			aleatorio = rand() % poblacion.size();
			padres[indice].push_back(aleatorio);
		}

		total = total + cantPadres;
		indice++;
	}
}

void generarHijosMeme(vector<vector<int>> padres, vector<vector<bool>> &hijos, vector<vector<bool>> poblacion)
{
	// int aleatorio1, aleatorio2;
	int piso, techo;
	// vector<int> soluciones, NoSoluciones;
	piso = 0;
	for (unsigned int i = 0; i < padres.size(); i++)
	{
		for (unsigned int j = 0; j < padres[i].size(); j++)
		{
			vector<bool> hijoNuevo;
			hijos.push_back(hijoNuevo);

			/*if (poblacion[padres[i][j]][poblacion[i].size() - 1])
			{
				soluciones.push_back(padres[i][j]);
			}
			else
			{
				NoSoluciones.push_back(padres[i][j]);
			}*/
		}

		techo = piso + padres[i].size();

		for (unsigned int j = 0; j < poblacion[i].size() - 1; j++)
		{
			bool valor1 = false;
			bool valor2 = true;

			for (unsigned int k = 0; k < padres[i].size(); k++)
			{
				valor1 = valor1 || poblacion[padres[i][k]][j];
				valor2 = valor2 && poblacion[padres[i][k]][j];
			}

			hijos[piso].push_back(valor1);
			hijos[piso + 1].push_back(!valor1);
			hijos[piso + 2].push_back(valor2);
			hijos[piso + 3].push_back(!valor2);

			/*
			int k = piso;

			while (k < techo)
			{
				aleatorio1 = rand() % 100;

				if (aleatorio1 > 70 && NoSoluciones.size() > 0)
				{
					aleatorio2 = rand() % NoSoluciones.size();
					hijos[k].push_back(poblacion[NoSoluciones[aleatorio2]][j]);
					k++;
				}
				else if (aleatorio1 <= 70 && soluciones.size() > 0)
				{
					aleatorio2 = rand() % soluciones.size();
					hijos[k].push_back(poblacion[soluciones[aleatorio2]][j]);
					k++;
				}
			}*/
		}
		piso = techo;
	}
}

// Algoritmo memético
int algoritmoMemetico(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta, int iteraciones)
{
	vector<vector<bool>> poblacion;
	vector<vector<int>> padres;
	vector<vector<bool>> hijos;

	vector<int> posibleSolucion;
	int tamano = 52;

	generarPoblacionMeme(poblacion, conexiones, tamano);

	for (int i = 0; i < iteraciones; i++)
	{
		// cout << "inicio: " << i << endl;
		seleccionarPadresMeme(padres, poblacion, tamano);
		// cout << "Seleccione Padres" << endl;
		generarHijosMeme(padres, hijos, poblacion);
		// cout << "Genere Hijos" << endl;
		poblacion = nuevaPoblacionMeme(poblacion, hijos, conexiones, tamano);
		// cout << "Genere Nueva Poblacion" << endl;
		hijos.clear();
		padres.clear();
		// cout << "fin: " << i << endl;
		//  cout << i << endl;
	}

	for (unsigned int i = 0; i < poblacion.size(); i++)
	{
		posibleSolucion = decodificarMeme(poblacion[i]);
		if (poblacion[i][poblacion[i].size() - 1] && posibleSolucion.size() < cubierta.size())
		{
			cubierta = posibleSolucion;
		}
	}

	return cubierta.size();
}

void reducirGrado(vector<vector<int>> conexiones, vector<Vertice> &todosVert, vector<int> cubierta)
{

	for (unsigned int i = 0; i < cubierta.size(); i++)
	{
		for (unsigned int j = 0; j < conexiones[cubierta[i] - 1].size(); j++)
		{
			todosVert[conexiones[cubierta[i] - 1][j] - 1].grado = todosVert[conexiones[cubierta[i] - 1][j] - 1].grado - 1;
		}
	}
}

void moverHormiga(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta, vector<int> feromonas)
{
	vector<int> RCL;
	vector<int> movimientosPosibles = conexiones[cubierta.back() - 1];
	float alpha = 0.7;
	int CostoMax = 0;
	int CostoMin = conexiones.size();
	int limite, indiceFeromona, aleatorio;
	int mayorFeromona;
	reducirGrado(conexiones, todosVert, cubierta);
	mayorFeromona = 0;
	indiceFeromona = -1;

	for (unsigned int i = 0; i < cubierta.size(); i++)
	{
		for (unsigned int j = 0; j < movimientosPosibles.size(); j++)
		{
			if (cubierta[i] == movimientosPosibles[j])
			{
				movimientosPosibles.erase(movimientosPosibles.begin() + j);
				break;
			}
		}
	}

	for (unsigned int i = 0; i < movimientosPosibles.size(); i++)
	{
		if (feromonas[movimientosPosibles[i] - 1] > mayorFeromona)
		{
			mayorFeromona = feromonas[movimientosPosibles[i] - 1];
			indiceFeromona = i;
		}
	}

	if (movimientosPosibles.size() == 0)
	{
		aleatorio = rand() % conexiones[cubierta.back() - 1].size();
		int movimiento = conexiones[cubierta.back() - 1][aleatorio];

		for (unsigned int j = 0; j < cubierta.size(); j++)
		{
			if (cubierta[j] == movimiento)
			{
				cubierta.erase(cubierta.begin() + j);
				break;
			}
		}

		cubierta.push_back(movimiento);
	}
	else if (indiceFeromona == -1)
	{
		for (unsigned int i = 0; i < movimientosPosibles.size(); i++)
		{
			if (todosVert[movimientosPosibles[i] - 1].grado > CostoMax)
			{
				CostoMax = todosVert[movimientosPosibles[i] - 1].grado;
			}

			if (todosVert[movimientosPosibles[i] - 1].grado < CostoMin)
			{
				CostoMin = todosVert[movimientosPosibles[i] - 1].grado;
			}
		}

		limite = CostoMax - alpha * (CostoMax - CostoMin);

		for (unsigned int i = 0; i < movimientosPosibles.size(); i++)
		{
			if (todosVert[movimientosPosibles[i] - 1].grado >= limite)
			{
				RCL.push_back(movimientosPosibles[i]);
			}
		}

		aleatorio = rand() % RCL.size();
		cubierta.push_back(RCL[aleatorio]);
	}
	else
	{
		cubierta.push_back(movimientosPosibles[indiceFeromona]);
	}
}

void dispersarFeromonas(vector<int> &feromonas)
{

	for (unsigned int i = 0; i < feromonas.size(); i++)
	{
		feromonas[i] = feromonas[i] * 0.6;
	}
}

bool hormigasEstaticas(vector<bool> estadoHormigas)
{

	bool estaticas = true;

	for (unsigned int i = 0; i < estadoHormigas.size(); i++)
	{
		if (!estadoHormigas[i])
		{
			estaticas = false;
			break;
		}
	}

	return estaticas;
}

// Colonia de Hormigas
int coloniaHormigas(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta, int hormigas)
{
	vector<int> feromonas;
	vector<vector<int>> soluciones;
	vector<bool> solucionCompletada;
	int aleatorio;

	for (unsigned int i = 0; i < conexiones.size(); i++)
	{
		feromonas.push_back(0);
	}

	for (int i = 0; i < hormigas; i++)
	{
		aleatorio = rand() % conexiones.size();
		vector<int> nuevaSolucion;
		nuevaSolucion.push_back(aleatorio + 1);
		soluciones.push_back(nuevaSolucion);
		solucionCompletada.push_back(false);
	}

	while (!hormigasEstaticas(solucionCompletada))
	{

		for (int i = 0; i < hormigas; i++)
		{
			if (!solucionCompletada[i])
			{

				moverHormiga(conexiones, todosVert, soluciones[i], feromonas);

				if (verificarSolucion(soluciones[i], conexiones))
				{
					solucionCompletada[i] = true;
				}
			}
		}

		for (unsigned int i = 0; i < soluciones.size(); i++)
		{
			feromonas[soluciones[i].back() - 1] = feromonas[soluciones[i].back() - 1] + 2;
		}
	}

	for (unsigned int i = 0; i < soluciones.size(); i++)
	{
		if (verificarSolucion(soluciones[i], conexiones) && soluciones[i].size() < cubierta.size())
		{
			cubierta = soluciones[i];
		}
	}

	return cubierta.size();
}

bool picoAnzuelo(vector<Vertice> todosVert, int indiceVertice, int tipo, int minimo, int seccion)
{

	int peso = todosVert[indiceVertice].grado;
	int aleatorio = rand() % 100;
	bool pico = false;

	if (peso >= minimo && peso < minimo + seccion)
	{
		if (tipo == 0 && aleatorio < 50)
		{
			pico = true;
		}
		else if (tipo == 1 && aleatorio < 20)
		{
			pico = true;
		}
		else if (tipo == 2 && aleatorio < 10)
		{
			pico = true;
		}
	}
	else if (peso >= minimo + seccion && peso < minimo + 2 * seccion)
	{
		if (tipo == 0 && aleatorio < 70 && 50 <= aleatorio)
		{
			pico = true;
		}
		else if (tipo == 1 && aleatorio < 70 && 20 <= aleatorio)
		{
			pico = true;
		}
		else if (tipo == 2 && aleatorio < 30 && 10 <= aleatorio)
		{
			pico = true;
		}
	}
	else if (peso >= minimo + 2 * seccion)
	{
		if (tipo == 0 && aleatorio < 80 && 70 <= aleatorio)
		{
			pico = true;
		}
		else if (tipo == 1 && aleatorio < 80 && 70 <= aleatorio)
		{
			pico = true;
		}
		else if (tipo == 2 && aleatorio < 80 && 30 <= aleatorio)
		{
			pico = true;
		}
	}

	return pico;
}

// La Pesca
void laPesca(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta, int peces, int iteraciones)
{

	struct Pez
	{
		int tipo;
		bool pescado;
		int posX;
		int posY;
	};

	int tamEstanque = todosVert.size();
	int estanque[tamEstanque][tamEstanque];
	int z = 0;
	int maximo = -1;
	int seccion = 0;
	int minimo = todosVert.size();
	bool todosLosPecesPescados = false;
	Pez pecesitos[peces];

	// Inicializamos el Estanque
	for (int i = 0; i < tamEstanque; i++)
	{
		for (int j = 0; j < tamEstanque; j++)
		{
			estanque[i][j] = -1;
		}
	}

	// Inicializamos los Peces
	for (int i = 0; i < peces; i++)
	{
		pecesitos[i].pescado = false;
		pecesitos[i].tipo = rand() % 3;
		pecesitos[i].posX = rand() % tamEstanque;
		pecesitos[i].posY = rand() % tamEstanque;
	}

	// Repartimos los nodos alrededor del estanque
	while (z < todosVert.size())
	{

		int x = rand() % tamEstanque;
		int y = rand() % tamEstanque;

		if (maximo < todosVert[z].grado)
		{
			maximo = todosVert[z].grado;
		}

		if (minimo > todosVert[z].grado)
		{
			minimo = todosVert[z].grado;
		}

		if (estanque[x][y] == -1)
		{
			estanque[x][y] = todosVert[z].nro - 1;
			z++;
		}
	}

	seccion = maximo - minimo / 3;
	z = 0;

	while (z < iteraciones && !todosLosPecesPescados)
	{
		todosLosPecesPescados = true;
		// Hacemos Acciones con cada pez
		for (int i = 0; i < peces; i++)
		{
			if (!pecesitos[i].pescado)
			{
				todosLosPecesPescados = false;
				// Verificamos si pica algun anzuelo
				for (int k = -1; k < 2; k++)
				{
					for (int j = -1; j < 2; j++)
					{
						if (k == j)
						{
							continue;
						}

						int x = pecesitos[i].posX + k;
						int y = pecesitos[i].posY + j;

						if (x < 0)
						{
							x = tamEstanque - 1;
						}
						else if (x >= tamEstanque)
						{
							x = 0;
						}

						if (y < 0)
						{
							y = tamEstanque - 1;
						}
						else if (y >= tamEstanque)
						{
							y = 0;
						}

						// Verificamos el estanque

						if (estanque[x][y] != -1)
						{

							if (picoAnzuelo(todosVert, estanque[x][y], pecesitos[i].tipo, minimo, seccion))
							{
								cubierta.push_back(estanque[x][y] + 1);
								estanque[x][y] = -1;
								pecesitos[i].pescado = true;
								break;
							}
						}
					}

					if (pecesitos[i].pescado)
					{
						break;
					}
				}

				// Movemos al pez
				if (!pecesitos[i].pescado)
				{
					int x = pecesitos[i].posX + (rand() % 3) - 1;
					int y = pecesitos[i].posY + (rand() % 3) - 1;

					if (x < 0)
					{
						x = tamEstanque - 1;
					}
					else if (x >= tamEstanque)
					{
						x = 0;
					}

					if (y < 0)
					{
						y = tamEstanque - 1;
					}
					else if (y >= tamEstanque)
					{
						y = 0;
					}

					pecesitos[i].posX = x;
					pecesitos[i].posY = y;
				}
			}
		}

		z++;
	}
}

int pescaIterativa(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta, int iteraciones)
{
	vector<int> nuevaCubierta;
	int peces = 8;

	if (peces < 0.008 * todosVert.size())
	{
		peces = 0.008 * todosVert.size();
	}

	if (peces % 2 != 0)
	{
		peces = peces + 1;
	}

	for (int i = 0; i < iteraciones; i++)
	{
		laPesca(conexiones, todosVert, nuevaCubierta, peces * todosVert.size(), 900);
		busquedaLocal(conexiones, nuevaCubierta);
		if (nuevaCubierta.size() < cubierta.size() && verificarSolucion(nuevaCubierta, conexiones))
		{
			cubierta = nuevaCubierta;
		}

		nuevaCubierta.clear();
	}

	return cubierta.size();
}

// Menu de Seleccion
// 1.Metodo Exacto (Backtracking)
// 2.Busqueda Local
// 3.Busqueda Local Iterada
// 4.Busqueda Tabú
// 5.Algoritmo Genetico
// 6.Algoritmo Memético
// 7.Colonia Hormigas
// 8.La Pesca
int SeleccionMetodo(int mode, string dir, int numberTest)
{

	int nroVer, nroAris, resultado;
	// Valores Cambiantes
	vector<vector<int>> conexiones;
	vector<Vertice> todosVert;
	vector<int> cubierta;
	unsigned t0, t1;
	double time;
	// Valores De Salva Guarda
	vector<vector<int>> TodasLasConexiones;
	vector<vector<int>> OriginalConexiones;
	vector<Vertice> OriginalTodosVert;
	vector<int> OriginalCubierta;

	int maximo = lecturaGrafo(dir, todosVert, conexiones, nroVer, nroAris);
	TodasLasConexiones = conexiones;
	OriginalTodosVert = todosVert;

	switch (mode)
	{
	case 1:

		OriginalCubierta = cubierta;

		cout << "Comenzando Busqueda Exacta " << endl;
		for (unsigned int i = 0; i < todosVert.size(); i++)
		{
			cubierta.push_back(i + 1);
		}

		t0 = clock();
		resultado = respExacta(cubierta, TodasLasConexiones, todosVert.size());
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Exacta: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		/*
		for (int i = 0; i < numberTest; i++)
		{
			cubierta = OriginalCubierta;
			t0 = clock();
			resultado = respAleatoria(TodasLasConexiones, cubierta);
			t1 = clock();
			time = (double(t1 - t0) / CLOCKS_PER_SEC);
			cout << "Busqueda Aleatoria: " << time << endl;
			cout << "Resultado: " << resultado << endl;
			cubierta.clear();
		}
		*/
		break;

	case 2:

		t0 = clock();
		resultado = respAproximada(todosVert, conexiones, nroVer, nroAris, cubierta, maximo);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Aproximada: " << time << endl;
		/*
		t0 = clock();
		resultado = respAleatoria(conexiones, cubierta);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Aleatoria: " << time << endl;
		*/
		/*
		for (unsigned int j = 0; j < TodasLasConexiones.size(); j++)
		{
			cout << "Vertice: " << j + 1 << "-> ";

			for (unsigned int i = 0; i < TodasLasConexiones[j].size(); i++)
			{
				cout << TodasLasConexiones[j][i] << " ";
			}
			cout << "\n";
		}
		cout << "\n";
		*/
		OriginalConexiones = TodasLasConexiones;
		OriginalTodosVert = todosVert;
		OriginalCubierta = cubierta;

		for (int i = 0; i < numberTest; i++)
		{
			todosVert = OriginalTodosVert;
			conexiones = OriginalConexiones;
			cubierta = OriginalCubierta;

			t0 = clock();
			resultado = busquedaLocal(conexiones, cubierta);
			t1 = clock();
			time = (double(t1 - t0) / CLOCKS_PER_SEC);
			cout << "Busqueda Local: " << time << endl;
			cout << "Es Solucion?" << verificarSolucion(cubierta, TodasLasConexiones) << endl;
			cout << "Resultado: " << resultado << endl;
			/*
			cout << "Posible Solucion-> ";
			for (unsigned int w = 0; w < cubierta.size(); w++)
			{
				cout << cubierta[w] << " ";
			}
			*/
			cout << endl;
		}

		break;

	case 3:

		t0 = clock();
		resultado = respAproximada(todosVert, conexiones, nroVer, nroAris, cubierta, maximo);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Aproximada: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		t0 = clock();
		resultado = busquedaLocalIterada(OriginalTodosVert, TodasLasConexiones, cubierta, numberTest);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Local Iterada: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 4:

		t0 = clock();
		resultado = respAleatoria(conexiones, cubierta);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Aleatoria: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		t0 = clock();
		resultado = busquedaTabu(TodasLasConexiones, cubierta, numberTest);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Tabu: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 5:

		for (unsigned int i = 0; i < todosVert.size(); i++)
		{
			cubierta.push_back(i + 1);
		}

		t0 = clock();
		resultado = algoritmoGenetico(TodasLasConexiones, todosVert, cubierta, numberTest);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Algoritmo Genetico: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 6:

		for (unsigned int i = 0; i < todosVert.size(); i++)
		{
			cubierta.push_back(i + 1);
		}

		t0 = clock();
		resultado = algoritmoMemetico(TodasLasConexiones, todosVert, cubierta, numberTest);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Algoritmo Memetico: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 7:

		for (unsigned int i = 0; i < todosVert.size(); i++)
		{
			cubierta.push_back(i + 1);
		}

		t0 = clock();
		resultado = coloniaHormigas(TodasLasConexiones, todosVert, cubierta, numberTest);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Colonia Hormigas: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 8:

		for (unsigned int i = 0; i < todosVert.size(); i++)
		{
			cubierta.push_back(i + 1);
		}

		t0 = clock();
		resultado = pescaIterativa(TodasLasConexiones, todosVert, cubierta, numberTest);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "La Pesca: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	default:
		break;
	}

	return 0;
}