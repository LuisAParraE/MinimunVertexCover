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

	cout << "Sali\n";
	cout << "Aristas: " << nroAris << "\n";
	cout << cubierta.size() << "\n";
	/*
	for (unsigned int j = 0; j < cubierta.size(); j++)
	{
		cout << cubierta[j] << "\n";
	}
	*/
	return cubierta.size();
}

// FUNCION DE SOLUCION ALEATORIA
int respAleatoria(vector<Vertice> todosVert, vector<vector<int>> conexiones, vector<int> &cubierta)
{
	vector<vector<int>> todasConexiones = conexiones;
	int nroAleatorio1, nroAleatorio2, verticeIndex, vertice2;
	Vertice vertice1;
	vector<int>::iterator inicio;

	while (!verificarSolucion(cubierta, todasConexiones))
	{
		nroAleatorio1 = rand() % todosVert.size();
		vertice1 = todosVert[nroAleatorio1];
		// QUITAMOS EL PRIMER VERTICE DE TODOS LOS VERTICES
		todosVert.erase(todosVert.begin() + nroAleatorio1);
		cubierta.push_back(vertice1.nro);

		if (conexiones[vertice1.nro - 1].size() > 0)
		{
			nroAleatorio2 = rand() % conexiones[vertice1.nro - 1].size();
			vertice2 = conexiones[vertice1.nro - 1][nroAleatorio2];

			cubierta.push_back(vertice2);

			// QUITAMOS EL SEGUNDO VERTICE DE TODOS LOS VERTICES
			for (unsigned int i = 0; i < todosVert.size(); i++)
			{
				if (vertice2 == todosVert[i].nro)
				{
					todosVert.erase(todosVert.begin() + i);
					break;
				}
			}

			// ELIMINAMOS CONEXIONES DEL SEGUNDO VERTICE
			verticeIndex = vertice2 - 1;
			for (unsigned int j = 0; j < conexiones[verticeIndex].size(); j++)
			{

				unsigned int aux = conexiones[verticeIndex][j] - 1;
				inicio = conexiones[aux].begin();

				// Eliminamos todas las Conexiones HACIA el vertice solucion
				for (unsigned int k = 0; k < conexiones[aux].size(); k++)
				{
					if ((int)(verticeIndex + 1) == conexiones[aux][k])
					{

						conexiones[aux].erase(inicio + k);

						break;
					}
				}
			}
			// Eliminamos Todas Las Conexiones Desde El vertice solucion
			conexiones[verticeIndex].clear();
		}

		// ELIMINAMOS CONEXIONES DEL PRIMER VERTICE
		verticeIndex = vertice1.grado - 1;
		for (unsigned int j = 0; j < conexiones[verticeIndex].size(); j++)
		{

			unsigned int aux = conexiones[verticeIndex][j] - 1;
			inicio = conexiones[aux].begin();

			// Eliminamos todas las Conexiones HACIA el vertice solucion
			for (unsigned int k = 0; k < conexiones[aux].size(); k++)
			{
				if ((int)(verticeIndex + 1) == conexiones[aux][k])
				{

					conexiones[aux].erase(inicio + k);

					break;
				}
			}
		}
		// Eliminamos Todas Las Conexiones Desde El vertice solucion
		conexiones[verticeIndex].clear();
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
	cout << "Aqui" << endl;
	sort(verticesOrdenados.begin(), verticesOrdenados.end(), greater<Vertice>());

	for (unsigned int i = 0; i < verticesOrdenados.size(); i++)
	{
		cubiertaOrdenada.push_back(verticesOrdenados[i].nro);
	}
	cout << "Aqui2" << endl;
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

	cout << "retorne" << endl;
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
int busquedaLocalIterada(vector<Vertice> todosVert, vector<vector<int>> conexiones, vector<int> &cubierta)
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

	for (int i = 0; i < 10; i++)
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
	int quitado, agregar;
	unsigned int tamMemoria = 5;
	bool encontrado;
	vector<int> ladosDeX;
	vector<vector<int>> listaLados = conexiones;
	vector<ParPerturbado> cambiosMemoria;

	vector<int> solucionInicial = cubierta;
	vector<int> posibleSolucion;
	vector<int> solucionFinal = cubierta;

	while (solucionInicial.size() > 0)
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
int busquedaTabu(vector<vector<int>> conexiones, vector<int> &cubierta)
{
	vector<vector<ParPerturbado>> memoria;
	vector<int> posibleSolucion;
	vector<int> solucionConTabu = cubierta;
	unsigned int menorCubierta;

	menorCubierta = cubierta.size();

	for (int i = 0; i < 100; i++)
	{
		cout << i << endl;
		busquedaLocalTabu(conexiones, solucionConTabu, memoria);

		if (verificarSolucion(solucionConTabu, conexiones))
		{
			cubierta = solucionConTabu;
			menorCubierta = cubierta.size();
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

	int aleatorio1, aleatorio2;
	aleatorio2 = rand() % poblacion.size();
	aleatorio1 = rand() % poblacion.size();
	while (padre1.size() < 20)
	{
		while (aleatorio2 == aleatorio1)
		{
			aleatorio2 = rand() % poblacion.size();
		}

		padre1.push_back(poblacion[aleatorio1]);
		padre2.push_back(poblacion[aleatorio2]);
	}
}

void generarPoblacion(vector<vector<bool>> &poblacion, vector<vector<int>> conexiones, vector<Vertice> todosVert, int tam)
{
	for (int i = 0; i < tam; i++)
	{
		vector<int> nuevaSolucion;
		respAleatoria(todosVert, conexiones, nuevaSolucion);
		poblacion.push_back(codificar(nuevaSolucion, conexiones.size()));
	}

	return;
}

void generarHijos(vector<vector<bool>> padre1, vector<vector<bool>> padre2, vector<vector<bool>> &hijo1, vector<vector<bool>> &hijo2)
{
	int probabilidad;
	for (unsigned int j = 0; j < padre1.size(); j++)
	{

		for (unsigned int i = 0; i < padre1.size(); i++)
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
	int probabilidad;
	for (unsigned int j = 0; j < hijo.size(); j++)
	{
		for (unsigned int i = 0; i < hijo.size(); i++)
		{
			probabilidad = rand() % 100;

			if (probabilidad > 94)
			{
				hijo[j][i] = !hijo[j][i];
			}
		}
	}
}

vector<vector<bool>> nuevaPoblacion(vector<vector<bool>> poblacion, vector<vector<bool>> hijo1, vector<vector<bool>> hijo2, vector<vector<int>> conexiones, int tam)
{
	int probabilidad, valorAp;
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
			valorAp = poblaDecodificada.size();

			if (mejores.size() == 0)
			{

				mejores.push_back(poblacion[i]);
				agregado = true;
			}
			else
			{

				for (unsigned int j = 0; j < mejores.size(); j++)
				{
					if (valorAp <= aptitud(mejores[j]))
					{

						mejores.insert(mejores.begin() + j, poblacion[i]);
						agregado = true;
						break;
					}
				}

				if (!agregado)
				{

					mejores.push_back(poblacion[i]);
				}
			}
		}
		else
		{

			peores.push_back(poblacion[i]);
		}
	}

	for (int i = 0; i < tam; i++)
	{
		probabilidad = rand() % 100;
		if (probabilidad < 95 && mejores.size() > 0)
		{
			poblacionNueva.push_back(mejores[0]);
			mejores.erase(mejores.begin());
		}
		else
		{
			if (peores.size() <= 0)
			{
				poblacionNueva.push_back(mejores[0]);
				mejores.erase(mejores.begin());
			}
			else
			{
				poblacionNueva.push_back(peores[0]);
				peores.erase(peores.begin());
			}
		}
	}

	return poblacionNueva;
}

// Algoritmo Genetico
int algoritmoGenetico(vector<vector<int>> conexiones, vector<Vertice> todosVert, vector<int> &cubierta)
{
	vector<vector<bool>> poblacion;
	vector<vector<bool>> padre1;
	vector<vector<bool>> padre2;
	vector<vector<bool>> hijo1;
	vector<vector<bool>> hijo2;
	vector<int> posibleSolucion;
	int tamano = 50;

	generarPoblacion(poblacion, conexiones, todosVert, tamano);

	for (int i = 0; i < 500; i++)
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
	}

	cubierta = decodificar(poblacion[0]);

	for (unsigned int i = 1; i < poblacion.size(); i++)
	{
		posibleSolucion = decodificar(poblacion[i]);
		if (verificarSolucion(posibleSolucion, conexiones) && posibleSolucion.size() < cubierta.size())
		{
			cubierta = posibleSolucion;
		}
	}

	return cubierta.size();
}

// Menu de Seleccion
// 1.Metodo Exacto (Backtracking)
// 2.Busqueda Local
// 3.Busqueda Local Iterada
// 4.Busqueda Tabú
// 5.Algoritmo Genetico
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

		break;

	case 2:

		t0 = clock();
		resultado = respAproximada(todosVert, conexiones, nroVer, nroAris, cubierta, maximo);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Aproximada: " << time << endl;
		/*
		t0 = clock();
		resultado = respAleatoria(todosVert, conexiones, cubierta);
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
		cout << OriginalTodosVert.size() << endl;
		t0 = clock();
		resultado = busquedaLocalIterada(OriginalTodosVert, TodasLasConexiones, cubierta);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Local Iterada: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 4:

		t0 = clock();
		resultado = respAproximada(todosVert, conexiones, nroVer, nroAris, cubierta, maximo);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Aproximada: " << time << endl;
		cout << OriginalTodosVert.size() << endl;
		t0 = clock();
		resultado = busquedaTabu(TodasLasConexiones, cubierta);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Busqueda Tabu: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	case 5:

		t0 = clock();
		resultado = algoritmoGenetico(TodasLasConexiones, todosVert, cubierta);
		t1 = clock();
		time = (double(t1 - t0) / CLOCKS_PER_SEC);
		cout << "Algoritmo Genetico: " << time << endl;
		cout << "Resultado: " << resultado << endl;
		break;

	default:
		break;
	}

	return 0;
}