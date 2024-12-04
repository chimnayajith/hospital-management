#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "menus.h"
#include "db.h"

// authentication using username and password
void authenticate_user(PGconn* conn) {
    char username[50], password[50];
    char query[256];
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    snprintf(query, sizeof(query), "SELECT user_id, password, role FROM users WHERE username = '%s';", username);
    PGresult* res = PQexec(conn, query);

    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        const char* user_id = PQgetvalue(res, 0, 0);
        const char* stored_pass = PQgetvalue(res, 0, 1);
        const char* role = PQgetvalue(res, 0, 2);

        if (strcmp(password, stored_pass) == 0) {
            printf("Login successful! Welcome, %s.\n", username);

            if (strcmp(role, "Admin") == 0) {
                admin_menu(conn);
            } else if (strcmp(role, "Doctor") == 0) {
                doctor_menu(conn, atoi(user_id));
            } else if (strcmp(role, "Receptionist") == 0) {
                receptionist_menu(conn);
            } else {
                printf("tf is this role ;-; error ig.\n");
            }
        } else {
            printf("Wrong password lol, negative aura fr!\n");
        }
    } else {
        printf("Hold up! You ain't registered! \n");
    }
    PQclear(res);
}

int main() {
    PGconn* conn = connect_to_database();
    initialize_database(conn);

    authenticate_user(conn);
}
