                printf("Enter new Destination or press Enter to keep [%s]: ", current->data.destination);
                fgets(input, sizeof(input), stdin);
                if (input[0] != '\n') {
                    input[strcspn(input, "\n")] = 0;  // trim newline
                    if (strlen(input) > 0) {
                        if (isDirectConnection(current->data.departure, input)) {
                            strcpy(current->data.destination, input);
                        } else {
                            printf("✘ Error: '%s' is not a direct connection from '%s'. Destination unchanged.\n", input, current->data.departure);
                            printf("Press Enter to continue...");
                            getchar();
                        }
                    }
                }
