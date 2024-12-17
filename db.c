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
    // initializing the tables and create them if it doesn't exist
    const char* create_tables[] = {
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "username VARCHAR(50) UNIQUE NOT NULL, "
        "email VARCHAR(100) UNIQUE NOT NULL, "
        "password VARCHAR(255) NOT NULL, "
        "role VARCHAR(20) NOT NULL, "
        "street VARCHAR(100) NOT NULL, "
        "city VARCHAR(50) NOT NULL, "
        "state VARCHAR(50) NOT NULL, "
        "zipcode VARCHAR(10) NOT NULL);",

        "CREATE TABLE IF NOT EXISTS doctors ("
        "doctor_id SERIAL PRIMARY KEY, "
        "user_id INT NOT NULL UNIQUE, "
        "specialization VARCHAR(50) NOT NULL, "
        "FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE);",

        "CREATE TABLE IF NOT EXISTS receptionists ("
        "receptionist_id SERIAL PRIMARY KEY, "
        "user_id INT NOT NULL UNIQUE, "
        "shift_time VARCHAR(20) NOT NULL, "
        "FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE);",

        "CREATE TABLE IF NOT EXISTS patients ("
        "patient_id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "room_number INT NOT NULL, "
        "visiting_hours TIME[] NOT NULL);",

        "CREATE TABLE IF NOT EXISTS visitors ("
        "visitor_id SERIAL PRIMARY KEY, "
        "name VARCHAR(50) NOT NULL, "
        "contact VARCHAR(15) NOT NULL, "
        "patient_id INT NOT NULL, "
        "check_in_time TIMESTAMP NOT NULL, "
        "check_out_time TIMESTAMP, "
        "FOREIGN KEY (patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE);",

        "CREATE TABLE IF NOT EXISTS appointments ("
        "appointment_id SERIAL PRIMARY KEY, "
        "patient_id INT NOT NULL, "
        "doctor_id INT NOT NULL, "
        "appointment_time TIMESTAMP NOT NULL, "
        "status VARCHAR(20) DEFAULT 'Scheduled', "
        "FOREIGN KEY (patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE, "
        "FOREIGN KEY (doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE);"
    };

    // iterate through the array and execute each table creation.z
    for (int i = 0; i < 6; i++) {
        PGresult* res = PQexec(conn, create_tables[i]);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "Error initializing database: %s\n", PQerrorMessage(conn));
        }
        PQclear(res);
    }

    printf("Database initialized successfully.\n");
}