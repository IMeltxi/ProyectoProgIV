// main.c
#include "menu.h"
#include "usuario.h"
#include <stdio.h>
#include <stdlib.h>



int main() {
    ListaUsuarios lu;
    Usuario u={0};
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
