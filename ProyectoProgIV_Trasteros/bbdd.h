#ifndef BBDD_H_
#define BBDD_H_
#include "sqlite3.h"
#include "usuario.h"
#include "trastero.h"
#define NOMBRE_BBDD "TrasterosDeusto.db"

int inicializarBBDD(sqlite3 **db);
void crearTablas(sqlite3 *db);
void aniadirUsuario(sqlite3 *db, Usuario u);
void aniadirTrasteroABBDD(sqlite3 *db, Trastero t);
void modificarDisponibilidadTrasteroABBDD(sqlite3 *db, Trastero t);
void borrarBBDD(sqlite3*db);
#endif /* BBDD_H_ */
