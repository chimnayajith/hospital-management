#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

void view_next_appointment(PGconn* conn, int doctor_id) {
    char query[256];
    snprintf(query, sizeof(query), 
             "SELECT * FROM appointments WHERE doctor_id = %d AND appointment_time > NOW() "
             "ORDER BY appointment_time LIMIT 1;", doctor_id);
    
    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        if (PQntuples(res) > 0) {
            printf("Next Appointment ID: %s, Patient ID: %s, Time: %s, Status: %s\n",
                   PQgetvalue(res, 0, 0), PQgetvalue(res, 0, 1), 
                   PQgetvalue(res, 0, 3), PQgetvalue(res, 0, 4));
        } else {
            printf("No upcoming appointments found.\n");
        }
    }
    PQclear(res);
}

void update_appointment_status(PGconn* conn, int doctor_id) {
    int appointment_id;
    char status[20];
    
    printf("Enter Appointment ID to update: ");
    scanf("%d", &appointment_id);
    printf("Enter new status (e.g., Completed, Cancelled): ");
    scanf("%s", status);
    
    char query[256];
    snprintf(query, sizeof(query), 
             "UPDATE appointments SET status = '%s' WHERE appointment_id = %d AND doctor_id = %d;", 
             status, appointment_id, doctor_id);
    
    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error updating appointment status: %s\n", PQerrorMessage(conn));
    } else {
        printf("Appointment status updated successfully.\n");
    }
    PQclear(res);
}


void view_today_appointments(PGconn* conn, int doctor_id) {
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT * FROM appointments WHERE doctor_id = %d AND DATE(appointment_time) = CURRENT_DATE;",
             doctor_id);

    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            printf("Appointment ID: %s, Patient ID: %s, Time: %s, Status: %s\n",
                   PQgetvalue(res, i, 0), PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 3), PQgetvalue(res, i, 4));
        }
    }
    PQclear(res);
}