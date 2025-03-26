#include "trastero.h"
#include <stdio.h>
void inicializarListaTrasteros(ListaTrasteros* lt){
	lt->numeroTrasteros=0;
}
void alquilarTrastero(Trastero *t) {
    t->disponible = 0;
}

void devolverTrastero(Trastero *t) {
    t->disponible = 1;
}

void aniadirTrastero(ListaTrasteros *lt, Trastero t) {
    if (lt->numeroTrasteros < 100) {
        lt->aTrasteros[lt->numeroTrasteros] = t;
        lt->numeroTrasteros++;
    } else {
        printf("No se pueden añadir más trasteros\n");
        fflush(stdout);
    }
}

int buscarTrastero(ListaTrasteros lt, int numeroTrastero) {
    int pos = 0, enc = 0;
    while (!enc && pos < lt.numeroTrasteros) {
        if (lt.aTrasteros[pos].numeroTrastero == numeroTrastero) {
            enc = 1;
        } else {
            pos++;
        }
    }
    return enc ? pos : -1;
}

void eliminarTrastero(ListaTrasteros *lt, Trastero t) {
    int i;
    int pos = buscarTrastero(*lt, t.numeroTrastero);
    if (pos == -1) {
        printf("ERROR: Ese trastero no existe\n");
    } else {
        for (i = pos; i < lt->numeroTrasteros - 1; i++) {
            lt->aTrasteros[i] = lt->aTrasteros[i + 1];
        }
        lt->numeroTrasteros--;
    }
}

void visualizarTrastero(Trastero t) {
	char* disponible;
	if(t.disponible==0){
		printf("%20d%20d%20d%20d%20d\n", t.numeroTrastero, t.metrosCuadrados, t.valoracion, t.precio, "NO");
	}else{
		printf("%20d%20d%20d%20d%20d\n", t.numeroTrastero, t.metrosCuadrados, t.valoracion, t.precio, "SI");
	}

}
void visualizarTrasteroDisponible(Trastero t) {
	if (t.disponible==0){
	    printf("%20d%20d%20d%20d%20d\n", t.numeroTrastero, t.metrosCuadrados, t.valoracion, t.precio, "SI");
	}
}

void visualizarTrasteroAlquilado(Trastero t) {
	if (t.disponible==1){
	    printf("%20d%20d%20d%20d%20d\n", t.numeroTrastero, t.metrosCuadrados, t.valoracion, t.precio, t.disponible);
	}
}

void visualizarTrasteros(ListaTrasteros lt) {
    int i;
    printf("%20s%20s%20s%20s%20s\n", "NºTRASTERO", "m²", "VALORACION", "PRECIO", "DISPONIBILIDAD");
    fflush(stdout);
    for (i = 0; i < lt.numeroTrasteros; i++) {
        visualizarTrastero(lt.aTrasteros[i]);
    }
}

void visualizarTrasterosDisponibles(ListaTrasteros lt) {
     int i;
     printf("%20s%20s%20s%20s%20s\n", "NºTRASTERO", "m²", "VALORACION", "PRECIO", "DISPONIBILIDAD");
     fflush(stdout);
     for (i = 0; i < lt.numeroTrasteros; i++) {
    	 visualizarTrasteroDisponible(lt.aTrasteros[i]);
     }
}

void visualizarTrasterosAlquilados(ListaTrasteros lt) {
     int i;
     printf("%20s%20s%20s%20s%20s\n", "NºTRASTERO", "m²", "VALORACION", "PRECIO", "DISPONIBILIDAD");
     fflush(stdout);
     for (i = 0; i < lt.numeroTrasteros; i++) {
    	 visualizarTrasteroAlquilado(lt.aTrasteros[i]);
     }
}
