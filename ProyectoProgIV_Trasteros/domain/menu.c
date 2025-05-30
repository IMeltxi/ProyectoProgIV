#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#include "trastero.h"
#include "../db/bbdd.h"


#define ADMIN_USER "admin"
#define ADMIN_PASS "1234"

ListaTrasteros listaTrasteros;
//Simula que limpia la consola empujando el contenido para arriba
void limpiarConsola() {
    for (int i = 0; i < 3; i++) {
        printf("...............................................................................................................\n");
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
    printf("0. Cerrar\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    fflush(stdin);
    scanf(" %c", &opcion);
    return opcion;
}

char menuCliente() {
	char opcion;

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
	return opcion;
}
char menuCatalogo(){
	char opcion;

	limpiarConsola();
	printf("CATALOGO\n");
	printf("----------------------------------------\n");
	printf("1. Ver todo el Catatalogo\n");
	printf("2. Filtrar por Precio\n");
	printf("3. Filtrar por Metros Cuadrados\n");
	printf("4. Filtrar por Valoracion\n");
	printf("0. Volver atras\n");
	printf("Seleccione una opcion\n: ");
	fflush(stdout);
	fflush(stdin);
	scanf(" %c", &opcion);
	return opcion;
}
void menuPerfil(Usuario u){

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
}
char volverAtras(){
	char opcion;
	printf("\nPulse 0 para volver atras \n: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%c", &opcion);
	return opcion;
}
void valoracionTrasteroDevuelto(Trastero t){
	int valoracion;
	do{
	printf("\nPulse 0 para volver atras \n: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &valoracion);
	if((valoracion<0||valoracion>5)){
		printf("La valoracion debe ser un numero 1-5");
	}
	}while(valoracion<0||valoracion>5);
}
void menuAlquilarTrastero(Usuario u, sqlite3 *db){
	int numTrastero;
	char opcionRetorno;
	printf("Inserte el numero del trastero: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%i",&numTrastero);
	//Buscamos el trastero por su nombre
	Trastero t=buscarTrasteroDDBB(db,numTrastero);
	visualizarTrastero(t);
	if(t.numeroTrastero==0){ //El trastero seleccionado no existe
		printf("Este trastero no existe en nuestro catalogo, prueba con otro\n");fflush(stdout);
		menuAlquilarTrastero(u, db);
	}else if(t.disponible!=0){//El trastero seleccionado esta disponible
		//añadimos trastero a la BD
		aniadirTrasteroAlquilado(db,t,u);
		printf("Trastero alquilado a nombre de %s\n", u.nombre);fflush(stdout);
		//Le devolvemos al menu
		menuCliente(u, db);
	}else{//El trastero seleccionado no esta disponible
		printf("Este trastero ya esta alquilado por otra persona\n\n");fflush(stdout);
		printf("Quieres intentar alquilar otro trastero trastero?(0=NO/1=SI)");
		do {
					fflush(stdout);
					fflush(stdin);
					scanf("%c",&opcionRetorno);
					if (opcionRetorno!=1 || opcionRetorno!=0){
						printf("Caracter incorrecto, seleccione entre\n"
								"- 0: Volver al menu cliente\n"
								"- 1: Volver a insertar un numero de trastero");
					}else continue;
		} while (opcionRetorno!=1 || opcionRetorno!=0);
		if(opcionRetorno==1){
			//Si desea volver le abrimos otra vez el menu de alquiler
			menuAlquilarTrastero(u, db);
		}else menuCliente(u, db);//Si desea volver atras le devolvemos al menu cliente
	}
}
char menuAdministrador() {
    char opcion;

    limpiarConsola();
    printf("Menu ADMINISTRADOR\n");
    printf("1. Añadir Trastero\n");
    printf("2. Eliminar Trastero\n");
    printf("3. Ver Clientes\n");
    printf("4. Ver Trasteros Disponibles\n");
    printf("5. Obtener Ficheros sobre los trasteros.\n");
    printf("0. Salir\n");
    printf("Seleccione una opcion\n: ");
    fflush(stdout);
    fflush(stdin);
    scanf(" %c", &opcion);
    while(getchar() != '\n');
    return opcion;
}

int autenticarAdministrador(char* usuario,char* contrasena) {
    if (strcmp(usuario, ADMIN_USER) == 0 && strcmp(contrasena, ADMIN_PASS) == 0) {
        return 1;
    } else {
        return 0;
    }
}

Trastero menuAniadirTrastero(){
	Trastero t;

	limpiarConsola();
	printf("AÑADIR TRASTERO");
	printf("\n-----------------\n");
	printf("Numero del trastero: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &t.numeroTrastero);
	printf("Metros Cuadrados: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &t.metrosCuadrados);
	printf("Precio: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%f", &t.precio);
	t.disponible=1;
	t.valoracion=0.0;
	t.numeroDeValoraciones=0;
	return t;
}
int menuEliminarTrastero(){
	int numTrastero;

	limpiarConsola();
	printf("ELIMINAR TRASTERO");
	printf("\n-----------------\n");
	printf("Numero del trastero: ");
	fflush(stdout);
	fflush(stdin);
	scanf("%d", &numTrastero);
	return numTrastero;
}

char menuTrasterosAdmin(){
	char opcion;
	    printf("VISUALIZAR TRASTEROS\n");
	    printf("1. VER TODOS LOS TRASTEROS\n");
	    printf("2. VER TRASTEROS ALQUILADOS\n");
	    printf("3. VER TRASTEROS DISPONIBLES\n");
	    printf("0. Salir\n");
	    printf("Seleccione una opcion\n: ");
	    fflush(stdout);
	    fflush(stdin);
	    scanf(" %c", &opcion);
	    while(getchar() != '\n');
	    return opcion;
}

char menuFicherosAdmin(){
	char opcion;
	    printf("OBTENER FICHERO.CSV\n");
	    printf("1. OBTENER CSV CON LOS TRASTEROS ACTUALMENTE ALQUILADOS\n");
	    printf("2. OBTENER CSV CON TODOS LOS TRASTEROS QUE HAN SIDO ALQUILADOS\n");
	    printf("0. Salir\n");
	    printf("Seleccione una opcion\n: ");
	    fflush(stdout);
	    fflush(stdin);
	    scanf(" %c", &opcion);
	    while(getchar() != '\n');
	    return opcion;
}


int registrarUsuario(sqlite3 *db,char* nombre, char* apellido, char* email, char* direccion, char* contrasena, char* confirmarContrasena, char* telefono, char* dni) {
    int dniComp;
    char c;
    dniComp = atoi(dni);
    if (usuarioRegistrado(db, dniComp) == 1) {
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

        return aniadirUsuarioABBDD(db, u);

    } else {
//    	printf("\033[1;31mLas contraseñas no coinciden\n\033[0m");
//        fflush(stdout);
    	return 2;
    }
}

int autenticarUsuario(sqlite3 *db,int dni,char* contrasena) {
		Usuario u;
		u = obtenerUsuario(db,dni);
		if(u.dni!=-1){
			//Usuario encontrado
			if (dni==u.dni && (strcmp(contrasena, u.contrasenia) == 0)) {
				//printf("\033[1;32mAcceso concedido.\n\033[0m");
				return u.dni;
				}
		}
	return -1;
}

char manejarCliente(sqlite3 *db) {
    char opcion;
    Usuario usuarioActual;

    do {
        opcion = menuIniReg();
        switch (opcion) {
            case '1':
//            	if(autenticarUsuario(db)){
//                printf("Iniciando sesión...\n");
//                strcpy(usuarioActual.nombre, "Usuario");
//                strcpy(usuarioActual.apellido, "Prueba");
//                usuarioActual.dni = 12345678;
//                menuCliente(usuarioActual, db);
//            	}
            	break;
            case '2':
//            	if(registrarUsuario(db)){
//            		printf("Usuario Registrado...\n");
//            	}
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != '1' && opcion != '2');
    return opcion;
}

void cerrarPrograma(sqlite3 *db, ListaUsuarios *lu, ListaTrasteros *lt) {
    // Liberar la memoria de las listas
	if (lu->aUsuarios != NULL) {
	        free(lu->aUsuarios);
	        lu->aUsuarios = NULL;  // Establecer a NULL después de liberar la memoria
	 }
	 lu->numUsuarios = 0;

    // Cerrar la base de datos
    if (db != NULL) {
        sqlite3_close(db);
        printf("\033[0;32mBase de datos cerrada correctamente.\033[0m\n");
    }

    // Aquí puedes agregar otras tareas de limpieza si es necesario
    // Por ejemplo, cerrar archivos, liberar memoria adicional, etc.

    printf("Fin del programa\n");
}



