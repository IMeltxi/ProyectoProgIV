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

void menuCliente(Usuario u) {
	char opcion;
	do{
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
		switch (opcion) {
			case '1':
				menuPerfil(u);
				break;
			default:
				break;
		}
    }while (opcion != '0');



}
void menuPerfil(Usuario u){
	char opcion;
	printf("DATOS DEL USUARIO");fflush(stdout);
	printf("- Nombre: %s",u.nombre);fflush(stdout);
	printf("- Apellido: %s",u.apellido);fflush(stdout);
	printf("- DNI: %i",u.dni);fflush(stdout);
	printf("-------------------------------------");fflush(stdout);
	printf("DATOS DE CONTACTO");fflush(stdout);
	printf("- Telefono: %i",u.telefono);fflush(stdout);
	printf("- Email: %s",u.email);fflush(stdout);
	printf("- Direccion: %s",u.direccion);fflush(stdout);
	printf("-------------------------------------");fflush(stdout);
	printf("Pulse 0 para volver atras \n: ");
	fflush(stdout);
	fflush(stdin);
	scanf(" %c", &opcion);
	if(opcion==0){
		menuCliente(u); //Si pulsa 0 vuelve a la pestaña anterior
	}else{
		menuPerfil(u); //Si pulsa cualquier otra cosa le volvera a llevar a la misma pestaña
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
void registrarUsuario(){
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
    //Esta ya registrado?
    if(usuarioRegistrado(NOMBRE_BBDD,dni)==1){
    	printf("Este DNI ya esta registrado");fflush(stdout);
    	registrarUsuario();
    }else if (strcmp(contrasena, confirmarContrasena) == 0 ) {
    	Usuario u ={nombre,apellido,atoi(dni),atoi(apellido),email,direccion,contrasena};
    	visualizarPerfilUsuario(u);
    	aniadirUsuarioABBDD(NOMBRE_BBDD,u);
           printf("Usuario registrado\n");
           aniadirUsuarioABBDD(NOMBRE_BBDD,u);
           menuCliente(u);
       }else {
    	   printf("Las contraseñas no coinciden");fflush(stdout);
    	   registrarUsuario();
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
            	if(registrarUsuario()){ DA ERROR
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
                    Trastero t;
                    char input[50];
                    printf("Ingrese Numero de Trastero: ");
                    fflush(stdout);
                    gets(input);
                    t.numeroTrastero = atoi(input);

                    printf("Ingrese Metros Cuadrados: ");
                    fflush(stdout);
                    gets(input);
                    t.metrosCuadrados = atoi(input);

                    printf("Ingrese Valoracion: ");
                    fflush(stdout);
                    gets(input);
                    t.valoracion = atoi(input);

                    printf("Ingrese Precio: ");
                    fflush(stdout);
                    gets(input);
                    t.precio = atoi(input);

                    t.disponible = 1;
                    aniadirTrastero(&listaTrasteros, t);
                    aniadirTrasteroABBDD(NOMBRE_BBDD,t);
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

