#include <stdio.h>
#include <stdlib.h>

void clearScreen();
void customerMenu();
void ownerMenu();
void flightMenu();
void passengerMenu();

void addFlight(), removeFlight(), editFlight(), viewFlights();
void addPassenger(), removePassenger(), editPassenger(), viewPassengers();

int main() {
    int choice;

    do {
        clearScreen();

        printf("==================================================\n");
        printf("          WELCOME TO FLIGHT RESERVATION          \n");
        printf("==================================================\n");
        printf("1. Customer\n");
        printf("2. Owner\n");
        printf("3. Exit\n");
        printf("==================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                customerMenu();
                break;
            case 2:
                ownerMenu();
                break;
            case 3:
                printf("\nThank you for using the Flight Reservation System!\n\n");
                break;
            default:
                printf("\nInvalid choice. Press Enter to try again...");
                getchar(); getchar();
        }
    } while (choice != 3);

    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void customerMenu() {
    clearScreen();
    printf("==================================================\n");
    printf("                  CUSTOMER MENU                   \n");
    printf("==================================================\n");
    printf("Press Enter to return to main menu...");
    getchar(); getchar();
}

void ownerMenu() {
    int choice;

    do {
        clearScreen();
        printf("==================================================\n");
        printf("                    OWNER MENU                    \n");
        printf("==================================================\n");
        printf("1. Flights\n");
        printf("2. Passengers\n");
        printf("3. Back to Main Menu\n");
        printf("==================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: flightMenu(); break;
            case 2: passengerMenu(); break;
            case 3: break;
            default:
                printf("\nInvalid choice. Press Enter to try again...");
                getchar(); getchar();
        }
    } while (choice != 3);
}

void flightMenu() {
    int choice;

    do {
        clearScreen();
        printf("==================================================\n");
        printf("                   FLIGHT MENU                    \n");
        printf("==================================================\n");
        printf("1. Add Flight\n");
        printf("2. Remove Flight\n");
        printf("3. Edit Flight\n");
        printf("4. View Flights\n");
        printf("5. Back to Owner Menu\n");
        printf("==================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addFlight(); break;
            case 2: removeFlight(); break;
            case 3: editFlight(); break;
            case 4: viewFlights(); break;
            case 5: break;
            default:
                printf("\nInvalid choice. Press Enter to try again...");
                getchar(); getchar();
        }
    } while (choice != 5);
}

void passengerMenu() {
    int choice;

    do {
        clearScreen();
        printf("==================================================\n");
        printf("                 PASSENGER MENU                   \n");
        printf("==================================================\n");
        printf("1. Add Passenger\n");
        printf("2. Remove Passenger\n");
        printf("3. Edit Passenger\n");
        printf("4. View Passengers\n");
        printf("5. Back to Owner Menu\n");
        printf("==================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addPassenger(); break;
            case 2: removePassenger(); break;
            case 3: editPassenger(); break;
            case 4: viewPassengers(); break;
            case 5: break;
            default:
                printf("\nInvalid choice. Press Enter to try again...");
                getchar(); getchar();
        }
    } while (choice != 5);
}

// Owner flight functions
void addFlight() {
    clearScreen();
    printf("==================================================\n");
    printf("                   ADD FLIGHT                     \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void removeFlight() {
    clearScreen();
    printf("==================================================\n");
    printf("                 REMOVE FLIGHT                    \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void editFlight() {
    clearScreen();
    printf("==================================================\n");
    printf("                  EDIT FLIGHT                     \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void viewFlights() {
    clearScreen();
    printf("==================================================\n");
    printf("                 VIEW FLIGHTS                     \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

// Owner passenger functions
void addPassenger() {
    clearScreen();
    printf("==================================================\n");
    printf("                  ADD PASSENGER                     \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void removePassenger() {
    clearScreen();
    printf("==================================================\n");
    printf("                REMOVE PASSENGER                    \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void editPassenger() {
    clearScreen();
    printf("==================================================\n");
    printf("                  EDIT PASSENGER                     \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void viewPassengers() {
    clearScreen();
    printf("==================================================\n");
    printf("                  VIEW PASSENGERS                     \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}
