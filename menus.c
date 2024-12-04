//  This file would contain all the menus for the users. main menu, admin menu and the receptionist menu.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "admin.h"
#include "doctor.h"
#include "receptionist.h"
#include "db.h"


void display_main_menu() {
    printf("\n--- Hospital Management System ---\n");
    printf("1. Admin Login\n");
    printf("2. Doctor Login\n");
    printf("3. Visitor Login\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void admin_menu(PGconn* conn) {
    int choice;
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Manage Doctors\n");
        printf("2. Manage Patients\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                manage_doctors(conn);
                break;
            case 2:
                manage_patients(conn);
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void doctor_menu(PGconn* conn, int doctor_id) {
    int choice;
    while (1) {
        printf("\n--- Doctor Menu ---\n");
        printf("1. View Today's Appointments\n");
        printf("2. View Next Appointment\n");
        printf("3. Update Appointment Status\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_today_appointments(conn, doctor_id);
                break;
            case 2:
                view_next_appointment(conn, doctor_id);
                break;
            case 3:
                update_appointment_status(conn, doctor_id);
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void receptionist_menu(PGconn* conn) {
    int choice;
    char name[50];
    printf("Enter your Name: ");
    scanf("%s", name);

    while (1) {
        printf("\n--- Receptionist Menu ---\n");
        printf("1. Manage Patients\n");
        printf("2. Register Visitor\n");
        printf("3. Check-Out\n");
        printf("4. View Visit History\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                manage_patients(conn);
            case 2:
                register_visitor(conn);
                break;
            case 3:
                check_out_visitor(conn);
                break;
            case 4:
                view_visitor_history(conn, name);
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}