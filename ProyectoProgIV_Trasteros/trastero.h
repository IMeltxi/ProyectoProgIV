
#ifndef TRASTERO_H_
#define TRASTERO_H_
#define MAX_TRASTEROS 100


typedef struct{
	int numeroTrastero;
	int metrosCuadrados;
	int valoracion;
	int precio;
	int disponible;
}Trastero;

typedef struct{
	Trastero aTrasteros[MAX_TRASTEROS];
	int numeroTrasteros;
}ListaTrasteros;

void inicializarListaTrasteros(ListaTrasteros *lt);
void alquilarTrastero(Trastero *t);
void devolverTrastero(Trastero *t);
int buscarTrastero(ListaTrasteros lt, int numeroTrastero);
void aniadirTrastero(ListaTrasteros *lt, Trastero t);
void eliminarTrastero(ListaTrasteros *lt, Trastero t);
void imprimirTrasterosDisponibles(ListaTrasteros lt);

void visualizarTrasteros(ListaTrasteros lt);
void visualizarTrastero(Trastero t);

void visualizarTrasterosDisponibles(ListaTrasteros lt);
void visualizarTrasteroDisponible(Trastero t);

void visualizarTrasterosAlquilados(ListaTrasteros lt);
void visualizarTrasteroAlquilado(Trastero t);




#endif /* TRASTERO_H_ */
