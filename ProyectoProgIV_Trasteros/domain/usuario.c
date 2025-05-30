#include "usuario.h"
#include <stdio.h> //printf, scanf, fflush(stdin), fflush(stdout) y NULL
#include <stdlib.h> //malloc y free
#include <string.h>
#define ADMIN_USER "admin"
#define ADMIN_PASS "1234"

void inicializarListaUsuarios(ListaUsuarios *lu){
	lu->numUsuarios = 0;
	lu->aUsuarios = NULL;
}
void aniadirUsuarioAListaUsuarios(ListaUsuarios *lu, Usuario u){
	int i;
	if(lu->aUsuarios == NULL){
		lu->aUsuarios = (Usuario*)malloc(1*sizeof(Usuario));
	}else{
		Usuario *aux = (Usuario*)malloc(lu->numUsuarios * sizeof(Usuario));
		for(i=0;i<lu->numUsuarios;i++){
			aux[i] = lu->aUsuarios[i];
		}
		free(lu->aUsuarios);
		lu->aUsuarios = (Usuario*)malloc((lu->numUsuarios+1)*sizeof(Usuario));
		for(i=0;i<lu->numUsuarios;i++){
			lu->aUsuarios[i] = aux[i];
		}
		free(aux);
	}
	lu->aUsuarios[lu->numUsuarios] = u;
	lu->numUsuarios++;

}
//Para listar los clientes
void visualizarUsuario(Usuario u){
	printf("\033[34m%s%15s%15i%15i%20s%15s%15s\033[0m\n", u.nombre, u.apellido, u.dni, u.telefono, u.email, u.direccion, u.contrasenia);
	fflush(stdout);
}
void visualizarListaUsuarios(ListaUsuarios lu){
	int i;
//	printf("\033[34m%s%15s%15s%15s%20s%15s%15s\033[0m\n", "NOMBRE", "APELLIDOS", "DNI", "TELEFONO", "EMAIL", "DIRECCION", "CONTRASEÑA");
//	fflush(stdout);

	for(i=0;i<lu.numUsuarios;i++){
		visualizarUsuario(lu.aUsuarios[i]);
		fflush(stdout);
	}

}
Usuario* iniciarSesion(ListaUsuarios lu, char* email, char* contra) {

    for (int i = 0; i < lu.numUsuarios; i++) {
        if (strcmp(lu.aUsuarios[i].email, email) == 0 && strcmp(lu.aUsuarios[i].contrasenia, contra) == 0) {
            return &lu.aUsuarios[i];  // Devolvemos un puntero al usuario encontrado
        }
    }
    printf("\033[31mNO SE HA ENCONTRADO AL USUARIO\033[0m\n");

    return 0;
}


void cargarUsuariosDesdeCSV(ListaUsuarios *lu, char *nombreArchivo) {
    FILE *pf;
    char linea[200];
    Usuario u;

    pf = fopen(nombreArchivo, "r");
    if (pf == NULL) {
    	printf("\033[31mError al abrir el archivo %s\033[0m\n", nombreArchivo);
        return;
    }

    while (fgets(linea, sizeof(linea), pf) != NULL) {
        char *nombre = strtok(linea, ";");
        char *apellido = strtok(NULL, ";");
        char *dni = strtok(NULL, ";");
        char *telefono = strtok(NULL, ";");
        char *email = strtok(NULL, ";");
        char *direccion = strtok(NULL, ";");
        char *contrasenia = strtok(NULL, ";\n");  // Para evitar el salto de línea

        if (nombre && apellido && dni && telefono && email && direccion && contrasenia) {
            strcpy(u.nombre, nombre);
            strcpy(u.apellido, apellido);
            u.dni = atoi(dni);
            u.telefono = atoi(telefono);
            strcpy(u.email, email);
            strcpy(u.direccion, direccion);
            strcpy(u.contrasenia, contrasenia);

            // Redimensionar array dinámico para agregar el nuevo usuario
            lu->numUsuarios++;
            lu->aUsuarios = realloc(lu->aUsuarios, lu->numUsuarios * sizeof(Usuario));

            if (lu->aUsuarios == NULL) {
            	printf("\033[31mError al asignar memoria\033[0m\n");
                fclose(pf);
                return;
            }

            // Agregar usuario a la lista
            lu->aUsuarios[lu->numUsuarios - 1] = u;
        }
    }

    fclose(pf);
}
int autenticarAdministrador(char* usuario,char* contrasena) {
    if (strcmp(usuario, ADMIN_USER) == 0 && strcmp(contrasena, ADMIN_PASS) == 0) {
        return 1;
    } else {
        return 0;
    }
}

