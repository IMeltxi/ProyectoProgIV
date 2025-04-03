#include "menu.h"
#include <stdio.h>
#include <string.h>
#include "usuario.h"
#include "trastero.h"
#include "bbdd.h"


#define ADMIN_USER "admin"
#define ADMIN_PASS "1234"
#define USER1 "user"
#define USER_PASS "1234"

ListaTrasteros listaTrasteros;
//Simula que limpia la consola empujando el contenido para arriba
void limpiarConsola() {
    for (int i = 0; i < 15; i++) {
        printf("\n");
    }
    fflush(stdout);
}
char mostrarMenuPrincipal() {
    char opcion;
    printf("---Menu PRINCIPAL----\n");
    printf("1. Area Administrador\n");
    printf("2. Area Cliente\n");
    printf("0. Cerrar Programa\n");
    printf("Seleccione una opcion\n: ");
    fflush(stdout);
    fflush(stdin);
    scanf(" %c", &opcion);
    return opcion;
}

char menuIniReg() {
    char opcion;
    printf("\n--- MENÚ CLIENTE ---\n");
    printf("1. Iniciar Sesión\n");
    printf("2. Registrarse\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    fflush(stdin);
    scanf(" %c", &opcion);
    return opcion;
}

void menuCliente(Usuario u) {
	char opcion;
	do{
		sleep(1);
		limpiarConsola();
		printf("MENÚ\n");
		printf("1. Perfil\n");
		printf("2. Catalogo\n");
		printf("3. Alquilar trastero\n");
		printf("4. Devolver trastero\n");
		printf("0. Cerrar Sesion\n");
		printf("Seleccione una opcion\n: ");
		fflush(stdout);
		fflush(stdin);
		scanf(" %c", &opcion);
		switch (opcion) {
		case '1': // Ver el perfil del usuario
			menuPerfil(u);
			break;
	    case '2': // Mostrar catálogo de trasteros (aquí necesitarías implementar la función que muestra el catálogo de trasteros disponibles)
	    	//menuCatalogo(u,lt);
	    	break;
		case '3': {// Alquilar un trastero
			menuAlquilarTrastero(u);
			break;
		}
		case '4': {
			char input[10];
			printf("Ingrese el número de trastero a devolver: ");
			fflush(stdout);
			gets(input);
			int num = atoi(input);
			int pos = buscarTrastero(listaTrasteros, num);
			if (pos != -1) {
				devolverTrastero(&listaTrasteros.aTrasteros[pos]);
				printf("Trastero devuelto con éxito.\n");
			} else {
				printf("Trastero no encontrado.\n");
			}
			break;
		}
		case '0':
			// Cerrar sesión
			printf("Cerrando sesión...\n");
			break;
			default:
				printf("Opción inválida. Intente nuevamente.\n");
		        }
    }while (opcion != '0');



}
void menuCatalogo(Usuario u,ListaTrasteros *lt){
	char opcion;
		do{
			sleep(1);
			limpiarConsola();
			printf("MENÚ\n");
			printf("1. Ver todo el Catatalogo\n");
			printf("2. Filtrar por Precio\n");
			printf("3. Filtrar por Metros Cuadrados\n");
			printf("4. Filtrar por Valoracion\n");
			printf("0. Volver atras\n");
			printf("Seleccione una opcion\n: ");
			fflush(stdout);
			fflush(stdin);
			scanf(" %c", &opcion);
			switch (opcion) {
			case '1': // Ver el perfil del usuario
				ordenarPorNumeroTrastero(lt);

				break;
		    case '2': // Mostrar catálogo de trasteros (aquí necesitarías implementar la función que muestra el catálogo de trasteros disponibles)
		    	menuCatalogo(u,lt);
		    	break;
			case '3': {// Alquilar un trastero
				menuAlquilarTrastero(u);
				break;
			}
			case '4': {
				char input[10];
				printf("Ingrese el número de trastero a devolver: ");
				fflush(stdout);
				gets(input);
				int num = atoi(input);
				int pos = buscarTrastero(listaTrasteros, num);
				if (pos != -1) {
					devolverTrastero(&listaTrasteros.aTrasteros[pos]);
					printf("Trastero devuelto con éxito.\n");
				} else {
					printf("Trastero no encontrado.\n");
				}
				break;
			}
			case '0':
				// Cerrar sesión
				printf("Cerrando sesión...\n");
				break;
				default:
					printf("Opción inválida. Intente nuevamente.\n");
			        }
	    }while (opcion != '0');
}
void menuPerfil(Usuario u){
	char opcion;
	sleep(1);
	limpiarConsola();
	printf("-------------------------------------");fflush(stdout);
	printf("\nPERFIL");fflush(stdout);
	printf("\n-------------------------------------");fflush(stdout);
	printf("\nDATOS DEL USUARIO");fflush(stdout);
	printf("\n- Nombre: %s",u.nombre);fflush(stdout);
	printf("\n- Apellido: %s",u.apellido);fflush(stdout);
	printf("\n- DNI: %i",u.dni);fflush(stdout);
	printf("\n-------------------------------------");fflush(stdout);
	printf("\nDATOS DE CONTACTO");fflush(stdout);
	printf("\n- Telefono: %i",u.telefono);fflush(stdout);
	printf("\n- Email: %s",u.email);fflush(stdout);
	printf("\n- Direccion: %s",u.direccion);fflush(stdout);
	printf("\nPulse 0 para volver atras \n: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%c", &opcion);


	if(opcion=='0'){
		menuCliente(u); //Si pulsa 0 vuelve a la pestaña anterior
	}else{
		menuPerfil(u); //Si pulsa cualquier otra cosa le volvera a llevar a la misma pestaña
	}
}
void menuAlquilarTrastero(Usuario u){
	int numTrastero;
	char opcionRetorno;
	printf("Inserte el numero del trastero: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%i",numTrastero);
	//Buscamos el trastero por su nombre
	Trastero t=buscarTrasteroDDBB(NOMBRE_BBDD,numTrastero);
	visualizarTrastero(t);
	if(t.numeroTrastero==0){ //El trastero seleccionado no existe
		printf("Este trastero no existe en nuestro catalogo, prueba con otro\n");fflush(stdout);
		menuAlquilarTrastero(u);
	}else if(t.disponible!=0){//El trastero seleccionado esta disponible
		//añadimos trastero a la BD
		aniadirTrasteroAlquilado(NOMBRE_BBDD,t,u);
		printf("Trastero alquilado a nombre de %s\n", u.nombre);fflush(stdout);
		//Le devolvemos al menu
		menuCliente(u);
	}else{//El trastero seleccionado no esta disponible
		printf("Este trastero ya esta alquilado por otra persona\n\n");fflush(stdout);
		printf("Quieres intentar alquilar otro trastero trastero?(0=NO/1=SI)");
		do {
					fflush(stdout);
					fflush(stdin);
					scanf("%c",opcionRetorno);
					if (opcionRetorno!=1 || opcionRetorno!=0){
						printf("Caracter incorrecto, seleccione entre\n"
								"- 0: Volver al menu cliente\n"
								"- 1: Volver a insertar un numero de trastero");
					}else continue;
		} while (opcionRetorno!=1 || opcionRetorno!=0);
		if(opcionRetorno==1){
			//Si desea volver le abrimos otra vez el menu de alquiler
			menuAlquilarTrastero(u);
		}else menuCliente(u);//Si desea volver atras le devolvemos al menu cliente
	}
}
char menuAdministrador() {
    char opcion;
    printf("Menu ADMINISTRADOR\n");
    printf("1. Aniadir Trastero\n");
    printf("2. Eliminar Trastero\n");
    printf("3. Ver Clientes\n");
    printf("4. Ver Trasteros Disponibles\n");
    printf("0. Salir\n");
    printf("Seleccione una opcion\n: ");
    fflush(stdout);
    fflush(stdin);
    scanf(" %c", &opcion);
    while(getchar() != '\n');
    return opcion;
}

int autenticarAdministrador() {
    char usuario[50], contrasena[50];
    printf("Ingrese usuario: ");
	fflush(stdout);
	fflush(stdin);
    gets(usuario);

    printf("Ingrese contrasena: ");
	fflush(stdout);
	fflush(stdin);
    gets(contrasena);

    if (strcmp(usuario, ADMIN_USER) == 0 && strcmp(contrasena, ADMIN_PASS) == 0) {
        printf("Acceso concedido.\n");
        sleep(1);
        return 1;
    } else {
        sleep(0.5);
    	printf("Acceso denegado.\n");fflush(stdout);
        sleep(1);
        limpiarConsola();
        return 0;
    }
}

Trastero menuAniadirTrastero(){
	Trastero t;
	printf("AÑADIR TRASTERO");
	printf("\n-----------------\n");
	printf("Numero del trastero: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &t.numeroTrastero);
	printf("Metros Cuadrados: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &t.numeroTrastero);
	printf("Precio: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &t.numeroTrastero);
	t.disponible=1;
	t.valoracion=0;
	return t;
}






int registrarUsuario() {
    char nombre[50], apellido[50], email[50], direccion[50];
    char contrasena[50], confirmarContrasena[50], telefono[9], dni[9];
    int c,dniComp;
    while ((c = getchar()) != '\n' && c != EOF);//Limpiar el buffer
    printf("Introduce el nombre: ");
    fflush(stdout);
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;

    printf("Introduce el apellido: ");
    fflush(stdout);
    fgets(apellido, sizeof(apellido), stdin);
    apellido[strcspn(apellido, "\n")] = 0;

    printf("Introduce el email: ");
    fflush(stdout);
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0;

    printf("Introduce la dirección: ");
    fflush(stdout);
    fgets(direccion, sizeof(direccion), stdin);
    direccion[strcspn(direccion, "\n")] = 0;

    printf("Introduce el DNI: ");
    fflush(stdout);
    fgets(dni, sizeof(dni), stdin);
    dni[strcspn(dni, "\n")] = 0;

    printf("Introduce el teléfono: ");
    fflush(stdout);
    fgets(telefono, sizeof(telefono), stdin);
    telefono[strcspn(telefono, "\n")] = 0;

    printf("Introduce la contraseña: ");
    fflush(stdout);
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = 0;

    printf("Confirma la contraseña: ");
    fflush(stdout);
    fgets(confirmarContrasena, sizeof(confirmarContrasena), stdin);
    confirmarContrasena[strcspn(confirmarContrasena, "\n")] = 0;

    printf("%i devuelve usuarioRegistrado\n", usuarioRegistrado(NOMBRE_BBDD, dni));
    fflush(stdout);
    dniComp = atoi(dni);
    if (usuarioRegistrado(NOMBRE_BBDD, dniComp) == 1) {
        printf("Este DNI ya está registrado\n");
        fflush(stdout);
        sleep(1);
        return 0;
    } else if (strcmp(contrasena, confirmarContrasena) == 0) {
        Usuario u;
        strcpy(u.nombre, nombre);
        strcpy(u.apellido, apellido);
        u.dni = atoi(dni);
        u.telefono = atoi(telefono);
        strcpy(u.email, email);
        strcpy(u.direccion, direccion);
        strcpy(u.contrasenia, contrasena);
        sleep(1);
        aniadirUsuarioABBDD(NOMBRE_BBDD, u);
        printf("Usuario registrado\n");
        sleep(1);
        fflush(stdout);
        menuCliente(u);
        return 1;
    } else {
        printf("Las contraseñas no coinciden\n");
        fflush(stdout);
        registrarUsuario();
        return 0;
    }
}

int autenticarUsuario() {
    char usuario[50], contrasena[50];
    printf("Ingrese usuario: ");
	fflush(stdout);
	fflush(stdin);
    gets(usuario);

    printf("Ingrese contrasena: ");
	fflush(stdout);
	fflush(stdin);
    gets(contrasena);

    if (strcmp(usuario, USER1) == 0 && strcmp(contrasena, USER_PASS) == 0) {
        printf("Acceso concedido.\n");
        return 1;
    } else {
        printf("Acceso denegado.\n");
        return 0;
    }
}

void manejarCliente(ListaUsuarios *lu) {
    char opcion;
    Usuario usuarioActual;

    do {
        opcion = menuIniReg();
        switch (opcion) {
            case '1':
            	if(autenticarUsuario()){
                printf("Iniciando sesión...\n");
                strcpy(usuarioActual.nombre, "Usuario");
                strcpy(usuarioActual.apellido, "Prueba");
                usuarioActual.dni = 12345678;
                menuCliente(usuarioActual);
            	}
            	break;
            case '2':
            	if(registrarUsuario()){
            		printf("Usuario Registrado...\n");
            	}
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != '1' && opcion != '2');

}
void manejarAdministrador() {
    char opcion;
    if (autenticarAdministrador()) {
        do {
            opcion = menuAdministrador();
            switch (opcion) {
                case '1': {
                    Trastero t = {0};
                    char input[50]={0};

                    fprintf( stdout,"Ingrese Numero de Trastero: ");
                    fflush(stdout);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                    	 input[strcspn(input, "\n")] = 0;
                    	t.numeroTrastero = atoi(input);

                    	printf("Numero de Trastero ingresado: %d\n", t.numeroTrastero);
                    }

                   fprintf(stdout,"Ingrese Metros Cuadrados: ");
                   fflush(stdout);
                   if (fgets(input, sizeof(input), stdin) != NULL) {
                	   input[strcspn(input, "\n")] = 0;
                       t.metrosCuadrados = atoi(input);

                       printf("Metros Cuadrados ingresados: %d\n", t.metrosCuadrados);
                    }

                    fprintf(stdout,"Ingrese Valoracion: ");
                    fflush(stdout);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        input[strcspn(input, "\n")] = 0;
                        t.valoracion = atoi(input);

                        printf("Valoracion ingresada: %d\n", t.valoracion);

                    }

                    fprintf(stdout,"Ingrese Precio: ");
                    fflush(stdout);
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                    	input[strcspn(input, "\n")] = 0;
                        t.precio = atoi(input);

                        printf("Precio ingresado: %d\n", t.precio);
                    }

                    t.disponible = 1;

                    aniadirTrastero(&listaTrasteros, t);
                    aniadirTrasteroABBDD(NOMBRE_BBDD, t);
                    printf("Trastero agregado correctamente.\n");
                    break;
                }
                case '2': {
                    char input[50];
                    printf("Ingrese Numero de Trastero a eliminar: ");
                    fflush(stdout);
                    gets(input);
                    int num = atoi(input);

                    Trastero t;
                    t.numeroTrastero = num;
                    eliminarTrastero(&listaTrasteros, t);
                    eliminarTrasteroDDBB(NOMBRE_BBDD,t.numeroTrastero);
                    break;
                }
                case '3':
                    visualizarTrasterosDisponibles(listaTrasteros);
                    break;
                case '4': {
                    char input[50];
                    printf("Ingrese Numero de Trastero a alquilar: ");
                    fflush(stdout);
                    gets(input);
                    int num = atoi(input);

                    int pos = buscarTrastero(listaTrasteros, num);
                    if (pos != -1) {
                        alquilarTrastero(&listaTrasteros.aTrasteros[pos]);
                    } else {
                        printf("Trastero no encontrado.\n");
                    }
                    break;
                }
                case '5': {
                    char input[50];
                    printf("Ingrese Numero de Trastero a devolver: ");
                    fflush(stdout);
                    gets(input);
                    int num = atoi(input);

                    int pos = buscarTrastero(listaTrasteros, num);
                    if (pos != -1) {
                        devolverTrastero(&listaTrasteros.aTrasteros[pos]);
                    } else {
                        printf("Trastero no encontrado.\n");
                    }
                    break;
                }
                case '0':
                    printf("Saliendo del administrador...\n");
                    break;
                default:
                    printf("Opcion invalida.\n");
            }
        } while (opcion != '0');
    }
}

