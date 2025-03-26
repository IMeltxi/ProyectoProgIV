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

    char opcion;
        do {
            opcion = mostrarMenuPrincipal();
            switch (opcion) {
                case '1':
                    manejarAdministrador();
                    break;
                case '2':
                    manejarCliente();
                    break;
                case '0':
                    printf("Cerrando programa...\n");
                    break;
                default:
                    printf("Opción inválida.\n");
            }
        } while (opcion != '0');
        return 0;
}
