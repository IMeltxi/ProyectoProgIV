#include "usuario.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	Usuario u={"Ander","Gonzalez",123456789,987654321,"ander@gmail.com","larra",NULL};
	ListaUsuarios lu;

	inicializarListaUsuarios(&lu);
	aniadirUsuarioAListaUsuarios(&lu,u);
	visualizarListaUsuarios(lu);

	printf("fin");
}
