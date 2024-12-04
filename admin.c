#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>


void manage_doctors(PGconn* conn) {
    int choice;
    char name[50], specialization[50], email[100], username[100], password[255], role[20] = "Doctor";

    printf("1. Add Doctor\n2. View Doctors\nEnter your choice: ");
    scanf("%d", &choice);

    // adding new doctors
    if (choice == 1) {
        printf("Enter Doctor Name: ");
        scanf("%s", name);
        printf("Enter Email: ");
        scanf("%s", email);
        printf("Enter Username: ");
        scanf("%s", username); 
        printf("Enter Password: ");
        scanf("%s", password);  //hash using crypt.h (later maybe hehe)
        printf("Enter Specialization: ");
        scanf("%s", specialization);

        char query[256];
        snprintf(query, sizeof(query), "INSERT INTO doctors (name, username, email, password, role, specialization) VALUES ('%s','%s' ,'%s', '%s', '%s', '%s') RETURNING user_id;", name,username, email, password, role, specialization);

        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            fprintf(stderr, "Error adding user: %s\n", PQerrorMessage(conn));
        }
    } else if (choice == 2) {
        PGresult* res = PQexec(conn, "SELECT * FROM doctors;");
        // TODO: Change this into a table too, for now just the id and specialization is being shown. that should be changed too. 
        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                printf("ID: %s, Specialization: %s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1));
            }
        }
        PQclear(res);
    } else {
        printf("Invalid choice.\n");
    }
}

void manage_receptionists(PGconn* conn) {
    int choice;
    char username[50], name[50], email[100], password[50], shift_time[20];
    printf("1. Add Receptionist\n2. View Receptionists\nEnter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Username: ");
        scanf("%s", username);
        printf("Enter Name: ");
        scanf("%[^\n]", name);
        printf("Enter Email: ");
        scanf("%s", email);
        printf("Enter Password: ");
        scanf("%s", password);
        printf("Enter Shift Time: ");
        scanf("%s", shift_time);

        char query[256];
        snprintf(query, sizeof(query),
                 "INSERT INTO receptionists (username, name, email, password, role, shift_time) "
                 "VALUES ('%s', '%s', '%s', '%s', 'Receptionist', '%s');", username, name, email, password, shift_time);

        PGresult* res = PQexec(conn, query);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error adding user: %s\n", PQerrorMessage(conn));
            PQclear(res);
            return;
        } else {
            printf("Receptionist added successfully.\n");
        }
        PQclear(res);

    } else if (choice == 2) {
        //list of receptionist
        // TODO: MAKE IT INTO A TABLE 
        PGresult* res = PQexec(conn, "SELECT u.username, u.name, r.shift_time "
                                      "FROM users u "
                                      "JOIN receptionists r ON u.user_id = r.user_id;");
        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                printf("Username: %s, Name: %s, Shift Time: %s\n",
                       PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
            }
        }
        PQclear(res);
    } else {
        printf("Invalid choice.\n");
    }
}
