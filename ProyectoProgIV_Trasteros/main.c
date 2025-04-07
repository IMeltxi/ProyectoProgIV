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

												limpiarConsola();
												visualizarTrasteros(lt);
												break;
											case '2':
												//VER LOS ALQUILADOS

												limpiarConsola();
												visualizarTrasterosAlquilados(lt);
												break;
											case '3':
												//VER LOS DISPONIBLES

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
															//(1);
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

											    // Esta línea es crucial
											    int temp_num;
											    if (scanf("%d", &temp_num) != 1) {
											        printf("Error al leer el número.\n");
											        // Limpia el buffer de entrada
											        while (getchar() != '\n');
											        break;
											    }
											    numTrastero = temp_num;

											    printf("Número leído: %d\n", numTrastero);
											t = buscarTrasteroDDBB(db, numTrastero);
											printf("DEBUG: Después de buscarTrasteroDDBB\n");
											if(t.numeroTrastero!=-1){
												printf("DEBUG: Después de if\n");
												if(t.disponible==0){
													printf("\033[0;33mLo sentimos, este trastero no está disponible en nuestro catálogo.\n\033[0m");
													printf("DEBUG: Después de if2\n");
												}else{
													alquilarTrastero(&t);
													printf("DEBUG: Después de alquilar\n");
													//Marcamos como no disponible en la BD
													aniadirTrasteroAlquilado(db,t,u);
													printf("\033[0;32mEl trastero con numero %d ha sido correctamente alquilado por %s.\033[0m\n", t.numeroTrastero, u.nombre);
												}

											}else{
												printf("\033[0;31mNo existe este trastero en nuestro catálogo.\n\033[0m");

											}

											break;
										case '4':
											//DEVOLVER TRASTERO
											printf("DEVOLVER TRASTERO");
											printf("\nIndique el numero de trastero que desea devolver \n: ");
											fflush(stdout);
											fflush(stdin);
											scanf("%i", &numTrastero);
											//Obetenemos el trastero que quiere alquilar
											t= buscarTrasteroDDBB(db,numTrastero);
											if(t.numeroTrastero!=-1){
												if(t.disponible==1){
													printf("\033[0;33mLo sentimos, este trastero ya está disponible en nuestro catálogo.\n\033[0m");
												}else if(verificarAlquiler(db,numTrastero,u.dni)==1){ //Verificamos que este usuario es el que ha alquilado el trastero
													//Marcamos como disponible en local
													devolverTrastero(&t);
													//Marcamos como no disponible en la BD
													devolverTrasteroBBDD(db,t);
													printf("\033[0;32mEl trastero con numero %d ha sido correctamente devuelto por %s.\033[0m\n", t.numeroTrastero, u.nombre);
													actualizarValoracion(&t);
												}else{
													printf("\033[0;31mEste trastero no ha sido alquilado por usted\n\033[0m");
												}

											}else{
												printf("\033[0;31mNo existe este trastero en nuestro catálogo.\n\033[0m");

											}

											break;
										case '0':
											//CERRAR SESION
											printf("\033[0;33mCerrando sesión...\033[0m");

											break;
										default:
											printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");
											fflush(stdout);
											break;
									}
								}while(opcionCliente!='0');
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
