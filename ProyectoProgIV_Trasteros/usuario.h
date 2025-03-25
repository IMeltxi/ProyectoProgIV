#ifndef USUARIO_H_
#define USUARIO_H_
typedef struct{
	char nombre[30];
	char apellido[30];
	int dni;
	int telefono;
	char email[30];
	char direccion[30];
	char contrasenia[30];
}Usuario;

typedef struct{
	Usuario * aUsuarios; //Array din�mico de usuarios ya que no sabemos cuantos se registraran
	int numUsuarios;
}ListaUsuarios;

void inicializarListaUsuarios(ListaUsuarios *lu);
void aniadirUsuarioAListaUsuarios(ListaUsuarios *lu, Usuario u);//Registrar usuario
void visualizarUsuario(Usuario u);
void visualizarListaUsuarios(ListaUsuarios lu);

Usuario* iniciarSesion(ListaUsuarios lu,char* email, char* contra);
void visualizarPerfilUsuario(Usuario u);

#endif
