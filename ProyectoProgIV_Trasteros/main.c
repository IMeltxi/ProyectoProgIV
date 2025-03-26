// main.c
#include "menu.h"
#include "usuario.h"
#include <stdio.h>
#include <stdlib.h>
#define ADMIN_USER "admin"
#define ADMIN_PASS "1234"

int autenticarAdministrador() {
    char usuario[50], contrasena[50];
    printf("Ingrese usuario: ");
    scanf("%s", usuario);
    printf("Ingrese contrasena: ");
    scanf("%s", contrasena);

    if (strcmp(usuario, ADMIN_USER) == 0 && strcmp(contrasena, ADMIN_PASS) == 0) {
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
                printf("Iniciando sesión...\n");
                break;
            case '2':
                printf("Registrando usuario...\n");
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
int main() {
    Usuario u = {"Ander", "Gonzalez", 123456789, 987654321, "ander@gmail.com", "larra", NULL};
    ListaUsuarios lu;

    printf("Iniciando el programa...\n");
    inicializarListaUsuarios(&lu);
    aniadirUsuarioAListaUsuarios(&lu, u);
    visualizarListaUsuarios(lu);

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
    return 0;
}
