#ifndef BBDD_H_
#define BBDD_H_
#include "sqlite3.h"
#include "usuario.h"
#include "trastero.h"
#define NOMBRE_BBDD "TrasterosDeusto.db"

//Funciones para la ejecucion de la bd
int inicializarBBDD(sqlite3 **db);
void crearTablas(sqlite3 *db);
//Funcion para añadir usuarios a la tabla Usuarios
void aniadirUsuario(sqlite3 *db, Usuario u);
//Funcion para añadir trasteros a la tabla Trasteros
void aniadirTrasteroABBDD(sqlite3 *db, Trastero t);
//Funcion para marcar si un trastero esta disponible o no (1=disponible)
void modificarDisponibilidadTrasteroABBDD(sqlite3 *db, Trastero t);
//Funcion para obtener la fecha en la que se realiza la operacion
void obtenerFechaActual(char *buffer, int buffer_size); //Generada con IA
//Funcion para guardar
void aniadirTrasteroAlquilado(sqlite3 *db, Trastero t,Usuario u);
//Funcion para guardar el trastero una vez se ha devuelto en la tabla HISTORIAL
void guardarTrasteroEnHistorial(sqlite3 *db, Trastero t);
//Funciones de extraccion de datos
Usuario obtenerUsuario(sqlite3 *db,int dni);
Trastero obtenerTrastero(sqlite3 *db,int numTrastero);
ListaTrasteros obtenerListaTrasterosAlquilados(sqlite3 *db);

//Funcion para borrar toda la base de datos (provisional)
void borrarBBDD(sqlite3*db);
#endif /* BBDD_H_ */
