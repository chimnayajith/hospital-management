#ifndef DOCTOR_H
#define DOCTOR_H

#include <libpq-fe.h>

void view_today_appointments(PGconn* conn, int doctor_id);
void view_next_appointment(PGconn* conn, int doctor_id);
void update_appointment_status(PGconn* conn, int doctor_id);

#endif