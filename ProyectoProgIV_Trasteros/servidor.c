// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "domain/usuario.h"
#include "domain/trastero.h"
#include "db/bbdd.h"

#include <stdio.h>
#include <winsock2.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET conn_socket; //el que lleva la conexion
	SOCKET comm_socket; //el que lo comunica
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512]; // lo que yo envio, lo que yo recibo

	printf("\nInitialising Winsock...\n"); // inicializa la libreria
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation creacion del socket( la primera estructura
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");
	// cual es la ip y cual es el puerto
	server.sin_addr.s_addr = inet_addr(SERVER_IP); //INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n"); //DEJAR EL SOCKET EN ESPERA

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);


	int fin = 0;
	do {
		//EMPIEZA EL PROGRAMA DEL SERVIDOR/
		ListaUsuarios lu;
		ListaTrasteros lt;
		sqlite3 *db; //Acceso a la bbdd
		int numTrastero,dniUser,trasteroAEliminar,result,i,dni,valoracion;
		char opcionPrincipal,opcionAdmin,opcionAdminTrastero,
		opcionUsuarioInicio,opcionCliente,opcionCatalogo,opcionVolverAtrasCat,opcionVolverAtrasPerfil;
		char usuarioAdmin[50],contrasena[50],contrasenaIS[50];
		//Variables para registrar usuario
		char nombre[50], apellido[50], email[100],direccion[100],contrasenia[50],confirmarContrasena[50],telefono[20],dniReg[20];
		int resultadoRegistro;
		Trastero t;
		Usuario u;

		result = inicializarBBDD(&db);
			if(result == SQLITE_OK){
				//printf("\033[1;32mEstableciendo la conexion con base de datos...\033[0m\n");
				crearTablas(db);
			}else{
				//printf("\033[1;31mNo se ha establecido la conexión con la BBDD\033[0m\n");
				return 0;
			}
		//limpiarConsola();
		//INICIALIZAMOS LAS LISTAS
		inicializarListaUsuarios(&lu);
		inicializarListaTrasteros(&lt);
		//CARGAMOS LAS LISTAS CON LOS VALORES DE LA BD
		cargarTrasterosDesdeDB(&lt, db);
		cargarUsuariosDesdeDB(&lu, db);
		//    cargarTrasterosDesdeCSV(&lt,NOM_ARCHIVO_TRASTEROS);
		//    cargarUsuariosDesdeCSV(&lu,NOM_ARCHIVO_USUARIOS);
		do {
		        //opcionPrincipal = mostrarMenuPrincipal();
			recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos opcion
			sscanf(recvBuff,"%c",&opcionPrincipal); //obtener datos

			switch (opcionPrincipal) { //La variable verdadera es opcionPrincipal
		        //PERFIL ADMIN
		            case '1':
		            	result =0;
		            	do{//Inicio de sesion
		            		memset(recvBuff, 0, sizeof(recvBuff));
		            		recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos usuario
							strcpy(usuarioAdmin,recvBuff);

							memset(recvBuff, 0, sizeof(recvBuff));
							recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos contraseña
							strcpy(contrasena,recvBuff);

							result = autenticarAdministrador(usuarioAdmin,contrasena);
							memset(sendBuff, 0, sizeof(sendBuff));
							if(result==1){
								strcpy(sendBuff,"1");
							}else{
								strcpy(sendBuff,"0");
							}
							send(comm_socket,sendBuff,strlen(sendBuff)+1,0);//enviamos resultado
		            	}while(result!=1);
		            		do {
		            			//MENU ADMIN
		            			//opcionAdmin = menuAdministrador();
		            			memset(recvBuff, 0, sizeof(recvBuff));
								recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos opcion
								sscanf(recvBuff,"%c",&opcionAdmin); //obtener datos

		            			switch (opcionAdmin) {
		                        	case '1':
		                        		//AÑADIR TRASTERO
		                        		//t= menuAniadirTrastero();
		                        		memset(recvBuff, 0, sizeof(recvBuff));
		                        		recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos numero de trastero del CLIENTE
				            			t.numeroTrastero = atoi(recvBuff); // convierte a int
										//Mandamos al cliente lo que hemos recibido
				            												sprintf(sendBuff, "El servidor recibio trastero: %s", recvBuff);
				            												send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
				            			memset(recvBuff, 0, sizeof(recvBuff));
				            			recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos metros cuadrados del CLIENTE
				            			t.metrosCuadrados = atoi(recvBuff); // convierte a int
				            			//Mandamos al cliente lo que hemos recibido
																			sprintf(sendBuff, "El servidor recibio estos metros cuadrados: %s", recvBuff);
																			send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
				            			memset(recvBuff, 0, sizeof(recvBuff));
				            			recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos precio del CLIENTE
				            			t.precio = atof(recvBuff); // convierte a int
																			//Mandamos al cliente lo que hemos recibido
																			sprintf(sendBuff, "El servidor recibio precio: %s", recvBuff);
																			send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
										t.valoracion = 0.0;
										t.numeroDeValoraciones = 0;
										t.disponible = 1;

										if(t.numeroTrastero <= 0){
											memset(sendBuff, 0, sizeof(sendBuff));
											sprintf(sendBuff, "%d", 3); // Flag para número inválido
											send(comm_socket, sendBuff, sizeof(sendBuff), 0);
										} else if(buscarTrastero(lt, t.numeroTrastero) == -1){
											aniadirTrastero(&lt, t);
											result = aniadirTrasteroABBDD(db, t);
											memset(sendBuff, 0, sizeof(sendBuff));
											sprintf(sendBuff, "%d", result);
											send(comm_socket, sendBuff, sizeof(sendBuff), 0);
										} else {
											memset(sendBuff, 0, sizeof(sendBuff));
											sprintf(sendBuff, "%d", 4); // Trastero ya existe
											send(comm_socket, sendBuff, sizeof(sendBuff), 0);
										}
		                        		break;
		                        	case '2':
		                        		//ELIMINAR TRASTERO
		                        		//trasteroAEliminar = menuEliminarTrastero();
		                        		memset(recvBuff, 0, sizeof(recvBuff));
		                        		recv(comm_socket, recvBuff, sizeof(recvBuff), 0); // Recibimos número de trastero del CLIENTE

		                        		// Confirmar al cliente qué número de trastero recibimos
		                        		memset(sendBuff, 0, sizeof(sendBuff));
		                        		strcpy(sendBuff, recvBuff);
		                        		send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0); // Enviamos confirmación

		                        		trasteroAEliminar = atoi(recvBuff); // Convierte a int
		                        		t = obtenerTrastero(lt, trasteroAEliminar);

		                        		if (t.numeroTrastero != -1) {
		                        		    eliminarTrastero(&lt, t);
		                        		    eliminarTrasteroDDBB(db, trasteroAEliminar);

		                        		    // Enviar flag de éxito
		                        		    memset(sendBuff, 0, sizeof(sendBuff));
		                        		    sprintf(sendBuff, "%d", 1); // Flag: trastero eliminado correctamente
		                        		    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
		                        		} else {
		                        		    // Enviar flag de error
		                        		    memset(sendBuff, 0, sizeof(sendBuff));
		                        		    sprintf(sendBuff, "%d", 0); // Flag: trastero no existe
		                        		    send(comm_socket, sendBuff, sizeof(sendBuff), 0);
		                        		}

		                        		break;
		                        	case '3':
		                        		 // VER TODOS LOS USUARIOS
		                        		    memset(sendBuff, 0, sizeof(sendBuff));
		                        		    sprintf(sendBuff, "%d", lu.numUsuarios);
		                        		    send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		    for(i = 0; i < lu.numUsuarios; i++){
		                        		        // Enviar nombre
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%s", lu.aUsuarios[i].nombre);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		        // Enviar apellidos
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%s", lu.aUsuarios[i].apellido);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		        // Enviar DNI
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%d", lu.aUsuarios[i].dni);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		        // Enviar teléfono
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%d", lu.aUsuarios[i].telefono);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		        // Enviar email
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%s", lu.aUsuarios[i].email);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		        // Enviar dirección
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%s", lu.aUsuarios[i].direccion);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

		                        		        // Enviar contraseña
		                        		        memset(sendBuff, 0, sizeof(sendBuff));
		                        		        sprintf(sendBuff, "%s", lu.aUsuarios[i].contrasenia);
		                        		        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
		                        		    }
		                        	    break;
		                        	case '4':
		                        		//VER TRASTEROS
		                        			do{
		                        				//opcionAdmin2 = menuTrasterosAdmin();
		                        				memset(recvBuff, 0, sizeof(recvBuff));
		                        				recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos opcion
		                        				sscanf(recvBuff,"%c",&opcionAdminTrastero); //obtener datos
		                        				memset(sendBuff, 0, sizeof(sendBuff));
		                        				sprintf(sendBuff,"El servidor recibio: %c",opcionAdminTrastero);
		                        				send(comm_socket,sendBuff,strlen(sendBuff)+1,0); //enviar
		                        				switch (opcionAdminTrastero) {
													case '1':
														//VER TODOS LOS TRASTEROS

														memset(sendBuff, 0, sizeof(sendBuff));
														sprintf(sendBuff, "%d", lt.numeroTrasteros);
														send(comm_socket, sendBuff, sizeof(sendBuff), 0);
														for(i = 0; i < lt.numeroTrasteros; i++){
														        // Enviar número del trastero
														        memset(sendBuff, 0, sizeof(sendBuff));
														        sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
														        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														        // Enviar metros cuadrados
														        memset(sendBuff, 0, sizeof(sendBuff));
														        sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
														        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														        // Enviar precio
														        memset(sendBuff, 0, sizeof(sendBuff));
														        sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
														        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														        // Enviar valoración
														        memset(sendBuff, 0, sizeof(sendBuff));
														        sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
														        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														        // Enviar número de valoraciones
														        memset(sendBuff, 0, sizeof(sendBuff));
														        sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
														        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														        // Enviar disponibilidad
														        memset(sendBuff, 0, sizeof(sendBuff));
														        sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
														        send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														}
														break;
													case '2':
														//VER LOS ALQUILADOS
														memset(sendBuff, 0, sizeof(sendBuff));
														sprintf(sendBuff, "%d", lt.numeroTrasteros);
														send(comm_socket, sendBuff, sizeof(sendBuff), 0);
														for(i = 0; i < lt.numeroTrasteros; i++){
															// Enviar número del trastero
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar metros cuadrados
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar precio
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar valoración
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar número de valoraciones
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar disponibilidad
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														}
														break;
													case '3':
														//VER LOS DISPONIBLES
														memset(sendBuff, 0, sizeof(sendBuff));
														sprintf(sendBuff, "%d", lt.numeroTrasteros);
														send(comm_socket, sendBuff, sizeof(sendBuff), 0);
														for(i = 0; i < lt.numeroTrasteros; i++){
															// Enviar número del trastero
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar metros cuadrados
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar precio
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar valoración
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar número de valoraciones
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

															// Enviar disponibilidad
															memset(sendBuff, 0, sizeof(sendBuff));
															sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
															send(comm_socket, sendBuff, sizeof(sendBuff), 0);

														}
														break;
													default:
														break;
												}
		                        			}while(opcionAdminTrastero!='0');
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
		            		// opcionUsuarioInicio = menuIniReg();
            				memset(recvBuff, 0, sizeof(recvBuff));
            				recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos opcion
            				sscanf(recvBuff,"%c",&opcionUsuarioInicio); //obtener datos
            				memset(sendBuff, 0, sizeof(sendBuff));
            				sprintf(sendBuff,"El servidor recibio: %c",opcionUsuarioInicio);
            				send(comm_socket,sendBuff,strlen(sendBuff)+1,0); //enviar

		            		switch (opcionUsuarioInicio) {
								case '1':
									// Recibir DNI
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									dni = atoi(recvBuff); // Convertir string recibido a int

									// Confirmar recepción del DNI
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "DNI recibido: %d", dni);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir contraseña
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(contrasenaIS, recvBuff);

									// Confirmar recepción de la contraseña
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Contraseña recibida: %s", contrasenaIS);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									//Devuelve el dniUser si ha iniciado sesion, si es incorrecto dniUser=-1
									dniUser = autenticarUsuario(db,dni,contrasenaIS);
									// ENVIAR RESULTADO DE AUTENTICACIÓN AL CLIENTE
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "%d", dniUser);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);
									//-1 = Login fallido
									if(dniUser!=-1){
										//Obtenemos el Usuario logeado
										u = obtenerUsuario(db,dniUser);

										do{
											//opcionCliente = menuCliente();
											memset(recvBuff, 0, sizeof(recvBuff));
											recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos opcion
											sscanf(recvBuff,"%c",&opcionCliente); //obtener datos
											memset(sendBuff, 0, sizeof(sendBuff));
											sprintf(sendBuff,"El servidor recibio: %c",opcionCliente);
											send(comm_socket,sendBuff,strlen(sendBuff)+1,0); //enviar

											switch (opcionCliente) {
												case '1':
													//PERFIL
													// Después de obtener el usuario
													u = obtenerUsuario(db, dniUser);
													//MANDAR NOMBRE
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%s", u.nombre);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													// Enviar apellidos
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%s", u.apellido);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													// Enviar DNI
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%d", u.dni);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													// Enviar teléfono
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%d", u.telefono);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													// Enviar email
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%s", u.email);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													// Enviar dirección
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%s", u.direccion);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													break;
												case '2':
													//CATALOGO
													do{
														//opcionCatalogo = menuCatalogo();
														memset(recvBuff, 0, sizeof(recvBuff));
														recv(comm_socket,recvBuff,sizeof(recvBuff),0);//recibimos opcion
														sscanf(recvBuff,"%c",&opcionCatalogo); //obtener datos
														memset(sendBuff, 0, sizeof(sendBuff));
														sprintf(sendBuff,"El servidor recibio: %c",opcionCatalogo);
														send(comm_socket,sendBuff,strlen(sendBuff)+1,0); //enviar

														switch (opcionCatalogo) {
															case '1':
																//Ordenamos la lista por numero de trastero
																	//(1);
																	ordenarPorNumeroTrastero(&lt);
																	memset(sendBuff, 0, sizeof(sendBuff));
																	sprintf(sendBuff, "%d", lt.numeroTrasteros);
																	send(comm_socket, sendBuff, sizeof(sendBuff), 0);
																	for(i = 0; i < lt.numeroTrasteros; i++){
																			// Enviar número del trastero
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar metros cuadrados
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar precio
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar valoración
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar número de valoraciones
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar disponibilidad
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																	}
																	break;
															case '2':
																//Ordenamos la lista por precio
																	ordenarPorPrecio(&lt);
																	memset(sendBuff, 0, sizeof(sendBuff));
																	sprintf(sendBuff, "%d", lt.numeroTrasteros);
																	send(comm_socket, sendBuff, sizeof(sendBuff), 0);
																	for(i = 0; i < lt.numeroTrasteros; i++){
																			// Enviar número del trastero
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar metros cuadrados
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar precio
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar valoración
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar número de valoraciones
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar disponibilidad
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																	}
																	break;

															case '3':
																//VER METROS
																//Ordenamos la lista por Metros Cuadrados
																	ordenarPorMetrosCuadrados(&lt);
																	memset(sendBuff, 0, sizeof(sendBuff));
																	sprintf(sendBuff, "%d", lt.numeroTrasteros);
																	send(comm_socket, sendBuff, sizeof(sendBuff), 0);
																	for(i = 0; i < lt.numeroTrasteros; i++){
																			// Enviar número del trastero
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar metros cuadrados
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar precio
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar valoración
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar número de valoraciones
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar disponibilidad
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																	}
																	break;

															case '4':
																//VER VALORACION
																//Ordenamos la lista por Valoracion
																	ordenarPorValoracion(&lt);
																	memset(sendBuff, 0, sizeof(sendBuff));
																	sprintf(sendBuff, "%d", lt.numeroTrasteros);
																	send(comm_socket, sendBuff, sizeof(sendBuff), 0);
																	for(i = 0; i < lt.numeroTrasteros; i++){
																			// Enviar número del trastero
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroTrastero);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar metros cuadrados
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].metrosCuadrados);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar precio
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].precio);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar valoración
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%.2f", lt.aTrasteros[i].valoracion);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar número de valoraciones
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%d", lt.aTrasteros[i].numeroDeValoraciones);
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																			// Enviar disponibilidad
																			memset(sendBuff, 0, sizeof(sendBuff));
																			sprintf(sendBuff, "%s", lt.aTrasteros[i].disponible ? "DISPONIBLE" : "OCUPADO");
																			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

																	}
																	break;

															case'0':
																//printf("\033[0;33mSaliendo del menú catálogo...\n\033[0m");
																break;
															default:
//																printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");
//																fflush(stdout);
																break;
														}
													}while(opcionCatalogo!='0');
													break;
												case '3':
													//ALQUILAR TRASTERO
													// Recibir numTrastero
													memset(recvBuff, 0, sizeof(recvBuff));
													recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
													numTrastero = atoi(recvBuff); // Convertir string recibido a int

													// Confirmar recepción del numTrastero
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "Trastero recibido: %d", numTrastero);
													send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);


													t = buscarTrasteroDDBB(db, numTrastero);
													if(t.numeroTrastero!=-1){

														if(t.disponible==0){
															//printf("\033[0;33mLo sentimos, este trastero no está disponible en nuestro catálogo.\n\033[0m");
															result=0;
														}else{
															alquilarTrastero(&t);

															//Marcamos como no disponible en la BD
															aniadirTrasteroAlquilado(db,t,u);
															result = 1;
														//	printf("\033[0;32mEl trastero con numero %d ha sido correctamente alquilado por %s.\033[0m\n", t.numeroTrastero, u.nombre);
														}

													}else{
													//	printf("\033[0;31mNo existe este trastero en nuestro catálogo.\n\033[0m");
														result = 2;
													}
													// Enviar nombre del usuario
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%s",u.nombre);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													// Enviar flag del resultado
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%d",result);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);
													break;

												case '4':
													//DEVOLVER TRASTERO
													//recibimos numero de trastero
													memset(recvBuff, 0, sizeof(recvBuff));
													recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
													numTrastero = atoi(recvBuff); // Convertir string recibido a int

													// Confirmar recepción del numTrastero
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "Trastero recibido: %d", numTrastero);
													send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

													// Enviar nombre del usuario
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%s",u.nombre);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													//Obetenemos el trastero que quiere alquilar
													t= buscarTrasteroDDBB(db,numTrastero);
													result = 3;
													if(t.numeroTrastero!=-1){
														if(t.disponible==1){
//															printf("\033[0;33mLo sentimos, este trastero ya está disponible en nuestro catálogo.\n\033[0m");
															result = 0;
														}else if(verificarAlquiler(db,numTrastero,u.dni)==1){ //Verificamos que este usuario es el que ha alquilado el trastero
															//Marcamos como disponible en local
															devolverTrastero(&t);
															//Marcamos como no disponible en la BD
															devolverTrasteroBBDD(db,t);
															//printf("\033[0;32mEl trastero con numero %d ha sido correctamente devuelto por %s.\033[0m\n", t.numeroTrastero, u.nombre);
															result = 1;

														}else{
															//printf("\033[0;31mEste trastero no ha sido alquilado por usted\n\033[0m");
															result = 2;
														}

													}else{
														//printf("\033[0;31mNo existe este trastero en nuestro catálogo.\n\033[0m");
														result = 3;
													}

													// Enviar flag del resultado
													memset(sendBuff, 0, sizeof(sendBuff));
													sprintf(sendBuff, "%d",result);
													send(comm_socket, sendBuff, sizeof(sendBuff), 0);

													break;
												case '0':
													//CERRAR SESION
												//	printf("\033[0;33mCerrando sesión...\033[0m");

													break;
												default:
													//printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");

													break;
											}
										}while(opcionCliente!='0');
									}
									break;
								case '2':
									// Recibir nombre
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(nombre, recvBuff);

									// Confirmar recepción del nombre
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Nombre recibido: %s", nombre);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir apellido
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(apellido, recvBuff);

									// Confirmar recepción del apellido
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Apellido recibido: %s", apellido);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir email
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(email, recvBuff);

									// Confirmar recepción del email
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Email recibido: %s", email);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir direccion
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(direccion, recvBuff);

									// Confirmar recepción de la direccion
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Direccion recibida: %s", direccion);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir contraseña
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(contrasenia, recvBuff);

									// Confirmar recepción de la contraseña
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Contraseña recibida: %s", contrasenia);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir confirmar contraseña
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(confirmarContrasena, recvBuff);

									// Confirmar recepción de confirmar contraseña
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Confirmacion contraseña recibida: %s", confirmarContrasena);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir telefono
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(telefono, recvBuff);

									// Confirmar recepción del telefono
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "Telefono recibido: %s", telefono);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// Recibir DNI
									memset(recvBuff, 0, sizeof(recvBuff));
									recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
									strcpy(dniReg, recvBuff);

									// Confirmar recepción del DNI
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "DNI recibido: %s", dniReg);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									// EJECUTAR REGISTRO
									resultadoRegistro = registrarUsuario(db, nombre, apellido, email, direccion, contrasenia, confirmarContrasena, telefono, dniReg);

									// ENVIAR RESULTADO AL CLIENTE
									memset(sendBuff, 0, sizeof(sendBuff));
									sprintf(sendBuff, "%d", resultadoRegistro);
									send(comm_socket, sendBuff, strlen(sendBuff) + 1, 0);

									break;
								case '0':
									//printf("Volviendo al menú principal...\n\n");
									break;
								default:
									//printf("\033[1;31mOpción inválida. Por favor, ingrese una opción válida.\n\033[0m");
									break;
							}

		            	}while(opcionUsuarioInicio!='0');


		                break;
		            }
		            case '0':
		                cerrarPrograma(db,&lu,&lt);
		                break;
		            default:
		            	//printf("Opción invalida. Por favor, ingrese una opción válida.\n");
		            	//fflush(stdout);
		            	break;
		        }
		    } while (opcionPrincipal != '0');





		//ACABA EL PROGRAMA DEL SERVIDOR/

	} while (fin == 0);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}
