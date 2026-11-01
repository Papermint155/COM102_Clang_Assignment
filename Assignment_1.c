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

char edit_menu[6][30] = {"Back/Return", "Edit Age", "Edit Martial Status", "Edit Annual Income", "Edit Number of Children", "Edit Special Healthcare Status"};
char login_menu[3][30] = {"Login", "Register New User", "Exit"};
char dash_menu[7][30] = {"Edit Citizen Info", "Lookup Tax Bracket", "Lookup Citizen Info", "Tax Dashboard", "Print Citizen Info", "Logout", "Exit"};
// char admin_menu[5][30] = {"View All Citizens", "Edit Citizen Info", "Delete Citizen Info", "Logout", "Exit"};


struct ChildAllowance {
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

int SelectOption(char option[][30], int total_choice)
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

double CheckValidDouble(char *message){ // puting * to wrong (solve)
    double value; 
    while (1){
        printf("%s", message);
        if (scanf("%lf",&value) == 1 && value > 0 && value < 2147483647){
            ClearInputBuffer();
            return value;
        } else {
            printf("Please enter a valid positive number for this field.\n");
            ClearInputBuffer();
        }
    }
    return value;
}

void SaveUserData()
{
    FILE *file_path = fopen("userdata.bin", "wb"); // 'wb' = write binary
    // FILE *file_path = fopen("userdata.txt", "w"); // 'wb' = write binary
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
    FILE *file_path = fopen("userdata.bin", "rb"); // 'rb' = read binary , if this can be use in the securuty
    // FILE *file_path = fopen("userdata.txt", "r"); // use txt file for visualise the data in it
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

        if (strlen(temp_input) != 12)
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

    registered_user++; // add the register user count
    SaveUserData();
    printf("\nNew User Registration Success! \nPress any key to continue...");
    getch(); // wait till the user want to proceed ;
}
// int age;
//     bool martialStatus;
//     double annualIncome;
//     int numberOfChildren;
//     int childrenUnder18;
//     bool specialHealthcare;
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

        while (1) {
            CLearScreen();
            printf("Enter Martial Status (0 for Single, 1 for Married): ");
            if (scanf("%d", &citizen->martialStatus) == 1 && (citizen->martialStatus == 0 || citizen->martialStatus == 1)) {
                ClearInputBuffer();
                break;
            } else {
                printf("Please enter 0 for Single or 1 for Married.\n");
                ClearInputBuffer();
            }
        }

        while (1) {
            ClearScreen();
            CheckValidDouble("Enter Annual Income: $");
            }
    }


    SaveUserData();
}

void EditCitizenInfo(int user_index, int field_choice)
{
    SaveUserData();
}

void CalculateTax(int user_index)
{
    SaveUserData();
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
        else
        {
            printf("[!] Error: Taxable income is out of the defined tax brackets.\n");
            return -1; // Return an error value
        }
    }
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

void PrintDashBoard(int user_index)
{
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
[1] Marital Status Deduction :
[2] Children Deduction :
[3] Special Healthcare Deduction :
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
    snprintf(filename, "Citizen_%s_Info.txt", citizen_list[user_index].citizenId);
    FILE *file_path = fopen(filename, "w"); // 'wb' = write binary
    // FILE *file_path = fopen("userdata.txt", "w"); // 'wb' = write binary
    if (file_path == NULL)
    {                                       // check weather it is no file
        printf("[!] Error saving data!\n"); // if cant save , print error data
        return;
    }
    // mark
    fprintf(file_path, "Citizen ID: %s\n", citizen_list[user_index].citizenId);
    fprintf(file_path, "Citizen Name: %s\n", citizen_list[user_index].citizenName);
    fprintf(file_path, "Age: %d\n", citizen_list[user_index].age);
    fclose(file_path);
}

int main()
{
    LoadUserData(); // Load user data at the start of the program
    // Your main program logic here
    return 0;
}