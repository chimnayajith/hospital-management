#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

void manage_patients(PGconn* conn) {
    int choice;
    char name[50];
    int room_number;
    char visiting_hours[50];
    
    printf("1. Add Patient\n2. View Patients\nEnter your choice: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        printf("Enter Patient Name: ");
        scanf("%s", name);
        printf("Enter Room Number: ");
        scanf("%d", &room_number);
        printf("Enter Visiting Hours (e.g., {09:00,12:00}): ");
        scanf("%s", visiting_hours);
        
        char query[256];
        snprintf(query, sizeof(query), 
                 "INSERT INTO patients (name, room_number, visiting_hours) VALUES ('%s', %d, '%s');", 
                 name, room_number, visiting_hours);
        
        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error adding patient: %s\n", PQerrorMessage(conn));
        } else {
            printf("Patient added successfully.\n");
        }
        PQclear(res);
    } else if (choice == 2) {
        PGresult* res = PQexec(conn, "SELECT * FROM patients;");
        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                printf("ID: %s, Name: %s, Room: %s, Visiting Hours: %s\n",
                       PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), 
                       PQgetvalue(res, i, 2), PQgetvalue(res, i, 3));
            }
        }
        PQclear(res);
    } else {
        printf("Invalid choice.\n");
    }
}

void register_visitor(PGconn* conn) {
    char name[50], contact[15];
    int patient_id;
    
    printf("Enter Visitor Name: ");
    scanf("%s", name);
    printf("Enter Contact Number: ");
    scanf("%s", contact);
    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);
    
    char query[256];
    snprintf(query, sizeof(query), 
             "INSERT INTO visitors (name, contact, patient_id, check_in_time) "
             "VALUES ('%s', '%s', %d, NOW());", 
             name, contact, patient_id);
    
    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error registering visitor: %s\n", PQerrorMessage(conn));
    } else {
        printf("Visitor registered successfully.\n");
    }
    PQclear(res);
}

void check_out_visitor(PGconn* conn) {
    int visitor_id;
    
    printf("Enter Visitor ID to check out: ");
    scanf("%d", &visitor_id);
    
    char query[256];
    snprintf(query, sizeof(query), 
             "UPDATE visitors SET check_out_time = NOW() WHERE visitor_id = %d AND check_out_time IS NULL;", 
             visitor_id);
    
    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Error checking out visitor: %s\n", PQerrorMessage(conn));
    } else {
        if (PQcmdTuples(res)[0] == '0') {
            printf("No active check-in found for Visitor ID: %d\n", visitor_id);
        } else {
            printf("Visitor checked out successfully.\n");
        }
    }
    PQclear(res);
}
// TODO: Change to table
void view_visitor_history(PGconn* conn, const char* visitor_name) {
    char query[256];
    snprintf(query, sizeof(query), 
             "SELECT * FROM visitors WHERE name = '%s' ORDER BY check_in_time DESC;", 
             visitor_name);
    
    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; i++) {
            printf("Visitor ID: %s, Name: %s, Check-in: %s, Check-out: %s\n",
                   PQgetvalue(res, i, 0), PQgetvalue(res, i, 1),
                   PQgetvalue(res, i, 4), PQgetvalue(res, i, 5));
        }
    }
    PQclear(res);
}
