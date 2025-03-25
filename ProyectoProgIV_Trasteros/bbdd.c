#include "bbdd.h"
#include <stdio.h>
int inicializarBBDD(sqlite3 **db){
	int result;

	result = sqlite3_open(NOMBRE_BBDD, db);
	return result;
}

void crearTablas(sqlite3 *db){
	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
	char sql[100];
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Usuario "
			"(nombre varchar(30),apellido varchar(30),dni int,telefono int, "
			"email varchar(30), direccion varchar(30),contrasenia varchar(30))");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Trastero "
				"(numeroTrastero int,metrosCuadrados int, precio float, valoracion int, disponible TINYINT(1))");//TINYINT(1) = boolean (1=true)
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

	sprintf(sql, "CREATE TABLE IF NOT EXISTS Alquilados "
					"(numeroTrastero int, dni int, valoracion int)");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

}
void aniadirUsuarioABBDD(sqlite3 *db, Usuario u){
	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
	char sql[100];
	sprintf(sql, "INSERT INTO Usuario VALUES('%s','%s',%i,%i,'%s','%s','%s')",
				u.nombre,u.apellido,u.dni,u.telefono,u.email,u.direccion,u.contrasenia);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
}
void aniadirTrasteroABBDD(sqlite3 *db, Trastero t){
	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
	char sql[100];
	sprintf(sql, "INSERT INTO Trastero VALUES(%i,%i,%i,%.2f,%i)",
				t.numeroTrastero,t.metrosCuadrados,t.precio,t.valoracion,t.disponible);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
}
void modificarDisponibilidadTrasteroABBDD(sqlite3 *db, Trastero t){
	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
	char sql[100];
	sprintf(sql, "UPDATE Trastero SET DISPONIBLE=%i WHERE numeroTrastero=%i",t.disponible,t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
}
void borrarBBDD(sqlite3 *db){
	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
	char sql[100];
	sprintf(sql, "DROP TABLE IF EXISTS Usuario");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
	sprintf(sql, "DROP TABLE IF EXISTS Trastero");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
	sprintf(sql, "DROP TABLE IF EXISTS Alquilados");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
}

