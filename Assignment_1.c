//---Rule---

// Function = ThisIsExample
// Variable = this_is_example
// Structure = thisIsExample

// Dont use Abbreviation, unless it is common
// Descripe as detail as possible */

#include <stdio.h>
#include <stdlib.h> // fixed system cls
#include <string.h>
#include <math.h>  // for isnan() function
#include <conio.h> // getch() function

int current_active_user = -1;
int registered_user = 0;
double temp_value = 0.0;

char edit_menu[8][40] = {"Back/Return", "Edit Age", "Edit Martial Status", "Edit Annual Income", "Edit Number of Children","Edit Number of Children Under 18", "Edit Special Healthcare Status"};
char login_menu[3][40] = {"Login", "Register New User", "Exit"};
char dash_menu[7][40] = {"Edit Citizen Info", "Lookup Tax Bracket", "Lookup Citizen Info", "Tax Dashboard", "Print Citizen Info", "Logout", "Exit"};
// char admin_menu[5][40] = {"View All Citizens", "Edit Citizen Info", "Delete Citizen Info", "Logout", "Exit"};


struct ChildAllowance {
    char category[25];
    int minChildren;
    int maxChildren;
    double amount;
};

struct ChildCategory {
    char category[25];
    int minChildren;
    int maxChildren;
    double amount;
};

// Initialize the lookup table based on the assignment sheet
struct ChildAllowance childTable[4] = {
    {"Child allowance A", 1, 2, 8000.0},
    {"Child allowance B", 3, 4, 10000.0},
    {"Child allowance C", 5, 6, 12000.0},
    {"Child allowance D", 7, 99, 14000.0}
};

char childTablestring[4][50] = {
    "Child allowance A: 1-2 children",
    "Child allowance B: 3-4 children",
    "Child allowance C: 5-6 children",
    "Child allowance D: 7 or more children"
};

struct Deduction_Category
{
    int category[3];
    double deductionAmount;
};

struct Citizen
{
    char citizenId[13];
    char citizenName[50];
    char citizenPassword[20];
    int LoginCount;
    int age;
    bool martialStatus;
    double annualIncome;
    int numberOfChildren;
    int childrenUnder18;
    bool specialHealthcare;
    double totalDeduction;
    double taxableIncome;
    double incomeTax;
    struct Deduction_Category deduction[1];
};

struct TaxBracket
{
    double minIncome;
    double maxIncome;
    double taxRate;
};

struct TaxBracket brackets[7] = {
    {0.0, 48000.0, 0.00},
    {48000.0, 60000.0, 0.05},
    {60000.0, 100000.0, 0.07},
    {100000.0, 200000.0, 0.09},
    {200000.0, 500000.0, 0.14},
    {500000.0, 1000000.0, 0.19},
    {1000000.0, 1.7976931348623157E+308, 0.25}};

struct Citizen citizen_list[30];

void RemoveNewLine(char *str)
{
    str[strcspn(str, "\n")] = '\0';
}

void ClearInputBuffer()
{ // Clear the memory that used to store the input temperory
    int c;
    while ((c = getchar()) != '\n')
        ; // Discard characters remaining in the input buffer to prevent skipping next input
}

int CheckInputBlank(char *str)
{ // debug at here, forgot it will return int
    if (strlen(str) == 0)
    {
        return 1; // 1 = true
    }
    for (int i = 0; str[i] != '\0'; i++)
    { // looping until the null pointer behind the string
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            return 0; // ' ' <- space, '\t' <- tab, '\n' <- newline char
    }
    return 1;
}

double CheckValidDouble(char *message)
{ // puting * to wrong (solve)
    double value;
    while (1)
    {
        printf("%s", message);
        if (scanf("%lf", &value) == 1 && value > 0 && value < 2147483647)
        {
            ClearInputBuffer();
            return value;
        }
        else
        {
            printf("Please enter a valid positive number for this field.\n");
            ClearInputBuffer();
        }
    }
    return value;
}

void ClearScreen()
{
    system("cls");
    // clrscr(); cant use (got bug)
    //  printf("\033[H\033[2J"); This method not working (By printing a lot of new line)
    //  fflush(stdout);
}

int SelectOption(char option[][40], int total_choice)
{ // havent conplete yet
    int current_selection = 0;
    int input;
    while (1)
    {
        ClearScreen();
        printf("Please use Arrow key(Up and down to select)\n"); // minor update forgot to put \n
        for (int i = 0; i < total_choice; i++)
        {
            if (i == current_selection)
            {
                printf("-> %s\n", option[i]);
            }
            else
            {
                printf("   %s\n", option[i]);
            }
        }
        int key_pressed = getch(); // force change to ascii
        if (key_pressed == 224)
        { // check the thing is it an unsign character https://stackoverflow.com/questions/13990600/how-to-detect-arrow-keys
            key_pressed = getch();
            if (key_pressed == 72)
                current_selection = (current_selection - 1 + total_choice) % total_choice; // code for up
            if (key_pressed == 80)
                current_selection = (current_selection + 1) % total_choice; // code for down
        }
        else if (key_pressed == 13)
            return current_selection; // code for enter
    }
    /*Press down
    0 + 1 = 1 % 3 = 1
    1 + 1 = 2 % 3 = 2
    2 + 1 = 3 % 3 = 0
    etc
    Press up
    0 - 1 + 3 = 2 % 3 = 2
    2 - 1 + 3 = 4 % 3 = 1
    1 - 1 + 3 = 3 % 3 = 0
    etc
    */
}

double lookupTaxRate(double taxableIncome, struct TaxBracket table[], int size)
{
    // Bracket 0: 0 to 48,000 inclusive
    if (taxableIncome <= table[0].maxIncome)
    {
        return table[0].taxRate;
    }

    for (int i = 1; i < size; i++)
    {
        if (taxableIncome > table[i].minIncome && taxableIncome <= table[i].maxIncome)
        {
            return table[i].taxRate; // found tax
        }
    }
    printf("[!] Error: Taxable income is out of the defined tax brackets.\n");
    return 0; // Return an error value
}

double lookupChildAllowance(int totalChildren, int under18, struct ChildAllowance table[], int size) {
    // Must have children, and ALL children must be below 18
    if (totalChildren <= 0 || totalChildren != under18) {
        return 0.0; 
    }

    // Loop through the table to "look up" the matching bracket
    for (int i = 0; i < size; i++) {
        if (totalChildren >= table[i].minChildren && totalChildren <= table[i].maxChildren) {
            return table[i].amount; // Found the match! Return the RM amount
        }
    }
    return 0.0;
}


void SaveUserData()
{
    //FILE *file_path = fopen("userdata.bin", "wb"); // 'wb' = write binary
     FILE *file_path = fopen("userdata.txt", "w"); // 'wb' = write binary
    if (file_path == NULL)
    {                                       // check weather it is no file
        printf("[!] Error saving data!\n"); // if cant save , print error data
        return;
    }
    // Save the count of users first
    fwrite(&registered_user, sizeof(int), 1, file_path);
    // Save the entire array of profiles
    fwrite(citizen_list, sizeof(struct Citizen), 30, file_path);
    fclose(file_path);
}

void LoadUserData()
{
    //FILE *file_path = fopen("userdata.bin", "rb"); // 'rb' = read binary , if this can be use in the securuty
     FILE *file_path = fopen("userdata.txt", "r"); // use txt file for visualise the data in it
    if (file_path == NULL)
    {           // check weather it is no file
        return; // No file yet, just return without value to exit the funtion
    }
    fread(&registered_user, sizeof(int), 1, file_path);         // size_t fread(void *ptr, size_t size, size_t count, FILE *stream) <- the syntax
    fread(citizen_list, sizeof(struct Citizen), 30, file_path); // read the file according to section ( sizeof(struct userProfile)), read 30 time, and read from file-path then save to student_list
    fclose(file_path);
}

void RegisterUser()
{
    ClearScreen(); // added clear screen
    if (registered_user >= 30)
    {                                            // check the count 30 5 , prevent overflow
        printf("Maximum user limit reached.\n"); // reject add account if already achieve the max number
        getch();                                 // wait till the user want to proceed
        return;
    }

    struct Citizen *new_citizen = &citizen_list[registered_user];
    char temp_input[150]; // Use a larger buffer to catch the overflow to let user enter over 50

    // Get Id
    while (1)
    {
        printf("---Register New User--- (press 0 to cancel)\n"); // update the message to register new user
        printf("Citizen ID (Max 12): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        // Check for Return/Cancel first
        if (strcmp(temp_input, "0\n") == 0)
            return; // if cancel then exit to main menu

        if (strchr(temp_input, '\n') == NULL)
        {
            printf("[!] Error: Invalid ID Number!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        if (strlen(temp_input) != 13)
        {
            printf("[!] Error: Invalid ID Number!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 20 || CheckInputBlank(temp_input))
        {
            printf("[!] Error: Username must be 1-50 characters.\n");
            getch();
            ClearScreen(); // ClearScreen if error message
            continue;
        }

        // Check redundancy id

        strcpy(new_citizen->citizenId, temp_input);
        break;
    }

    // Get Username
    while (1)
    {
        printf("Username (Max 50): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        // Check for Return/Cancel first
        if (strcmp(temp_input, "0\n") == 0)
            return; // if cancel then exit to main menu

        if (strchr(temp_input, '\n') == NULL)
        {
            printf("[!] Error: Username too long!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 20 || CheckInputBlank(temp_input))
        {
            printf("[!] Error: Username must be 1-50 characters.\n");
            getch();
            ClearScreen(); // ClearScreen if error message
            continue;
        }
        strcpy(new_citizen->citizenName, temp_input);
        break;
    }

    // Get Password
    while (1)
    {
        printf("Password (Max 20): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strchr(temp_input, '\n') == NULL)
        {
            printf("[!] Error: Password too long!\n");
            ClearInputBuffer();
            getch();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 20 || CheckInputBlank(temp_input))
        {
            printf("[!] Error: Password must be 1-19 characters.\n");
            ClearInputBuffer();
            getch();
            continue;
        }
        strcpy(new_citizen->citizenPassword, temp_input);
        break;
    }

    registered_user++;
    new_citizen->LoginCount = 0; // add the register user count
    SaveUserData();
    printf("\nNew User Registration Success! \nPress any key to continue...");
    getch(); // wait till the user want to proceed ;
}

int UserLogin(){ // same logic as above so i direct use the code from register part and change the message and the logic of check username and password
    int user_index;
    char input_ID[25], input_password[25]; // Use slightly larger buffers to detect
    ClearScreen();
    if(registered_user == 0){ // if no user in the system, then directly return to main menu
        printf("No user found. Please register first! \nPress any key to return...");
        getch();
        return -1; // return -1 to indicate login failure
    }
    //Get Username
    while (1) {
        printf("---Login--- (press 0 to cancel)\n"); // update the message to login
        printf("ID (Max 12): ");
        fgets(input_ID, sizeof(input_ID), stdin);

        // Check for Return/Cancel first
        if (strcmp(input_ID, "0\n") == 0) return -1; //if cancel then exit to main menu

        if (strchr(input_ID, '\n') == NULL) {
            printf("[!] Error: ID too long!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(input_ID);
        if (strlen(input_ID) != 12 || CheckInputBlank(input_ID)) {
            printf("[!] Error: ID must be 1-12 characters.\n");
            getch();
            ClearScreen();
            continue;
        }
        break;
    }

    //Get password
    while (1) {
        printf("Password (Max 20): ");
        fgets(input_password, sizeof(input_password), stdin);

        if (strchr(input_password, '\n') == NULL) {
            printf("[!] Error: Password too long!\n");
            ClearInputBuffer();
            getch();
            continue;
        }

        RemoveNewLine(input_password);
        if (strlen(input_password) >= 20 || CheckInputBlank(input_password)) {
            printf("[!] Error: Password must be 1-19 characters.\n");
            continue;
        }
        break;
        ClearInputBuffer();
    }
    for (int i = 0; i < registered_user; i++) { //loop through the student list to find the username and the password got match??
        if (strcmp(input_ID, citizen_list[i].citizenId) == 0 && strcmp(input_password, citizen_list[i].citizenPassword) == 0) {
            citizen_list[i].LoginCount++; // Increment login count
            return i; // if have match return the index of the student in the array 
        }
    }
    printf("\n[!] Invalid login. \nPress any key to continue...");
    getch();
    return -1;
}

void GetCitizenInfo(int user_index){
    struct Citizen *citizen = &citizen_list[user_index];
    while (1) {
        ClearScreen();
        printf("Enter Age: ");
        if (scanf("%d", &citizen->age) == 1 && citizen->age > 0 && citizen->age < 150) {
            ClearInputBuffer();
            break;
        } else {
            printf("Please enter a valid age between 1 and 149.\n");
            ClearInputBuffer();
        }
    }
        while (1) {
            ClearScreen();
            printf("Enter Martial Status (0 for Single, 1 or other for Married): ");
            if (scanf("%d", &citizen->martialStatus) == 1 && (citizen->martialStatus == 0 || citizen->martialStatus == 1)) {
                ClearInputBuffer();
                if (citizen->martialStatus == 0) {
                    citizen->deduction[0].category[0] = 0; // Single
                } else {
                    citizen->deduction[0].category[0] = 1; // Married
                }
                break;
            } else {
                printf("Please enter 0 for Single or 1 for Married.\n");
                ClearInputBuffer();
            }
        }

        while (1) {
            ClearScreen();
            citizen->annualIncome = CheckValidDouble("Enter Annual Income: $");
            break;
        }

        while(1) {
            ClearScreen();
            printf("Enter Number of Children: ");
            if (scanf("%d", &citizen->numberOfChildren) == 1 && citizen->numberOfChildren >= 0 && citizen->numberOfChildren < 20) {
                ClearInputBuffer();
                break;
            } else {
                printf("Please enter a valid number of children (0-19).\n");
                ClearInputBuffer();
            }
        }

        while(1) {
            ClearScreen();
            printf("Enter Number of Children Under 18: ");
            if (scanf("%d", &citizen->childrenUnder18) == 1 && citizen->childrenUnder18 >= 0 && citizen->childrenUnder18 <= citizen->numberOfChildren) {
                ClearInputBuffer();
                switch (citizen->childrenUnder18) {
                    case 1:
                    case 2:
                        citizen->deduction[0].category[1] = 1;
                        break;
                    case 3:
                    case 4:
                        citizen->deduction[0].category[1] = 2;
                        break;
                    case 5:
                    case 6:
                        citizen->deduction[0].category[1] = 3;
                        break;
                    default:
                        citizen->deduction[0].category[1] = 4;
                        break;
                }
                break;
            } else {
                printf("Please enter a valid number of children under 18 (0-%d).\n", citizen->numberOfChildren);
                ClearInputBuffer();
            }
        }

        while(1) {
            ClearScreen();
            printf("Special Healthcare Status (any people in your household with special healthcare needs)(0 for No, 1 for Yes): ");
            if (scanf("%d", &citizen->specialHealthcare) == 1 && (citizen->specialHealthcare == 0 || citizen->specialHealthcare == 1)) {
                ClearInputBuffer();
                if (citizen->specialHealthcare == 0) {
                    citizen->deduction[0].category[2] = 0; // No special healthcare
                } else {
                    citizen->deduction[0].category[2] = 1; // Yes special healthcare
                }
                break;
            } else {
                printf("Please enter 0 for No or 1 for Yes.\n");
                ClearInputBuffer();
            }
    }
    SaveUserData();
}

void EditCitizenInfo(int user_index, int field_choice)
{
    struct Citizen *citizen = &citizen_list[user_index];
    switch (field_choice)
    {
    case 1: // Edit Age
        while (1) {
            ClearScreen();
            printf("Enter New Age: ");
            if (scanf("%d", &citizen->age) == 1 && citizen->age > 0 && citizen->age < 150) {
                ClearInputBuffer();
                break;
            } else {
                printf("Please enter a valid age between 1 and 149.\n");
                ClearInputBuffer();
            }
        }
        break;
    case 2: // Edit Martial Status
        while (1) {
            ClearScreen();
            printf("Enter New Martial Status (0 for Single, 1 or other for Married): ");
            if (scanf("%d", &citizen->martialStatus) == 1 && (citizen->martialStatus == 0 || citizen->martialStatus == 1)) {
                ClearInputBuffer();
                if (citizen->martialStatus == 0) {
                    citizen->deduction[0].category[0] = 0; // Single
                } else {
                    citizen->deduction[0].category[0] = 1; // Married
                }
                break;
            } else {
                printf("Please enter 0 for Single or 1 for Married.\n");
                ClearInputBuffer();
            }
        }
        break;
    case 3: // Edit Annual Income
        while (1) {
            ClearScreen();
            citizen->annualIncome = CheckValidDouble("Enter New Annual Income: $");
            break;
        }
        break;
    case 4: // Edit Number of Children
        while(1) {
            ClearScreen();
            printf("Enter New Number of Children: ");
            if (scanf("%d", &citizen->numberOfChildren) == 1 && citizen->numberOfChildren >= 0 && citizen->numberOfChildren < 20) {
                ClearInputBuffer();
            } else {
                printf("Please enter a valid number of children (0-19).\n");
                ClearInputBuffer();
            }
        }
    case 5: // Edit Number of Children Under 18
        while(1) {
            ClearScreen();
            printf("Enter New Number of Children Under 18: ");
            if (scanf("%d", &citizen->childrenUnder18) == 1 && citizen->childrenUnder18 >= 0 && citizen->childrenUnder18 <= citizen->numberOfChildren) {
                ClearInputBuffer();
                switch (citizen->childrenUnder18) {
                    case 1:
                    case 2:
                        citizen->deduction[0].category[1] = 1;
                        break;
                    case 3:
                    case 4:
                        citizen->deduction[0].category[1] = 2;
                        break;
                    case 5:
                    case 6:
                        citizen->deduction[0].category[1] = 3;
                        break;
                    default:
                        citizen->deduction[0].category[1] = 4;
                        break;
                }
                break;
            } else {
                printf("Please enter a valid number of children under 18 (0-%d).\n", citizen->numberOfChildren);
                ClearInputBuffer();
            }
        }
        break;
    case 6: // Edit Special Healthcare Status
        while(1) {
            ClearScreen();
            printf("Special Healthcare Status (any people in your household with special healthcare needs)(0 for No, 1 for Yes): ");
            if (scanf("%d", &citizen->specialHealthcare) == 1 && (citizen->specialHealthcare == 0 || citizen->specialHealthcare == 1)) {
                ClearInputBuffer();
                if (citizen->specialHealthcare == 0) {
                    citizen->deduction[0].category[2] = 0; // No special healthcare
                } else {
                    citizen->deduction[0].category[2] = 1; // Yes special healthcare
                }
                break;
            } else {
                printf("Please enter 0 for No or 1 for Yes.\n");
                ClearInputBuffer();
            }
        }
        break;
    default:
        printf("[!] Error: Invalid field choice.\n");
    }
    SaveUserData();
}

void CalculateTax(int user_index){
    struct Citizen *citizen = &citizen_list[user_index];
    citizen->totalDeduction = 0.0;
    // Calculate deductions based on marital status
    if (citizen->martialStatus) {
        citizen->totalDeduction += 9000.0; // Married deduction
    }
    if (citizen->numberOfChildren > 0) {
        citizen->totalDeduction += lookupChildAllowance(citizen->numberOfChildren, citizen->childrenUnder18, childTable, 4);
    }
    if (citizen->specialHealthcare) {
        citizen->totalDeduction += 8000.0; // Special healthcare deduction
    }
    citizen->taxableIncome = citizen->annualIncome - citizen->totalDeduction;
    if (citizen->taxableIncome < 0) {
        citizen->taxableIncome = 0.0; // Ensure taxable income doesn't go negative
    }
    citizen->incomeTax = citizen->taxableIncome * lookupTaxRate(citizen->taxableIncome, brackets, 7);
    SaveUserData();
}


void PrintDashBoard(int user_index)
{
    CalculateTax(user_index); // Ensure tax calculations are up-to-date
    ClearScreen();
    struct Citizen *citizen = &citizen_list[user_index];
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("========================================= Tax Dashboard =========================================\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Hi, %s\n",citizen->citizenName);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Citizen ID: %s\n", citizen->citizenId);
    printf("Age: %d\n", citizen->age);
    printf("Marital Status: %s\n", citizen->martialStatus ? "Married" : "Single");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Number of Children: %d\n", citizen->numberOfChildren);
    printf("Number of Children Under 18: %d\n", citizen->childrenUnder18);
    printf("Special Healthcare Status: %s\n", citizen->specialHealthcare ? "Yes" : "No");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Annual Income: %82.2f\n", citizen->annualIncome);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Deduction Category Available:\n");
    printf("[1] Marital Status Deduction: %s %63.2f\n", citizen->deduction[0].category[0] ? "Yes" : "No", citizen->martialStatus ? 9000.0 : 0.0);
    printf("[2] Children Deduction (Deduction Category): %s %20.2f\n", childTablestring[citizen->deduction[0].category[1]], childTable[citizen->deduction[0].category[1]].amount);
    printf("[3] Special Healthcare Deduction: %s %59.2f\n", citizen->deduction[0].category[2] ? "Yes" : "No", citizen->specialHealthcare ? 5000.0 : 0.0);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Total Deduction: %80.2f\n", citizen->totalDeduction);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Taxable Income: %81.2f\n", citizen->taxableIncome);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Tax Rate: %86.2f%%\n", lookupTaxRate(citizen->taxableIncome, brackets, 7) * 100);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Income Tax: %85.2f\n", citizen->incomeTax);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Tax Per Month: %82.2f\n", citizen->incomeTax / 12);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Press Any key to Return");
    getch();
}

/* Plan the layout of the dashboard
-------------------------------------------------------------------------------------
=================================== Tax Dashboard ===================================
-------------------------------------------------------------------------------------
Hi xxx
-------------------------------------------------------------------------------------
Id :
Name :
Age :
Marital Status :
-------------------------------------------------------------------------------------
Number of Children :
Number of Children Under 18 :
Special Healthcare Status :
-------------------------------------------------------------------------------------
Annual Income :
-------------------------------------------------------------------------------------
Deduction Category Available:
[1] Marital Status Deduction (Yes/No)):
[2] Children Deduction (Deduction Category):
[3] Special Healthcare Deduction (Yes/No):
-------------------------------------------------------------------------------------
Total Deduction :
-------------------------------------------------------------------------------------
Taxable Income :
-------------------------------------------------------------------------------------
Tax Rate :
-------------------------------------------------------------------------------------
Income Tax :
-------------------------------------------------------------------------------------
Tax Per Month :
-------------------------------------------------------------------------------------



*/

void PrintCitizenInfo(int user_index)
{
    char filename[100];
    snprintf(filename,100, "Citizen_%s_Info.txt", citizen_list[user_index].citizenId);
    FILE *file_path = fopen(filename, "w"); // 'wb' = write binary
    // FILE *file_path = fopen("userdata.txt", "w"); // 'wb' = write binary
    if (file_path == NULL)
    {                                       // check weather it is no file
        printf("[!] Error saving data!\n"); // if cant save , print error data
        return;
    }
    struct Citizen *Citizen = &citizen_list[user_index];
    // mark
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"========================================= Tax Dashboard =========================================\n");
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Hi, %s\n", Citizen->citizenName);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Citizen ID: %s\n", Citizen->citizenId);
    fprintf(file_path,"Age: %d\n", Citizen->age);
    fprintf(file_path,"Marital Status: %s\n", Citizen->martialStatus ? "Married" : "Single");
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Number of Children: %d\n", Citizen->numberOfChildren);
    fprintf(file_path,"Number of Children Under 18: %d\n", Citizen->childrenUnder18);
    fprintf(file_path,"Special Healthcare Status: %s\n", Citizen->specialHealthcare ? "Yes" : "No");
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Annual Income: %82.2f\n", Citizen->annualIncome);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Deduction Category Available:\n");
    fprintf(file_path,"[1] Marital Status Deduction: %s %63.2f\n", Citizen->deduction[0].category[0] ? "Yes" : "No", Citizen->martialStatus ? 9000.0 : 0.0);
    fprintf(file_path,"[2] Children Deduction (Deduction Category): %s %20.2f\n", childTablestring[Citizen->deduction[0].category[1]], childTable[Citizen->deduction[0].category[1]].amount);
    fprintf(file_path,"[3] Special Healthcare Deduction: %s %59.2f\n", Citizen->deduction[0].category[2] ? "Yes" : "No", Citizen->specialHealthcare ? 5000.0 : 0.0);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Total Deduction: %80.2f\n", Citizen->totalDeduction);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Taxable Income: %81.2f\n", Citizen->taxableIncome);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Tax Rate: %86.2f%%\n", lookupTaxRate(Citizen->taxableIncome, brackets, 7) * 100);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Income Tax: %85.2f\n", Citizen->incomeTax);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path,"Tax Per Month: %82.2f\n", Citizen->incomeTax / 12);
    fprintf(file_path,"-------------------------------------------------------------------------------------------------\n");
    fclose(file_path);
}

int main()
{
    LoadUserData(); // Load user data at the start of the program
    int running = 1;

    while (running)
    {
        int choice = SelectOption(login_menu, 3);
        if (choice == 0)
        { // Login
            current_active_user = UserLogin();
            if (current_active_user != -1)
            { // If login is successful
                int logged_in = 1;
                while (logged_in)
                {
                    if (citizen_list[current_active_user].LoginCount == 1){
                        GetCitizenInfo(current_active_user);
                        citizen_list[current_active_user].LoginCount++; // Increment to avoid re-entering info
                    }
                    int dash_choice = SelectOption(dash_menu, 7);
                    switch (dash_choice)
                    {
                    case 0:
                        EditCitizenInfo(current_active_user, SelectOption(edit_menu, 7));
                        ClearScreen();
                        break;
                    case 1:
                        ClearScreen();
                        temp_value = CheckValidDouble("Please enter the taxable income to lookup the tax bracket: $");
                        double tax_rate = lookupTaxRate(temp_value, brackets, 7);
                        for (int i = 0; i < 6; i++){
                            printf("Bracket %d: %.2f to %.2f, Tax Rate: %.2f%%\n", i + 1, brackets[i].minIncome, brackets[i].maxIncome, brackets[i].taxRate * 100);
                        }
                        printf("Bracket 7: %.2f and above, Tax Rate: %.2f%%\n", brackets[6].minIncome, brackets[6].taxRate * 100);
                        printf("\n--------------------------------------------------------------------------\n");
                        printf("Your taxable income of %.2f falls into a tax rate of %.2f%%\n", temp_value, tax_rate * 100);
                        printf("Press any key to return...");
                        getch();
                        break;
                    case 2:
                        ClearScreen();
                        temp_value = CheckValidDouble("Please enter the children below 18 count to lookup the child allowance: ");
                        double child_allowance = lookupChildAllowance((int)temp_value, (int)temp_value, childTable, 4);
                        for (int i = 0; i < 4; i++){
                            printf("Child Allowance %c: %d to %d children, Amount: %.2f\n", 'A' + i, childTable[i].minChildren, childTable[i].maxChildren, childTable[i].amount);
                        }
                        printf("\n--------------------------------------------------------------------------\n");
                        printf("Your children count of %d falls into a child allowance of %.2f\n", (int)temp_value, child_allowance);
                        printf("Press any key to return...");
                        getch();
                        break;
                    case 3:
                        PrintDashBoard(current_active_user);
                        break;
                    case 4:
                        ClearScreen();
                        PrintCitizenInfo(current_active_user);
                        printf("Citizen information saved to file successfully.\nPress any key to return...");
                        getch();
                        break;
                    case 5:
                        logged_in = 0; // Logout
                        break;
                    case 6:
                        running = 0; // Exit
                        logged_in = 0; // Also exit the logged-in loop
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                        getch();
                    }
                }
            }
        }
        else if (choice == 1)
        { // Register New User
            RegisterUser();
        }
        else if (choice == 2)
        { // Exit
            running = 0;
        }
    }
    return 0;
}