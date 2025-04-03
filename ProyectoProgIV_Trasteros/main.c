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
    sqlite3 *db; //Acceso a la bbdd
    int result;


    lt.numeroTrasteros = 5;  // Número de trasteros en la lista

       lt.aTrasteros[0] = (Trastero){1, 10, 4, 200, 1};
       lt.aTrasteros[1] = (Trastero){2, 20, 5, 350, 1};
       lt.aTrasteros[2] = (Trastero){3, 15, 3, 250, 0};
       lt.aTrasteros[3] = (Trastero){4, 30, 2, 500, 1};
       lt.aTrasteros[4] = (Trastero){5, 25, 5, 400, 0};

       printf("Lista de trasteros inicial:\n");
       visualizarTrasteros(lt);
       ordenarPorMetrosCuadrados(&lt);
       visualizarTrasteros(lt);
       ordenarPorPrecio(&lt);
       visualizarTrasteros(lt);
       ordenarPorValoracion(&lt);
       visualizarTrasteros(lt);







    printf("Iniciando el programa...\n");fflush(stdout);
    sleep(1);
    result = inicializarBBDD(&db);
    	if(result == SQLITE_OK){
    		printf("Estableciendo la conexion con base de datos...");fflush(stdout);
    		crearTablas(db);
    	}else{
    		printf("No se ha establecido la conexi�n con la BBDD\n");
    		fflush(stdout);
    		return 0;
    	}
    sleep(2);
    limpiarConsola();
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
    sqlite3_close(db);
    return 0;
}
