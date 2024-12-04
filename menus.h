#ifndef MENUS_H
#define MENUS_H

#include <libpq-fe.h>

void display_main_menu();
void admin_menu(PGconn* conn);
void doctor_menu(PGconn* conn, int doctor_id);
void receptionist_menu(PGconn* conn);

#endif