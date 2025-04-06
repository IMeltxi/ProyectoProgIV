#ifndef MENU_H_
#define MENU_H_
#include "usuario.h"
#include "trastero.h"
#include "../db/bbdd.h"

void limpiarConsola();
char mostrarMenuPrincipal();
char menuIniReg();
char menuCliente();
char menuCatalogo();
void menuPerfil(Usuario u);
void menuAlquilarTrastero(Usuario u, sqlite3 *db);

char volverAtras();
void valoracionTrasteroDevuelto(Trastero t);
char menuAdministrador();
int autenticarAdministrador();
Trastero menuAniadirTrastero();
int menuEliminarTrastero();
char menuTrasterosAdmin();
char menuFicherosAdmin();
//Cambiar metodo

int autenticarUsuario(sqlite3 *db);

void registrarUsuario(sqlite3 *db);
char manejarCliente(sqlite3 *db);
void cerrarPrograma(sqlite3 *db, ListaUsuarios *lu, ListaTrasteros *lt);
#endif /* MENU_H_ */
