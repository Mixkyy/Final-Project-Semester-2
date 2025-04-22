#include <stdio.h>
#include <stdlib.h>

void clearScreen();
void customerMenu();
void ownerMenu();

int main() {
    int choice;

    do {
        clearScreen();

        printf("==================================================\n");
        printf("          WELCOME TO FLIGHT RESERVATION             \n");
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
                printf("\nInvalid choice. Please try again.\n");
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
    clearScreen();
    printf("==================================================\n");
    printf("                    OWNER MENU                    \n");
    printf("==================================================\n");
    printf("Press Enter to return to main menu...");
    getchar(); getchar();
}
