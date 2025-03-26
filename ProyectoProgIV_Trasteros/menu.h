#ifndef MENU_H_
#define MENU_H_
#include "usuario.h"

char mostrarMenuPrincipal();
char menuIniReg();
void menuCliente(Usuario u);
void menuPerfil(Usuario u);
char menuAdministrador();
int autenticarAdministrador();
int registrarUsuario();
int autenticarUsuario();
void manejarCliente();
void manejarAdminiistrador();
#endif /* MENU_H_ */
