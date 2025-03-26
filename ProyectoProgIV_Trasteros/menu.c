#include "menu.h"
#include <stdio.h>
#include <string.h>
#include "usuario.h"
#include "trastero.h"



#define ADMIN_USER "admin"
#define ADMIN_PASS "1234"
#define USER1 "user"
#define USER_PASS "1234"

ListaTrasteros listaTrasteros;


char mostrarMenuPrincipal() {
    char opcion;
    printf("Menu PRINCIPAL\n");
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

char menuCliente() {
    char opcion;
    printf("Menu CLIENTE\n");
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
        return 1;
    } else {
        printf("Acceso denegado.\n");
        return 0;
    }
}
 int registrarUsuario(){
	char nombre[50],apellido[50],email[50],direccion[50], contrasena[50],confirmarContrasena[50],telefono[9],dni[9];
	printf("Introduce el nomre:");
	fflush(stdout);
	fflush(stdin);
    gets(nombre);
	printf("Introduce el apellido:");
	fflush(stdout);
	fflush(stdin);
    gets(apellido);
	printf("Introduce el email:");
	fflush(stdout);
	fflush(stdin);
    gets(email);
	printf("Introduce el direccion:");
	fflush(stdout);
	fflush(stdin);
    gets(direccion);
	printf("Introduce el dni:");
	fflush(stdout);
	fflush(stdin);
    gets(dni);
	printf("Introduce el telefono:");
	fflush(stdout);
	fflush(stdin);
    gets(telefono);
	printf("Introducela contraseña:");
	fflush(stdout);
	fflush(stdin);
    gets(contrasena);
	printf("Confirma Contraseña:");
	fflush(stdout);
	fflush(stdin);
    gets(confirmarContrasena);
    if (strcmp(contrasena, confirmarContrasena) == 0 ) {
    	Usuario u ={nombre,apellido,atoi(dni),atoi(apellido),email,direccion,contrasena};
    	visualizarPerfilUsuario(u);
           printf("Usuario registrado\n");

           return 0;
       }else return 1;

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

void manejarCliente() {
    char opcion;
    do {
        opcion = menuIniReg();
        switch (opcion) {
            case '1':
            	if(autenticarUsuario()){
                printf("Iniciando sesión...\n");
            	}break;
            case '2':
            	if(registrarUsuario()){
            		printf("Usuario Registrado...\n");
            	}
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != '1' && opcion != '2');

    do {
        opcion = menuCliente();
        switch (opcion) {
            case '1':
                printf("Mostrando perfil...\n");
                break;
            case '2':
                printf("Mostrando catálogo...\n");
                break;
            case '3':
                printf("Alquilando trastero...\n");
                break;
            case '4':
                printf("Devolviendo trastero...\n");
                break;
            case '0':
                printf("Cerrando sesión...\n");
                break;
            default:
                printf("ERROR.\n");
        }
    } while (opcion != '0');
}
void manejarAdministrador() {
    char opcion;
    if (autenticarAdministrador()) {
        do {
            opcion = menuAdministrador();
            switch (opcion) {
                case '1': {
                    Trastero nuevo;
                    printf("Ingrese Numero de Trastero: ");
                    scanf("%d", &nuevo.numeroTrastero);
                    printf("Ingrese Metros Cuadrados: ");
                    scanf("%d", &nuevo.metrosCuadrados);
                    printf("Ingrese Valoracion: ");
                    scanf("%d", &nuevo.valoracion);
                    printf("Ingrese Precio: ");
                    scanf("%d", &nuevo.precio);
                    nuevo.disponible = 1;
                    aniadirTrastero(&listaTrasteros, nuevo);
                    printf("%s", nuevo);
                    break;
                }
                case '2': {
                    int num;
                    printf("Ingrese Numero de Trastero a eliminar: ");
                    scanf("%d", &num);
                    Trastero t;
                    t.numeroTrastero = num;
                    eliminarTrastero(&listaTrasteros, t);
                    break;
                }
                case '3':
                    visualizarTrasterosDisponibles(listaTrasteros);
                    break;
                case '4': {
                    int num;
                    printf("Ingrese Numero de Trastero a alquilar: ");
                    scanf("%d", &num);
                    int pos = buscarTrastero(listaTrasteros, num);
                    if (pos != -1) {
                        alquilarTrastero(&listaTrasteros.aTrasteros[pos]);
                    } else {
                        printf("Trastero no encontrado.\n");
                    }
                    break;
                }
                case '5': {
                    int num;
                    printf("Ingrese Numero de Trastero a devolver: ");
                    scanf("%d", &num);
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
