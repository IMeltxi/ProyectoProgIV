#include "bbdd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>



int inicializarBBDD(sqlite3 **db) {
    int result = sqlite3_open(NOMBRE_BBDD, db);

    return result;
}

void crearTablas(sqlite3 *db){
	sqlite3_stmt *stmt1,*stmt2,*stmt3,*stmt4;
	char sql[300];
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Usuario "
			"(nombre varchar(30), apellido varchar(30), dni int, telefono int, "
			"email varchar(30), direccion varchar(30), contrasenia varchar(30))");
	sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL); //Preparar la sentencia
	sqlite3_step(stmt1); //Ejecutar la sentencia

	sqlite3_finalize(stmt1); //Cerrar la sentencia

	//TABLA TRASTERO
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Trastero "
				"(numeroTrastero int,metrosCuadrados int, precio float, valoracion float,numeroValoraciones int, disponible TINYINT(1))");//TINYINT(1) = boolean (1=true)
	sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL); //Preparar la sentencia
	sqlite3_step(stmt2); //Ejecutar la sentencia

	sqlite3_finalize(stmt2); //Cerrar la sentencia

	//TABLA HISTORIAL (HISTORIAL DE LOS TRASTEROS RENTADOS)
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Historial "
					"(numeroTrastero int, dni int, valoracion float, numeroValoraciones int, diaInicio varchar(20), diaFinal varchar(20))");
	sqlite3_prepare_v2(db, sql, -1, &stmt3, NULL); //Preparar la sentencia
	sqlite3_step(stmt3); //Ejecutar la sentencia

	sqlite3_finalize(stmt3); //Cerrar la sentencia

	//TABLA DE TRASTEROS ACTUALMENTE ALQUILADOS
	sprintf(sql, "CREATE TABLE IF NOT EXISTS Alquilados "
						"(numeroTrastero int, dni int, valoracion float, numeroValoraciones int, diaInicio varchar(20))");
	sqlite3_prepare_v2(db, sql, -1, &stmt4, NULL); //Preparar la sentencia
	sqlite3_step(stmt4); //Ejecutar la sentencia

	sqlite3_finalize(stmt4); //Cerrar la sentencia


}
int aniadirUsuarioABBDD(sqlite3 *db, Usuario u){
	sqlite3_stmt *stmt; // Acceso a ejecución de sentencias
	int result=0;
	    char sql[300]; // Aumenta el tamaño del buffer para la sentencia SQL
	    sprintf(sql, "INSERT INTO Usuario (nombre, apellido, dni, telefono, email, direccion, contrasenia) "
	                 "VALUES('%s','%s',%i,%i,'%s','%s','%s')",
	            u.nombre, u.apellido, u.dni, u.telefono, u.email, u.direccion, u.contrasenia);

	    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); // Preparar la sentencia
	    if (rc != SQLITE_OK) {
//	        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
	        sqlite3_close(db);

	        return 0;
	    }

	    rc = sqlite3_step(stmt); // Ejecutar la sentencia
	    if (rc != SQLITE_DONE) {
//	        printf("Error al insertar datos: %s\n", sqlite3_errmsg(db));

	        sqlite3_close(db);
	        return  0;

	    } else {
//	        printf("Usuario añadido correctamente.\n");
	    	result= 1;
	    }

	    sqlite3_finalize(stmt); // Cerrar la sentencia
		return result;
}
int aniadirTrasteroABBDD(sqlite3 *db, Trastero t){
    int result;
    sqlite3_stmt *stmt1, *stmt2;
    char sql[500];
    int totalTrasteros = 0; // Inicializar

    sprintf(sql, "SELECT COUNT(*) FROM Trastero");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt1) == SQLITE_ROW) {
            totalTrasteros = sqlite3_column_int(stmt1, 0);
        }
    }
    sqlite3_finalize(stmt1);

    if (totalTrasteros < 100) {
        sprintf(sql, "INSERT INTO Trastero (numeroTrastero, metrosCuadrados, precio, valoracion, numeroDeValoraciones, disponible)"
                " VALUES (%d, %d, %.2f, %.2f, %d, %d)",
                t.numeroTrastero, t.metrosCuadrados, t.precio, t.valoracion, t.numeroDeValoraciones, t.disponible);

        if (sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt2) == SQLITE_DONE) { // Verificar que se ejecutó correctamente
                sqlite3_finalize(stmt2);
                return 1; // Éxito
            } else {
                sqlite3_finalize(stmt2);
                return 0; // Error en la ejecución
            }
        } else {
            return 0; // Error en la preparación
        }
    } else {
        return 2; // Sin espacio
    }
}// Función para obtener la fecha actual en formato "YYYY-MM-DD"
//Generada con Chat-GPT
void obtenerFechaActual(char *buffer, int buffer_size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(buffer, buffer_size, "%Y-%m-%d", &tm);
}

void aniadirTrasteroAlquilado(sqlite3 *db, Trastero t, Usuario u){
	sqlite3_stmt *stmt1,*stmt2;
	char sql[500];
	char fecha[11]; // Buffer para la fecha

	obtenerFechaActual(fecha, sizeof(fecha)); // Obtener la fecha actual

	sprintf(sql, "INSERT INTO Alquilados (numeroTrastero, dni, valoracion,numeroValoraciones, diaInicio) VALUES (%i, %i, %.2f,%d,'%s')",
			t.numeroTrastero, u.dni, t.valoracion,t.numeroDeValoraciones, fecha);
	sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL); //Preparar la sentencia
	sqlite3_step(stmt1); //Ejecutar la sentencia
	sqlite3_finalize(stmt1); //Cerrar la sentencia

	// Construir la consulta SQL con sprintf
	sprintf(sql, "UPDATE Trastero SET disponible = 0 WHERE numeroTrastero = %i", t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL); //Preparar la sentencia
	sqlite3_step(stmt2); //Ejecutar la sentencia
	sqlite3_finalize(stmt2); //Cerrar la sentencia
}
//Funcion que al devolver el trastero se guarde la operacion de quiern y que a reservado
void devolverTrasteroBBDD(sqlite3 *db, Trastero t) {
    sqlite3_stmt *stmt1,*stmt2,*stmt3;
    char sql[500];
    char fechaDevolucion[11];

    obtenerFechaActual(fechaDevolucion, sizeof(fechaDevolucion)); // Obtener la fecha actual como fecha de devolución

    // Construir la consulta SQL para mover el trastero a Historial con la fecha de devolucion
    sprintf(sql, "INSERT INTO Historial (numeroTrastero, dni, valoracion,numeroValoraciones, diaInicio, diaFinal) "
                 "SELECT numeroTrastero, dni, valoracion,numeroValoraciones, diaInicio, '%s' FROM Alquilados "
                 "WHERE numeroTrastero = %i", fechaDevolucion, t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL); //Preparar la sentencia
	sqlite3_step(stmt1); //Ejecutar la sentencia
	sqlite3_finalize(stmt1); //Cerrar la sentencia

    // Eliminar el trastero de la tabla Alquilados ya que se ha devuelto
    sprintf(sql, "DELETE FROM Alquilados WHERE numeroTrastero = %i", t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL); //Preparar la sentencia
	sqlite3_step(stmt2); //Ejecutar la sentencia
	sqlite3_finalize(stmt2); //Cerrar la sentencia

	//Modificar el estado del trastero a disponible
	sprintf(sql, "UPDATE Trasteros SET disponible = 1 WHERE numeroTrastero = %i", t.numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt3, NULL); //Preparar la sentencia
	sqlite3_step(stmt3); //Ejecutar la sentencia
	sqlite3_finalize(stmt3); //Cerrar la sentencia

}
Usuario obtenerUsuario(sqlite3 *db, int dni) {
    sqlite3_stmt *stmt;
    char sql[100];
    Usuario u; // Inicializamos el usuario a valores por defecto
    u.dni=-1;
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
	    char sql[500];
	    int result = 0;  // Inicializamos el resultado a 0 (por defecto)

	    // Preparamos la consulta SQL para verificar si el usuario está registrado
	    sprintf(sql, "SELECT COUNT(*) FROM Usuario WHERE dni = %d", dni);
	    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	    if (rc != SQLITE_OK) {
	       // printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
	        return 0;  // Retorna 0 si hay un error al preparar la consulta
	    }

	    // Ejecutamos la consulta
	    rc = sqlite3_step(stmt);
	    if (rc == SQLITE_ROW) {
	        result = sqlite3_column_int(stmt, 0);  // Si hay resultados, obtenemos el conteo
	    } else {
	       // printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
	    }

	    sqlite3_finalize(stmt);  // Siempre finaliza la sentencia después de usarla

	    return result!=0;  // Si el resultado es mayor que 0, el usuario está registrado
}
Trastero buscarTrasteroDDBB(sqlite3 *db, int numeroTrastero){
    sqlite3_stmt *stmt;
    char sql[500];
    Trastero t;  // Inicializamos la estructura Trastero con valores por defecto.
    t.numeroTrastero=-1;

    sprintf(sql, "SELECT numeroTrastero, metrosCuadrados, precio, valoracion, numeroValoraciones, disponible FROM Trastero WHERE numeroTrastero = %i",numeroTrastero);
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // Ejecutamos la consulta
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Si encontramos el trastero, llenamos la estructura Trastero
        t.numeroTrastero = sqlite3_column_int(stmt, 0);
        t.metrosCuadrados = sqlite3_column_int(stmt, 1);
        t.precio = sqlite3_column_double(stmt, 2);
        t.valoracion = sqlite3_column_double(stmt, 3);
        t.numeroDeValoraciones = sqlite3_column_int(stmt, 4);
        t.disponible = sqlite3_column_int(stmt, 5);
    } else {
        // Si no se encuentra el trastero, dejamos la estructura Trastero con valores por defecto (0).
        //printf("Trastero no encontrado con el número: %d\n", numeroTrastero);
    }

    // Finalizamos la sentencia SQL
    sqlite3_finalize(stmt);

    return t;
}
void eliminarTrasteroDDBB(sqlite3 *db,int numeroTrastero){
	sqlite3_stmt *stmt;
	char sql[100];
	sprintf(sql, "DELETE FROM Trasteros WHERE numeroTrastero = %i", numeroTrastero);
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL); //Preparar la sentencia
	sqlite3_step(stmt); //Ejecutar la sentencia
	sqlite3_finalize(stmt); //Cerrar la sentencia
}
int verificarAlquiler(sqlite3 *db, int numeroTrastero, int dni){
		sqlite3_stmt *stmt;
		char sql[150];
		int encontrado = -1;

		// Crear la consulta SQL
		sprintf(sql, "SELECT 1 FROM Alquilados WHERE numeroTrastero = %i AND dni = %i LIMIT 1", numeroTrastero, dni);

		// Preparar la sentencia
		sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

		// Ejecutar la sentencia
		if (sqlite3_step(stmt) == SQLITE_ROW){
			encontrado = 1; // Existe el alquiler
		}

		// Cerrar la sentencia
		sqlite3_finalize(stmt);

		return encontrado;

}
void cargarTrasterosDesdeDB(ListaTrasteros *lt, sqlite3 *db) {
    sqlite3_stmt *stmt;
    char sql[500];
    Trastero t;

    // Consulta SQL para obtener todos los trasteros de la base de datos
    sprintf(sql, "SELECT numeroTrastero, metrosCuadrados, precio, valoracion, numeroValoraciones, disponible FROM Trastero");

    // Preparamos la consulta SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Ejecutamos la consulta y procesamos los resultados
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Obtener los valores de cada columna
        t.numeroTrastero = sqlite3_column_int(stmt, 0);
        t.metrosCuadrados = sqlite3_column_int(stmt, 1);
        t.precio = sqlite3_column_double(stmt, 2);
        t.valoracion = sqlite3_column_double(stmt, 3);
        t.numeroDeValoraciones = sqlite3_column_int(stmt, 4);
        t.disponible = sqlite3_column_int(stmt, 5);

        // Añadir el trastero a la lista
        if (lt->numeroTrasteros < 100) {
            lt->aTrasteros[lt->numeroTrasteros] = t;
            lt->numeroTrasteros++;
        } else {
            printf("No se pueden añadir más trasteros\n");
        }
    }

    // Finalizar la sentencia SQL
    sqlite3_finalize(stmt);
}
void cargarUsuariosDesdeDB(ListaUsuarios *lu, sqlite3 *db) {
    sqlite3_stmt *stmt;
    char sql[500];
    Usuario u;

    // Consulta SQL para obtener todos los usuarios de la base de datos
    sprintf(sql, "SELECT nombre, apellido, dni, telefono, email, direccion, contrasenia FROM Usuario");

    // Preparamos la consulta SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
       // printf("Error al preparar la consulta SQL: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Ejecutamos la consulta y procesamos los resultados
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Obtener los valores de cada columna
        const char *nombre = (const char *)sqlite3_column_text(stmt, 0);
        const char *apellido = (const char *)sqlite3_column_text(stmt, 1);
        u.dni = sqlite3_column_int(stmt, 2);
        u.telefono = sqlite3_column_int(stmt, 3);
        const char *email = (const char *)sqlite3_column_text(stmt, 4);
        const char *direccion = (const char *)sqlite3_column_text(stmt, 5);
        const char *contrasenia = (const char *)sqlite3_column_text(stmt, 6);

        if (nombre && apellido && email && direccion && contrasenia) {
            // Asignar los valores a la estructura Usuario
            strcpy(u.nombre, nombre);
            strcpy(u.apellido, apellido);
            strcpy(u.email, email);
            strcpy(u.direccion, direccion);
            strcpy(u.contrasenia, contrasenia);

            // Redimensionar el array dinámico para agregar el nuevo usuario
            lu->numUsuarios++;
            lu->aUsuarios = realloc(lu->aUsuarios, lu->numUsuarios * sizeof(Usuario));

            if (lu->aUsuarios == NULL) {
                //printf("\033[31mError al asignar memoria\033[0m\n");
                sqlite3_finalize(stmt);
                return;
            }

            // Agregar usuario a la lista
            lu->aUsuarios[lu->numUsuarios - 1] = u;
        }
    }

    // Finalizar la sentencia SQL
    sqlite3_finalize(stmt);
}
int registrarUsuario(sqlite3 *db,char* nombre, char* apellido, char* email, char* direccion, char* contrasena, char* confirmarContrasena, char* telefono, char* dni) {
    int dniComp;
    char c;
    dniComp = atoi(dni);
    if (usuarioRegistrado(db, dniComp) == 1) {
    	return 0;

    } else if (strcmp(contrasena, confirmarContrasena) == 0) {
        Usuario u;
        strcpy(u.nombre, nombre);
        strcpy(u.apellido, apellido);
        u.dni = atoi(dni);
        u.telefono = atoi(telefono);
        strcpy(u.email, email);
        strcpy(u.direccion, direccion);
        strcpy(u.contrasenia, contrasena);

        return aniadirUsuarioABBDD(db, u);

    } else {
//    	printf("\033[1;31mLas contraseñas no coinciden\n\033[0m");
//        fflush(stdout);
    	return 2;
    }
}

void cerrarPrograma(sqlite3 *db, ListaUsuarios *lu, ListaTrasteros *lt) {
    // Liberar la memoria de las listas
	if (lu->aUsuarios != NULL) {
	        free(lu->aUsuarios);
	        lu->aUsuarios = NULL;  // Establecer a NULL después de liberar la memoria
	 }
	 lu->numUsuarios = 0;

    // Cerrar la base de datos
    if (db != NULL) {
        sqlite3_close(db);
       // printf("\033[0;32mBase de datos cerrada correctamente.\033[0m\n");
    }

   // printf("Fin del programa\n");
}
int autenticarUsuario(sqlite3 *db,int dni,char* contrasena) {
		Usuario u;
		u = obtenerUsuario(db,dni);
		if(u.dni!=-1){
			//Usuario encontrado
			if (dni==u.dni && (strcmp(contrasena, u.contrasenia) == 0)) {
				//printf("\033[1;32mAcceso concedido.\n\033[0m");
				return u.dni;
				}
		}
	return -1;
}




