#include "menu.h"
#include <stdio.h>

void mostrarMenuPrincipal() {
    int opcion;
    do {
        printf("\nMENÚ PRINCIPAL\n");
        printf("1. Área Administrador\n");
        printf("2. Área Cliente\n");
        printf("0. Cerrar Programa\n");
        printf("Seleccione una opción: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no válida, intente de nuevo.\n");
            while (getchar() != '\n'); // Limpiar el buffer
            continue;
        }

        switch (opcion) {
            case 1:
                menuAdministrador();
                break;
            case 2:
                menuCliente();
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida, intente de nuevo.\n");
        }
    } while (opcion != 0);
}

void menuCliente() {
    int opcion;
    do {
        printf("\nMENÚ CLIENTE\n");
        printf("1. Perfil\n");
        printf("2. Catálogo\n");
        printf("3. Alquilar trastero\n");
        printf("4. Devolver trastero\n");
        printf("0. Cerrar Sesión\n");
        printf("Seleccione una opción: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no válida, intente de nuevo.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                printf("Mostrando perfil...\n");
                break;
            case 2:
                printf("Mostrando catálogo...\n");
                break;
            case 3:
                printf("Alquilando trastero...\n");
                break;
            case 4:
                printf("Devolviendo trastero...\n");
                break;
            case 0:
                printf("Cerrando sesión...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
}

void menuAdministrador() {
    int opcion;
    do {
        printf("\nMENÚ ADMINISTRADOR\n");
        printf("1. Añadir Trastero\n");
        printf("2. Eliminar Trastero\n");
        printf("3. Ver Clientes\n");
        printf("4. Ver Trasteros Disponibles\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no válida, intente de nuevo.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                printf("Añadiendo trastero...\n");
                break;
            case 2:
                printf("Eliminando trastero...\n");
                break;
            case 3:
                printf("Mostrando clientes...\n");
                break;
            case 4:
                printf("Mostrando trasteros disponibles...\n");
                break;
            case 0:
                printf("Saliendo del menú administrador...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
}
