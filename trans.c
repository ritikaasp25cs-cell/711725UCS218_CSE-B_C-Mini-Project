#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define PASSWORD "ritika@123"

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function Prototypes
void login();
unsigned int enterChoice(void);

void initializeFile(FILE *fPtr);
void textFile(FILE *readPtr);

void newRecord(FILE *fPtr);
void updateRecord(FILE *fPtr);
void withdrawAmount(FILE *fPtr);
void transferAmount(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void displayAll(FILE *fPtr);
void searchRecord(FILE *fPtr);

void totalBalance(FILE *fPtr);
void richestAccount(FILE *fPtr);

// MAIN FUNCTION
int main()
{
    FILE *cfPtr;
    unsigned int choice;

    login();

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            exit(1);
        }

        initializeFile(cfPtr);
    }

    while ((choice = enterChoice()) != 11)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            withdrawAmount(cfPtr);
            break;

        case 5:
            transferAmount(cfPtr);
            break;

        case 6:
            deleteRecord(cfPtr);
            break;

        case 7:
            displayAll(cfPtr);
            break;

        case 8:
            searchRecord(cfPtr);
            break;

        case 9:
            totalBalance(cfPtr);
            break;

        case 10:
            richestAccount(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("Program Closed Successfully.\n");

    return 0;
}

// PASSWORD LOGIN
void login()
{
    char pass[20];

    printf("=========== BANK LOGIN ===========\n");

    do
    {
        printf("Enter Password: ");
        scanf("%s", pass);

        if (strcmp(pass, PASSWORD) != 0)
        {
            printf("Wrong Password! Try Again.\n");
        }

    } while (strcmp(pass, PASSWORD) != 0);

    printf("Login Successful.\n");
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n=========== MENU ===========\n");
    printf("1  - Export to Text File\n");
    printf("2  - Deposit / Update Account\n");
    printf("3  - Add New Account\n");
    printf("4  - Withdraw Amount\n");
    printf("5  - Transfer Amount\n");
    printf("6  - Delete Account\n");
    printf("7  - Display All Accounts\n");
    printf("8  - Search Account\n");
    printf("9  - Display Total Bank Balance\n");
    printf("10 - Richest Account\n");
    printf("11 - Exit\n");

    printf("Enter Choice: ");
    scanf("%u", &choice);

    return choice;
}

// INITIALIZE FILE
void initializeFile(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};

    rewind(fPtr);

    for (int i = 0; i < MAX_RECORDS; i++)
    {
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);
    }

    printf("File Initialized Successfully.\n");
}

// EXPORT TO TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("Unable to create text file.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-15s%-15s%s\n",
            "Acct", "LastName", "FirstName", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-15s%-15s%.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("Accounts Exported to accounts.txt\n");
}

// ADD NEW ACCOUNT
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter New Account Number (1-100): ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX_RECORDS)
    {
        printf("Invalid Account Number.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account Already Exists.\n");
        return;
    }

    printf("Enter Last Name First Name Balance: ");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("New Account Added Successfully.\n");
}

// DEPOSIT / UPDATE
void updateRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);

    printf("Enter Deposit Amount: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Amount Deposited Successfully.\n");
}

// WITHDRAW FEATURE
void withdrawAmount(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);

    printf("Enter Withdraw Amount: ");
    scanf("%lf", &amount);

    if (amount > client.balance)
    {
        printf("Insufficient Balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Withdrawal Successful.\n");
}

// TRANSFER FEATURE
void transferAmount(FILE *fPtr)
{
    struct clientData sender, receiver;

    unsigned int fromAcc, toAcc;
    double amount;

    printf("Enter Sender Account: ");
    scanf("%u", &fromAcc);

    printf("Enter Receiver Account: ");
    scanf("%u", &toAcc);

    printf("Enter Amount to Transfer: ");
    scanf("%lf", &amount);

    // Sender
    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&sender, sizeof(struct clientData), 1, fPtr);

    // Receiver
    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&receiver, sizeof(struct clientData), 1, fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid Account Number.\n");
        return;
    }

    if (amount > sender.balance)
    {
        printf("Insufficient Balance.\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    // Update Sender
    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&sender, sizeof(struct clientData), 1, fPtr);

    // Update Receiver
    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&receiver, sizeof(struct clientData), 1, fPtr);

    printf("Transfer Successful.\n");
}

// DELETE ACCOUNT
void deleteRecord(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter Account Number to Delete: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account Deleted Successfully.\n");
}

// DISPLAY ALL ACCOUNTS
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-15s%-15s%s\n",
           "Acct", "LastName", "FirstName", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-15s%-15s%.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// SEARCH ACCOUNT
void searchRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int acc;

    printf("Enter Account Number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account Not Found.\n");
    }
    else
    {
        printf("\nAccount Found\n");

        printf("Account Number : %d\n", client.acctNum);
        printf("Last Name     : %s\n", client.lastName);
        printf("First Name    : %s\n", client.firstName);
        printf("Balance       : %.2f\n", client.balance);
    }
}

// TOTAL BALANCE FEATURE
void totalBalance(FILE *fPtr)
{
    struct clientData client;
    double total = 0;

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        total += client.balance;
    }

    printf("Total Bank Balance = %.2f\n", total);
}

// RICHEST ACCOUNT FEATURE
void richestAccount(FILE *fPtr)
{
    struct clientData client;
    struct clientData richest = {0, "", "", 0.0};

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.balance > richest.balance)
        {
            richest = client;
        }
    }

    if (richest.acctNum == 0)
    {
        printf("No Accounts Found.\n");
    }
    else
    {
        printf("\nRichest Account Details\n");

        printf("Account Number : %d\n", richest.acctNum);
        printf("Name           : %s %s\n",
               richest.firstName,
               richest.lastName);

        printf("Balance        : %.2f\n", richest.balance);
    }
}