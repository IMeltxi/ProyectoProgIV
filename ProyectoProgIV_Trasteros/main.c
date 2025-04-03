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
    char opcionPrincipal,opcionAdmin,opcionMenuAdmin;
    Trastero t;
    Usuario u;
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
    cargarTrasterosDesdeCSV(&lt, NOM_ARCHIVO);


    do {
        opcionPrincipal = mostrarMenuPrincipal();
        switch (opcionPrincipal) {
            case '1':
            	if (autenticarAdministrador()) {
            		do {
            			opcionAdmin = menuAdministrador();
            			switch (opcionAdmin) {
                        	case '1':
                        		do{
                        			opcionMenuAdmin = menuAdministrador();
                        			switch (opcionMenuAdmin) {
										case '1':
											t= menuAniadirTrastero();
											aniadirTrastero(&lt,t);
											break;
										default:
											break;
									}
                        		}while(opcionAdmin!=0);

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
