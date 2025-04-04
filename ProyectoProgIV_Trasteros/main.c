// main.c
#include "domain/menu.h"
#include "domain/usuario.h"
#include "domain/trastero.h"
#include "db/bbdd.h"
#include <stdio.h>
#include <stdlib.h>



int main() {
    ListaUsuarios lu;
    ListaTrasteros lt;
    sqlite3 *db; //Acceso a la bbdd
    int result,trasteroAEliminar;
    char opcionPrincipal,opcionAdmin,opcionVisualizarTrasterosAdmin;
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
    cargarTrasterosDesdeCSV(&lt, NOM_ARCHIVO_TRASTEROS);
    cargarUsuariosDesdeCSV(&lu,NOM_ARCHIVO_USUARIOS);
    visualizarTrasteros(lt);
    visualizarListaUsuarios(lu);


    do {
        opcionPrincipal = mostrarMenuPrincipal();
        switch (opcionPrincipal) {
            case '1':
            	if (autenticarAdministrador()) {
            		do {
            			opcionAdmin = menuAdministrador();
            			switch (opcionAdmin) {
                        	case '1':
                        		t= menuAniadirTrastero();
                        		aniadirTrastero(&lt,t);
                        		aniadirTrasteroABBDD(NOMBRE_BBDD,t);
                        		break;
                        	case '2':
//                        		trasteroAEliminar = menuEliminarTrastero();
//                        		t = buscarTrastero(lt,trasteroAEliminar);
//                        		eliminarTrastero(&lt,t);
//                        		eliminarTrasteroDDBB(NOMBRE_BBDD,trasteroAEliminar);
                        		break;
                        	case '3':
                        		sleep(1);
                        		limpiarConsola();
                        		visualizarListaUsuarios(lu);
                        		break;
                        	case '4':
                        			do{
                        				opcionVisualizarTrasterosAdmin = menuTrasterosAdmin();
                        				switch (opcionVisualizarTrasterosAdmin) {
											case '1':
												sleep(1);
												limpiarConsola();
												visualizarTrasteros(lt);
												break;
											case '2':
												sleep(1);
												limpiarConsola();
												visualizarTrasterosAlquilados(lt);
												break;
											case '3':
												sleep(1);
												limpiarConsola();
												visualizarTrasterosDisponibles(lt);
												break;
											default:
												break;
										}
                        			}while(opcionVisualizarTrasterosAdmin!='0');
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
                manejarCliente(&lu, &lt);
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
