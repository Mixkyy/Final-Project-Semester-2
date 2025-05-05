# Flight Reservation System in C

A terminal-based Flight Reservation System built in the C programming language. This project supports both customer and owner modes with functionalities for booking flights, managing passenger records, and navigating flight routes using graph algorithms.

## Features

### Customer Mode
- Search for available flights by destination
- View and select connecting flight routes
- Choose seat class: First, Business, or Economy
- Interactive seat map with availability
- Enter booking information and preferences (luggage, meal, WiFi)
- View past bookings via email lookup

### Owner Mode
- Add, edit, remove, and list available flights
- Manage passenger records by flight
- View passengers grouped by class

## Technologies Used

- C Programming Language
- Linked Lists for flight and passenger records
- Binary Search Tree (BST) for destination-based flight searching
- Graph + Dijkstra’s Algorithm for shortest path between airports
- CSV File I/O for persistent data storage

## Example Input/Output

### Customer Booking Example

```
WELCOME TO FLIGHT RESERVATION

1. Customer
2. Owner
3. Exit
Enter your choice: 1

SELECT ROUTE
Available airports: BKK, HKT, SYD, NRT, GRU, BER, YYZ

Enter your departure airport: BKK
Enter your destination airport: YYZ
Finding shortest route...

Shortest path: BKK -> BER -> YYZ
Total Distance: 14900 km
```

```
Available Flights from BKK to BER on 2025-06-01:
FlightID  From   To     Date         Time   PlaneID  Available
1001      BKK    BER    2025-06-01   08:00  A320     180

Enter Flight ID to book (0 to cancel): 1001
```

```
SELECT CLASS TYPE
1. First Class
2. Business Class
3. Economy Class
Enter your choice: 3

SEAT MAP - ECONOMY CLASS
[A] [B] [C]    [D] [E] [F]    [G] [H] [I]
[ ] [X] [ ]    [ ] [ ] [ ]    [ ] [X] [ ]

Enter your seat: C1
Enter luggage size (carry-on, medium, large): medium
Meal preference (standard, vegan, halal): vegan
Do you want WiFi? (yes/no): yes
Booking confirmed!
```

### Owner Add Flight Example

```
OWNER MENU

1. Flights
2. Passengers
3. Back to Main Menu
Enter your choice: 1

FLIGHT MENU

1. Add Flight
2. Remove Flight
3. Edit Flight
4. View Flights
Enter your choice: 1
```

```
Available Planes:
No.  AirplaneID   Model             First  Business  Economy
1    A320         Airbus A320       8      24        150

Select a plane: 1
Enter Flight ID: 2001
Enter Departure: BKK
Enter Destination: NRT
Enter Date (YYYY-MM-DD): 2025-06-10
Enter Time (HH:MM): 09:00
Enter Base Ticket Price: 3500.00

Flight added successfully!
```

## File Structure

- `main.c` — Main application source code
- `flights.csv` — Stores all flight records
- `passengers.csv` — Stores booking information
- `plane.csv` — List of airplane models and seating
- `history.csv` — Archived booking data
