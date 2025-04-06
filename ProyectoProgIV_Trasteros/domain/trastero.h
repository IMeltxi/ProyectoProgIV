
#ifndef TRASTERO_H_
#define TRASTERO_H_
#define MAX_TRASTEROS 100
#define NOM_ARCHIVO_TRASTEROS "trasteros.csv"

typedef struct{
	int numeroTrastero;
	int metrosCuadrados;
	float valoracion;
	float precio;
	int disponible;
	int numeroDeValoraciones;
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
Trastero obtenerTrastero(ListaTrasteros lt, int numeroTrastero);
void actualizarValoracion(Trastero t);
void visualizarTrasteros(ListaTrasteros lt);
void visualizarTrastero(Trastero t);

void visualizarTrasterosDisponibles(ListaTrasteros lt);
void visualizarTrasterosAlquilados(ListaTrasteros lt);

void ordenarPorPrecio(ListaTrasteros *lt);
void ordenarPorMetrosCuadrados(ListaTrasteros *lt);
void ordenarPorValoracion(ListaTrasteros *lt);
void ordenarPorNumeroTrastero(ListaTrasteros *lt);

void cargarTrasterosDesdeCSV(ListaTrasteros *lt,char *nombreArchivo);

#endif /* TRASTERO_H_ */
