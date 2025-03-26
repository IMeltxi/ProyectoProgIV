// main.c
#include "menu.h"
#include "usuario.h"
#include <stdio.h>
#include <stdlib.h>

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
            case '1': {
                char opcionAdmin;
                do {
                    opcionAdmin = menuAdministrador();
                    switch (opcionAdmin) {
                        case '1':
                            printf("aniadiendo trastero...\n");
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
                            printf("Saliendo del menu administrador...\n");
                            break;
                        default:
                            printf("ERROR\n");
                    }
                } while (opcionAdmin != '0');
                break;
            }
            case '2': {
                char opcionCliente;
                do {
                    opcionCliente = menuCliente();
                    switch (opcionCliente) {
                        case '1':
                            printf("Mostrando perfil...\n");
                            break;
                        case '2':
                            printf("Mostrando cat\xE1logo...\n");
                            break;
                        case '3':
                            printf("Alquilando trastero...\n");
                            break;
                        case '4':
                            printf("Devolviendo trastero...\n");
                            break;
                        case '0':
                            printf("Cerrando sesion...\n");
                            break;
                        default:
                            printf("ERROR.\n");
                    }
                } while (opcionCliente != '0');
                break;
            }
            case '0':
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("ERROR.\n");
        }
    } while (opcionPrincipal != '0');

    printf("Presiona Enter para salir...\n");
    getchar();
    printf("Fin del programa\n");
    return 0;
}
