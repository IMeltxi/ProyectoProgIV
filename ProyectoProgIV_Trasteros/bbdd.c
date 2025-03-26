#include "bbdd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>


int inicializarBBDD(sqlite3 **db) {
    int result = sqlite3_open(NOMBRE_BBDD, db);
    if (result != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        fflush(stdout);
    } else {
        printf("Base de datos abierta correctamente.\n");
        fflush(stdout);
    }
    return result;
}

void crearTablas(sqlite3 *db){
	printf("Crear tablas abierto\n");fflush(stdout);
	sqlite3_stmt *stmt;
	char sql[300];
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Usuario "
			"(nombre varchar(30), apellido varchar(30), dni int, telefono int, "
			"email varchar(30), direccion varchar(30), contrasenia varchar(30))");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

	//TABLA TRASTERO
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Trastero "
				"(numeroTrastero int,metrosCuadrados int, precio float, valoracion int, disponible TINYINT(1))");//TINYINT(1) = boolean (1=true)
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

	//TABLA HISTORIAL (HISTORIAL DE LOS TRASTEROS RENTADOS)
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Historial "
					"(numeroTrastero int, dni int, valoracion int,diaInicio varchar(20),diaFinal varchar(20))");
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

	//TABLA DE TRASTEROS ACTUALMENTE ALQUILADOS
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Alquilados "
						"(numeroTrastero int, dni int, valoracion int,diaInicio varchar(20))");
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
	sprintf(sql, "INSERT INTO Trastero VALUES(%i,%i,%.2f,%i,%i)",
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
// Función para obtener la fecha actual en formato "YYYY-MM-DD"
//Generada con Chat-GPT
void obtenerFechaActual(char *buffer, int buffer_size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(buffer, buffer_size, "%Y-%m-%d", &tm);
}

void aniadirTrasteroAlquilado(sqlite3 *db, Trastero t, Usuario u){
	sqlite3_stmt *stmt;
	char sql[100];
	char fecha[11]; // Buffer para la fecha

	obtenerFechaActual(fecha, sizeof(fecha)); // Obtener la fecha actual

	// Construir la consulta SQL con sprintf
	sprintf(sql, "INSERT INTO Alquilados (numeroTrastero, dni, valoracion, diaInicio) VALUES (%i, %i, %i, '%s')",
			t.numeroTrastero, u.dni, t.valoracion, fecha);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
}
//Funcion que al devolver el trastero se guarde la operacion de quiern y que a reservao
void guardarTrasteroEnHistorial(sqlite3 *db, Trastero t) {
    sqlite3_stmt *stmt;
    char sql[200];
    char fechaDevolucion[11];

    obtenerFechaActual(fechaDevolucion, sizeof(fechaDevolucion)); // Obtener la fecha actual como fecha de devolución

    // Construir la consulta SQL para mover el trastero a Historial con la fecha de devolucion
    sprintf(sql, "INSERT INTO Historial (numeroTrastero, dni, valoracion, diaInicio, diaFinal) "
                 "SELECT numeroTrastero, dni, valoracion, diaInicio, '%s' FROM Alquilados "
                 "WHERE numeroTrastero = %i", fechaDevolucion, t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

    // Eliminar el trastero de la tabla Alquilados ya que se ha devuelto
    sprintf(sql, "DELETE FROM Alquilados WHERE numeroTrastero = %i", t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia

}
Usuario obtenerUsuario(sqlite3 *db, int dni) {
    sqlite3_stmt *stmt;
    char sql[100];
    Usuario u = {0}; // Inicializamos el usuario a valores por defecto

    sprintf(sql, "SELECT nombre, apellido, telefono, email, direccion, contrasenia FROM Usuario WHERE dni = %d", dni);

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(u.nombre, (const char*)sqlite3_column_text(stmt, 0));
        strcpy(u.apellido, (const char*)sqlite3_column_text(stmt, 1));
        u.dni = dni;
        u.telefono = sqlite3_column_int(stmt, 2);
        strcpy(u.email, (const char*)sqlite3_column_text(stmt, 3));
        strcpy(u.direccion, (const char*)sqlite3_column_text(stmt, 4));
        strcpy(u.contrasenia, (const char*)sqlite3_column_text(stmt, 5));
    }

    sqlite3_finalize(stmt);
    return u;
}
int usuarioRegistrado(sqlite3 *db,int dni){
	sqlite3_stmt *stmt;
	char sql[100];
	int result;
	sprintf(sql, "SELECT COUNT(*) FROM Usuario WHERE dni = %d", dni);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	result = sqlite3_column_int(stmt, 0);
	if(result!=0){
		return 1;//Esta registrado
	}else{
		return 0;//No esta registrado
	}
	sqlite3_finalize(stmt);
	return result;
}
Trastero buscarTrasteroDDBB(sqlite3 *db, int numeroTrastero){
    sqlite3_stmt *stmt;
    char sql[100];
    Trastero t = {0};  // Inicializamos la estructura Trastero con valores por defecto.


    sprintf(sql, "SELECT numeroTrastero, metrosCuadrados, precio, valoracion, disponible FROM Trastero WHERE numeroTrastero = %i",numeroTrastero);
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // Ejecutamos la consulta
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Si encontramos el trastero, llenamos la estructura Trastero
        t.numeroTrastero = sqlite3_column_int(stmt, 0);
        t.metrosCuadrados = sqlite3_column_int(stmt, 1);
        t.precio = sqlite3_column_double(stmt, 2);
        t.valoracion = sqlite3_column_int(stmt, 3);
        t.disponible = sqlite3_column_int(stmt, 4);
    } else {
        // Si no se encuentra el trastero, dejamos la estructura Trastero con valores por defecto (0).
        printf("Trastero no encontrado con el número: %d\n", numeroTrastero);
    }

    // Finalizamos la sentencia SQL
    sqlite3_finalize(stmt);

    return t;
}
//TODO: Modificar la funcion para que cumpla con su nombre
void obtenerListaTrasterosAlquiladosCSV(sqlite3 *db) {
    sqlite3_stmt *stmt;
    char sql[100];
    int numeroTrastero, cantidad,i=0;
    Trastero t;

    // Contamos el numero de trasteros
    sprintf(sql, "SELECT COUNT(*) FROM Alquilados");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cantidad = sqlite3_column_int(stmt, 0);
    } else {
        cantidad = 0;
    }
    sqlite3_finalize(stmt);

    sprintf(sql, "SELECT numeroTrastero FROM Alquilados");
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW && i < cantidad) {
        numeroTrastero = sqlite3_column_int(stmt, 0);
        t=buscarTrasteroDDBB(NOMBRE_BBDD,numeroTrastero);
		visualizarTrastero(t);
        i++;
    }
    sqlite3_finalize(stmt);
}



