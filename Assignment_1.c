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
#include <ctype.h>

#define MAX_CITIZENS 30

int current_active_user = -1;
int registered_user = 0;
double temp_value = 0.0;

char edit_menu[7][40] = {
    "Back/Return",
    "Edit Age",
    "Edit Marital Status",
    "Edit Annual Income",
    "Edit Number of Children",
    "Edit Number of Children Under 18",
    "Edit Special Healthcare Status"
};

char login_menu[3][40] = {"Login", "Register New User", "Exit"};

char dash_menu[7][40] = {
    "Edit Citizen Info",
    "Lookup Tax Bracket",
    "Lookup Child Allowance",
    "Tax Dashboard",
    "Print Citizen Info",
    "Logout",
    "Exit"
};

char admin_menu[8][40] = {
    "View All Citizens",
    "Search Citizen Info",
    "Add Citizen",
    "Edit Citizen Info",
    "Overall Statistics",
    "Print Overall Statistics",
    "Logout",
    "Exit"
};

// Updated 13-option category selector with comma fixed after "Income Tax"
char admin_category_Selection[13][40] = {
    "Done / Proceed",
    "ID",
    "Name",
    "Age",
    "Marital Status",
    "Annual Income",
    "Number of Children",
    "Children Under 18",
    "Special Healthcare Status",
    "Total Deduction",
    "Taxable Income",
    "Income Tax",
    "All Categories (Default)"
};

char sort_order_menu[2][40] = {
    "Ascending Order (A-Z / Low to High)",
    "Descending Order (Z-A / High to Low)"
};

// Added "Done / Show Dashboard" and "All Summary Categories (Default)"
char adminSummaryDashboardChoice[17][60] = {
    "Done / Show Selected Summary Dashboard",
    "Total Number of Registered Citizens",
    "Total Number of Citizens with Special Healthcare Needs",
    "Total Number of Married Citizens",
    "Total Number of Single Citizens",
    "Total Number of Citizens with Annual Income above Threshold",
    "Average Annual Income of All Citizens",
    "Average Taxable Income of All Citizens",
    "Total Income Tax Collected from All Citizens",
    "Total Exemptions/Deductions Claimed by All Citizens",
    "Total Number of Citizens with Children",
    "Total Number of Citizens with Children Under 18",
    "Mean income tax per citizen",
    "Median income tax per citizen",
    "Standard deviation of income tax per citizen",
    "All Summary Categories (Default)",
    "Back to Admin Menu"
};

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

char childTablestring[5][50] = {
    "No Child Allowance (Not Eligible)",
    "Child allowance A: 1-2 children",
    "Child allowance B: 3-4 children",
    "Child allowance C: 5-6 children",
    "Child allowance D: 7 or more children"
};

struct Deduction_Category{
    int category[3];
    double deductionAmount;
};

struct Citizen{
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

struct TaxBracket{
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

struct Citizen citizen_list[MAX_CITIZENS];

void CalculateTax(int user_index);
void PrintadminSummaryDashboard(int selected_cats[], int cat_count, double threshold);
void PrintCustomCitizenTableToFile(struct Citizen list[], int count, int dashboard_ntb_print[], int counter);

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

void ClearScreen()
{
    system("cls");
    // clrscr(); cant use (got bug)
    //  printf("\033[H\033[2J"); This method not working (By printing a lot of new line)
    //  fflush(stdout);
}

int ConfirmAction(const char *prompt) {
    printf("%s (Press 'Y' or Enter to confirm, 'N' to cancel/redo): ", prompt);
    while (1) {
        int ch = getch();
        if (ch == 'y' || ch == 'Y' || ch == 13) {
            printf("Yes\n");
            return 1;
        } else if (ch == 'n' || ch == 'N') {
            printf("No\n");
            return 0;
        }
    }
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

void SelectCitizenDashboardCategories(int dashboard_ntb_print[], int *counter) {
    *counter = 0;
    int current_selection = 0;

    while (*counter < 11) {
        ClearScreen();
        printf("Please select the Categories to display/print on the Dashboard:\n");
        printf("(Select 'All Categories (Default)' for all, or 'Done / Proceed' when finished)\n");
        printf("-------------------------------------------------------------------------------------------------\n");
        printf("Currently Selected (%d): ", *counter);
        if (*counter == 0) {
            printf("None");
        } else {
            for (int k = 0; k < *counter; k++) {
                printf("[%s] ", admin_category_Selection[dashboard_ntb_print[k]]);
            }
        }
        printf("\n-------------------------------------------------------------------------------------------------\n\n");

        for (int i = 0; i < 13; i++) {
            if (i == current_selection) {
                printf("-> %s\n", admin_category_Selection[i]);
            } else {
                printf("   %s\n", admin_category_Selection[i]);
            }
        }

        int key_pressed = getch();
        if (key_pressed == 224 || key_pressed == 0) {
            key_pressed = getch();
            if (key_pressed == 72)
                current_selection = (current_selection - 1 + 13) % 13;
            if (key_pressed == 80)
                current_selection = (current_selection + 1) % 13;
        } else if (key_pressed == 13) {
            if (current_selection == 0) { // Done / Proceed
                if (*counter == 0) {
                    // Default to all 11 categories if user pressed Done immediately
                    for (int c = 1; c <= 11; c++) {
                        dashboard_ntb_print[c - 1] = c;
                    }
                    *counter = 11;
                }
                break;
            } else if (current_selection == 12) { // All Categories (Default)
                for (int c = 1; c <= 11; c++) {
                    dashboard_ntb_print[c - 1] = c;
                }
                *counter = 11;
                break;
            } else {
                // Check if already selected to avoid duplicates
                int already_added = 0;
                for (int k = 0; k < *counter; k++) {
                    if (dashboard_ntb_print[k] == current_selection) {
                        already_added = 1;
                        break;
                    }
                }
                if (!already_added) {
                    dashboard_ntb_print[*counter] = current_selection;
                    (*counter)++;
                }
            }
        }
    }
}


int CheckIdUniqueness(char *id) {
    for (int i = 0; i < registered_user; i++) {
        if (strcmp(citizen_list[i].citizenId, id) == 0) {
            return 0; // ID is not unique
        }
    }
    return 1; // ID is unique
}



double lookupTaxRate(double taxableIncome, struct TaxBracket table[], int size) {
    if (taxableIncome <= table[0].maxIncome) {
        return table[0].taxRate;
    }
    for (int i = 1; i < size; i++) {
        if (taxableIncome > table[i].minIncome && taxableIncome <= table[i].maxIncome) {
            return table[i].taxRate;
        }
    }
    printf("[!] Error: Taxable income is out of the defined tax brackets.\n");
    return 0.0;
}

double lookupChildAllowance(int totalChildren, int under18, struct ChildAllowance table[], int size) {
    if (totalChildren <= 0 || totalChildren != under18) {
        return 0.0;
    }
    for (int i = 0; i < size; i++) {
        if (totalChildren >= table[i].minChildren && totalChildren <= table[i].maxChildren) {
            return table[i].amount;
        }
    }
    return 0.0;
}

void UpdateChildCategoryIndex(struct Citizen *citizen) {
    if (citizen->numberOfChildren <= 0 || citizen->numberOfChildren != citizen->childrenUnder18) {
        citizen->deduction[0].category[1] = 0;
    } else if (citizen->childrenUnder18 <= 2) {
        citizen->deduction[0].category[1] = 1;
    } else if (citizen->childrenUnder18 <= 4) {
        citizen->deduction[0].category[1] = 2;
    } else if (citizen->childrenUnder18 <= 6) {
        citizen->deduction[0].category[1] = 3;
    } else {
        citizen->deduction[0].category[1] = 4;
    }
}


void SaveUserData(){
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

void RegisterUser() {
    ClearScreen();
    if (registered_user >= MAX_CITIZENS) {
        printf("Maximum user limit (%d) reached.\n", MAX_CITIZENS);
        getch();
        return;
    }

    struct Citizen *new_citizen = &citizen_list[registered_user];
    memset(new_citizen, 0, sizeof(struct Citizen));
    char temp_input[150];

    while (1) {
        printf("--- Register New User --- (press 0 to cancel)\n");
        printf("Citizen ID (Exact 12 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strcmp(temp_input, "0\n") == 0)
            return;

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Invalid ID Number!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) != 12 || CheckInputBlank(temp_input)) {
            printf("[!] Error: Citizen ID must be exactly 12 characters!\n");
            getch();
            ClearScreen();
            continue;
        }

        if (!CheckIdUniqueness(temp_input)) {
            printf("[!] Error: Citizen ID already exists!\n");
            getch();
            ClearScreen();
            continue;
        }

        if (!ConfirmAction("Confirm Citizen ID?")) {
            ClearScreen();
            continue;
        }

        strcpy(new_citizen->citizenId, temp_input);
        break;
    }

    while (1) {
        printf("Username (Max 49 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strcmp(temp_input, "0\n") == 0)
            return;

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Username too long!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 50 || CheckInputBlank(temp_input)) {
            printf("[!] Error: Username must be 1-49 characters.\n");
            getch();
            ClearScreen();
            continue;
        }

        if (!ConfirmAction("Confirm Username?")) {
            continue;
        }

        strcpy(new_citizen->citizenName, temp_input);
        break;
    }

    while (1) {
        printf("Password (Max 19 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Password too long!\n");
            ClearInputBuffer();
            getch();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 20 || CheckInputBlank(temp_input)) {
            printf("[!] Error: Password must be 1-19 characters.\n");
            getch();
            continue;
        }

        if (!ConfirmAction("Confirm Password?")) {
            continue;
        }

        strcpy(new_citizen->citizenPassword, temp_input);
        break;
    }

    registered_user++;
    new_citizen->LoginCount = 0;
    SaveUserData();
    printf("\nNew User Registration Success!\nPress any key to continue...");
    getch();

    // Get Password
    while(1)
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

int UserLogin() {
    char input_ID[50], input_password[50];
    ClearScreen();

    while (1) {
        printf("--- Login --- (press 0 to cancel)\n");
        printf("ID (12 characters): ");
        fgets(input_ID, sizeof(input_ID), stdin);

        if (strcmp(input_ID, "0\n") == 0)
            return -1;

        if (strchr(input_ID, '\n') == NULL) {
            printf("[!] Error: ID too long!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(input_ID);
        if (strcmp(input_ID, "admin") != 0 && (strlen(input_ID) != 12 || CheckInputBlank(input_ID))) {
            printf("[!] Error: ID must be 12 characters.\n");
            getch();
            ClearScreen();
            continue;
        }
        break;
    }

    while (1) {
        printf("Password (Max 19): ");
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
    }

    if (strcmp(input_ID, "admin") == 0 && strcmp(input_password, "admin123") == 0) {
        return -2; // Admin login
    }

    if (registered_user == 0) {
        printf("No user found. Please register first!\nPress any key to return...");
        getch();
        return -1;
    }

    for (int i = 0; i < registered_user; i++) {
        if (strcmp(input_ID, citizen_list[i].citizenId) == 0 &&
            strcmp(input_password, citizen_list[i].citizenPassword) == 0) {
            citizen_list[i].LoginCount++;
            return i;
        }
    }

    printf("\n[!] Invalid login.\nPress any key to continue...");
    getch();
    return -1;
}

void GetCitizenInfo(int user_index) {
    struct Citizen *citizen = &citizen_list[user_index];
    while (1) {
        ClearScreen();
        printf("Enter Age (18 - 149): ");
        if (scanf("%d", &citizen->age) == 1 && citizen->age >= 18 && citizen->age < 150) {
            ClearInputBuffer();
            if (ConfirmAction("Confirm Age?"))
                break;
        } else {
            printf("Please enter a valid age between 18 and 149.\n");
            ClearInputBuffer();
            getch();
        }
    }

    while (1) {
        ClearScreen();
        printf("Enter Marital Status (0 for Single, 1 for Married): ");
        if (scanf("%d", &citizen->martialStatus) == 1 &&
            (citizen->martialStatus == 0 || citizen->martialStatus == 1)) {
            ClearInputBuffer();
            if (ConfirmAction("Confirm Marital Status?")) {
                citizen->deduction[0].category[0] = citizen->martialStatus;
                break;
            }
        } else {
            printf("Please enter 0 for Single or 1 for Married.\n");
            ClearInputBuffer();
            getch();
        }
    }

    ClearScreen();
    citizen->annualIncome = CheckValidDouble("Enter Annual Income: RM ");

    while (1) {
        ClearScreen();
        printf("Enter Number of Children (0 - 19): ");
        if (scanf("%d", &citizen->numberOfChildren) == 1 &&
            citizen->numberOfChildren >= 0 && citizen->numberOfChildren < 20) {
            ClearInputBuffer();
            if (ConfirmAction("Confirm Number of Children?"))
                break;
        } else {
            printf("Please enter a valid number of children (0-19).\n");
            ClearInputBuffer();
            getch();
        }
    }

    while (1) {
        ClearScreen();
        printf("Enter Number of Children Under 18 (0 - %d): ", citizen->numberOfChildren);
        if (scanf("%d", &citizen->childrenUnder18) == 1 &&
            citizen->childrenUnder18 >= 0 &&
            citizen->childrenUnder18 <= citizen->numberOfChildren) {
            ClearInputBuffer();
            if (ConfirmAction("Confirm Number of Children Under 18?")) {
                UpdateChildCategoryIndex(citizen);
                break;
            }
        } else {
            printf("Please enter a valid number of children under 18 (0-%d).\n", citizen->numberOfChildren);
            ClearInputBuffer();
            getch();
        }
    }

    while (1) {
        ClearScreen();
        printf("Special Healthcare Status (0 for No, 1 for Yes): ");
        if (scanf("%d", &citizen->specialHealthcare) == 1 &&
            (citizen->specialHealthcare == 0 || citizen->specialHealthcare == 1)) {
            ClearInputBuffer();
            if (ConfirmAction("Confirm Special Healthcare Status?")) {
                citizen->deduction[0].category[2] = citizen->specialHealthcare;
                break;
            }
        } else {
            printf("Please enter 0 for No or 1 for Yes.\n");
            ClearInputBuffer();
            getch();
        }
    }

    CalculateTax(user_index);
    SaveUserData();
}

void EditCitizenInfo(int user_index, int field_choice) {
    if (field_choice == 0)
        return;

    struct Citizen *citizen = &citizen_list[user_index];
    switch (field_choice) {
    case 1:
        while (1) {
            ClearScreen();
            printf("Enter New Age (18 - 149): ");
            if (scanf("%d", &citizen->age) == 1 && citizen->age >= 18 && citizen->age < 150) {
                ClearInputBuffer();
                if (ConfirmAction("Confirm New Age?"))
                    break;
            } else {
                printf("Please enter a valid age between 18 and 149.\n");
                ClearInputBuffer();
                getch();
            }
        }
        break;
    case 2:
        while (1) {
            ClearScreen();
            printf("Enter New Marital Status (0 for Single, 1 for Married): ");
            if (scanf("%d", &citizen->martialStatus) == 1 &&
                (citizen->martialStatus == 0 || citizen->martialStatus == 1)) {
                ClearInputBuffer();
                if (ConfirmAction("Confirm New Marital Status?")) {
                    citizen->deduction[0].category[0] = citizen->martialStatus;
                    break;
                }
            } else {
                printf("Please enter 0 for Single or 1 for Married.\n");
                ClearInputBuffer();
                getch();
            }
        }
        break;
    case 3:
        ClearScreen();
        citizen->annualIncome = CheckValidDouble("Enter New Annual Income: RM ");
        break;
    case 4:
        while (1) {
            ClearScreen();
            printf("Enter New Number of Children (0 - 19): ");
            if (scanf("%d", &citizen->numberOfChildren) == 1 &&
                citizen->numberOfChildren >= 0 && citizen->numberOfChildren < 20) {
                ClearInputBuffer();
                if (ConfirmAction("Confirm New Number of Children?")) {
                    if (citizen->childrenUnder18 > citizen->numberOfChildren) {
                        citizen->childrenUnder18 = citizen->numberOfChildren;
                    }
                    UpdateChildCategoryIndex(citizen);
                    break;
                }
            } else {
                printf("Please enter a valid number of children (0-19).\n");
                ClearInputBuffer();
                getch();
            }
        }
        break;
    case 5:
        while (1) {
            ClearScreen();
            printf("Enter New Number of Children Under 18 (0 - %d): ", citizen->numberOfChildren);
            if (scanf("%d", &citizen->childrenUnder18) == 1 &&
                citizen->childrenUnder18 >= 0 &&
                citizen->childrenUnder18 <= citizen->numberOfChildren) {
                ClearInputBuffer();
                if (ConfirmAction("Confirm New Number of Children Under 18?")) {
                    UpdateChildCategoryIndex(citizen);
                    break;
                }
            } else {
                printf("Please enter a valid number of children under 18 (0-%d).\n", citizen->numberOfChildren);
                ClearInputBuffer();
                getch();
            }
        }
        break;
    case 6:
        while (1) {
            ClearScreen();
            printf("Special Healthcare Status (0 for No, 1 for Yes): ");
            if (scanf("%d", &citizen->specialHealthcare) == 1 &&
                (citizen->specialHealthcare == 0 || citizen->specialHealthcare == 1)) {
                ClearInputBuffer();
                if (ConfirmAction("Confirm Special Healthcare Status?")) {
                    citizen->deduction[0].category[2] = citizen->specialHealthcare;
                    break;
                }
            } else {
                printf("Please enter 0 for No or 1 for Yes.\n");
                ClearInputBuffer();
                getch();
            }
        }
        break;
    default:
        printf("[!] Error: Invalid field choice.\n");
        break;
    }

    CalculateTax(user_index);
    SaveUserData();
}

void CalculateTax(int user_index) {
    struct Citizen *citizen = &citizen_list[user_index];
    citizen->totalDeduction = 0.0;

    if (citizen->martialStatus) {
        citizen->totalDeduction += 5000.0;
    }
    if (citizen->numberOfChildren > 0) {
        citizen->totalDeduction += lookupChildAllowance(
            citizen->numberOfChildren, citizen->childrenUnder18, childTable, 4);
    }
    if (citizen->specialHealthcare) {
        citizen->totalDeduction += 20000.0;
    }

    UpdateChildCategoryIndex(citizen);
    citizen->deduction[0].deductionAmount = citizen->totalDeduction;

    citizen->taxableIncome = citizen->annualIncome - citizen->totalDeduction;
    if (citizen->taxableIncome < 0.0) {
        citizen->taxableIncome = 0.0;
    }

    citizen->incomeTax = citizen->taxableIncome * lookupTaxRate(citizen->taxableIncome, brackets, 7);
    SaveUserData();
}

void SortCitizens(struct Citizen list[], int count, int sortbycategory, int sortorder) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            int cmp = 0;
            switch (sortbycategory) {
            case 1:
                cmp = _stricmp(list[j].citizenId, list[j + 1].citizenId);
                break;
            case 2:
                cmp = _stricmp(list[j].citizenName, list[j + 1].citizenName);
                break;
            case 3:
                cmp = (list[j].age > list[j + 1].age) - (list[j].age < list[j + 1].age);
                break;
            case 4:
                cmp = (list[j].martialStatus > list[j + 1].martialStatus) - (list[j].martialStatus < list[j + 1].martialStatus);
                break;
            case 5:
                cmp = (list[j].annualIncome > list[j + 1].annualIncome) - (list[j].annualIncome < list[j + 1].annualIncome);
                break;
            case 6:
                cmp = (list[j].numberOfChildren > list[j + 1].numberOfChildren) - (list[j].numberOfChildren < list[j + 1].numberOfChildren);
                break;
            case 7:
                cmp = (list[j].childrenUnder18 > list[j + 1].childrenUnder18) - (list[j].childrenUnder18 < list[j + 1].childrenUnder18);
                break;
            case 8:
                cmp = (list[j].specialHealthcare > list[j + 1].specialHealthcare) - (list[j].specialHealthcare < list[j + 1].specialHealthcare);
                break;
            case 9:
                cmp = (list[j].totalDeduction > list[j + 1].totalDeduction) - (list[j].totalDeduction < list[j + 1].totalDeduction);
                break;
            case 10:
                cmp = (list[j].taxableIncome > list[j + 1].taxableIncome) - (list[j].taxableIncome < list[j + 1].taxableIncome);
                break;
            case 11:
                cmp = (list[j].incomeTax > list[j + 1].incomeTax) - (list[j].incomeTax < list[j + 1].incomeTax);
                break;
            default:
                cmp = 0;
                break;
            }

            if ((sortorder == 0 && cmp > 0) || (sortorder == 1 && cmp < 0)) {
                struct Citizen temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void PrintCustomCitizenTableToFile(struct Citizen list[], int count, int dashboard_ntb_print[], int counter) {
    FILE *fp = fopen("Citizen_Tax_report.txt", "w");
    if (fp == NULL) {
        printf("[!] Error: Unable to write to 'Citizen_Tax_report.txt'!\n");
        getch();
        return;
    }

    fprintf(fp, "============================================================================================================\n");
    fprintf(fp, "                                  CUSTOM CITIZEN TAX DASHBOARD REPORT                                       \n");
    fprintf(fp, "============================================================================================================\n");

    for (int j = 0; j < counter; j++) {
        fprintf(fp, "%-18s | ", admin_category_Selection[dashboard_ntb_print[j]]);
    }
    fprintf(fp, "\n------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        struct Citizen *citizen = &list[i];
        for (int j = 0; j < counter; j++) {
            int category = dashboard_ntb_print[j];
            switch (category) {
            case 1:
                fprintf(fp, "%-18s | ", citizen->citizenId);
                break;
            case 2:
                fprintf(fp, "%-18.18s | ", citizen->citizenName);
                break;
            case 3:
                fprintf(fp, "%-18d | ", citizen->age);
                break;
            case 4:
                fprintf(fp, "%-18s | ", citizen->martialStatus ? "Married" : "Single");
                break;
            case 5:
                fprintf(fp, "RM %-15.2f | ", citizen->annualIncome);
                break;
            case 6:
                fprintf(fp, "%-18d | ", citizen->numberOfChildren);
                break;
            case 7:
                fprintf(fp, "%-18d | ", citizen->childrenUnder18);
                break;
            case 8:
                fprintf(fp, "%-18s | ", citizen->specialHealthcare ? "Yes" : "No");
                break;
            case 9:
                fprintf(fp, "RM %-15.2f | ", citizen->totalDeduction);
                break;
            case 10:
                fprintf(fp, "RM %-15.2f | ", citizen->taxableIncome);
                break;
            case 11:
                fprintf(fp, "RM %-15.2f | ", citizen->incomeTax);
                break;
            }
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "============================================================================================================\n");
    fclose(fp);
    printf("\n[+] Selected dashboard categories successfully printed to 'Citizen_Tax_report.txt'!\n");
}

void ShowAndPrintCitizenTable(struct Citizen matched_citizens[], int found) {
    int dashboard_ntb_print[12];
    int counter = 0;

    // 1. Admin selects which categories to show (or "All Categories (Default)")
    SelectCitizenDashboardCategories(dashboard_ntb_print, &counter);

    // 2. Build a dynamic Sort menu containing ONLY the categories chosen above
    char dynamic_sort_menu[13][40];
    strcpy(dynamic_sort_menu[0], "No Sorting / Proceed");
    for (int i = 0; i < counter; i++) {
        int cat_idx = dashboard_ntb_print[i];
        strcpy(dynamic_sort_menu[i + 1], admin_category_Selection[cat_idx]);
    }

    // Ask which of the selected categories to sort by
    int sort_choice = SelectOption(dynamic_sort_menu, counter + 1);
    if (sort_choice > 0) {
        // Map the chosen option back to its actual category ID (1 to 11)
        int actual_sort_category = dashboard_ntb_print[sort_choice - 1];
        int sortorder = SelectOption(sort_order_menu, 2);
        SortCitizens(matched_citizens, found, actual_sort_category, sortorder);
    }

    // 3. Display only the selected categories on screen
    ClearScreen();
    printf("Dashboard View (%d Citizens, %d Categories Selected):\n", found, counter);
    printf("------------------------------------------------------------------------------------------------------------\n");
    for (int j = 0; j < counter; j++) {
        printf("%-18s | ", admin_category_Selection[dashboard_ntb_print[j]]);
    }
    printf("\n------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < found; i++) {
        struct Citizen *citizen = &matched_citizens[i];
        for (int j = 0; j < counter; j++) {
            int category = dashboard_ntb_print[j];
            switch (category) {
            case 1:
                printf("%-18s | ", citizen->citizenId);
                break;
            case 2:
                printf("%-18.18s | ", citizen->citizenName);
                break;
            case 3:
                printf("%-18d | ", citizen->age);
                break;
            case 4:
                printf("%-18s | ", citizen->martialStatus ? "Married" : "Single");
                break;
            case 5:
                printf("RM %-15.2f | ", citizen->annualIncome);
                break;
            case 6:
                printf("%-18d | ", citizen->numberOfChildren);
                break;
                case 7:
                printf("%-18d | ", citizen->childrenUnder18);
                break;
            case 8:
                printf("%-18s | ", citizen->specialHealthcare ? "Yes" : "No");
                break;
            case 9:
                printf("RM %-15.2f | ", citizen->totalDeduction);
                break;
            case 10:
                printf("RM %-15.2f | ", citizen->taxableIncome);
                break;
            case 11:
                printf("RM %-15.2f | ", citizen->incomeTax);
                break;
            }
        }
        printf("\n");
    }
    printf("------------------------------------------------------------------------------------------------------------\n");

    // 4. Ask if admin wants to print these exact selected categories to file
    if (ConfirmAction("Do you want to print this custom dashboard to 'Citizen_Tax_report.txt'?")) {
        PrintCustomCitizenTableToFile(matched_citizens, found, dashboard_ntb_print, counter);
    }
    printf("Press any key to return...");
    getch();
}

// int CompareStrings(const void *a, const void *b) {
//     const struct Citizen *citizenA = (const struct Citizen *)a;
//     const struct Citizen *citizenB = (const struct Citizen *)b;
    
//     // _stricmp automatically handles the lowercase conversion during comparison
//     return _stricmp(citizenA->citizenName, citizenB->citizenName); 
// }

int CompareDoubles(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}


int SelectSummaryDashboardCategories(int selected_cats[], int *cat_count, double *threshold) {
    *cat_count = 0;
    *threshold = 50000.0;
    int current_selection = 0;

    while (*cat_count < 14) {
        ClearScreen();
        printf("Select Summary Categories to include in the Dashboard:\n");
        printf("(Choose 'All Summary Categories (Default)' for all, or 'Done' when ready)\n");
        printf("-------------------------------------------------------------------------------------------\n");
        printf("Selected Count: %d / 14\n", *cat_count);
        printf("-------------------------------------------------------------------------------------------\n\n");

        for (int i = 0; i < 17; i++) {
            if (i == current_selection) {
                printf("-> %s\n", adminSummaryDashboardChoice[i]);
            } else {
                printf("   %s\n", adminSummaryDashboardChoice[i]);
            }
        }

        int key_pressed = getch();
        if (key_pressed == 224 || key_pressed == 0) {
            key_pressed = getch();
            if (key_pressed == 72)
                current_selection = (current_selection - 1 + 17) % 17;
            if (key_pressed == 80)
                current_selection = (current_selection + 1) % 17;
        } else if (key_pressed == 13) {
            if (current_selection == 16) { // Back to Admin Menu
                return 0;
            } else if (current_selection == 0) { // Done / Show Selected
                if (*cat_count == 0) {
                    for (int c = 1; c <= 14; c++)
                        selected_cats[c - 1] = c;
                    *cat_count = 14;
                }
                return 1;
            } else if (current_selection == 15) { // All Summary Categories (Default)
                for (int c = 1; c <= 14; c++)
                    selected_cats[c - 1] = c;
                *cat_count = 14;
                return 1;
            } else {
                int exists = 0;
                for (int k = 0; k < *cat_count; k++) {
                    if (selected_cats[k] == current_selection) {
                        exists = 1;
                        break;
                    }
                }
                if (!exists) {
                    if (current_selection == 5) {
                        ClearScreen();
                        *threshold = CheckValidDouble("Enter Annual Income Threshold: RM ");
                    }
                    selected_cats[*cat_count] = current_selection;
                    (*cat_count)++;
                }
            }
        }
    }
    return 1;
}

void SearchInfobyCategory(int category_choice, char *search_term) {
    ClearScreen();
    struct Citizen matched_citizens[MAX_CITIZENS];
    int found = 0;

    for (int i = 0; i < registered_user; i++) {
        CalculateTax(i);
        struct Citizen *citizen = &citizen_list[i];
        int match = 0;

        switch (category_choice) {
        case 1: // ID
            if (strstr(citizen->citizenId, search_term) != NULL)
                match = 1;
            break;
        case 2: // Name
            if (strstr(citizen->citizenName, search_term) != NULL)
                match = 1;
            break;
        case 3: // Age
            if (atoi(search_term) == citizen->age)
                match = 1;
            break;
        case 4: // Marital Status
            if ((_stricmp(search_term, "Married") == 0 && citizen->martialStatus) ||
                (_stricmp(search_term, "Single") == 0 && !citizen->martialStatus))
                match = 1;
            break;
        case 5: // Annual Income (>= search_term)
            if (citizen->annualIncome >= atof(search_term))
                match = 1;
            break;
        case 6: // Number of Children
            if (atoi(search_term) == citizen->numberOfChildren)
                match = 1;
            break;
        case 7: // Children Under 18
            if (atoi(search_term) == citizen->childrenUnder18)
                match = 1;
            break;
        case 8: // Special Healthcare Status
            if ((_stricmp(search_term, "Yes") == 0 && citizen->specialHealthcare) ||
                (_stricmp(search_term, "No") == 0 && !citizen->specialHealthcare))
                match = 1;
            break;
        case 9: // Total Deduction (>= search_term)
            if (citizen->totalDeduction >= atof(search_term))
                match = 1;
            break;
        case 10: // Taxable Income (>= search_term)
            if (citizen->taxableIncome >= atof(search_term))
                match = 1;
            break;
        case 11: // Income Tax (>= search_term)
            if (citizen->incomeTax >= atof(search_term))
                match = 1;
            break;
        default:
            printf("[!] Error: Invalid category choice.\n");
            getch();
            return;
        }

        if (match) {
            matched_citizens[found] = *citizen;
            found++;
        }
    }

    if (found == 0) {
        printf("[!] No matching citizens found.\nPress any key to return...");
        getch();
        return;
    }

    ShowAndPrintCitizenTable(matched_citizens, found);
}


void adminSummaryDashboard(){
    if (registered_user == 0) {
        ClearScreen();
        printf("[!] No citizens registered yet.\nPress any key to return...");
        getch();
        return;
    }

    int selected_cats[15];
    int cat_count = 0;
    double threshold = 50000.0;

    if (!SelectSummaryDashboardCategories(selected_cats, &cat_count, &threshold)) {
        return;
    }

    ClearScreen();
    printf("-------------------------------------------------------------------------------------------\n");
    printf("------------------------ Summary of All Citizens Dashboard --------------------------------\n");
    printf("-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < cat_count; i++) {
        WriteSingleSummaryStat(stdout, selected_cats[i], threshold);
        printf("-------------------------------------------------------------------------------------------\n");
    }

    // Ask print dashboard after showing the summary of all citizens
    if (ConfirmAction("Do you want to print this Summary Dashboard to 'Citizen_Tax_report.txt'?")) {
        PrintadminSummaryDashboard(selected_cats, cat_count, threshold);
    }
    printf("Press any key to return...");
    getch();
    
}



/* draft od summard board 
-------------------------------------------------------------------------------------------
------------------------ Summary of All Citizens Dashboard --------------------------------
-------------------------------------------------------------------------------------------
Selected category 1 
-----------------------------------------------------------------------------------
Selected Cat 2 
-----------------------------------------------------------------------------------
etc
*/



void PrintadminSummaryDashboard(int selected_cats[], int cat_count, double threshold) {
    FILE *fp = fopen("Citizen_Tax_report.txt", "w");
    if (fp == NULL) {
        printf("[!] Error: Could not open 'Citizen_Tax_report.txt' for writing.\n");
        return;
    }

    fprintf(fp, "-------------------------------------------------------------------------------------------\n");
    fprintf(fp, "------------------------ Summary of All Citizens Dashboard --------------------------------\n");
    fprintf(fp, "-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < cat_count; i++) {
        WriteSingleSummaryStat(fp, selected_cats[i], threshold);
        fprintf(fp, "-------------------------------------------------------------------------------------------\n");
    }

    fclose(fp);
    printf("\n[+] Summary Dashboard successfully printed to 'Citizen_Tax_report.txt'!\n");
}





void PrintDashBoard(int user_index) {
    CalculateTax(user_index);
    ClearScreen();
    struct Citizen *citizen = &citizen_list[user_index];
    int child_cat = citizen->deduction[0].category[1];
    double child_deduction_amt = lookupChildAllowance(citizen->numberOfChildren, citizen->childrenUnder18, childTable, 4);

    printf("-------------------------------------------------------------------------------------------------\n");
    printf("========================================= Tax Dashboard =========================================\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Hi, %s\n", citizen->citizenName);
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
    printf("[1] Marital Status Deduction: %-5s %61.2f\n",citizen->martialStatus ? "Yes" : "No",citizen->martialStatus ? 5000.0 : 0.0);
    printf("[2] Children Deduction: %-42s %30.2f\n",childTablestring[child_cat],child_deduction_amt);
    printf("[3] Special Healthcare Deduction: %-5s %57.2f\n",citizen->specialHealthcare ? "Yes" : "No",citizen->specialHealthcare ? 20000.0 : 0.0);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Total Deduction: %80.2f\n", citizen->totalDeduction);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Taxable Income: %81.2f\n", citizen->taxableIncome);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Tax Rate: %86.2f%%\n", lookupTaxRate(citizen->taxableIncome, brackets, 7) * 100);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Income Tax: %85.2f\n", citizen->incomeTax);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Tax Per Month: %82.2f\n", citizen->incomeTax / 12.0);
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

void PrintCitizenInfo(int user_index) {
    CalculateTax(user_index);
    char filename[100];
    snprintf(filename, sizeof(filename), "Citizen_%s_Info.txt", citizen_list[user_index].citizenId);
    FILE *file_path = fopen(filename, "w");
    if (file_path == NULL) {
        printf("[!] Error saving data!\n");
        return;
    }

    struct Citizen *Citizen = &citizen_list[user_index];
    int child_cat = Citizen->deduction[0].category[1];
    double child_deduction_amt = lookupChildAllowance(Citizen->numberOfChildren, Citizen->childrenUnder18, childTable, 4);

    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "========================================= Tax Dashboard =========================================\n");
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Hi, %s\n", Citizen->citizenName);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Citizen ID: %s\n", Citizen->citizenId);
    fprintf(file_path, "Age: %d\n", Citizen->age);
    fprintf(file_path, "Marital Status: %s\n", Citizen->martialStatus ? "Married" : "Single");
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Number of Children: %d\n", Citizen->numberOfChildren);
    fprintf(file_path, "Number of Children Under 18: %d\n", Citizen->childrenUnder18);
    fprintf(file_path, "Special Healthcare Status: %s\n", Citizen->specialHealthcare ? "Yes" : "No");
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Annual Income: %82.2f\n", Citizen->annualIncome);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Deduction Category Available:\n");
    fprintf(file_path, "[1] Marital Status Deduction: %-5s %61.2f\n",
            Citizen->martialStatus ? "Yes" : "No",
            Citizen->martialStatus ? 5000.0 : 0.0);
    fprintf(file_path, "[2] Children Deduction: %-42s %30.2f\n",
            childTablestring[child_cat],
            child_deduction_amt);
    fprintf(file_path, "[3] Special Healthcare Deduction: %-5s %57.2f\n",
            Citizen->specialHealthcare ? "Yes" : "No",
            Citizen->specialHealthcare ? 20000.0 : 0.0);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Total Deduction: %80.2f\n", Citizen->totalDeduction);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Taxable Income: %81.2f\n", Citizen->taxableIncome);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Tax Rate: %86.2f%%\n", lookupTaxRate(Citizen->taxableIncome, brackets, 7) * 100);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Income Tax: %85.2f\n", Citizen->incomeTax);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Tax Per Month: %82.2f\n", Citizen->incomeTax / 12.0);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fclose(file_path);
}

void ViewAllCitizens() {
    ClearScreen();
    if (registered_user == 0) {
        printf("[!] No citizens registered yet.\nPress any key to return...");
        getch();
        return;
    }

    struct Citizen all_citizens[MAX_CITIZENS];
    for (int i = 0; i < registered_user; i++) {
        CalculateTax(i);
        all_citizens[i] = citizen_list[i];
    }

    ShowAndPrintCitizenTable(all_citizens, registered_user);
}

void AdminAddCitizen() {
    ClearScreen();
    if (registered_user >= MAX_CITIZENS) {
        printf("[!] Maximum user limit (%d) reached.\n", MAX_CITIZENS);
        getch();
        return;
    }

    int idx = registered_user;
    struct Citizen *new_citizen = &citizen_list[idx];
    memset(new_citizen, 0, sizeof(struct Citizen));
    char temp_input[150];

    while (1) {
        printf("--- Admin: Add New Citizen --- (press 0 to cancel)\n");
        printf("Citizen ID (Exact 12 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strcmp(temp_input, "0\n") == 0)
            return;

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Input too long!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) != 12 || CheckInputBlank(temp_input)) {
            printf("[!] Error: Citizen ID must be 12 characters!\n");
            getch();
            ClearScreen();
            continue;
        }

        if (!CheckIdUniqueness(temp_input)) {
            printf("[!] Error: Citizen ID already exists!\n");
            getch();
            ClearScreen();
            continue;
        }

        if (!ConfirmAction("Confirm Citizen ID?")) {
            ClearScreen();
            continue;
        }

        strcpy(new_citizen->citizenId, temp_input);
        strcpy(new_citizen->citizenPassword, temp_input); // Password = Citizen ID
        break;
    }

    while (1) {
        printf("Citizen Full Name (Max 49 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Name too long!\n");
            ClearInputBuffer();
            getch();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 50 || CheckInputBlank(temp_input)) {
            printf("[!] Error: Name must be 1-49 characters.\n");
            continue;
        }

        if (!ConfirmAction("Confirm Citizen Name?")) {
            continue;
        }

        strcpy(new_citizen->citizenName, temp_input);
        break;
    }

    registered_user++;
    new_citizen->LoginCount = 2;
    GetCitizenInfo(idx);
    printf("\n[+] Citizen added! Default password set to Citizen ID (%s).\nPress any key to return...", new_citizen->citizenId);
    getch();
}

void AdminMenu(int *running) {
    int admin_logged_in = 1;
    while (admin_logged_in) {
        int choice = SelectOption(admin_menu, 8);
        switch (choice) {
        case 0: // View All Citizens (Choose Categories + Print Option)
            ViewAllCitizens();
            break;
        case 1: { // Search Citizen Info (Choose Search Field -> Choose Display Categories -> Print Option)
            int cat = SelectOption(admin_category_Selection, 12);
            if (cat >= 1 && cat <= 11) {
                ClearScreen();
                char search_term[100];
                printf("Searching by %s\n", admin_category_Selection[cat]);
                printf("Enter search value: ");
                fgets(search_term, sizeof(search_term), stdin);
                RemoveNewLine(search_term);
                SearchInfobyCategory(cat, search_term);
            }
            break;
        }
        case 2: // Add Citizen (Password = ID)
            AdminAddCitizen();
            break;
        case 3: { // Edit Citizen Info
            ClearScreen();
            char target_id[30];
            printf("Enter Citizen ID to edit: ");
            fgets(target_id, sizeof(target_id), stdin);
            RemoveNewLine(target_id);
            int found_idx = -1;
            for (int i = 0; i < registered_user; i++) {
                if (strcmp(citizen_list[i].citizenId, target_id) == 0) {
                    found_idx = i;
                    break;
                }
            }
            if (found_idx == -1) {
                printf("[!] Citizen ID '%s' not found.\nPress any key to return...", target_id);
                getch();
            } else {
                EditCitizenInfo(found_idx, SelectOption(edit_menu, 7));
            }
            break;
        }
        case 4: // Overall Statistics (Choose Summary Categories -> Show -> Ask to Print)
            adminSummaryDashboard();
            break;
        case 5: { // Print Overall Statistics (Choose Summary Categories or Table Categories to Print)
            int selected_cats[15];
            int cat_count = 0;
            double threshold = 50000.0;
            if (SelectSummaryDashboardCategories(selected_cats, &cat_count, &threshold)) {
                ClearScreen();
                PrintadminSummaryDashboard(selected_cats, cat_count, threshold);
                printf("Press any key to return...");
                getch();
            }
            break;
        }
        case 6: // Logout
            ClearScreen();
            if (ConfirmAction("Logout ?")){
                admin_logged_in = 0;
            }

            break;
        case 7: // Exit
            ClearScreen();
            if (ConfirmAction("Exit ?"))
            {
                admin_logged_in = 0;
                *running = 0;
            }
            break;
        }
    }
}

int main() {
    LoadUserData();
    int running = 1;

    while (running) {
        int choice = SelectOption(login_menu, 3);
        if (choice == 0) {
            current_active_user = UserLogin();
            if (current_active_user == -2) {
                AdminMenu(&running);
            } else if (current_active_user != -1) {
                int logged_in = 1;
                while (logged_in) {
                    if (citizen_list[current_active_user].LoginCount == 1) {
                        GetCitizenInfo(current_active_user);
                        citizen_list[current_active_user].LoginCount++;
                    }
                    int dash_choice = SelectOption(dash_menu, 7);
                    switch (dash_choice) {
                    case 0:
                        EditCitizenInfo(current_active_user, SelectOption(edit_menu, 7));
                        ClearScreen();
                        break;
                    case 1: {
                        ClearScreen();
                        temp_value = CheckValidDouble("Please enter the taxable income to lookup the tax bracket: RM ");
                        double tax_rate = lookupTaxRate(temp_value, brackets, 7);
                        for (int i = 0; i < 6; i++) {
                            printf("Bracket %d: %.2f to %.2f, Tax Rate: %.2f%%\n",
                                   i + 1, brackets[i].minIncome, brackets[i].maxIncome, brackets[i].taxRate * 100);
                        }
                        printf("Bracket 7: %.2f and above, Tax Rate: %.2f%%\n",
                               brackets[6].minIncome, brackets[6].taxRate * 100);
                        printf("\n--------------------------------------------------------------------------\n");
                        printf("Your taxable income of %.2f falls into a tax rate of %.2f%%\n", temp_value, tax_rate * 100);
                        printf("Press any key to return...");
                        getch();
                        break;
                    }
                    case 2: {
                        ClearScreen();
                        temp_value = CheckValidDouble("Please enter the children below 18 count to lookup the child allowance: ");
                        double child_allowance = lookupChildAllowance((int)temp_value, (int)temp_value, childTable, 4);
                        for (int i = 0; i < 4; i++) {
                            printf("Child Allowance %c: %d to %d children, Amount: %.2f\n",'A' + i, childTable[i].minChildren, childTable[i].maxChildren, childTable[i].amount);
                        }
                        printf("\n--------------------------------------------------------------------------\n");
                        printf("Your children count of %d falls into a child allowance of %.2f\n", (int)temp_value, child_allowance);
                        printf("Press any key to return...");
                        getch();
                        break;
                    }
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
                        ClearScreen();
                        if (ConfirmAction("Logout?"))
                        {
                            logged_in = 0;
                        }
                        break;
                    case 6:
                        ClearScreen();
                        if(ConfirmAction("Exit?")){
                            running = 0;
                            logged_in = 0;
                        }
                        break;
                    }
                }
            }
        } else if (choice == 1) {
            RegisterUser();
        } else if (choice == 2) {
            ClearScreen();
            if (ConfirmAction("Exit ?")){
                running = 0;
            }
        }
    }
    return 0;
}

/*Need to be added ;
Confirm key for every input
Check unique ID 
Age must >= 18
change back to bin file
Add admin
Add search function for admin
admin add citizen then the password is equal to the citizen ID


*/