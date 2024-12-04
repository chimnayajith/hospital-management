#ifndef DB_H
#define DB_H

#include <libpq-fe.h>

PGconn* connect_to_database();
void close_connection(PGconn* conn);
void initialize_database(PGconn* conn);

#endif