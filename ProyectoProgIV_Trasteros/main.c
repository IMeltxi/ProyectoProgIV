#include "usuario.h"
#include <stdio.h>
#include <stdlib.h>
#include "menu.h" // Asegurarse de que es "menu.h"

int main() {
    // Crear usuario y lista
    Usuario u = {"Ander", "Gonzalez", 123456789, 987654321, "ander@gmail.com", "larra", NULL};
    ListaUsuarios lu;

    printf("Iniciando el programa...\n");

    // Inicializar lista antes de usarla
    inicializarListaUsuarios(&lu);

    // Añadir usuario a la lista
    aniadirUsuarioAListaUsuarios(&lu, u);

    // Mostrar lista de usuarios
    visualizarListaUsuarios(lu);

    // Mostrar menú
    mostrarMenuPrincipal();

    printf("Presiona Enter para salir...\n");
    getchar();  // Espera entrada antes de cerrar

    printf("Fin del programa\n");

    return 0; // Ahora sí terminamos correctamente
}
