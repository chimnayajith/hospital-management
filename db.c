#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

PGconn* connect_to_database() {
    char *conninfo = "dbname=mydatabase user=chimnayyyy password=chimnayyyy host=localhost";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    return conn;
}

void close_connection(PGconn* conn) {
    PQfinish(conn);
// ddl commands
// TODO: Gives an useless output when the table already exists, ( try and get rid of it )

//  chimnayyyy@archie  ~/Study/c/hvm  ./hvm
// NOTICE:  relation "users" already exists, skipping
// NOTICE:  relation "doctors" already exists, skipping
// NOTICE:  relation "receptionists" already exists, skipping
// NOTICE:  relation "patients" already exists, skipping
// NOTICE:  relation "visitors" already exists, skipping
// NOTICE:  relation "appointments" already exists, skipping
// Database initialized successfully.
void initialize_database(PGconn* conn) {
    const char* create_tables[] = {
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "username VARCHAR(50) UNIQUE NOT NULL, "
        "email VARCHAR(100) UNIQUE NOT NULL, "
        "password VARCHAR(255) NOT NULL, "
        "role VARCHAR(20) NOT NULL);",

        "CREATE TABLE IF NOT EXISTS doctors ("
        "specialization VARCHAR(50) NOT NULL, "
        "PRIMARY KEY (user_id)) "
        "INHERITS (users);",

        "CREATE TABLE IF NOT EXISTS receptionists ("
        "shift_time VARCHAR(20) NOT NULL, "
        "PRIMARY KEY (user_id)) "
        "INHERITS (users);",

        "CREATE TABLE IF NOT EXISTS patients ("
        "patient_id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "room_number INT NOT NULL, "
        "visiting_hours TIME[] NOT NULL);",

        "CREATE TABLE IF NOT EXISTS visitors ("
        "visitor_id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "contact VARCHAR(15) NOT NULL, "
        "patient_id INT REFERENCES patients(patient_id), "
        "check_in_time TIMESTAMP NOT NULL, "
        "check_out_time TIMESTAMP);",

        "CREATE TABLE IF NOT EXISTS appointments ("
        "appointment_id SERIAL PRIMARY KEY, "
        "patient_id INT REFERENCES patients(patient_id), "
        "doctor_id INT REFERENCES doctors(user_id), "
        "appointment_time TIMESTAMP NOT NULL, "
        "status VARCHAR(20) DEFAULT 'Scheduled');"
    };

    // iterate through the array and execute each table creation.
    for (int i = 0; i < 6; i++) {
        PGresult* res = PQexec(conn, create_tables[i]);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error initializing database: %s\n", PQerrorMessage(conn));
        }
        PQclear(res);
    }

    printf("Database initialized successfully.\n");
}