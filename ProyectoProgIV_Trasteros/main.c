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


    char opcionPrincipal;
    do {
        opcionPrincipal = mostrarMenuPrincipal();
        switch (opcionPrincipal) {
            case '1':
            	if (autenticarAdministrador()) {
            		char opcionAdmin;
            		do {
            			opcionAdmin = menuAdministrador();
            			switch (opcionAdmin) {
                        	case '1':
                        		printf("Aniadiendo trastero...\n");
                        		break;
                        	case '2':
                        		printf("Eliminando trastero...\n");
                        		break;
                        	case '3':
                        		printf("Mostrando clientes...\n");
                        		break;
                        	case '4':
                            printf("Mostrando trasteros disponibles...\n");
                            break;
                        	case '0':
                        		printf("Saliendo del menú administrador...\n");
                        		break;
                        	default:
                        		printf("ERROR\n");
            			}
            	} while (opcionAdmin != '0');
            	break;
            case '2': {
            	manejarCliente();
            	break;
            }
            case '0':
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("ERROR.\n");
        }
         }
    } while (opcionPrincipal != '0');

    printf("Presiona Enter para salir...\n");
    getchar();
    printf("Fin del programa\n");
    return 0;
}
