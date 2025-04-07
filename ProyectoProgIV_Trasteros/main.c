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
    int result,numTrastero,dniUser,trasteroAEliminar;
    char opcionPrincipal,opcionAdmin,opcionAdmin2,
	opcionUsuarioInicio,opcionCliente,opcionCatalogo,opcionVolverAtrasCat,opcionVolverAtrasPerfil;
    Trastero t;
    Usuario u;

    printf("\033[1;32mIniciando el programa...\033[0m\n");fflush(stdout);
    sleep(1);
    result = inicializarBBDD(&db);
    	if(result == SQLITE_OK){
    		printf("\033[1;32mEstableciendo la conexion con base de datos...\033[0m\n");
    		fflush(stdout);
    		crearTablas(db);
    	}else{
    		printf("\033[1;31mNo se ha establecido la conexión con la BBDD\033[0m\n");
    		fflush(stdout);
    		return 0;
    	}



    sleep(2);
    limpiarConsola();
    //INICIALIZAMOS LAS LISTAS
    inicializarListaUsuarios(&lu);
    inicializarListaTrasteros(&lt);
    //CARGAMOS LAS LISTAS CON LOS VALORES DEL ARCHIVO
    //TODO: CARGAR LAS LISTAS CON LOS VALORES DE LA BD (FALTAN METER PRIMERO LOS VALORES)

    cargarTrasterosDesdeCSV(&lt, NOM_ARCHIVO_TRASTEROS);
    cargarUsuariosDesdeCSV(&lu,NOM_ARCHIVO_USUARIOS);

    do {
        opcionPrincipal = mostrarMenuPrincipal();
        switch (opcionPrincipal) {
        //PERFIL ADMIN
            case '1':
            	if (autenticarAdministrador()) {
            		do {
            			opcionAdmin = menuAdministrador();
            			switch (opcionAdmin) {
                        	case '1':
                        		//AÑADIR TRASTERO
                        		t= menuAniadirTrastero();
                        		if(t.numeroTrastero<=0){ //Verificar que el numero sea mayor a 0 para evitar problemas con los flags creados
                        			printf("\033[1;33mEl numero del trastero debe ser mayor a 0\033[0m");
                        		}else if(buscarTrastero(lt,t.numeroTrastero)==-1){ //Verificar si el trastero ya existe
                        			aniadirTrastero(&lt,t);
                        			aniadirTrasteroABBDD(db,t);
                        		}else{
                        			printf("\033[38;5;214mEl trastero con numero %d ya existe.\033[0m\n", t.numeroTrastero);
                        		}
                        		break;
                        	case '2':
                        		//ELIMINAR TRASTERO
                        		trasteroAEliminar = menuEliminarTrastero();
                        		t=obtenerTrastero(lt,trasteroAEliminar);
                        		if(t.numeroTrastero!=-1){
                        			eliminarTrastero(&lt,t);
                        			eliminarTrasteroDDBB(db,trasteroAEliminar);
                        		}else{
                        			printf("\033[31mEl trastero con numero %d no existe.\033[0m\n", trasteroAEliminar);
                        		}

                        		break;
                        	case '3':
                        		//VER CLIENTES
                        		sleep(1);
                        		limpiarConsola();
                        		visualizarListaUsuarios(lu);

                        		break;
                        	case '4':
                        		//VER TRASTEROS
                        			do{
                        				opcionAdmin2 = menuTrasterosAdmin();
                        				switch (opcionAdmin2) {
											case '1':
												//VER TODOS LOS TRASTEROS
												sleep(1);
												limpiarConsola();
												visualizarTrasteros(lt);
												break;
											case '2':
												//VER LOS ALQUILADOS
												sleep(1);
												limpiarConsola();
												visualizarTrasterosAlquilados(lt);
												break;
											case '3':
												//VER LOS DISPONIBLES
												sleep(1);
												limpiarConsola();
												visualizarTrasterosDisponibles(lt);
												break;
											default:
												break;
										}
                        			}while(opcionAdmin2!='0');
                            break;
                        	case '5':
                        		//OBTENER FICHEROS
                        		opcionAdmin2=menuFicherosAdmin();
                        		do{
                        			switch (opcionAdmin2) {
										case '1':
											//OBTENER FICHERO DE LOS TRASTEROS ACTUALMENTE ALQUILADOS
											crearFicheroAlquilados(db);
											break;
										case '2':
											//OBTENER FICHERO DE TODOS LOS TRASTEROS QUE HAN SIDO ALQUILADOS
											crearFicheroHistorial(db);
											break;
			                        	case '0':
			                        		printf("Saliendo de la opcion ficheros...\n");
			                        		break;
										default:
											printf("Opción invalida. Por favor, ingrese una opción válida.\n");
											fflush(stdout);
											break;
									}
                        		}while(opcionAdmin2!='0');
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
            	//PERFIL USUARIO
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

								do{
									opcionCliente = menuCliente();
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
													case'0':
														printf("\033[0;33mSaliendo del menú catálogo...\n\033[0m");
														break;
													default:
														printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");
														fflush(stdout);
														break;
												}
											}while(opcionCatalogo!='0');
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
													devolverTrastero(&t);
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
											actualizarValoracion(&t);
											break;
										case '0':
											//CERRAR SESION
											printf("Cerrando sesion...");
											sleep(1);
											break;
										default:
											printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");
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
							printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");
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
