// main.c
#include "menu.h"
#include "usuario.h"
#include "trastero.h"
#include "bbdd.h"
#include <stdio.h>
#include <stdlib.h>



int main() {
    ListaUsuarios lu;
    ListaTrasteros lt;
    Usuario u={0};
    sqlite3 *db; //Acceso a la bbdd
    int result;
    printf("Iniciando el programa...\n");

    result = inicializarBBDD(&db);
    	if(result == SQLITE_OK){
    		crearTablas(db);
    	}else{
    		printf("No se ha establecido la conexi�n con la BBDD\n");
    		fflush(stdout);
    	}
    inicializarListaUsuarios(&lu);
    inicializarListaTrasteros(&lt);


    char opcion;
        do {
            opcion = mostrarMenuPrincipal();
            switch (opcion) {
                case '1':
                    manejarAdministrador();
                    break;
                case '2':
                    manejarCliente();
                    break;
                case '0':
                    printf("Cerrando programa...\n");
                    break;
                default:
                    printf("Opción inválida.\n");
            }
        } while (opcion != '0');
        return 0;
}
