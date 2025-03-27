#ifndef MENU_H_
#define MENU_H_
#include "usuario.h"

void limpiarConsola();
char mostrarMenuPrincipal();
char menuIniReg();
void menuCliente(Usuario u);
void menuPerfil(Usuario u);
void menuAlquilarTrastero(Usuario u);
char menuAdministrador();
int autenticarAdministrador();
int registrarUsuario();
int autenticarUsuario();
void manejarCliente();
void manejarAdminiistrador();
#endif /* MENU_H_ */
