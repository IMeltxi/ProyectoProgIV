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
    int result,numTrastero,dniUser;
    char opcionPrincipal,opcionAdmin,opcionVisualizarTrasterosAdmin,
	opcionUsuarioInicio,opcionCliente,opcionVolverAtrasPerfil,opcionCatalogo,opcionVolverAtrasCat;
    Trastero t;
    Usuario u;



    int borrarTablaTrasteros(sqlite3 *db) {
        sqlite3_stmt *stmt;
        char sql[100];

        // Sentencia SQL para borrar la tabla Trastero
        sprintf(sql, "DROP TABLE IF EXISTS Trastero");

        // Preparar la sentencia SQL
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            fprintf(stderr, "Error al preparar la sentencia SQL para borrar la tabla: %s\n", sqlite3_errmsg(db));
            return -1; // Retornar error
        }

        // Ejecutar la sentencia SQL
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error al ejecutar la sentencia SQL para borrar la tabla: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return -1; // Retornar error
        }

        // Finalizar la sentencia
        sqlite3_finalize(stmt);
        return 0; // Todo ha salido bien
    }


    printf("Iniciando el programa...\n");fflush(stdout);
    sleep(1);
    result = inicializarBBDD(&db);
    	if(result == SQLITE_OK){
    		printf("Estableciendo la conexion con base de datos...");fflush(stdout);
    		borrarTablaTrasteros(db);
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
    							printf("Opción invalida. Por favor, ingrese una opción válida.\n");
    							fflush(stdout);
    							break;
            			}
            	} while (opcionAdmin != '0');
            	break;
            case '2': {
            	do{
            		 opcionUsuarioInicio = menuIniReg();
            		 switch (opcionUsuarioInicio) {
						case '1':
							dniUser = autenticarUsuario(db);

							//-1 = Login fallido
							if(dniUser!=-1){
								//Obtenemos el Usuario logeado
								u = obtenerUsuario(db,dniUser);
								opcionCliente = menuCliente();
								do{
									switch (opcionCliente) {
										case '1':
											//PERFIL
											do{
												menuPerfil(u);
												opcionVolverAtrasPerfil= volverAtras();
												if(opcionVolverAtrasPerfil=='0'){
												printf("Caracter invalido\n: ");
												}
											}while(opcionVolverAtrasPerfil!='0');

											break;
										case '2':
											//CATALOGO
											do{
												opcionCatalogo = menuCatalogo();
												switch (opcionCatalogo) {
													case '1':
														do{
														//Ordenamos la lista por numero de trastero
															sleep(1);
															limpiarConsola();
															ordenarPorNumeroTrastero(&lt);
															visualizarTrasteros(lt);
															opcionVolverAtrasCat= volverAtras();
															if(opcionVolverAtrasCat=='0'){
																printf("Caracter invalido\n: ");
															}
														}while(opcionVolverAtrasCat!='0');
														break;
													case '2':
														do{
														//Ordenamos la lista por precio
															sleep(1);
															limpiarConsola();
															ordenarPorPrecio(&lt);
															visualizarTrasteros(lt);
															opcionVolverAtrasCat= volverAtras();
															if(opcionVolverAtrasCat=='0'){
																printf("Caracter invalido\n: ");
															}
														}while(opcionVolverAtrasCat!='0');
														break;
													case '3':
														//VER METROS
														do{
														//Ordenamos la lista por Metros Cuadrados
															sleep(1);
															limpiarConsola();
															ordenarPorMetrosCuadrados(&lt);
															visualizarTrasteros(lt);
															opcionVolverAtrasCat= volverAtras();
															if(opcionVolverAtrasCat=='0'){
																printf("Caracter invalido\n: ");
															}
														}while(opcionVolverAtrasCat!='0');
														break;
													case '4':
														//VER VALORACION
														do{
														//Ordenamos la lista por Valoracion
															sleep(1);
															limpiarConsola();
															ordenarPorValoracion(&lt);
															visualizarTrasteros(lt);
															opcionVolverAtrasCat= volverAtras();
															if(opcionVolverAtrasCat=='0'){
																printf("Caracter invalido\n: ");
															}
														}while(opcionVolverAtrasCat!='0');
														break;
													default:
														printf("Opción invalida. Por favor, ingrese una opción válida.\n");
														fflush(stdout);
														break;
												}
											}while(opcionCatalogo!=0);
											break;
										case '3':
											//ALQUILAR TRASTERO
											printf("ALQUILAR TRASTERO");
											printf("\nIndique el numero de trastero que desea alquilar \n: ");
											fflush(stdout);
											fflush(stdin);
											scanf("%d", &numTrastero);
											//Obetenemos el trastero que quiere alquilar
											t= buscarTrasteroDDBB(db,numTrastero);
											if(t.numeroTrastero!=-1){
												if(t.disponible==0){
													printf("Lo sentimos, este trastero esta alquilado por otra persona.\n");
												}else{
													//Marcamos como no disponible en local
													alquilarTrastero(&t);
													//Marcamos como no disponible en la BD
													aniadirTrasteroAlquilado(db,t,u);
													printf("El trastero con numero %d ha sido alquilado por %s.",t.numeroTrastero,u.nombre);
												};

											}else{
												printf("No existe este trastero en nuestro catalogo.\n");
											}
											break;
										case '4':
											//DEVOLVER TRASTERO
											printf("DEVOLVER TRASTERO");
											printf("\nIndique el numero de trastero que desea devolver \n: ");
											fflush(stdout);
											fflush(stdin);
											scanf("%d", &numTrastero);
											//Obetenemos el trastero que quiere alquilar
											t= buscarTrasteroDDBB(db,numTrastero);
											if(t.numeroTrastero!=-1){
												if(t.disponible==1){
													printf("Lo sentimos, este trastero ya esta disponible en nuestro catalogo.\n");
												}else if(verificarAlquiler(db,numTrastero,u.dni)==1){ //Verificamos que este usuario es el que ha alquilado el trastero
													//Marcamos como disponible en local
													devolverTrastero(&lt);
													//Marcamos como no disponible en la BD
													devolverTrasteroBBDD(db,t);
													printf("El trastero con numero %d ha sido correctamente devuelto por %s.\n",t.numeroTrastero,u.nombre);
													sleep(1);
												}else{
													printf("Este trastero no ha sido alquilado por usted\n");
												}

											}else{
												printf("No existe este trastero en nuestro catalogo.\n");
											}
											break;
										case '0':
											//CERRAR SESION
											printf("Cerrando sesion...");
											sleep(1);
											break;
										default:
											printf("Opción invalida. Por favor, ingrese una opción válida.\n");
											fflush(stdout);
											break;
									}
								}while(opcionCliente!=0);
							}
							break;
						case '2':
							registrarUsuario(db);
							break;
						case '0':
							printf("Volviendo al menú principal...\n\n");
							fflush(stdout);
							break;
						default:
							printf("Opción invalida. Por favor, ingrese una opción válida.\n");
							fflush(stdout);
							break;
					}

            	}while(opcionUsuarioInicio!='0');


                break;
            }
            case '0':
                cerrarPrograma(db,&lu,&lt);
                break;
            default:
            	printf("Opción invalida. Por favor, ingrese una opción válida.\n");
            	fflush(stdout);
        }
         }
    } while (opcionPrincipal != '0');

    return 0;
}
