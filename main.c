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

typedef struct {
    int row;
    char column;
    int occupied;
} Seat;

// ========================== DATA STRUCTURES ==========================

// Node for Flight Linked List
typedef struct FlightNode {
    Flight data;
    struct FlightNode* next;
} FlightNode;

// Passenger Struct (You need it too)
typedef struct {
    int passengerID;
    char firstName[50];
    char lastName[50];
    char gender[10];
    char dob[20];
    char passportNumber[20];
    char nationality[30];
    char phoneNumber[20];
    char email[50];
    char seatNumber[10];
    int flightID;
    char classType[20];
    char specialRequest[100];
    char bookingDate[20];
} Passenger;

// Node for Passenger Linked List
typedef struct PassengerNode {
    Passenger data;
    struct PassengerNode* next;
} PassengerNode;

// Graph Connection between Airports
typedef struct Connection {
    char destinationCode[10];
    float firstClassPrice;
    float businessClassPrice;
    float economyClassPrice;
    int distanceKM;
    struct Connection* next;
} Connection;

// Airport Struct
typedef struct Airport {
    char airportCode[10];
    Connection* connections;
} Airport;

// BST Node for Flight Searching
typedef struct FlightTreeNode {
    char destination[10]; // Key
    FlightNode* flights;   // Linked list of flights to this destination
    struct FlightTreeNode* left;
    struct FlightTreeNode* right;
} FlightTreeNode;


// ===================== FUNCTION DECLARATIONS =====================
void clearScreen();
void customerMenu();
void ownerMenu();
void flightMenu();
void passengerMenu();
void addFlight(), removeFlight(), editFlight(), viewFlights();
void addPassenger(), removePassenger(), editPassenger(), viewPassengers();
void loadFlights();
void loadPassengers();
void loadAirports();
void initializeAirport(const char* code);
Airport* findAirport(const char* code);
void addConnection(const char* from, const char* to, int distanceKM);
void searchFlightsByDestination();
void chooseClassAndSeat(FlightNode* chosenFlight);
void initializeSeatMap(FlightNode* chosenFlight, char* classType);
int dijkstra(const char* start, const char* end, char path[][10], int* pathLength);
void searchFlightRoute(char start[], char destination[], char path[][10], int* pathLength);

// ========================== BST FUNCTION DECLARATIONS ==========================
FlightTreeNode* insertFlightTree(FlightTreeNode* root, Flight flight);
void buildFlightTree();
FlightTreeNode* searchFlightTree(FlightTreeNode* root, const char* destination);

// ========================== GLOBAL VARIABLES ==========================
FlightNode* flightHead = NULL;
PassengerNode* passengerHead = NULL;
Airport airports[10];
int airportCount = 0;
FlightTreeNode* flightTreeRoot = NULL;

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

// ===================== TIME HANDLING =====================
void addHoursToTime(const char* timeStr, int hoursToAdd, char* resultTime) {
    int hh, mm;
    sscanf(timeStr, "%2d:%2d", &hh, &mm);

    hh += hoursToAdd;
    while (hh >= 24) hh -= 24;

    sprintf(resultTime, "%02d:%02d", hh, mm);
}

int isTimeAfterOrEqual(const char* time1, const char* time2) {
    int h1, m1, h2, m2;
    sscanf(time1, "%2d:%2d", &h1, &m1);
    sscanf(time2, "%2d:%2d", &h2, &m2);

    if (h1 > h2) return 1;
    if (h1 == h2 && m1 >= m2) return 1;
    return 0;
}

// Approximate flight durations (can adjust if needed)
int getFlightDurationHours(const char* from, const char* to) {
    if ((strcmp(from, "BKK") == 0 && strcmp(to, "NRT") == 0) ||
        (strcmp(from, "NRT") == 0 && strcmp(to, "BKK") == 0)) return 6;
    if ((strcmp(from, "NRT") == 0 && strcmp(to, "YYZ") == 0) ||
        (strcmp(from, "YYZ") == 0 && strcmp(to, "NRT") == 0)) return 12;
    if ((strcmp(from, "BKK") == 0 && strcmp(to, "BER") == 0) ||
        (strcmp(from, "BER") == 0 && strcmp(to, "BKK") == 0)) return 11;
    if ((strcmp(from, "BER") == 0 && strcmp(to, "YYZ") == 0) ||
        (strcmp(from, "YYZ") == 0 && strcmp(to, "BER") == 0)) return 9;
    if ((strcmp(from, "YYZ") == 0 && strcmp(to, "GRU") == 0) ||
        (strcmp(from, "GRU") == 0 && strcmp(to, "YYZ") == 0)) return 10;
    if ((strcmp(from, "BKK") == 0 && strcmp(to, "SYD") == 0) ||
        (strcmp(from, "SYD") == 0 && strcmp(to, "BKK") == 0)) return 9;
    if ((strcmp(from, "SYD") == 0 && strcmp(to, "GRU") == 0) ||
        (strcmp(from, "GRU") == 0 && strcmp(to, "SYD") == 0)) return 15;
    if ((strcmp(from, "BKK") == 0 && strcmp(to, "HKT") == 0) ||
        (strcmp(from, "HKT") == 0 && strcmp(to, "BKK") == 0)) return 1;
    return 8; // Default if unknown
}


// ========================== LOAD FUNCTIONS ==========================

void loadFlights() {
    FILE *file = fopen("flights.csv", "r");
    if (!file) return;

    char line[512];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        FlightNode* newNode = (FlightNode*)malloc(sizeof(FlightNode));
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%f,%d,%d",
               &newNode->data.flightID,
               newNode->data.departure,
               newNode->data.destination,
               newNode->data.flight_date,
               newNode->data.flight_time,
               newNode->data.airplaneID,
               newNode->data.airplaneModel,
               &newNode->data.firstClassSeats,
               &newNode->data.businessClassSeats,
               &newNode->data.economyClassSeats,
               &newNode->data.price,
               &newNode->data.capacity,
               &newNode->data.seatsAvailable);
        newNode->next = flightHead;
        flightHead = newNode;
    }
    fclose(file);
}

void loadPassengers() {
    FILE *file = fopen("passengers.csv", "r");
    if (!file) return;

    char line[512];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        PassengerNode* newNode = (PassengerNode*)malloc(sizeof(PassengerNode));
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^\\n]",
               &newNode->data.passengerID,
               newNode->data.firstName,
               newNode->data.lastName,
               newNode->data.gender,
               newNode->data.dob,
               newNode->data.passportNumber,
               newNode->data.nationality,
               newNode->data.phoneNumber,
               newNode->data.email,
               newNode->data.seatNumber,
               &newNode->data.flightID,
               newNode->data.classType,
               newNode->data.specialRequest,
               newNode->data.bookingDate);
        newNode->next = passengerHead;
        passengerHead = newNode;
    }
    fclose(file);
}

void initializeAirport(const char* code) {
    strcpy(airports[airportCount].airportCode, code);
    airports[airportCount].connections = NULL;
    airportCount++;
}

Airport* findAirport(const char* code) {
    for (int i = 0; i < airportCount; i++) {
        if (strcmp(airports[i].airportCode, code) == 0)
            return &airports[i];
    }
    return NULL;
}

void addConnection(const char* from, const char* to, int distanceKM) {
    Airport* origin = findAirport(from);
    if (!origin) return;

    Connection* newConn = (Connection*)malloc(sizeof(Connection));
    strcpy(newConn->destinationCode, to);
    newConn->distanceKM = distanceKM;
    newConn->economyClassPrice = distanceKM * 0.1;
    newConn->businessClassPrice = distanceKM * 0.2;
    newConn->firstClassPrice = distanceKM * 0.3;
    newConn->next = origin->connections;
    origin->connections = newConn;
}

void loadAirports() {
    const char* airportCodes[] = {"BKK", "HKT", "SYD", "NRT", "GRU", "BER", "YYZ"};
    int total = sizeof(airportCodes) / sizeof(airportCodes[0]);
    for (int i = 0; i < total; i++) {
        initializeAirport(airportCodes[i]);
    }

    addConnection("BKK", "HKT", 675);
    addConnection("BKK", "SYD", 7520);
    addConnection("BKK", "NRT", 4600);
    addConnection("BKK", "BER", 8600);
    addConnection("HKT", "BKK", 675);
    addConnection("HKT", "SYD", 7170);
    addConnection("SYD", "BKK", 7520);
    addConnection("SYD", "GRU", 13300);
    addConnection("NRT", "BKK", 4600);
    addConnection("NRT", "YYZ", 10300);
    addConnection("NRT", "BER", 9000);
    addConnection("GRU", "SYD", 13300);
    addConnection("GRU", "YYZ", 8300);
    addConnection("BER", "BKK", 8600);
    addConnection("BER", "NRT", 9000);
    addConnection("BER", "YYZ", 6300);
    addConnection("YYZ", "NRT", 10300);
    addConnection("YYZ", "GRU", 8300);
    addConnection("YYZ", "BER", 6300);
}

// ========================== BST FUNCTIONS ==========================

// Insert one flight into the BST
FlightTreeNode* insertFlightTree(FlightTreeNode* root, Flight flight) {
    if (!root) {
        FlightTreeNode* newNode = (FlightTreeNode*)malloc(sizeof(FlightTreeNode));
        strcpy(newNode->destination, flight.destination);
        newNode->flights = NULL;
        newNode->left = newNode->right = NULL;

        // Add first flight into this node
        FlightNode* newFlight = (FlightNode*)malloc(sizeof(FlightNode));
        newFlight->data = flight;
        newFlight->next = NULL;
        newNode->flights = newFlight;
        return newNode;
    }

    int cmp = strcmp(flight.destination, root->destination);
    if (cmp == 0) {
        // Destination already exists, add new flight to linked list
        FlightNode* newFlight = (FlightNode*)malloc(sizeof(FlightNode));
        newFlight->data = flight;
        newFlight->next = root->flights;
        root->flights = newFlight;
    } else if (cmp < 0) {
        root->left = insertFlightTree(root->left, flight);
    } else {
        root->right = insertFlightTree(root->right, flight);
    }
    return root;
}

// Wrapper function to insert from memory flight linked list
void buildFlightTree() {
    FlightNode* current = flightHead;
    while (current) {
        flightTreeRoot = insertFlightTree(flightTreeRoot, current->data);
        current = current->next;
    }
}

// Search flights by destination
FlightTreeNode* searchFlightTree(FlightTreeNode* root, const char* destination) {
    if (!root) return NULL;
    int cmp = strcmp(destination, root->destination);
    if (cmp == 0) return root;
    else if (cmp < 0) return searchFlightTree(root->left, destination);
    else return searchFlightTree(root->right, destination);
}

// =========================== MAIN ===========================
int main() {
    int choice;

    loadFlights();
    loadPassengers();
    loadAirports();
    buildFlightTree();

    do {
        clearScreen();
        printf("===================================================================\n");
        printf("                   WELCOME TO FLIGHT RESERVATION                      \n");
        printf("===================================================================\n");
        printf("1. Customer\n");
        printf("2. Owner\n");
        printf("3. Exit\n");
        printf("===================================================================\n");
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
    int choice;
    do {
        clearScreen();
        printf("===================================================================\n");
        printf("                           CUSTOMER MENU                                \n");
        printf("===================================================================\n");
        printf("1. Book Flight\n");
        printf("2. View History\n");
        printf("3. Back to Main Menu\n");
        printf("===================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear buffer after scanf

        switch (choice) {
            case 1:
                searchFlightsByDestination();
                break;
            case 2:
                // View History (can leave empty for now)
                break;
            case 3:
                return; // Exit customer menu
            default:
                printf("\nInvalid choice. Press Enter to try again...");
                getchar();
        }

    } while (choice != 3);
}

// Helper to print all destinations in BST (Inorder traversal)
void printDestinations(FlightTreeNode* root) {
    if (!root) return;
    printDestinations(root->left);
    printf("%s ", root->destination);
    printDestinations(root->right);
}

// ===================== SEARCH FLIGHT BY DESTINATION =====================

void searchFlightsByDestination() {
    clearScreen();

    char start[10], destination[10];
    char path[10][10];
    int pathLength = 0;

    searchFlightRoute(start, destination, path, &pathLength);

    if (pathLength < 2) {
        printf("Invalid path. Press Enter to return...");
        getchar(); getchar();
        return;
    }

    char travelDate[20];
    printf("\nEnter Travel Date (YYYY-MM-DD): ");
    scanf("%s", travelDate);

    static char lastArrivalTime[10] = "";
    static int lastFlightSelected = 0;

    for (int i = 0; i < pathLength - 1; i++) {
        char from[10], to[10];
        strcpy(from, path[i]);
        strcpy(to, path[i + 1]);

        clearScreen();
        printf("===================================================================\n");
        printf("               BOOKING SEGMENT %d/%d: %s to %s                     \n", i + 1, pathLength - 1, from, to);
        printf("===================================================================\n");

        FlightTreeNode* result = searchFlightTree(flightTreeRoot, to);

        if (!result) {
            printf("\nNo flights found to %s.\n", to);
            printf("Press Enter to return...");
            getchar(); getchar();
            return;
        }

        char expectedArrivalTime[10] = "";
        if (i > 0 && lastFlightSelected) {
            int durationHours = getFlightDurationHours(path[i - 1], path[i]);
            addHoursToTime(lastArrivalTime, durationHours, expectedArrivalTime);

            printf("\nExpected arrival time at %s: %s\n", from, expectedArrivalTime);
            printf("Only showing flights departing after arrival time...\n");
        }

        printf("\nAvailable Flights from %s to %s on %s:\n", from, to, travelDate);
        printf("%-8s %-8s %-8s %-12s %-6s %-10s %-12s\n",
               "FlightID", "From", "To", "Date", "Time", "PlaneID", "Available");
        printf("-------------------------------------------------------------------\n");

        FlightNode* f = result->flights;
        int flightsFound = 0;

        while (f) {
            if (strcmp(f->data.departure, from) == 0 &&
                strcmp(f->data.destination, to) == 0 &&
                strcmp(f->data.flight_date, travelDate) == 0) {

                if (i == 0 || isTimeAfterOrEqual(f->data.flight_time, expectedArrivalTime)) {
                    printf("%-8d %-8s %-8s %-12s %-6s %-10s %-12d\n",
                           f->data.flightID, f->data.departure, f->data.destination,
                           f->data.flight_date, f->data.flight_time,
                           f->data.airplaneID, f->data.seatsAvailable);
                    flightsFound++;
                }
            }
            f = f->next;
        }

        if (flightsFound == 0) {
            printf("\nNo valid connecting flights found for this segment.\n");
            printf("Press Enter to return...");
            getchar(); getchar();
            return;
        }

        printf("\nEnter Flight ID to book (0 to cancel): ");
        int selectedFlightID;
        scanf("%d", &selectedFlightID);

        if (selectedFlightID == 0) {
            printf("Booking cancelled. Press Enter to return...");
            getchar(); getchar();
            return;
        }

        // Re-find selected flight
        f = result->flights;
        while (f) {
            if (f->data.flightID == selectedFlightID &&
                strcmp(f->data.flight_date, travelDate) == 0 &&
                strcmp(f->data.departure, from) == 0 &&
                strcmp(f->data.destination, to) == 0) {

                if (i == 0 || isTimeAfterOrEqual(f->data.flight_time, expectedArrivalTime)) {
                    break;
                }
            }
            f = f->next;
        }

        if (!f) {
            printf("Invalid Flight ID or flight time too early. Press Enter to return...");
            getchar(); getchar();
            return;
        }

        // Save departure time for next connection
        strcpy(lastArrivalTime, f->data.flight_time);
        lastFlightSelected = 1;

        chooseClassAndSeat(f);  // Book seat for this segment
    }
}

// ==================== DIJKSTRA'S ALGORITHM ========================

int dijkstra(const char* start, const char* end, char path[][10], int* pathLength) {
    int dist[airportCount];
    int visited[airportCount];
    int previous[airportCount];
    
    for (int i = 0; i < airportCount; i++) {
        dist[i] = 1e9;
        visited[i] = 0;
        previous[i] = -1;
    }

    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < airportCount; i++) {
        if (strcmp(airports[i].airportCode, start) == 0) startIndex = i;
        if (strcmp(airports[i].airportCode, end) == 0) endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1) {
        printf("Invalid airport codes.\n");
        return -1;
    }

    dist[startIndex] = 0;

    for (int count = 0; count < airportCount - 1; count++) {
        int minDist = 1e9, u = -1;
        for (int i = 0; i < airportCount; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        Connection* conn = airports[u].connections;
        while (conn) {
            int v = -1;
            for (int i = 0; i < airportCount; i++) {
                if (strcmp(airports[i].airportCode, conn->destinationCode) == 0) {
                    v = i;
                    break;
                }
            }

            if (v != -1 && dist[u] + conn->distanceKM < dist[v]) {
                dist[v] = dist[u] + conn->distanceKM;
                previous[v] = u;
            }
            conn = conn->next;
        }
    }

    if (dist[endIndex] == 1e9) {
        printf("No path found.\n");
        return -1;
    }

    int temp[airportCount];
    int len = 0;
    for (int at = endIndex; at != -1; at = previous[at]) {
        temp[len++] = at;
    }

    *pathLength = len;
    for (int i = 0; i < len; i++) {
        strcpy(path[i], airports[temp[len - i - 1]].airportCode);
    }

    return dist[endIndex]; // Return total distance
}

// ===================== SEARCH FOR FLIGHT =====================

void searchFlightRoute(char start[], char destination[], char path[][10], int* pathLength) {
    clearScreen();
    printf("===================================================================\n");
    printf("                     SELECT ROUTE                                \n");
    printf("===================================================================\n");
    printf("Available airports: BKK, HKT, SYD, NRT, GRU, BER, YYZ\n");

    printf("Enter your departure airport (Example: BKK): ");
    scanf("%s", start);

    printf("Enter your destination airport (Example: YYZ): ");
    scanf("%s", destination);

    printf("\nFinding shortest route...\n");

    int totalDistance = dijkstra(start, destination, path, pathLength);
    if (totalDistance == -1) {
        printf("Could not find a path. Returning...\n");
        getchar(); getchar();
        return;
    }

    printf("\nShortest path: ");
    for (int i = 0; i < *pathLength; i++) {
        printf("%s", path[i]);
        if (i != *pathLength - 1) printf(" -> ");
    }
    printf("\nTotal Distance: %d km\n", totalDistance);

    printf("\nPress Enter to continue...");
    getchar(); getchar();
}

// ===================== CHOOSE CLASS AND SEAT =====================

void chooseClassAndSeat(FlightNode* chosenFlight) {
    clearScreen();
    printf("===================================================================\n");
    printf("                        SELECT CLASS TYPE                           \n");
    printf("===================================================================\n");

    int choice;
    printf("1. First Class\n");
    printf("2. Business Class\n");
    printf("3. Economy Class\n");
    printf("4. Cancel\n");
    printf("===================================================================\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if (choice == 4) {
        printf("\nBooking cancelled. Press Enter to return...");
        getchar(); getchar();
        return;
    }

    char classType[20];
    int totalSeats = 0;
    int seatRows = 0;
    int seatsPerRow = 0;

    switch (choice) {
        case 1:
            strcpy(classType, "First");
            totalSeats = chosenFlight->data.firstClassSeats;
            seatsPerRow = 4; // Example: First Class usually 1-2-1 or 2-2
            break;
        case 2:
            strcpy(classType, "Business");
            totalSeats = chosenFlight->data.businessClassSeats;
            seatsPerRow = 6; // Example: Business Class 2-2-2
            break;
        case 3:
            strcpy(classType, "Economy");
            totalSeats = chosenFlight->data.economyClassSeats;
            seatsPerRow = 9; // Example: Economy Class 3-3-3
            break;
        default:
            printf("\nInvalid choice. Press Enter to return...");
            getchar(); getchar();
            return;
    }

    seatRows = (totalSeats + seatsPerRow - 1) / seatsPerRow; // Calculate number of rows

    initializeSeatMap(chosenFlight, classType);
}

void initializeSeatMap(FlightNode* chosenFlight, char* classType) {
    clearScreen();
    printf("===================================================================\n");
    printf("                      SEAT MAP - %s CLASS                           \n", classType);
    printf("===================================================================\n");


    int totalSeats = 0;
    int seatsPerRow = 0;
    int startRow = 1;
    char seatColumns[10]; // Max 10 columns like A-I

    if (strcmp(classType, "First") == 0) {
        totalSeats = chosenFlight->data.firstClassSeats;
        seatsPerRow = 4;
        strcpy(seatColumns, "ABCD");
    } else if (strcmp(classType, "Business") == 0) {
        totalSeats = chosenFlight->data.businessClassSeats;
        seatsPerRow = 4; // assuming 2-2-2 layout
        strcpy(seatColumns, "ABCD");
    } else if (strcmp(classType, "Economy") == 0) {
        totalSeats = chosenFlight->data.economyClassSeats;
        seatsPerRow = 9; // 3-3-3 layout
        strcpy(seatColumns, "ABCDEFGHI");
    }

    int rows = (totalSeats + seatsPerRow - 1) / seatsPerRow;
    int seatIndex = 0;

    // Print column headers
        printf("\n    "); // Start some margin before the seat columns
        for (int i = 0; seatColumns[i] != '\0'; i++) {
            printf(" [%c] ", seatColumns[i]); // Print letter inside [ ] box spacing
            if (strcmp(classType, "Economy") == 0 && (i + 1) % 3 == 0 && seatColumns[i+1] != '\0') {
                printf("    "); // Extra aisle space only for economy
            }
        }
        printf("\n-------------------------------------------------------------------\n");
    // Print rows and seats
    for (int r = startRow; r < startRow + rows; r++) {
        printf("%-4d", r);
        for (int c = 0; seatColumns[c] != '\0'; c++) {
            if (seatIndex < totalSeats) {
                // Here you would check if the seat is booked (for now assume all available)
                printf(" [ ] ");
                seatIndex++;
            } else {
                printf("     ");
            }
            if (strcmp(classType, "Economy") == 0 && (c + 1) % 3 == 0 && seatColumns[c+1] != '\0') {
                printf("   "); // Add aisle space only in Economy
            }
        }
        printf("\n");
    }

    printf("\nPress Enter to continue...");
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
    printf("%-4s %-12s %-25s %-8s %-10s %-8s\n",
    "No.", "AirplaneID", "Model", "First", "Business", "Economy");
    printf("---------------------------------------------------------------------------\n");

    for (int i = 0; i < planeCount; i++) {
        printf("%-4d %-12s %-25s %-8d %-10d %-8d\n",
        i + 1,
        planes[i].airplaneID,
        planes[i].model,
        planes[i].firstClassSeats,
        planes[i].businessClassSeats,
        planes[i].economyClassSeats);
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