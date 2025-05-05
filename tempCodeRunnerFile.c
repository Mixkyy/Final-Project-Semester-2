PassengerNode* newNode = (PassengerNode*)malloc(sizeof(PassengerNode));
        newNode->data = p;
        newNode->next = passengerHead;
        passengerHead = newNode;