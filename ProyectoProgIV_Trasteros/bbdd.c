//#include "bbdd.h"
//#include <stdio.h>
#include "sqlite3.h"
//#include <time.h>
//int inicializarBBDD(sqlite3 **db){
//	int result;
//
//	result = sqlite3_open(NOMBRE_BBDD, db);
//	return result;
//}
//
//void crearTablas(sqlite3 *db){
//	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
//	char sql[100];
//	sprintf(sql, "CREATE TABLE IF NOT EXISTS Usuario "
//			"(nombre varchar(30),apellido varchar(30),dni int,telefono int, "
//			"email varchar(30), direccion varchar(30),contrasenia varchar(30))");
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//	sprintf(sql, "CREATE TABLE IF NOT EXISTS Trastero "
//				"(numeroTrastero int,metrosCuadrados int, precio float, valoracion int, disponible TINYINT(1))");//TINYINT(1) = boolean (1=true)
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//
//	sprintf(sql, "CREATE TABLE IF NOT EXISTS Historial "
//					"(numeroTrastero int, dni int, valoracion int,diaInicio varchar(20),diaFinal varchar(20))");
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//	sprintf(sql, "CREATE TABLE IF NOT EXISTS Alquilados "
//						"(numeroTrastero int, dni int, valoracion int,diaInicio varchar(20))");
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//
//}
//void aniadirUsuarioABBDD(sqlite3 *db, Usuario u){
//	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
//	char sql[100];
//	sprintf(sql, "INSERT INTO Usuario VALUES('%s','%s',%i,%i,'%s','%s','%s')",
//				u.nombre,u.apellido,u.dni,u.telefono,u.email,u.direccion,u.contrasenia);
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//}
//void aniadirTrasteroABBDD(sqlite3 *db, Trastero t){
//	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
//	char sql[100];
//	sprintf(sql, "INSERT INTO Trastero VALUES(%i,%i,%.2f,%i,%i)",
//				t.numeroTrastero,t.metrosCuadrados,t.precio,t.valoracion,t.disponible);
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//}
//void modificarDisponibilidadTrasteroABBDD(sqlite3 *db, Trastero t){
//	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
//	char sql[100];
//	sprintf(sql, "UPDATE Trastero SET DISPONIBLE=%i WHERE numeroTrastero=%i",t.disponible,t.numeroTrastero);
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//}
//void borrarBBDD(sqlite3 *db){
//	sqlite3_stmt *stmt; //Acceso a ejecuci�n de sentencias
//	char sql[100];
//	sprintf(sql, "DROP TABLE IF EXISTS Usuario");
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//	sprintf(sql, "DROP TABLE IF EXISTS Trastero");
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//	sprintf(sql, "DROP TABLE IF EXISTS Alquilados");
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//}
//
//// Función para obtener la fecha actual en formato "YYYY-MM-DD"
////Generada con Chat-GPT
//void obtenerFechaActual(char *buffer, int buffer_size) {
//    time_t t = time(NULL);
//    struct tm tm = *localtime(&t);
//    strftime(buffer, buffer_size, "%Y-%m-%d", &tm);
//}
//void obtenerFechaActual(char *buffer, int buffer_size) {
//    time_t t = time(NULL);
//    struct tm tm = *localtime(&t);
//    strftime(buffer, buffer_size, "%Y-%m-%d", &tm);
//}
//
//void aniadirTrasteroAlquilado(sqlite3 *db, Trastero t, Usuario u){
//	 sqlite3_stmt *stmt;
//	char sql[100];
//	char fecha[11]; // Buffer para la fecha
//
//	obtenerFechaActual(fecha, sizeof(fecha)); // Obtener la fecha actual
//
//	// Construir la consulta SQL con sprintf
//	sprintf(sql, "INSERT INTO Alquilados (numeroTrastero, dni, valoracion, diaInicio) VALUES (%i, %i, %i, '%s')",
//			t.numeroTrastero, u.dni, t.valoracion, fecha);
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//}
//
//void guardarTrasteroEnHistorial(sqlite3 *db, Trastero t) {
//    sqlite3_stmt *stmt;
//    char sql[200];
//    char fechaDevolucion[11];
//
//    obtenerFechaActual(fechaDevolucion, sizeof(fechaDevolucion)); // Obtener la fecha actual como fecha de devolución
//
//    // Construir la consulta SQL para mover el trastero a Historial con la fecha de devolucion
//    sprintf(sql, "INSERT INTO Historial (numeroTrastero, dni, valoracion, diaInicio, diaFinal) "
//                 "SELECT numeroTrastero, dni, valoracion, diaInicio, '%s' FROM Alquilados "
//                 "WHERE numeroTrastero = %i", fechaDevolucion, t.numeroTrastero);
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//
//    // Eliminar el trastero de la tabla Alquilados ya que se ha devuelto
//    sprintf(sql, "DELETE FROM Alquilados WHERE numeroTrastero = %i", t.numeroTrastero);
//	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
//	sqlite3_step(stmt); //Ejecutar la sentencia
//	sqlite3_finalize(stmt); //Cerrar la sentencia
//
//}
///** TODO: Verificar que se han realizado todas las funciones de insercion a la bd necesarias
// * 		Realizar funciones para obtener informacion de la bd (obtener Trastero, obtener Usuario ...)
// */
