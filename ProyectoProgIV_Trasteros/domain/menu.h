#ifndef MENU_H_
#define MENU_H_
#include "usuario.h"
#include "trastero.h"

void limpiarConsola();
char mostrarMenuPrincipal();
char menuIniReg();
void menuCliente(Usuario u);
void menuCatalogo(Usuario u,ListaTrasteros *lt);
void menuPerfil(Usuario u);
void menuAlquilarTrastero(Usuario u);


char menuAdministrador();
int autenticarAdministrador();
Trastero menuAniadirTrastero();
int menuEliminarTrastero();
char menuTrasterosAdmin();
//Cambiar metodo
int registrarUsuario();
int autenticarUsuario();
void manejarCliente();
void manejarAdminiistrador();
#endif /* MENU_H_ */
