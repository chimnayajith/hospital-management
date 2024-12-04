#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H

#include <libpq-fe.h>

void manage_patients(PGconn* conn);
void register_visitor(PGconn* conn);
void check_out_visitor(PGconn* conn);
void view_visitor_history(PGconn* conn, const char* visitor_name);

#endif