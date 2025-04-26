#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// ========================== STRUCT ==========================
typedef struct {
    int flightID;
    char departure[10];
    char destination[10];
    char flight_date[20];
    char flight_time[10];
    char airplaneID[10];           
    char airplaneModel[50];         
    int firstClassSeats;           
    int businessClassSeats;        
    int economyClassSeats;    
    float price;
    int capacity;
    int seatsAvailable;
} Flight;

typedef struct {
    char airplaneID[10];          
    char model[50];              
    int totalSeats;           
    int firstClassSeats;       
    int businessClassSeats;   
    int economyClassSeats; 
    char status[20]; 
} Plane;

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

// FLIGHT BOOKING MENU



// ===================== CUSTOMER MENU =====================
void customerMenu() {
    int choice;
    clearScreen();
    printf("==================================================\n");
    printf("                  CUSTOMER MENU                   \n");
    printf("==================================================\n");
    printf("1. Book Flight\n");
    printf("2. View History\n");
    printf("3. Back to Main Menu\n");
    printf("==================================================\n");
    printf("Press Enter to return to main menu...");
    scanf("%d", &choice);
        switch (choice) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                return;
            default:
                printf("\nInvalid choice. Press Enter to try again...");
                getchar(); getchar();

    } while (choice != 3);
    return;

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
    const char *flightFile = "flights.csv";
    const char *planeFile = "plane.csv";
    Plane planes[100];
    int planeCount = 0;

    // Load planes
    FILE *pf = fopen(planeFile, "r");
    if (!pf) {
        printf("Error opening plane file.\n");
        getchar(); getchar();
        return;
    }

    char line[256];
    fgets(line, sizeof(line), pf); // Skip header

    while (fgets(line, sizeof(line), pf)) {
        Plane p;
        if (sscanf(line, "%[^,],%[^,],%d,%d,%d,%d,%s",
                   p.airplaneID, p.model, &p.totalSeats, &p.firstClassSeats, &p.businessClassSeats, &p.economyClassSeats, p.status) == 7) {
            if (strcmp(p.status, "Available") == 0) {
                planes[planeCount++] = p;
            }
        }
    }
    fclose(pf);

    if (planeCount == 0) {
        printf("No available planes found.\n");
        printf("Press Enter to return...");
        getchar(); getchar();
        return;
    }

    // Show available planes
    printf("Available Airplanes:\n");
    for (int i = 0; i < planeCount; i++) {
        printf("%d. ID: %s | Model: %s | Seats (First: %d, Business: %d, Economy: %d)\n",
               i + 1, planes[i].airplaneID, planes[i].model,
               planes[i].firstClassSeats, planes[i].businessClassSeats, planes[i].economyClassSeats);
    }

    int selected;
    do {
        printf("Select an airplane by number (1-%d): ", planeCount);
        scanf("%d", &selected);
        if (selected < 1 || selected > planeCount)
            printf("Invalid selection. Try again.\n");
    } while (selected < 1 || selected > planeCount);

    Plane chosen = planes[selected - 1];

    printf("Enter Flight ID: ");
    scanf("%d", &f.flightID);

    printf("Enter Departure Airport (Airport Code): ");
    scanf("%s", f.departure);

    printf("Enter Destination Airport (Airport Code): ");
    scanf("%s", f.destination);

    // Validate date
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

    printf("Enter Base Ticket Price: ");
    do {
        scanf("%f", &f.price);
        if (f.price <= 0)
            printf("Price must be positive. Try again: ");
    } while (f.price <= 0);

    // Fill airplane data
    strcpy(f.airplaneID, chosen.airplaneID);
    strcpy(f.airplaneModel, chosen.model);
    f.firstClassSeats = chosen.firstClassSeats;
    f.businessClassSeats = chosen.businessClassSeats;
    f.economyClassSeats = chosen.economyClassSeats;
    f.capacity = f.firstClassSeats + f.businessClassSeats + f.economyClassSeats;
    f.seatsAvailable = f.capacity;

    // Write to flights.csv
    FILE *ff = fopen(flightFile, "r");
    int fileExists = (ff != NULL);
    if (ff) fclose(ff);

    ff = fopen(flightFile, "a");
    if (!ff) {
        printf("Error opening flights file.\n");
        getchar(); getchar();
        return;
    }

    if (!fileExists) {
        fprintf(ff, "FlightID,Departure,Destination,Date,Time,AirplaneID,AirplaneModel,FirstClassSeats,BusinessClassSeats,EconomyClassSeats,Price,Capacity,SeatsAvailable\n");
    }

    fprintf(ff, "%d,%s,%s,%s,%s,%s,%s,%d,%d,%d,%.2f,%d,%d\n",
            f.flightID, f.departure, f.destination, f.flight_date, f.flight_time,
            f.airplaneID, f.airplaneModel, f.firstClassSeats, f.businessClassSeats, f.economyClassSeats,
            f.price, f.capacity, f.seatsAvailable);

    fclose(ff);

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
    printf("                   VIEW FLIGHTS                   \n");
    printf("==================================================\n");

    FILE *file = fopen("flights.csv", "r");
    if (!file) {
        printf("No flight data found. Press Enter to return...");
        getchar(); getchar();
        return;
    }

    char line[512];
    int count = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    // Print nice header
    printf("%-8s %-8s %-8s %-12s %-6s %-8s %-20s %-8s %-10s %-10s %-12s %-12s %-12s\n",
        "FlightID", "From", "To", "Date", "Time", "PlaneID", "Model", "First", "Business", "Economy", "Price", "Capacity", "Available");
    
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        int flightID, firstClass, businessClass, economyClass, capacity, available;
        float price;
        char from[10], to[10], date[20], time[10], airplaneID[10], airplaneModel[50];

        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%f,%d,%d",
                   &flightID, from, to, date, time, airplaneID, airplaneModel,
                   &firstClass, &businessClass, &economyClass, &price, &capacity, &available) == 13) {
                    printf("%-8d %-8s %-8s %-12s %-6s %-8s %-20s %-8d %-10d %-10d %-12.2f %-12d %-12d\n",
                        flightID, from, to, date, time, airplaneID, airplaneModel,
                        firstClass, businessClass, economyClass, price, capacity, available);
                    
            count++;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("No flights available.\n");
    }

    printf("\nPress Enter to return...");
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
