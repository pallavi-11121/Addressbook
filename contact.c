                            #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
int validname(const char *name)
{
    if (strlen(name) == 0)
        return 0;

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalpha(name[i]) && name[i] != ' ') //check it contains only alphabtes or not
        {
            printf("enter a valid name\n");
            return 0;
        }
    }

    return 1;
}

int validphone(AddressBook *addressBook, const char *phone)
{
    if (strlen(phone) != 10) //check lenght must=10
    {
        printf("enter a valid phone no.\n");
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(phone[i])) //check phone number contains only digit
        {
            printf("invalid phoneno\n");
            return 0;
        }
    }
    //check uniq are not
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            printf("This phone no already exist,enter valid phone no\n");
            return 0; // phone already exists
        }
    }

    return 1;
}

int validmail(AddressBook *addressBook,const char *email)
{
    int count=0;
    int index=-1;
    int len=strlen(email);
    for(int i=0 ;i<len;i++)
    {
        if(email[i]=='@')
        {
            count++;
            index=i;
        }
    }
    if((count!=1 )||                             //@ only one
       (index<=0 || index>=len-1)||             //@ shouldn,t in first and last position
       (strstr(email+index+1,".com")==NULL)||   //CHECKS .COM COMES AFTER @
       (strchr(email, ' ') != NULL))     //no spaces in email
    {
        printf("enter a valid emailid\n");
        return 0;
    }
    //to check uniquniess
    
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0) 
        {
            printf("This email is already in use.\n");
            return 0;

        }
    }
    //printf("valid emailid:%s",email);
    return 1;
}


void listContacts(AddressBook *addressBook, int sortCriteria)
{
    Contact temp;
    if (addressBook->contactCount == 0)
    {
        printf("contact not found");
        return;
    }
    // sorting contacts based on the choosen criteria
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++) //bubble sort
        {
            int cmp = 0;
            if (sortCriteria == 1)
            {
                cmp = strcmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name);
            }
            else if (sortCriteria == 2)
            {
                cmp = strcmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone);
            }
            else if (sortCriteria == 3)
            {
                cmp = strcmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email);
            }
            if (cmp > 0)          //if cmp>0 means swap values
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
     // Print sorted contact list
    printf("sorted contact list\n");
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf(" %s\t", addressBook->contacts[i].name);
        printf(" %s\t", addressBook->contacts[i].phone);
        printf(" %s\n", addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    // populateAddressBook(addressBook);

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

void createContact(AddressBook *addressBook)
{
    char name[50], phone[11], email[50];
    int result = 0;
    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("Address book is full.\n");
        return;
    }
    do
    {
        printf("enter the name:");
        scanf(" %[^\n]", name);
        result = validname(name);
    } while (result == 0);
    do
    {

        printf("enter phone number:");
        scanf("%s",phone);

        result = validphone(addressBook, phone);

    } while (result == 0);

    do
    {
        printf("enter the emailid:");
        scanf("%s", email);
        while ((getchar()) != '\n');
        result = validmail(addressBook, email);

    } while (result == 0);
    // printf("contact added successfully\n");l

    // for saving details
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);
    addressBook->contactCount++;
    printf("contact created successfully:\n");
}
void searchContact(AddressBook *addressBook)
{
    char searchInput[50];
    int choice, found = 0;

    printf("Enter your choice to search by:\n1. Name\n2. Phone\n3. Email\n");
    scanf("%d", &choice);
    getchar();
    // newline
    printf("enter the existing value to search:");
    scanf("%s", searchInput);

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        char fieldToSearch[100];
        if (choice == 1)
            strcpy(fieldToSearch, addressBook->contacts[i].name);
        else if (choice == 2)
            strcpy(fieldToSearch, addressBook->contacts[i].phone);
        else if (choice == 3)
            strcpy(fieldToSearch, addressBook->contacts[i].email);
        else
            continue; //skip invalid choice

    // Check if search input is found in the selected field
    if (strstr(fieldToSearch, searchInput) != NULL)
        {
            // Print contact if match found
            printf("Contact found: %s, %s, %s\n",
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            found = 1;
        }
    }  

    if (!found)
    {
        printf("Contact not found.\n");
    }
}

void editContact(AddressBook *addressBook)
{
    char searchInput[50];
    int choice, matchcount = 0;
    int matches[50]; // Store indixes of matched contacts
    int editindex;

    printf("Enter your choice to search by:\n1. Name\n2. Phone\n3. Email\n");
    scanf("%d", &choice);
    getchar(); // Clear newline

    printf("Enter the value to search: ");
    scanf(" %[^\n]", searchInput);

    // Search matching contacts
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, searchInput) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, searchInput) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, searchInput) == 0))
        {
            matches[matchcount++] = i;    // Save matching contact index
        }
    }

    if (matchcount == 0)
    {
        printf("No contact found.\n");
        return;
    }
    //one match

    if (matchcount == 1)
    {
        editindex = matches[0];  // Directly select that contact

    }
    else
    {
        printf("Multiple contacts found:\n"); //for multiple contacts
        for (int i = 0; i < matchcount; i++)
        {
            int idx = matches[i];
            printf("%d. Name: %s | Phone: %s | Email: %s\n", 
                   i + 1, 
                   addressBook->contacts[idx].name,
                   addressBook->contacts[idx].phone,
                   addressBook->contacts[idx].email);
        }
        // Ask user to select which contact to edit
        int selection;
        printf("Enter the number of the contact you want to edit: ");
        scanf("%d", &selection);
        getchar();

        if (selection < 1 || selection > matchcount)
        {
            printf("Invalid selection.\n");
            return;
        }
        editindex = matches[selection - 1];
    }

    // Print selected contact details
    printf("\nSelected Contact:\n");
    printf("Name : %s\n", addressBook->contacts[editindex].name);
    printf("Phone: %s\n", addressBook->contacts[editindex].phone);
    printf("Email: %s\n\n", addressBook->contacts[editindex].email);

    //  Ask what to edit
    int fieldchoice, isvalid;
    printf("What do you want to edit?\n1. Name\n2. Phone\n3. Email\n");
    scanf("%d", &fieldchoice);
    getchar();
     //edit name
    if (fieldchoice == 1)  
    {
        char newName[50];
        do
        {
            printf("Enter new name: ");
            scanf(" %[^\n]", newName);

            isvalid = validname(newName);
            if (!isvalid)
                printf("Invalid name. Try again.\n");
            else
                strcpy(addressBook->contacts[editindex].name, newName);

        } while (!isvalid);
    }
    //edit phoneno
    else if (fieldchoice == 2) 
    {
        char newPhone[11];
        do
        {
            printf("Enter new phone number: ");
            scanf("%s", newPhone);

            isvalid = validphone(addressBook, newPhone);
            if (!isvalid)
                printf("Invalid phone. Try again.\n");
            else
                strcpy(addressBook->contacts[editindex].phone, newPhone);

        } while (!isvalid);
    }
    //edit email
    else if (fieldchoice == 3) 
    {
        char newEmail[50];
        do
        {
            printf("Enter new email: ");
            scanf("%s", newEmail);

            isvalid = validmail(addressBook, newEmail);
            if (!isvalid)
                printf("Invalid email. Try again.\n");
            else
                strcpy(addressBook->contacts[editindex].email, newEmail);

        } while (!isvalid);
    }
    else
    {
        printf("Invalid option.\n");
        return;
    }

    printf("Contact updated successfully.\n");
}

void deleteContact(AddressBook *addressBook)
{
    char searchInput[50];
    int choice, found = 0;
    int matches[50];
    int matchcount = 0;
    int delindex;

    printf("Enter your choice to delete by:\n1. Name\n2. Phone\n3. Email\n");
    scanf("%d", &choice);
    getchar(); // consume newline

    printf("Enter the existing value to delete: ");
    scanf(" %[^\n]", searchInput);

    // Step 1: Find all matches
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, searchInput) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, searchInput) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, searchInput) == 0))
        {
            matches[matchcount++] = i;
        }
    }

    // Step 2: Handle cases
    if (matchcount == 0)
    {
        printf("No contact found.\n");
        return;
    }
    //only one match
    else if (matchcount == 1)
    {
        delindex = matches[0];
    }
    else
    {
        //display multiple contacts
        printf("Multiple contacts found:\n");
        for (int i = 0; i < matchcount; i++)
        {
            int index = matches[i];
            printf("%d. Name: %s | Phone: %s | Email: %s\n",
                   i + 1,
                   addressBook->contacts[index].name,
                   addressBook->contacts[index].phone,
                   addressBook->contacts[index].email);
        }
        
        int selection;
        printf("Enter the number of the contact you want to delete: ");
        scanf("%d", &selection);

        if (selection < 1 || selection > matchcount)
        {
            printf("Invalid selection. No contact deleted.\n");
            return;
        }

        delindex = matches[selection - 1];
    }
    printf("\nSelected Contact:\n");
    printf("Name : %s\n", addressBook->contacts[delindex].name);
    printf("Phone: %s\n", addressBook->contacts[delindex].phone);
    printf("Email: %s\n\n", addressBook->contacts[delindex].email);

    //ask user confirmation before deleting
    char confirm;
    printf("Are you sure you want to delete this contact? (y/n): ");
    getchar(); // consume newline left in buffer
    scanf("%c", &confirm);

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Deletion cancelled.\n");
        return;
    }


    // Step 3: Delete by shifting
    for (int j = delindex; j < addressBook->contactCount - 1; j++)
    {
        addressBook->contacts[j] = addressBook->contacts[j + 1];
    }
    addressBook->contactCount--;
    printf("Contact deleted successfully.\n");
}
