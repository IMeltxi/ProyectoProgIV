#include "usuario.h"
#include <stdio.h> //printf, scanf, fflush(stdin), fflush(stdout) y NULL
#include <stdlib.h> //malloc y free
#include <string.h>

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
	printf("%s%15s%15i%15i%20s%15s",u.nombre,u.apellido,u.dni,u.telefono,u.email,u.direccion);
	fflush(stdout);
	fflush(stdin);
}
void visualizarListaUsuarios(ListaUsuarios lu){
	int i;
	printf("%s%15s%15s%15s%20s%15s\n","NOMBRE","APELLIDOS","DNI","TELEFONO","EMAIL","DIRECCION");
	fflush(stdout);
	fflush(stdin);
	for(i=0;i<sizeof(lu);i++){
		visualizarUsuario(lu.aUsuarios[i]);
	}

}
void resgistrarUsuario(Usuario u);
void iniciarSesion(Usuario u);
