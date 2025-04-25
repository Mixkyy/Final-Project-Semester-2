#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ========================== STRUCT ==========================
typedef struct {
    int flightID;
    char departure[10];
    char destination[10];
    char flight_date[20];
    char flight_time[10];
    float price;
    int capacity;
    int seatsAvailable;
} Flight;

// ===================== FUNCTION DECLARATIONS =====================
void clearScreen();
void customerMenu();
void ownerMenu();
void flightMenu();
void passengerMenu();

void addFlight(), removeFlight(), editFlight(), viewFlights();
void addPassenger(), removePassenger(), editPassenger(), viewPassengers();

// ===================== DATE AND TIME CHECKER  =====================
int isValidDate(const char *date) {
    int y, m, d;
    if (sscanf(date, "%4d-%2d-%2d", &y, &m, &d) != 3)
        return 0;
    if (y < 2024 || m < 1 || m > 12 || d < 1 || d > 31)
        return 0;

    // Days per month check
    int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (m == 2) {
        int isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (d > (isLeap ? 29 : 28)) return 0;
    } else if (d > daysInMonth[m - 1]) {
        return 0;
    }
    return 1;
}

int isValidTime(const char *time) {
    int h, m;
    if (sscanf(time, "%2d:%2d", &h, &m) != 2)
        return 0;
    if (h < 0 || h > 23 || m < 0 || m > 59)
        return 0;
    return 1;
}
// =========================== MAIN ===========================
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

// ===================== SCREEN CLEAR =====================
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// ===================== CUSTOMER MENU =====================
void customerMenu() {
    clearScreen();
    printf("==================================================\n");
    printf("                  CUSTOMER MENU                   \n");
    printf("==================================================\n");
    printf("Press Enter to return to main menu...");
    getchar(); getchar();
}

// ===================== OWNER MENU =====================
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

// ===================== FLIGHT MENU =====================
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

// ===================== PASSENGER MENU =====================
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

// ===================== FLIGHT FUNCTIONS =====================
void addFlight() {
    clearScreen();
    printf("==================================================\n");
    printf("                   ADD FLIGHT                     \n");
    printf("==================================================\n");

    Flight f;
    const char *filename = "flights.csv";

    printf("Enter Flight ID: ");
    scanf("%d", &f.flightID);
    printf("Enter Departure Airport (Airport Code): ");
    scanf("%s", f.departure);
    printf("Enter Destination Airport (Airport Code): ");
    scanf("%s", f.destination);
    do {
        printf("Enter Date (YYYY-MM-DD): ");
        scanf("%s", f.flight_date);
        if (!isValidDate(f.flight_date))
            printf("Invalid date format or value. Try again.\n");
    } while (!isValidDate(f.flight_date));

    // Validate time
    do {
        printf("Enter Departure Time (HH:MM): ");
        scanf("%s", f.flight_time);
        if (!isValidTime(f.flight_time))
            printf("Invalid time format or value. Try again.\n");
    } while (!isValidTime(f.flight_time));
    
    do {
        printf("Enter Price: ");
        scanf("%f", &f.price);
        if (f.price <= 0)
            printf("Price must be a positive number. Try again.\n");
    } while (f.price <= 0);
    
    do {
        printf("Enter Capacity: ");
        scanf("%d", &f.capacity);
        if (f.capacity <= 0)
            printf("Capacity must be a positive number. Try again.\n");
    } while (f.capacity <= 0);

    f.seatsAvailable = f.capacity;

    // Check if file exists
    FILE *file = fopen(filename, "r");
    int fileExists = (file != NULL);
    if (file) fclose(file);

    file = fopen(filename, "a");
    if (!file) {
        printf("Error opening %s\n", filename);
        getchar(); getchar();
        return;
    }

    // Write headers if file is newly created
    if (!fileExists) {
        fprintf(file, "FlightID,departure,destination,flight_date,flight_time,Price,Capacity,SeatsAvailable\n");
    }

    fprintf(file, "%d,%s,%s,%s,%s,%.2f,%d,%d\n",
        f.flightID, f.departure, f.destination, f.flight_date, f.flight_time, f.price, f.capacity, f.seatsAvailable);

    fclose(file);
    printf("\nFlight added successfully!\n");
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

    FILE *file = fopen("flights.csv", "r");
    if (!file) {
        printf("No flight data found. Press Enter to return...");
        getchar(); getchar();
        return;
    }

    char line[256];
    int count = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    printf("%-10s %-10s %-10s %-12s %-8s %-12s %-10s %-10s\n",
        "FlightID", "From", "To", "Date", "Time", "Price", "Capacity", "Available");
    printf("-----------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        int id, capacity, available;
        char from[10], to[10], date[20], time[10];
        float price;

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%f,%d,%d",
                   &id, from, to, date, time, &price, &capacity, &available) == 8) {
            printf("%-10d %-10s %-10s %-12s %-8s %-12.2f %-10d %-10d\n",
                   id, from, to, date, time, price, capacity, available);
            count++;
        }
    }
    printf("-----------------------------------------------------------------------------------------\n");
    fclose(file);

    if (count == 0) {
        printf("No flights available.\n");
    }
   
    printf("Press Enter to return...");
    getchar(); getchar();
}

// ===================== PASSENGER FUNCTIONS =====================
void addPassenger() {
    clearScreen();
    printf("==================================================\n");
    printf("                 ADD PASSENGER                    \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void removePassenger() {
    clearScreen();
    printf("==================================================\n");
    printf("               REMOVE PASSENGER                   \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void editPassenger() {
    clearScreen();
    printf("==================================================\n");
    printf("                 EDIT PASSENGER                   \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}

void viewPassengers() {
    clearScreen();
    printf("==================================================\n");
    printf("                VIEW PASSENGERS                   \n");
    printf("==================================================\n");
    printf("Press Enter to return...");
    getchar(); getchar();
}
