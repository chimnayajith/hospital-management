#ifndef ADMIN_H
#define ADMIN_H

#include <libpq-fe.h>

void manage_doctors(PGconn* conn);
void manage_receptionist(PGconn* conn);

#endif
