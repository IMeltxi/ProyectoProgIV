#ifndef BBDD_H_
#define BBDD_H_
#include "sqlite3.h"
#include "../domain/usuario.h"
#include "../domain/trastero.h"
#define NOMBRE_BBDD "TrasterosDeusto.db"

//Funciones para la ejecucion de la bd
int inicializarBBDD(sqlite3 **db);
void crearTablas(sqlite3 *db);
//Funcion para añadir usuarios a la tabla Usuarios
int aniadirUsuarioABBDD(sqlite3 *db, Usuario u);
//Funcion para añadir trasteros a la tabla Trasteros
int aniadirTrasteroABBDD(sqlite3 *db, Trastero t);
//Funcion para obtener la fecha en la que se realiza la operacion
void obtenerFechaActual(char *buffer, int buffer_size); //Generada con IA
//Funcion para guardar
void aniadirTrasteroAlquilado(sqlite3 *db, Trastero t,Usuario u);
//Funcion para guardar el trastero una vez se ha devuelto en la tabla HISTORIAL
void devolverTrasteroBBDD(sqlite3 *db, Trastero t);
//Funciones de extraccion de datos
Usuario obtenerUsuario(sqlite3 *db,int dni);
void aniadirTrasteroAlquilado(sqlite3 *db, Trastero t, Usuario u);
int usuarioRegistrado(sqlite3 *db,int dni);
Trastero buscarTrasteroDDBB(sqlite3 *db, int numeroTrastero);
void eliminarTrasteroDDBB(sqlite3 *db,int numeroTrastero);
int verificarAlquiler(sqlite3 *db, int numeroTrastero, int dni);

void cargarTrasterosDesdeDB(ListaTrasteros *lt, sqlite3 *db);
void cargarUsuariosDesdeDB(ListaUsuarios *lu, sqlite3 *db);
int registrarUsuario(sqlite3 *db,char* nombre, char* apellido, char* email, char* direccion, char* contrasena, char* confirmarContrasena, char* telefono, char* dni);
void cerrarPrograma(sqlite3 *db, ListaUsuarios *lu, ListaTrasteros *lt);
int autenticarUsuario(sqlite3 *db,int dni,char* contrasena);

#endif /* BBDD_H_ */
