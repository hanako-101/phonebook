#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "phonebook.txt"//le nom du fichier où nos contacts seront sauvegardés

typedef struct Contact {
    char *name;
    char country_code[4];
    long int mobile_no;
    char *gender;
    char *email;
} Contact;

typedef struct Node {
    Contact *contact;
    struct Node *next;
} Node;

Node *head = NULL;

void loadContacts() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    char name[50], country_code[4], gender[10], email[50];
    long int mobile_no;
    while (fscanf(file, "%49s %3s %ld %9s %49s", name, country_code, &mobile_no, gender, email) == 5) {
        Contact *newContact = (Contact *)malloc(sizeof(Contact));//Allocation mémoire
        newContact->name = strdup(name);
        strcpy(newContact->country_code, country_code);
        newContact->mobile_no = mobile_no;
        newContact->gender = strdup(gender);
        newContact->email = strdup(email);

        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->contact = newContact;
        newNode->next = head;
        head = newNode;
    }

    fclose(file);
}

void saveContacts() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        perror("Unable to open file for saving");
        return;
    }

    Node *current = head;
    while (current) {
        fprintf(file, "%s %s %ld %s %s\n", current->contact->name, current->contact->country_code,
                current->contact->mobile_no, current->contact->gender, current->contact->email);
        current = current->next;
    }

    fclose(file);
}

int phoneExists(const char *country_code, long int mobile_no) {
    Node *current = head;
    while (current != NULL) {
        if (strcmp(current->contact->country_code, country_code) == 0 && current->contact->mobile_no == mobile_no) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void addContact() {
    char name[50], country_code[4], gender[10], email[50];
    long int mobile_no;
    printf("Enter name: ");
    scanf("%49s", name);
    printf("Enter country code: ");
    scanf("%3s", country_code);
    printf("Enter mobile number: ");
    scanf("%ld", &mobile_no);
    printf("Enter gender: ");
    scanf("%9s", gender);
    printf("Enter email: ");
    scanf("%49s", email);

    if (phoneExists(country_code, mobile_no)) {
        printf("Phone number already exists.\n");
        return;
    }

    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    newContact->name = strdup(name);
    strcpy(newContact->country_code, country_code);
    newContact->mobile_no = mobile_no;
    newContact->gender = strdup(gender);
    newContact->email = strdup(email);

    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->contact = newContact;
    newNode->next = head;
    head = newNode;

    saveContacts();
}

void deleteContact() {
    char name[50];
    printf("Enter name of contact to delete: ");
    scanf("%49s", name);

    Node *current = head;
    Node *previous = NULL;

    while (current != NULL && strcmp(current->contact->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Contact not found.\n");
        return;
    }

    if (previous == NULL) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current->contact->name);
    free(current->contact->gender);
    free(current->contact->email);
    free(current->contact);
    free(current);

    saveContacts();
    printf("Contact deleted.\n");
}

void modifyContact() {
    char name[50];
    printf("Enter name of contact to modify: ");
    scanf("%49s", name);

    Node *current = head;
    while (current != NULL && strcmp(current->contact->name, name) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Contact not found.\n");
        return;
    }

    char country_code[4], gender[10], email[50];
    long int mobile_no;
    printf("Enter new country code: ");
    scanf("%3s", country_code);
    printf("Enter new mobile number: ");
    scanf("%ld", &mobile_no);
    printf("Enter new gender: ");
    scanf("%9s", gender);
    printf("Enter new email: ");
    scanf("%49s", email);

    if (phoneExists(country_code, mobile_no)) {
        printf("Phone number already exists.\n");
        return;
    }

    free(current->contact->gender);
    free(current->contact->email);
    strcpy(current->contact->country_code, country_code);
    current->contact->mobile_no = mobile_no;
    current->contact->gender = strdup(gender);
    current->contact->email = strdup(email);

    saveContacts();
    printf("Contact modified.\n");
}

void displayContacts() {
    Node *current = head;
    if (current == NULL) {
        printf("No contacts.\n");
        return;
    }

    printf("\n\t\t*************************** Contacts ****************************\n");
    while (current) {
        printf("\t\t*---------------------------------------------------------------*\n");
        printf("\t\t| Name: %-50s |\n", current->contact->name);
        printf("\t\t| Country Code: %-45s |\n", current->contact->country_code);
        printf("\t\t| Mobile Number: %-44ld |\n", current->contact->mobile_no);
        printf("\t\t| Sex: %-52s |\n", current->contact->gender);
        printf("\t\t| Email: %-50s |\n", current->contact->email);
        printf("\t\t*---------------------------------------------------------------*\n");
        current = current->next;
    }
    printf("\t\t*****************************************************************\n\n");
}

void searchContact() {
    char country_code[4];
    long int mobile_no;
    printf("Enter country code: ");
    scanf("%3s", country_code);
    printf("Enter mobile number: ");
    scanf("%ld", &mobile_no);

    Node *current = head;
    while (current != NULL && (strcmp(current->contact->country_code, country_code) != 0 || current->contact->mobile_no != mobile_no)) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Contact not found.\n");
    } else {
        printf("\n\t\t************************** Contact ******************************\n");
        printf("\t\t*---------------------------------------------------------------*\n");
        printf("\t\t| Name: %-50s |\n", current->contact->name);
        printf("\t\t| Country Code: %-45s |\n", current->contact->country_code);
        printf("\t\t| Mobile Number: %-44ld |\n", current->contact->mobile_no);
        printf("\t\t| Sex: %-52s |\n", current->contact->gender);
        printf("\t\t| Email: %-50s |\n", current->contact->email);
        printf("\t\t*---------------------------------------------------------------*\n");
        printf("\t\t*****************************************************************\n\n");
    }
}

void deleteAllContacts() {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;

        free(temp->contact->name);
        free(temp->contact->gender);
        free(temp->contact->email);
        free(temp->contact);
        free(temp);
    }
    head = NULL;
    saveContacts();
    printf("All contacts deleted.\n");
}

void freeMemory() {
    deleteAllContacts();

}

void displayMenu() {
    printf("\t\t                                *\n");
    printf("\t\t                               **\n");    
    printf("\t\t                              ****\n");
    printf("\t\t                             ******\n");
    printf("\t\t                         **************\n");
    printf("\t\t                        ****************\n");
    printf("\t\t                       ******************\n");
    printf("\t\t                ********************************\n");
    printf("\t\t               ***********  WELCOM **************\n");
    printf("\t\t              ************************************\n");
    printf("\t\t             ********** TO My Phonebook **********\n");
    printf("\t\t            ****************************************\n");
    printf("\t\t********************** Phonebook Menu  ***********************\n");
    printf("\t\t* 1. View All Contacts                                       *\n");
    printf("\t\t* 2. Add Contact                                             *\n");
    printf("\t\t* 3. Remove Contact                                          *\n");
    printf("\t\t* 4. Search Contact by Phone Number                          *\n");
    printf("\t\t* 5. Update Contact Details                                  *\n");
    printf("\t\t* 6. Delete Entire Phonebook                                 *\n");
    printf("\t\t* 7.Exit                                                     *\n");
    printf("\t\t**************************************************************\n\n");
    printf("\t\tEnter your choice: ");
}


int main() {
    loadContacts();

    int choice;
    do {
        system("cls"); // Clear screen for Windows
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                displayContacts();
                break;
            case 2:
                system("cls");
                addContact();
                break;
            case 3:
                system("cls");
                deleteContact();
                break;
            case 4:
                system("cls");
                searchContact();
                break;
            case 5:
                system("cls");
                modifyContact();
                break;
            case 6:
                system("cls");
                deleteAllContacts();
                break;
            case 7:
                freeMemory();
                system("cls");
                printf("\n\t\tExiting...\n");
                break;
            default:
                printf("\n\t\tInvalid choice. Try again.\n");
        }
        printf("\n\t\tPress Enter to continue...");
        getchar();
        getchar();
    } while (choice != 7);

    return 0;
}

