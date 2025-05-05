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

// ========================== DATA STRUCTURES ==========================

// Node for Flight Linked List
typedef struct FlightNode {
    Flight data;
    struct FlightNode* next;
} FlightNode;

// Passenger Struct
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
    char luggageSize[20];         
    char mealPreference[20];      
    char wifiPreference[10];
    char specialAssistance[100];  
    float price;

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
void viewHistory(const char *email);

// ========================== ADDITIONAL SERVICE PRICES ==========================

    #define WIFI_FEE 150
    #define LUGGAGE_MEDIUM_FEE 300
    #define LUGGAGE_LARGE_FEE 500
    #define MEAL_VEGAN_FEE 100
    #define MEAL_HALAL_FEE 100

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
int latestPassengerID = 0;

// ===================== DATE AND TIME CHECKER  =====================
int isValidDate(const char *date) {
    int y, m, d;
    if (sscanf(date, "%4d-%2d-%2d", &y, &m, &d) != 3)
        return 0;
    if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31)
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

// Set Flight Durations
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
    return 8;
}


// ========================== LOAD FUNCTIONS ==========================

void loadFlights() {
    FlightNode* current = flightHead;
    while (current) {
        FlightNode* temp = current;
        current = current->next;
        free(temp);
    }
    flightHead = NULL;
    
    FILE *file = fopen("flights.csv", "r");
    if (!file) return;

    char line[512];
    fgets(line, sizeof(line), file);

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

void saveFlights() {
    FILE *file = fopen("flights.csv", "w");
    if (!file) return;

    fprintf(file, "FlightID,Departure,Destination,Date,Time,AirplaneID,AirplaneModel,FirstClassSeats,BusinessClassSeats,EconomyClassSeats,Price,Capacity,SeatsAvailable\n");

    FlightNode* current = flightHead;
    while (current) {
        Flight f = current->data;
        fprintf(file, "%d,%s,%s,%s,%s,%s,%s,%d,%d,%d,%.2f,%d,%d\n",
            f.flightID, f.departure, f.destination, f.flight_date, f.flight_time,
            f.airplaneID, f.airplaneModel, f.firstClassSeats, f.businessClassSeats,
            f.economyClassSeats, f.price, f.capacity, f.seatsAvailable);
        current = current->next;
    }
    fclose(file);
}

void loadPlanes(Plane planes[], int* planeCount) {
    FILE *file = fopen("plane.csv", "r");
    if (!file) return;

    char line[256];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        Plane p;
        sscanf(line, "%[^,],%[^,],%d,%d,%d,%d,%[^,\n]",
            p.airplaneID, p.model, &p.totalSeats, &p.firstClassSeats,
            &p.businessClassSeats, &p.economyClassSeats, p.status);
        planes[(*planeCount)++] = p;
    }
    fclose(file);
}

void loadPassengers() {
    FILE *file = fopen("passengers.csv", "r");
    if (!file) return;

    char line[512];
    fgets(line, sizeof(line), file); 

    while (fgets(line, sizeof(line), file)) {
        PassengerNode* newNode = (PassengerNode*)malloc(sizeof(PassengerNode));
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
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
            newNode->data.bookingDate,
            newNode->data.luggageSize,
            newNode->data.mealPreference,
            newNode->data.wifiPreference,
            newNode->data.specialAssistance);

            if (newNode->data.passengerID > latestPassengerID) {
                latestPassengerID = newNode->data.passengerID;
            }                    

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

int loginWithEmail(const char *email, char *cuntUser) {
    FILE *fp = fopen("passengers.csv", "r");
    if (fp == NULL) {
        printf("Error: Cannot open passengers.csv\n");
        return 0;
    }

    char line[256];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), fp)) {
        lineNumber++;
        if (lineNumber == 1) continue;

        char fName[50], mail[100];
        sscanf(line, "%*[^,],%49[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%99[^,],", fName, mail);

        if (strcmp(mail, email) == 0) {
            snprintf(cuntUser, 100, "%s", mail);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void ViewHistoryMenu(){
    clearScreen();
    printf("===================================================================\n");
    printf("                          View History                             \n");
    printf("===================================================================\n");

    char email[100], cuntUser[100] = "Guest";

    printf("Enter your email: ");
    scanf("%s", email);

    while (getchar() != '\n');

    if (loginWithEmail(email, cuntUser)) {
        viewHistory(email);
    } else {
        printf("Login failed.\n");
    }
}

void viewHistory(const char *email){
    float total;

    FILE *fp = fopen("history.csv", "r");
    if (fp == NULL) {
        printf("History file not found.\n");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), fp); 

    int found = 0;
    printf("\n%-12s %-25s %-10s %-8s %-6s %-15s %-12s %-10s %-8s %-9s %-8s\n",
        "Date", "Email", "Name", "Class", "Seat", "Request", "Luggage", "Meal", "Wifi", "FlightID", "Total");
    printf("---------------------------------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp)) {
        char date[20], mail[50], name[30], cls[10], seat[5];
        char req[30], lug[20], meal[20], wifi[10];
        int flightID;

        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f",
            date, mail, name, cls, seat, req, lug, meal, wifi, &flightID, &total);

        if (strcmp(email, mail) == 0) {
            printf("%-12s %-25s %-10s %-8s %-6s %-15s %-12s %-10s %-8s %-9d %-8.2f\n",
                date, mail, name, cls, seat, req, lug, meal, wifi, flightID, total);         
            found = 1;
        }
    }

    if (!found) {
        printf("No booking history found for this email.\n");
    }

   fclose(fp);
   printf("\nPress ENTER to return to menu...");
   getchar();

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
                ViewHistoryMenu();
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
    do {
        printf("\nEnter Travel Date (YYYY-MM-DD): ");
        scanf("%s", travelDate);
        if (!isValidDate(travelDate)) {
            printf("Invalid date format. Please try again.\n");
        }
    } while (!isValidDate(travelDate));

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

        chooseClassAndSeat(f); 
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

    seatRows = (totalSeats + seatsPerRow - 1) / seatsPerRow; 

    initializeSeatMap(chosenFlight, classType);
}

// ========= CHECK FOR OCCUPIED SEAT ==========

int isSeatOccupied(int flightID, const char* classType, const char* seat) {
    PassengerNode* p = passengerHead;
    while (p) {
        if (p->data.flightID == flightID &&
            strcmp(p->data.classType, classType) == 0 &&
            strcmp(p->data.seatNumber, seat) == 0) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

// ====================== PAYMENT SUMMARY FUNCTION ==============================

void displayPaymentSummary(Passenger p, Flight f) {
    float multiplier = 1.0;
    if (strcmp(p.classType, "Business") == 0) multiplier = 5.0;
    else if (strcmp(p.classType, "First") == 0) multiplier = 12.0;
    float base = f.price * multiplier;

    float wifiFee = (strcmp(p.wifiPreference, "yes") == 0) ? WIFI_FEE : 0;
    
    float luggageFee = 0;
    if (strcmp(p.luggageSize, "medium") == 0) luggageFee = LUGGAGE_MEDIUM_FEE;
    else if (strcmp(p.luggageSize, "large") == 0) luggageFee = LUGGAGE_LARGE_FEE;

    float mealFee = 0;
    if (strcmp(p.mealPreference, "vegan") == 0) mealFee = MEAL_VEGAN_FEE;
    else if (strcmp(p.mealPreference, "halal") == 0) mealFee = MEAL_HALAL_FEE;

    float total = base + wifiFee + luggageFee + mealFee;

    printf("\n===================================================\n");
    printf("                  PAYMENT SUMMARY\n");
    printf("===================================================\n");
    printf("Passenger: %s %s\n", p.firstName, p.lastName);
    printf("Flight: %s -> %s (Flight ID: %d)\n", f.departure, f.destination, f.flightID);
    printf("Class: %s | Seat: %s\n", p.classType, p.seatNumber);

    printf("\nBase Price:           %8.2f\n", base);
    if (wifiFee > 0)        printf("WiFi:                 +%7.2f\n", wifiFee);
    if (luggageFee > 0)     printf("Luggage (%s):     +%7.2f\n", p.luggageSize, luggageFee);
    if (mealFee > 0)        printf("Meal (%s):        +%7.2f\n", p.mealPreference, mealFee);

    printf("---------------------------------\n");
    printf("Total:                %8.2f\n", total);
    printf("\nConfirm Payment? (Y/N): ");
}

float calculateTotal(Passenger p) {
    float multiplier = 1.0;
    if (strcmp(p.classType, "Business") == 0) multiplier = 1.5;
    else if (strcmp(p.classType, "First") == 0) multiplier = 2.0;
    float base = p.price * multiplier;

    float wifiFee = (strcmp(p.wifiPreference, "yes") == 0) ? WIFI_FEE : 0;
    
    float luggageFee = 0;
    if (strcmp(p.luggageSize, "medium") == 0) luggageFee = LUGGAGE_MEDIUM_FEE;
    else if (strcmp(p.luggageSize, "large") == 0) luggageFee = LUGGAGE_LARGE_FEE;

    float mealFee = 0;
    if (strcmp(p.mealPreference, "vegan") == 0) mealFee = MEAL_VEGAN_FEE;
    else if (strcmp(p.mealPreference, "halal") == 0) mealFee = MEAL_HALAL_FEE;

    return base + wifiFee + luggageFee + mealFee;
}

// ==================== INITIALIZE SEAT MAP ==============================

void initializeSeatMap(FlightNode* chosenFlight, char* classType) {
    clearScreen();
    printf("===================================================================\n");
    printf("                      SEAT MAP - %s CLASS                           \n", classType);
    printf("===================================================================\n");

    int totalSeats = 0;
    int seatsPerRow = 0;
    int startRow = 1;
    char seatColumns[10]; 

    if (strcmp(classType, "First") == 0) {
        totalSeats = chosenFlight->data.firstClassSeats;
        seatsPerRow = 4;
        strcpy(seatColumns, "ABCD");
    } else if (strcmp(classType, "Business") == 0) {
        totalSeats = chosenFlight->data.businessClassSeats;
        seatsPerRow = 4;
        strcpy(seatColumns, "ABCD");
    } else if (strcmp(classType, "Economy") == 0) {
        totalSeats = chosenFlight->data.economyClassSeats;
        seatsPerRow = 9;
        strcpy(seatColumns, "ABCDEFGHI");
    }

    int rows = (totalSeats + seatsPerRow - 1) / seatsPerRow;
    int seatIndex = 0;

    printf("\n    ");
    for (int i = 0; seatColumns[i] != '\0'; i++) {
        printf(" [%c] ", seatColumns[i]);
        if (strcmp(classType, "Economy") == 0 && (i + 1) % 3 == 0 && seatColumns[i + 1] != '\0') {
            printf("    ");
        }
    }
    printf("\n-------------------------------------------------------------------\n");

    for (int r = startRow; r < startRow + rows; r++) {
        printf("%-4d", r);
        for (int c = 0; seatColumns[c] != '\0'; c++) {
            if (seatIndex < totalSeats) {
                char seat[10];
                sprintf(seat, "%c%d", seatColumns[c], r);
                if (isSeatOccupied(chosenFlight->data.flightID, classType, seat)) {
                    printf(" [X] ");
                } else {
                    printf(" [ ] ");
                }
                seatIndex++;
            } else {
                printf("     ");
            }
    
            if (strcmp(classType, "Economy") == 0 && (c + 1) % 3 == 0 && seatColumns[c + 1] != '\0') {
                printf("   ");
            }
        }
        printf("\n");
    }
    
    Passenger p;

    printf("\nEnter your first name: ");
    scanf("%s", p.firstName);
    while (getchar() != '\n');

    printf("Enter your last name: ");
    scanf("%s", p.lastName);
    while (getchar() != '\n');

    do {
        printf("Enter your gender (M/F): ");
        char genderInput[10];
        scanf("%s", genderInput);
        while (getchar() != '\n');
    
        if (strlen(genderInput) == 1 && (genderInput[0] == 'M' || genderInput[0] == 'm' || genderInput[0] == 'F' || genderInput[0] == 'f')) {
            p.gender[0] = toupper(genderInput[0]);
            p.gender[1] = '\0';
            break;
        } else {
            printf("Invalid gender. Please enter 'M' or 'F'.\n");
        }
    } while (1);    

    do {
        printf("Enter your date of birth (YYYY-MM-DD): ");
        scanf("%s", p.dob);
        if (!isValidDate(p.dob)) {
            printf("Invalid date format. Please follow YYYY-MM-DD.\n");
        }
    } while (!isValidDate(p.dob));

    printf("Enter your passport number: ");
    scanf("%s", p.passportNumber);
    while (getchar() != '\n');

    printf("Enter your nationality: ");
    scanf("%s", p.nationality);
    while (getchar() != '\n');

    int isValidPhone;
    do {
        isValidPhone = 1;
        printf("Enter your phone number: ");
        scanf("%s", p.phoneNumber);
        while (getchar() != '\n');

        for (int i = 0; i < strlen(p.phoneNumber); i++) {
            if (!isdigit(p.phoneNumber[i])) {
                isValidPhone = 0;
                break;
            }
        }

    if (!isValidPhone) {
        printf("Phone number must contain digits only. Try again.\n");
    }
    } while (!isValidPhone);

    do {
        printf("Enter your email address: ");
        scanf("%s", p.email);
        while (getchar() != '\n');
    
        if (strstr(p.email, "@") && strstr(p.email, ".com")) {
            break;
        } else {
            printf("Invalid email format. Please include '@' and end with '.com'.\n");
        }
    } while (1);    

    int validSeat = 0;
    do {
        printf("Enter your desired seat number (e.g., A1, B3): ");
        scanf("%s", p.seatNumber);

        char column;
        int row;
        if (sscanf(p.seatNumber, "%c%d", &column, &row) != 2) {
            printf("Invalid format. Use format like A1, B2, etc.\n");
            continue;
        }

        column = toupper(column);
        if (strchr(seatColumns, column) == NULL) {
            printf("Invalid seat column '%c'. Must be one of [%s].\n", column, seatColumns);
            continue;
        }

        int maxRow = (totalSeats + seatsPerRow - 1) / seatsPerRow;
        if (row < 1 || row > maxRow) {
            printf("Invalid seat row '%d'. Must be between 1 and %d.\n", row, maxRow);
            continue;
        }

        if (isSeatOccupied(chosenFlight->data.flightID, classType, p.seatNumber)) {
            printf("Seat %s is already occupied. Please choose another seat.\n", p.seatNumber);
        } else {
            validSeat = 1;
        }
    } while (!validSeat);

    do {
        printf("Enter your luggage size (carry-on, medium, large): ");
        scanf("%s", p.luggageSize);
        if (strcmp(p.luggageSize, "carry-on") != 0 && strcmp(p.luggageSize, "medium") != 0 && strcmp(p.luggageSize, "large") != 0) {
            printf("Invalid size. Choose from carry-on, medium, large.\n");
        }
    } while (strcmp(p.luggageSize, "carry-on") != 0 && strcmp(p.luggageSize, "medium") != 0 && strcmp(p.luggageSize, "large") != 0);

    do {
        printf("Enter your meal preference (standard, vegan, halal): ");
        scanf("%s", p.mealPreference);
        if (strcmp(p.mealPreference, "standard") != 0 && strcmp(p.mealPreference, "vegan") != 0 && strcmp(p.mealPreference, "halal") != 0) {
            printf("Invalid option. Choose from standard, vegan, halal.\n");
        }
    } while (strcmp(p.mealPreference, "standard") != 0 && strcmp(p.mealPreference, "vegan") != 0 && strcmp(p.mealPreference, "halal") != 0);

    do {
        printf("Do you want wifi on this flight? (yes/no): ");
        scanf("%s", p.wifiPreference);
        if (strcmp(p.wifiPreference, "yes") != 0 && strcmp(p.wifiPreference, "no") != 0) {
            printf("Please enter 'yes' or 'no'.\n");
        }
    } while (strcmp(p.wifiPreference, "yes") != 0 && strcmp(p.wifiPreference, "no") != 0);

    printf("Any special assistance needed? (press Enter to skip): ");
    getchar();
    fgets(p.specialAssistance, sizeof(p.specialAssistance), stdin);
    size_t len = strlen(p.specialAssistance);
    if (len > 0 && p.specialAssistance[len - 1] == '\n') {
        p.specialAssistance[len - 1] = '\0';
    }

    latestPassengerID++;
    p.passengerID = latestPassengerID;
    strcpy(p.classType, classType);
    p.flightID = chosenFlight->data.flightID;
    p.price = chosenFlight->data.price;


    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(p.bookingDate, sizeof(p.bookingDate), "%Y-%m-%d", t);

    strcpy(p.specialRequest, "-");

    displayPaymentSummary(p, chosenFlight->data);

    char confirm;
    scanf(" %c", &confirm);
    if (tolower(confirm) != 'y') {
        printf("Payment cancelled. Press Enter to return...");
        getchar(); getchar();
        return;
    }
    
    FILE *pf = fopen("passengers.csv", "a");
    if (pf) {
        fprintf(pf, "%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s,%s,%s\n",
            p.passengerID, p.firstName, p.lastName, p.gender, p.dob, p.passportNumber,
            p.nationality, p.phoneNumber, p.email, p.seatNumber,
            p.flightID, p.classType, p.specialRequest, p.bookingDate,
            p.luggageSize, p.mealPreference, p.wifiPreference, p.specialAssistance);

            FILE *hf = fopen("history.csv", "a");
            if (hf != NULL) {
                fseek(hf, 0, SEEK_END);
                long size = ftell(hf);
                if (size == 0) {
                    fprintf(hf, "BookingDate,Email,FirstName,Class,Seat,SpecialRequest,LuggageSize,Meal,Wifi,FlightID,Total\n");
                }
                
                printf("DEBUG: Price from flight = %.2f\n", chosenFlight->data.price);
                float total = calculateTotal(p);
            
                fprintf(hf, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%.2f\n",
                    p.bookingDate, p.email, p.firstName, p.classType,
                    p.seatNumber, p.specialRequest, p.luggageSize,
                    p.mealPreference, p.wifiPreference, p.flightID, total);

                fclose(hf);
            }            
                    
        fclose(pf);
        PassengerNode* newNode = (PassengerNode*)malloc(sizeof(PassengerNode));
        newNode->data = p;
        newNode->next = passengerHead;
        passengerHead = newNode;
        printf("\nBooking successful! Your Passenger ID is %d\n", p.passengerID);

    } else {
        printf("Error: Could not open passengers.csv for writing.\n");
    }
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
    Plane planes[100];
    int planeCount = 0;
    loadPlanes(planes, &planeCount);

    int availableIndexes[100];
    int availableCount = 0;

    printf("%-4s %-12s %-25s %-8s %-10s %-8s\n",
        "No.", "AirplaneID", "Model", "First", "Business", "Economy");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < planeCount; i++) {
        if (strcmp(planes[i].status, "Available") == 0) {
            availableIndexes[availableCount] = i;
            printf("%-4d %-12s %-25s %-8d %-10d %-8d\n",
                availableCount + 1,
                planes[i].airplaneID,
                planes[i].model,
                planes[i].firstClassSeats,
                planes[i].businessClassSeats,
                planes[i].economyClassSeats);
            availableCount++;
        }
    }

    if (availableCount == 0) {
        printf("\nNo available planes. Cannot add flight. Press Enter...");
        getchar(); getchar();
        return;
    }

    int choice;
    printf("\nSelect a plane by number: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > availableCount) {
        printf("Invalid selection. Press Enter...");
        getchar(); getchar();
        return;
    }

    int selected = availableIndexes[choice-1];

    FlightNode* newNode = (FlightNode*)malloc(sizeof(FlightNode));

    printf("Enter Flight ID: ");
    scanf("%d", &newNode->data.flightID);

    const char* validAirports[] = {"BKK", "HKT", "NRT", "SYD", "BER", "GRU", "YYZ"};
    int isValidAirport = 0;

    do {
        printf("Enter Departure Airport (BKK, HKT, NRT, SYD, BER, GRU, YYZ): ");
        scanf("%s", newNode->data.departure);
        for (int i = 0; newNode->data.departure[i]; i++) {
            newNode->data.departure[i] = toupper(newNode->data.departure[i]);
        }
        isValidAirport = 0;
        for (int i = 0; i < 7; i++) {
            if (strcmp(newNode->data.departure, validAirports[i]) == 0) {
                isValidAirport = 1;
                break;
            }
        }
        if (!isValidAirport) printf("Invalid airport code. Try again.\n");
    } while (!isValidAirport);

    do {
        printf("Enter Destination Airport (BKK, HKT, NRT, SYD, BER, GRU, YYZ): ");
        scanf("%s", newNode->data.destination);
        for (int i = 0; newNode->data.destination[i]; i++) {
            newNode->data.destination[i] = toupper(newNode->data.destination[i]);
        }
        isValidAirport = 0;
        for (int i = 0; i < 7; i++) {
            if (strcmp(newNode->data.destination, validAirports[i]) == 0) {
                isValidAirport = 1;
                break;
            }
        }
        if (!isValidAirport || strcmp(newNode->data.departure, newNode->data.destination) == 0)
            printf("Invalid destination (must be valid and different from departure). Try again.\n");
    } while (!isValidAirport || strcmp(newNode->data.departure, newNode->data.destination) == 0);

    do {
        printf("Enter Date (YYYY-MM-DD): ");
        scanf("%s", newNode->data.flight_date);
        if (!isValidDate(newNode->data.flight_date))
            printf("Invalid date. Try again.\n");
    } while (!isValidDate(newNode->data.flight_date));

    do {
        printf("Enter Time (HH:MM): ");
        scanf("%s", newNode->data.flight_time);
        if (!isValidTime(newNode->data.flight_time))
            printf("Invalid time. Try again.\n");
    } while (!isValidTime(newNode->data.flight_time));

    strcpy(newNode->data.airplaneID, planes[selected].airplaneID);
    strcpy(newNode->data.airplaneModel, planes[selected].model);
    newNode->data.firstClassSeats = planes[selected].firstClassSeats;
    newNode->data.businessClassSeats = planes[selected].businessClassSeats;
    newNode->data.economyClassSeats = planes[selected].economyClassSeats;
    newNode->data.capacity = planes[selected].totalSeats;
    newNode->data.seatsAvailable = planes[selected].totalSeats;

    printf("Enter Base Ticket Price: ");
    scanf("%f", &newNode->data.price);

    newNode->next = flightHead;
    flightHead = newNode;

    saveFlights();

    printf("\nFlight added successfully! Press Enter to continue...");
    getchar(); getchar();
}


void removeFlight() {
    clearScreen();
    printf("==================================================\n");
    printf("                  REMOVE FLIGHT                   \n");
    printf("==================================================\n");
    if (!flightHead) {
        printf("No flights to remove. Press Enter...");
        getchar(); getchar();
        return;
    }
    printf("%-8s %-8s %-8s %-12s %-6s %-8s %-20s %-8s %-10s %-10s %-12s %-12s %-12s\n",
        "FlightID", "From", "To", "Date", "Time", "PlaneID", "Model", "First", "Business", "Economy", "Price", "Capacity", "Available");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");
    FlightNode* ptr = flightHead;
    int editableFound = 0;
    while (ptr) {
        if (ptr->data.seatsAvailable == ptr->data.capacity) {
            editableFound = 1;
            Flight f = ptr->data;
            printf("%-8d %-8s %-8s %-12s %-6s %-8s %-20s %-8d %-10d %-10d %-12.2f %-12d %-12d\n",
                f.flightID, f.departure, f.destination, f.flight_date, f.flight_time,
                f.airplaneID, f.airplaneModel, f.firstClassSeats, f.businessClassSeats,
                f.economyClassSeats, f.price, f.capacity, f.seatsAvailable);
        }
        ptr = ptr->next;
    }

    if (!editableFound) {
        printf("\nNo editable flights found (only fully unbooked flights can be edited). Press Enter...");
        getchar(); getchar();
        return;
    }
    

    int id;
    printf("Enter Flight ID to remove: ");
    scanf("%d", &id);

    FlightNode *current = flightHead, *prev = NULL;
    while (current) {
        if (current->data.flightID == id) {
            if (!prev) flightHead = current->next;
            else prev->next = current->next;
            free(current);
            saveFlights();
            printf("\nFlight removed successfully! Press Enter...");
            getchar(); getchar();
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("\nFlight ID not found. Press Enter...");
    getchar(); getchar();
}

void editFlight() {
    clearScreen();
    printf("==================================================\n");
    printf("                  EDIT FLIGHT                     \n");
    printf("==================================================\n");
    if (!flightHead) {
        printf("No flights available. Press Enter...");
        getchar(); getchar();
        return;
    }

    // Show all flights first
    printf("%-8s %-8s %-8s %-12s %-6s %-8s %-20s %-8s %-10s %-10s %-12s %-12s %-12s\n",
        "FlightID", "From", "To", "Date", "Time", "PlaneID", "Model", "First", "Business", "Economy", "Price", "Capacity", "Available");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");
    FlightNode* ptr = flightHead;
    int editableFound = 0;
    while (ptr) {
        if (ptr->data.seatsAvailable == ptr->data.capacity) {
            editableFound = 1;
            Flight f = ptr->data;
            printf("%-8d %-8s %-8s %-12s %-6s %-8s %-20s %-8d %-10d %-10d %-12.2f %-12d %-12d\n",
                f.flightID, f.departure, f.destination, f.flight_date, f.flight_time,
                f.airplaneID, f.airplaneModel, f.firstClassSeats, f.businessClassSeats,
                f.economyClassSeats, f.price, f.capacity, f.seatsAvailable);
        }
        ptr = ptr->next;
    }

    if (!editableFound) {
        printf("\nNo editable flights found (only fully unbooked flights can be edited). Press Enter...");
        getchar(); getchar();
        return;
    }

    int id;
    printf("\nEnter Flight ID to edit: ");
    scanf("%d", &id);
    getchar(); // Clear newline left by scanf

    FlightNode* current = flightHead;
    while (current) {
        if (current->data.flightID == id) {
            clearScreen();
            printf("==================================================\n");
            printf("              EDITING SELECTED FLIGHT             \n");
            printf("==================================================\n");
                
                printf("%-8s %-8s %-8s %-12s %-6s %-8s %-20s %-8s %-10s %-10s %-12s %-12s %-12s\n",
                    "FlightID", "From", "To", "Date", "Time", "PlaneID", "Model", "First", "Business", "Economy", "Price", "Capacity", "Available");
                printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
            
                FlightNode* current = flightHead;
                    Flight f = current->data;
                    printf("%-8d %-8s %-8s %-12s %-6s %-8s %-20s %-8d %-10d %-10d %-12.2f %-12d %-12d\n",
                        f.flightID, f.departure, f.destination, f.flight_date, f.flight_time,
                        f.airplaneID, f.airplaneModel, f.firstClassSeats, f.businessClassSeats,
                        f.economyClassSeats, f.price, f.capacity, f.seatsAvailable);
      
            
                char input[100];

                // Change Destination
                printf("Enter new Destination or press Enter to keep [%s]: ", current->data.destination);
                fgets(input, sizeof(input), stdin);
                if (input[0] != '\n') {
                    input[strcspn(input, "\n")] = 0;
                    if (strlen(input) > 0) strcpy(current->data.destination, input);
                }
    
                // Change Airplane
                Plane planes[100];
                int planeCount = 0;
                loadPlanes(planes, &planeCount);
    
                printf("\n%-4s %-12s %-25s %-8s %-10s %-8s\n", "No.", "AirplaneID", "Model", "First", "Business", "Economy");
                printf("-------------------------------------------------------------------------------\n");
                for (int i = 0; i < planeCount; i++) {
                    if (strcmp(planes[i].status, "Available") == 0) {
                        printf("%-4d %-12s %-25s %-8d %-10d %-8d\n",
                            i + 1, planes[i].airplaneID, planes[i].model,
                            planes[i].firstClassSeats, planes[i].businessClassSeats, planes[i].economyClassSeats);
                    }
                }
                int planeChoice;
                printf("\nSelect new plane number or press 0 to keep current: ");
                scanf("%d", &planeChoice);
                getchar();
                if (planeChoice > 0 && planeChoice <= planeCount && strcmp(planes[planeChoice - 1].status, "Available") == 0) {
                    Plane chosen = planes[planeChoice - 1];
                    strcpy(current->data.airplaneID, chosen.airplaneID);
                    strcpy(current->data.airplaneModel, chosen.model);
                    current->data.firstClassSeats = chosen.firstClassSeats;
                    current->data.businessClassSeats = chosen.businessClassSeats;
                    current->data.economyClassSeats = chosen.economyClassSeats;
                    current->data.capacity = chosen.totalSeats;
                    current->data.seatsAvailable = chosen.totalSeats; // reset availability
                }
    
                getchar(); // flush
                // Edit date
                do {
                    printf("\nEnter new Date (YYYY-MM-DD) or press Enter to keep [%s]: ", current->data.flight_date);
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        input[strcspn(input, "\n")] = 0;
                        if (isValidDate(input)) {
                            strcpy(current->data.flight_date, input);
                            break;
                        } else {
                            printf("Invalid date. Try again.\n");
                        }
                    } else {
                        break;
                    }
                } while (1);
    
                // Edit time
                do {
                    printf("Enter new Time (HH:MM) or press Enter to keep [%s]: ", current->data.flight_time);
                    fgets(input, sizeof(input), stdin);
                    if (input[0] != '\n') {
                        input[strcspn(input, "\n")] = 0;
                        if (isValidTime(input)) {
                            strcpy(current->data.flight_time, input);
                            break;
                        } else {
                            printf("Invalid time. Try again.\n");
                        }
                    } else {
                        break;
                    }
                } while (1);
    
                // Edit price
                printf("Enter new Price or press Enter to keep [%.2f]: ", current->data.price);
                fgets(input, sizeof(input), stdin);
                if (input[0] != '\n') {
                    float newPrice = atof(input);
                    if (newPrice > 0) current->data.price = newPrice;
                }
    
                saveFlights();
                printf("\nFlight updated successfully. Press Enter...");
                getchar();
                return;
            }
            current = current->next;
        }
    
        printf("Flight ID not found. Press Enter...");
        getchar(); getchar();
}
void viewFlights() {
    clearScreen();
    printf("==================================================\n");
    printf("                   VIEW FLIGHTS                   \n");
    printf("==================================================\n");

    if (!flightHead) {
        printf("No flights available. Press Enter...");
        getchar(); getchar();
        return;
    }

    printf("%-8s %-8s %-8s %-12s %-6s %-8s %-20s %-8s %-10s %-10s %-12s %-12s %-12s\n",
        "FlightID", "From", "To", "Date", "Time", "PlaneID", "Model", "First", "Business", "Economy", "Price", "Capacity", "Available");
        printf("------------------------------------------------------------------------------------------------------------------------------------------------\n");

    FlightNode* current = flightHead;
    while (current) {
        Flight f = current->data;
        printf("%-8d %-8s %-8s %-12s %-6s %-8s %-20s %-8d %-10d %-10d %-12.2f %-12d %-12d\n",
            f.flightID, f.departure, f.destination, f.flight_date, f.flight_time,
            f.airplaneID, f.airplaneModel, f.firstClassSeats, f.businessClassSeats,
            f.economyClassSeats, f.price, f.capacity, f.seatsAvailable);
        current = current->next;
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
 
    FlightNode* ptr = flightHead;
    int count = 0;
    printf("%-4s %-10s %-8s %-8s %-12s %-6s %-10s %-12s\n", "No", "FlightID", "From", "To", "Date", "Time", "PlaneID", "Available");
    printf("-------------------------------------------------------------------------------\n");
    while (ptr) {
        printf("%-4d %-10d %-8s %-8s %-12s %-6s %-10s %-12d\n",
            count + 1, ptr->data.flightID, ptr->data.departure,
            ptr->data.destination, ptr->data.flight_date,
            ptr->data.flight_time, ptr->data.airplaneID, ptr->data.seatsAvailable);
        ptr = ptr->next;
        count++;
    }

    if (count == 0) {
        printf("No flights available. Press Enter...");
        getchar(); getchar();
        return;
    }

    int choice;
    printf("\nSelect a flight by number (1-%d): ", count);
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > count) {
        printf("Invalid flight selection. Press Enter...");
        getchar(); getchar();
        return;
    }

    // Get selected flight
    FlightNode* selected = flightHead;
    for (int i = 1; i < choice; i++) selected = selected->next;

    // Reuse booking UI and seat map logic
    chooseClassAndSeat(selected);

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
 
    if (!passengerHead) {
        printf("No passengers found.\n");
        printf("Press Enter to return...");
        getchar(); getchar();
        return;
    }

    // Step 1: Show flights with bookings
    int shown = 0;
    FlightNode* f = flightHead;
    printf("Flights with bookings:\n");
    printf("%-10s %-8s %-8s %-12s %-6s\n", "FlightID", "From", "To", "Date", "Time");
    printf("----------------------------------------------------------\n");

    while (f) {
        PassengerNode* p = passengerHead;
        int found = 0;
        while (p) {
            if (p->data.flightID == f->data.flightID) {
                found = 1;
                break;
            }
            p = p->next;
        }

        if (found) {
            printf("%-10d %-8s %-8s %-12s %-6s\n",
                   f->data.flightID, f->data.departure,
                   f->data.destination, f->data.flight_date,
                   f->data.flight_time);
            shown++;
        }
        f = f->next;
    }

    if (shown == 0) {
        printf("\nNo flights with passengers.\n");
        printf("Press Enter to return...");
        getchar(); getchar();
        return;
    }

    // Step 2: Ask for Flight ID
    int targetID;
    printf("\nEnter Flight ID to view passengers: ");
    scanf("%d", &targetID);
    getchar();

    // Step 3: Ask sorting preference
    int sortOption;
    printf("\nChoose sort option:\n");
    printf("1. Sort by PassengerID (default)\n");
    printf("2. Group by Class\n");
    printf("Enter your choice: ");
    scanf("%d", &sortOption);
    getchar();

    // Step 4: Copy matching passengers into a temporary array for sorting
    PassengerNode* p = passengerHead;
    Passenger matching[500]; // Max 500 passengers
    int count = 0;

    while (p) {
        if (p->data.flightID == targetID) {
            matching[count++] = p->data;
        }
        p = p->next;
    }

    clearScreen();

    if (count == 0) {
        printf("No passengers found for Flight ID %d.\n", targetID);
        printf("\nPress Enter to return...");
        getchar();
        return;
    }

    printf("Total passengers for Flight %d: %d\n", targetID, count);

    // Sort by passengerID before display
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (matching[j].passengerID > matching[j + 1].passengerID) {
                Passenger temp = matching[j];
                matching[j] = matching[j + 1];
                matching[j + 1] = temp;
            }
        }
    }

    if (sortOption == 2) {
        // Grouped by class: First, Business, Economy
        const char* classes[] = {"First", "Business", "Economy"};
        for (int c = 0; c < 3; c++) {
            int classFound = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(matching[i].classType, classes[c]) == 0) {
                    classFound = 1;
                    break;
                }
            }
            if (!classFound) continue;

            printf("\n%s Class Passengers:\n", classes[c]);
            printf("%-4s %-10s %-20s %-4s %-6s %-12s %-10s %-6s %-30s\n",
                   "ID", "First", "Last", "Sex", "Seat", "Luggage", "Meal", "Wifi", "Email");
            printf("--------------------------------------------------------------------------------------------------------\n");
            for (int i = 0; i < count; i++) {
                if (strcmp(matching[i].classType, classes[c]) == 0) {
                    printf("%-4d %-10s %-20s %-4s %-6s %-12s %-10s %-6s %-30s\n",
                           matching[i].passengerID,
                           matching[i].firstName,
                           matching[i].lastName,
                           matching[i].gender,
                           matching[i].seatNumber,
                           matching[i].luggageSize,
                           matching[i].mealPreference,
                           matching[i].wifiPreference,
                           matching[i].email);
                }
            }
        }
    } else {
        printf("Passengers for Flight %d:\n", targetID);
        printf("%-4s %-10s %-20s %-4s %-6s %-8s %-12s %-10s %-6s %-30s\n",
               "ID", "First", "Last", "Sex", "Seat", "Class", "Luggage", "Meal", "Wifi", "Email");
        printf("---------------------------------------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < count; i++) {
            printf("%-4d %-10s %-20s %-4s %-6s %-8s %-12s %-10s %-6s %-30s\n",
                   matching[i].passengerID,
                   matching[i].firstName,
                   matching[i].lastName,
                   matching[i].gender,
                   matching[i].seatNumber,
                   matching[i].classType,
                   matching[i].luggageSize,
                   matching[i].mealPreference,
                   matching[i].wifiPreference,
                   matching[i].email);
        }
    }

    printf("\nPress Enter to return...");
    getchar();
}

