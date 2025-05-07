#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
  
    FILE *fptr;
    fptr=fopen("contacts.csv","w");
    if(fptr==NULL)
    {
        printf("contacts.csv not found\n");
        return;
    }
    for(int i=0;i<addressBook->contactCount;i++)
    {
        fprintf(fptr,"%s,%s,%s\n",addressBook->contacts[i].name,
            addressBook->contacts[i].phone,
            addressBook->contacts[i].email);
    }
    fclose;
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fp;
    fp=fopen("contacts.csv","r");
    if(fp==NULL)
    {
        printf("contacts.csv is not found\n");
        return;
    }
    int i;
    for (i = 0; ; i++)
    {
        if (fscanf(fp, "%[^,],%[^,],%[^\n]\n",
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email) != 3)
        {
            break; // stop if fscanf fails 
        }
    }
    addressBook->contactCount = i;
    fclose(fp);     
}
