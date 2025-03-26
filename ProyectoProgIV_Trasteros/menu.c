#include "menu.h"
#include <stdio.h>

char mostrarMenuPrincipal() {
    char opcion;
    printf("Menu PRINCIPAL\n");
    printf("1. \xC1rea Administrador\n");
    printf("2. \xC1rea Cliente\n");
    printf("0. Cerrar Programa\n");
    printf("Seleccione una opci\xF3n: ");
    fflush(stdout);
    fflush(stdin);
    scanf(" %c", &opcion);
    return opcion;
}

char menuCliente() {
    char opcion;
    printf("Menu CLIENTE\n");
    printf("1. Perfil\n");
    printf("2. Cat\xE1logo\n");
    printf("3. Alquilar trastero\n");
    printf("4. Devolver trastero\n");
    printf("0. Cerrar Sesi\xF3n\n");
    printf("Seleccione una opci\xF3n: ");
    scanf(" %c", &opcion);
    return opcion;
}

char menuAdministrador() {
    char opcion;
    printf("Menu ADMINISTRADOR\n");
    printf("1. A\xF1adir Trastero\n");
    printf("2. Eliminar Trastero\n");
    printf("3. Ver Clientes\n");
    printf("4. Ver Trasteros Disponibles\n");
    printf("0. Salir\n");
    printf("Seleccione una opci\xF3n: ");
    scanf(" %c", &opcion);
    return opcion;
}
