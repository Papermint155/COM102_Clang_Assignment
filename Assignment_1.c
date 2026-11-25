//---Rule---

// Function = ThisIsExample
// Variable = this_is_example
// Structure = thisIsExample

// Dont use Abbreviation, unless it is common
// Descripe as detail as possible */
//Ciallo～ (∠・ω< )⌒★
#include <stdio.h>
#include <stdlib.h> // fixed system cls,  atoi, atof and qsort
#include <string.h>
#include <math.h>  // for isnan() function
#include <conio.h> // getch() function
#include <stdbool.h>
#include <ctype.h>

#define MAX_CITIZENS 30 //max user
#define MAX_BRACKETS 20
#define MAX_CHILD_ALLOWANCE 20
#define MAX_DEDUCTIONS 20

int current_active_user = -1; // store the index of the currently logged-in user, -1 means no user is logged in , -2 is admin
int registered_user = 0; // counter for the number of registered users
int bracket_count = 0;
int child_table_count = 0;
int deduction_count = 0;
double temp_value = 0.0; //temp variable to used in the taxbracket and child allowance look up

//Menu to edit profile info
char edit_menu[7][40] = {
    "Back/Return",
    "Edit Age",
    "Edit Marital Status",
    "Edit Annual Income",
    "Edit Number of Children",
    "Edit Number of Children Under 18",
    "Edit Special Healthcare Status"
};

// the login menu of the sys
char login_menu[3][40] = {"Login", "Register New User", "Exit"};

//menu for citizen
char dash_menu[7][40] = {
    "Edit Citizen Info",
    "Lookup Tax Bracket",
    "Lookup Child Allowance",
    "Tax Dashboard",
    "Print Citizen Info",
    "Logout",
    "Exit"
};

// menu for admin 
char admin_menu[11][40] = {
    "View All Citizens",
    "Search Citizen Info",
    "Add Citizen",
    "Edit Citizen Info",
    "Delete Citizen",
    "Watch Citizen Personal Report",
    "Overall Statistics",
    "Print Overall Statistics",
    "Manage Tax & Allowance Rules",
    "Logout",
    "Exit"
};

//menu for admin selection need to show in the report
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

// menu for sorting 
char sort_order_menu[2][40] = {
    "Ascending Order (A-Z / Low to High)",
    "Descending Order (Z-A / High to Low)"
};


//menu for admin to do the summary
char adminSummaryDashboardChoice[19][60] = {
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
    "Total Annual Income of All Citizens",
    "Total Taxable Income of All Citizens",
    "All Summary Categories (Default)",
    "Back to Admin Menu"
};
//Ciallo～ (∠・ω< )⌒★
struct ChildAllowance {
    char category[25];
    int minChildren;
    int maxChildren;
    double amount;
};

//Table data for child allowance
struct ChildAllowance childTable[MAX_CHILD_ALLOWANCE];

//Structure of the Deduction (save the fixed deduction amount, child allowance is in childTable)
struct Deduction {
    char category[25];
    double amount;
};

//Table data for the Deduction
struct Deduction deductionTable[MAX_DEDUCTIONS];

//declaration of the Deduction Category (which deduction the citizen get)
struct Deduction_Category{
    int category[3];
    double deductionAmount;
};

//Structure of the citizen
struct Citizen{
    char citizenID[13];
    char citizenName[50];
    char citizenPassword[20];
    int LoginCount;
    int age;
    int maritalStatus;
    double annualIncome;
    int numberOfChildren;
    int childrenUnder18;
    int specialHealthcare;
    double totalDeduction;
    double taxableIncome;
    double incomeTax;
    struct Deduction_Category deduction[1];
};

//struct for TaxBracket 
struct TaxBracket{
    double minIncome;
    double maxIncome;
    double taxRate;
};

//Initalize data in the Tax Bracket

struct TaxBracket brackets[MAX_BRACKETS];

//save the citizen struct in a citizenlist array 
struct Citizen citizen_list[MAX_CITIZENS];

// decalration of the function (as it add after)
void CalculateTax(int user_index);
void PrintadminSummaryDashboard(int selected_cats[], int cat_count, double threshold);
void PrintCustomCitizenTableToFile(struct Citizen list[], int count, int dashboard_ntb_print[], int counter);

// remove new line indicator when user press enter 
void RemoveNewLine(char *str)
{
    str[strcspn(str, "\n")] = '\0'; // find and replace /n to /0
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
    // clrscr(); cant u se (got bug)
    //  printf("\033[H\033[2J"); This method not working (By printing a lot of new line)
    //  fflush(stdout);
}

int ConfirmAction(const char *prompt) { // send the pointer to the message in here 
    printf("%s (Press 'Y' or Enter to confirm, 'N' to cancel/redo): ", prompt); // print the prompt
    while (1) { // looping 
        int ch = getch();
        if (ch == 'y' || ch == 'Y' || ch == 13) { // check y, Y,and enter 
            printf("Yes\n");
            return 1;
        } else if (ch == 'n' || ch == 'N') {
            printf("No\n");
            return 0;
        }
    }
}//mark 

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

double CheckValidDoubleRange(const char *message, double min_val, double max_val) {
    char buf[100];
    char *endptr;
    double value;

    while (1) {
        printf("%s", message);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (strchr(buf, '\n') == NULL) {
            printf("[!] Error: Input too long!\n");
            ClearInputBuffer();
            continue;
        }
        RemoveNewLine(buf);
        if (CheckInputBlank(buf)) {
            printf("[!] Error: Input cannot be blank.\n");
            continue;
        }
        value = strtod(buf, &endptr);
        while (*endptr == ' ' || *endptr == '\t') endptr++; // allow trailing spaces only
        if (*endptr != '\0' || isnan(value) || value < min_val || value > max_val) {
            printf("[!] Error: Please enter a valid number between %.2f and %.2f.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}

double CheckValidDouble(char *message) {
    return CheckValidDoubleRange(message, 0.0, 1.7976931348623157E+308);
}

int CheckValidInt(const char *message, int min_val, int max_val) {
    char buf[100];
    char *endptr;
    long value;

    while (1) {
        printf("%s", message);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (strchr(buf, '\n') == NULL) {
            printf("[!] Error: Input too long!\n");
            ClearInputBuffer();
            continue;
        }
        RemoveNewLine(buf);
        if (CheckInputBlank(buf)) {
            printf("[!] Error: Input cannot be blank.\n");
            continue;
        }
        value = strtol(buf, &endptr, 10);
        while (*endptr == ' ' || *endptr == '\t') endptr++;
        if (*endptr != '\0' || value < min_val || value > max_val) {
            printf("[!] Error: Please enter a valid whole number between %d and %d.\n", min_val, max_val);
            continue;
        }
        return (int)value;
    }
}

int IsValidName(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]) && str[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int IsValidID(const char *str) {
    if (strlen(str) != 12) return 0;
    for (int i = 0; i < 12; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

void GetValidCategoryName(char *dest, int max_len, const char *prompt) {
    char buf[100];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) continue;
        if (strchr(buf, '\n') == NULL) {
            printf("[!] Error: Name too long (max %d chars)!\n", max_len - 1);
            ClearInputBuffer();
            continue;
        }
        RemoveNewLine(buf);
        if (CheckInputBlank(buf) || (int)strlen(buf) >= max_len) {
            printf("[!] Error: Name must be 1-%d characters.\n", max_len - 1);
            continue;
        }
        if (strchr(buf, '"') != NULL) {
            printf("[!] Error: Double quotes (\") are not allowed in names.\n");
            continue;
        }
        strcpy(dest, buf);
        return;
    }
}

int SelectOption(char option[][40], int total_choice)
{ // havent conplete yet //Ciallo～ (∠・ω< )⌒★
    int current_selection = 0;
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
    1 - 1 + 3 = 3 % 3 = 0x
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
                printf("[%s] ", admin_category_Selection[dashboard_ntb_print[k]]); // print the cate selected 
            }
        }
        printf("\n-------------------------------------------------------------------------------------------------\n\n");

        for (int i = 0; i < 13; i++) {
            if (i == current_selection) {
                printf("-> %s\n", admin_category_Selection[i]); // print the arrow aside the current selction 
            } else {
                printf("   %s\n", admin_category_Selection[i]);
            }
        }
        // similar thing to the selected option
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
                    dashboard_ntb_print[c - 1] = c;// add the indicator one by one to the array ntb print 
                }
                *counter = 11;
                break;
            } else {
                // Check if already selected to avoid duplicates
                int already_added = 0;
                for (int k = 0; k < *counter; k++) {
                    if (dashboard_ntb_print[k] == current_selection) {// loop few time to check selection is it inside
                        already_added = 1;
                        break;
                    }
                }
                if (!already_added) {
                    dashboard_ntb_print[*counter] = current_selection; // if not added then added it and counter
                    (*counter)++;
                }
            }
        }
    }
}

int CheckIdUniqueness(char *id) { // check uquinees in id
    for (int i = 0; i < registered_user; i++) {
        if (strcmp(citizen_list[i].citizenID, id) == 0) { // cmp one b 1 to check is have the same?
            return 0; // ID is not unique
        }
    }
    return 1; // ID is unique
}



double lookupTaxRate(double taxableIncome, struct TaxBracket table[], int size) {
    if (size <= 0) return 0.0;
    if (taxableIncome <= table[0].maxIncome) {
        return table[0].taxRate; // if less then first category direct return, save calculation
    }
    for (int i = 1; i < size; i++) {
        if (taxableIncome > table[i].minIncome && taxableIncome <= table[i].maxIncome) { // search and compare 1b1 
            return table[i].taxRate; // return tax rate if it fall in the cate
        }
    }
    printf("[!] Error: Taxable income is out of the defined tax brackets.\n"); // error code if not find 
    return 0.0;
}

double lookupChildAllowance(int totalChildren, int under18, struct ChildAllowance table[], int size) { // this is same logic as the lookuptax
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
    citizen->deduction[0].category[1] = 0; // 0 = Not Eligible
    if (citizen->numberOfChildren <= 0 || citizen->numberOfChildren != citizen->childrenUnder18) {
        return;
    }
    for (int i = 0; i < child_table_count; i++) {
        if (citizen->childrenUnder18 >= childTable[i].minChildren &&
            citizen->childrenUnder18 <= childTable[i].maxChildren) {
            citizen->deduction[0].category[1] = i + 1;
            return;
        }
    }
}

void SaveUserData(){
    FILE *file_path = fopen("userdata.bin", "wb"); // 'wb' = write binary
    //FILE *file_path = fopen("userdata.txt", "w"); // 'wb' = write binary
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
    //FILE *file_path = fopen("userdata.txt", "r"); // use txt file for visualise the data in it
    if (file_path == NULL)
    {           // check weather it is no file
        return; // No file yet, just return without value to exit the funtion
    }
    fread(&registered_user, sizeof(int), 1, file_path);         // size_t fread(void *ptr, size_t size, size_t count, FILE *stream) <- the syntax
    fread(citizen_list, sizeof(struct Citizen), 30, file_path); // read the file according to section ( sizeof(struct userProfile)), read 30 time, and read from file-path then save to student_list
    fclose(file_path);
}

void LoadSystemData() {
    FILE *fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("[!] Error: data.txt not found!\n");
        getch();
        return;
    }

    fscanf(fp, "%d %d %d\n", &bracket_count, &child_table_count, &deduction_count);

    if (bracket_count < 0) bracket_count = 0;
    if (bracket_count > MAX_BRACKETS) bracket_count = MAX_BRACKETS;
    if (child_table_count < 0) child_table_count = 0;
    if (child_table_count > MAX_CHILD_ALLOWANCE) child_table_count = MAX_CHILD_ALLOWANCE;
    if (deduction_count < 0) deduction_count = 0;
    if (deduction_count > MAX_DEDUCTIONS) deduction_count = MAX_DEDUCTIONS;

    for (int i = 0; i < bracket_count; i++) {
        fscanf(fp, "%lf, %lf, %lf\n",&brackets[i].minIncome, &brackets[i].maxIncome, &brackets[i].taxRate);
    }
    for (int i = 0; i < child_table_count; i++) {
        fscanf(fp, " \"%24[^\"]\", %d, %d, %lf\n", childTable[i].category, &childTable[i].minChildren,&childTable[i].maxChildren, &childTable[i].amount);
    }
    for (int i = 0; i < deduction_count; i++) {
        fscanf(fp, " \"%24[^\"]\", %lf\n",deductionTable[i].category, &deductionTable[i].amount);
    }
    fclose(fp);
}

void SaveSystemData() {
    FILE *fp = fopen("data.txt", "w");
    if (fp == NULL) {
        printf("[!] Error: Could not save to data.txt!\n");
        getch();
        return;
    }

    fprintf(fp, "%d %d %d\n", bracket_count, child_table_count, deduction_count);

    for (int i = 0; i < bracket_count; i++) {
        fprintf(fp, "%.1f, %.17g, %.2f\n",
                brackets[i].minIncome, brackets[i].maxIncome, brackets[i].taxRate);
    }
    for (int i = 0; i < child_table_count; i++) {
        fprintf(fp, "\"%s\", %d, %d, %.1f\n",
                childTable[i].category, childTable[i].minChildren,
                childTable[i].maxChildren, childTable[i].amount);
    }
    for (int i = 0; i < deduction_count; i++) {
        fprintf(fp, "\"%s\", %.1f\n",
                deductionTable[i].category, deductionTable[i].amount);
    }
    fclose(fp);
}

void RegisterUser() { // use to register user 
    ClearScreen();
    if (registered_user >= MAX_CITIZENS) { // check the database is it full 
        printf("Maximum user limit (%d) reached.\n", MAX_CITIZENS); // error code
        getch();
        return;
    }

    struct Citizen *new_citizen = &citizen_list[registered_user]; //point a pointer to the the original struct
    memset(new_citizen, 0, sizeof(struct Citizen));// function in C and C++ fills a specified block of memory with a constant byte value. It is commonly used to zero out buffers, initialize arrays, or clear sensitive data like passwords
    // zeroing the struct
    char temp_input[150]; // large range to have buffer

    while (1) {
        printf("--- Register New User --- (press 0 to cancel)\n");
        printf("Citizen ID (Exact 12 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strcmp(temp_input, "0\n") == 0) // it is O ? to exit 
            return;

        if (strchr(temp_input, '\n') == NULL) { //check is it just enter  
            printf("[!] Error: Invalid ID Number!\n");
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input); //remove the enter indicator 
        if (!IsValidID(temp_input)) {
            printf("[!] Error: Citizen ID must be exactly 12 digits (0-9)!\n");
            getch();
            ClearScreen();
            continue;
        }   

        if (!CheckIdUniqueness(temp_input)) { // check the id is it unique ?
            printf("[!] Error: Citizen ID already exists!\n"); //if not print erro code 
            getch();
            ClearScreen();
            continue;
        }

        if (!ConfirmAction("Confirm Citizen ID?")) { // confirm the enter value
            ClearScreen();
            continue;
        }

        strcpy(new_citizen->citizenID, temp_input); // after all just cpy the temp value to the struct 
        break;
    }

    while (1) {
        printf("Username (Max 49 characters): "); 
        fgets(temp_input, sizeof(temp_input), stdin); // get username 

        if (strcmp(temp_input, "0\n") == 0) // see user want to return 
            return;

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Username too long!\n"); // if the name is cut off then mean no \n aka name too long
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 50 || CheckInputBlank(temp_input) || !IsValidName(temp_input)) {
            printf("[!] Error: Name must be 1-49 letters/spaces only.\n");
            getch();
            ClearScreen();
            continue;
        }

        if (!ConfirmAction("Confirm Username?")) { // action confrimation 
            continue;
        }

        strcpy(new_citizen->citizenName, temp_input); // cpy the input to struct
        break;
    }
    while (1) {
        printf("Password (Max 19 characters): ");
        fgets(temp_input, sizeof(temp_input), stdin);

        if (strchr(temp_input, '\n') == NULL) {
            printf("[!] Error: Password too long!\n"); // same logic as above  (cutoff the \n mean too long )
            ClearInputBuffer();
            getch();
            continue;
        }

        RemoveNewLine(temp_input);
        if (strlen(temp_input) >= 20 || CheckInputBlank(temp_input)) { //double check
            printf("[!] Error: Password must be 1-19 characters.\n");
            getch();
            continue;
        }

        if (!ConfirmAction("Confirm Password?")) { // confirmation
            continue;
        }

        strcpy(new_citizen->citizenPassword, temp_input); // copy to struct 
        break;
    }


    registered_user++; // add the count of register counter 
    new_citizen->LoginCount = 0; // set the login count to o0 as need to intialize their info for first  log in 
    SaveUserData();
    printf("\nNew User Registration Success! \nPress any key to continue...");//Ciallo～ (∠・ω< )⌒★
    getch(); // wait till the user want to proceed ;
}

int UserLogin() { //login function 
    char input_ID[50], input_password[50];
    ClearScreen();

    while (1) {
        printf("--- Login --- (press 0 to cancel)\n");
        printf("ID (12 characters): ");
        fgets(input_ID, sizeof(input_ID), stdin);

        if (strcmp(input_ID, "0\n") == 0) // check he want to exit or not 
            return -1;

        if (strchr(input_ID, '\n') == NULL) {
            printf("[!] Error: ID too long!\n"); // check for cut off 
            ClearInputBuffer();
            getch();
            ClearScreen();
            continue;
        }

        RemoveNewLine(input_ID);
        if (strcmp(input_ID, "admin") != 0 && (strlen(input_ID) != 12 || CheckInputBlank(input_ID))) { //the is format is correct ??
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
            printf("[!] Error: Password too long!\n");//check for cut off
            ClearInputBuffer();
            getch();
            continue;
        }
        RemoveNewLine(input_password);
        if (strlen(input_password) >= 20 || CheckInputBlank(input_password)) {
            printf("[!] Error: Password must be 1-19 characters.\n"); //double check the format 
            continue;
        }
        break;
    }

    if (strcmp(input_ID, "admin") == 0 && strcmp(input_password, "admin123") == 0) {
        return -2; // Admin login only 
    }

    if (registered_user == 0) { // ensure O(1) prevent search trought entire thing if there is no record 
        printf("No user found. Please register first!\nPress any key to return...");
        getch();
        return -1;
    }

    for (int i = 0; i < registered_user; i++) {
        if (strcmp(input_ID, citizen_list[i].citizenID) == 0 &&
            strcmp(input_password, citizen_list[i].citizenPassword) == 0) { //loop trough the list to compare the password and the id ans retrun index of that user 
            citizen_list[i].LoginCount++;
            return i;
        }
    }

    printf("\n[!] Invalid login.\nPress any key to continue..."); // login fail error prompt 
    getch();
    return -1;
}

void GetCitizenInfo(int user_index) { // get the basic info of the citizen
    struct Citizen *citizen = &citizen_list[user_index];
    while (1) {
            ClearScreen();
            int val = CheckValidInt("Enter Age (18 - 149): ", 18, 149);
            if (ConfirmAction("Confirm Age?")) {
                citizen->age = val;
                break;
            }
        }

    // Marital Status
    while (1) {
        ClearScreen();
        int val = CheckValidInt("Enter Marital Status (0 for Single, 1 for Married): ", 0, 1);
        if (ConfirmAction("Confirm Marital Status?")) {
            citizen->maritalStatus = val;
            citizen->deduction[0].category[0] = val;
            break;
        }
    }

    ClearScreen();//Ciallo～ (∠・ω< )⌒★
    citizen->annualIncome = CheckValidDouble("Enter Annual Income: RM "); // get the Annual income and also check the data valid

    // Number of Children
    while (1) {
        ClearScreen();
        int val = CheckValidInt("Enter Number of Children (0 - 19): ", 0, 19);
        if (ConfirmAction("Confirm Number of Children?")) {
            citizen->numberOfChildren = val;
            break;
        }
    }

    // Children Under 18
    while (1) {
        ClearScreen();
        char prompt[80];
        snprintf(prompt, sizeof(prompt), "Enter Number of Children Under 18 (0 - %d): ", citizen->numberOfChildren);
        int val = CheckValidInt(prompt, 0, citizen->numberOfChildren);
        if (ConfirmAction("Confirm Number of Children Under 18?")) {
            citizen->childrenUnder18 = val;
            UpdateChildCategoryIndex(citizen);
            break;
        }
    }

    // Special Healthcare Status
    while (1) {
        ClearScreen();
        int val = CheckValidInt("Special Healthcare Status (0 for No, 1 for Yes): ", 0, 1);
        if (val == 1 && citizen->maritalStatus == 0 && citizen->numberOfChildren == 0) {
            printf("[!] Error: Special healthcare is only for spouse or child. Single citizen with no child cannot claim it.\n");
            getch();
            continue;
        }
        if (ConfirmAction("Confirm Special Healthcare Status?")) {
            citizen->specialHealthcare = val;
            citizen->deduction[0].category[2] = val;
            break;
        }
    }

    CalculateTax(user_index); // calc the tax
    SaveUserData(); // save the data once it change
}

void EditCitizenInfo(int user_index, int field_choice) { //edit the user info
    if (field_choice == 0) //check user want to exit
        return;

    struct Citizen *citizen = &citizen_list[user_index];
    switch (field_choice) { // switch case
    case 1: //edit age 
        while (1) {
            ClearScreen();
            int val = CheckValidInt("Enter Age (18 - 149): ", 18, 149);
            if (ConfirmAction("Confirm Age?")) {
                citizen->age = val;
                break;
            }
        }
        break;
    case 2: // edit marital stat
        while (1) {
            ClearScreen();
            int val = CheckValidInt("Enter Marital Status (0 for Single, 1 for Married): ", 0, 1);
            if (ConfirmAction("Confirm Marital Status?")) {
                citizen->maritalStatus = val;
                citizen->deduction[0].category[0] = val;
                break;
            }
        }
        break;
    case 3: // edit income
        ClearScreen();
        citizen->annualIncome = CheckValidDouble("Enter New Annual Income: RM ");
        break;
    case 4: // edit num child
        while (1) {
            ClearScreen();
            int val = CheckValidInt("Enter New Number of Children (0 - 19): ", 0, 19);
            if (ConfirmAction("Confirm New Number of Children?")) {
                citizen->numberOfChildren = val;
                if (citizen->childrenUnder18 > citizen->numberOfChildren) {
                    citizen->childrenUnder18 = citizen->numberOfChildren;
                }
                UpdateChildCategoryIndex(citizen);
                break;
            }
        }
        break;
    case 5: // edit num of child under 18
        while (1) {
            ClearScreen();
            char prompt[80];
            snprintf(prompt, sizeof(prompt), "Enter New Number of Children Under 18 (0 - %d): ", citizen->numberOfChildren);
            int val = CheckValidInt(prompt, 0, citizen->numberOfChildren);
            if (ConfirmAction("Confirm New Number of Children Under 18?")) {
                citizen->childrenUnder18 = val;
                UpdateChildCategoryIndex(citizen);
                break;
            }
        }
        break;
    case 6: // edit healthcare stat
        while (1) {
            ClearScreen();
            int val = CheckValidInt("Special Healthcare Status (0 for No, 1 for Yes): ", 0, 1);
            if (val == 1 && citizen->maritalStatus == 0 && citizen->numberOfChildren == 0) {
                printf("[!] Error: Special healthcare is only for spouse or child. Single citizen with no child cannot claim it.\n");
                getch();
                continue;
            }
            if (ConfirmAction("Confirm Special Healthcare Status?")) {
                citizen->specialHealthcare = val;
                citizen->deduction[0].category[2] = val;
                break;
            }
        }
        break;
    default:
        printf("[!] Error: Invalid field choice.\n");
        break;
    }

    CalculateTax(user_index); // calc agina to prevent worng vale of data 
    SaveUserData(); //save data after change 
}

void CalculateTax(int user_index) { //calc tax function```17-6
    struct Citizen *citizen = &citizen_list[user_index];
    citizen->totalDeduction = 0.0; // intialize the value 

    if (citizen->maritalStatus) { // married
        citizen->totalDeduction += deductionTable[0].amount; // marital allowance
    }
    if (citizen->numberOfChildren > 0) { // have child
        citizen->totalDeduction += lookupChildAllowance(citizen->numberOfChildren, citizen->childrenUnder18, childTable, child_table_count); // check the vlaue in the table
    }
    if (citizen->specialHealthcare && (citizen->maritalStatus || citizen->numberOfChildren > 0)) { // got healthcare AND have spouse or child
        citizen->totalDeduction += deductionTable[1].amount; // special healthcare allowance
    } else {
        citizen->specialHealthcare = 0; // single with no child cannot claim, so reset to No
        citizen->deduction[0].category[2] = 0;
    }

    UpdateChildCategoryIndex(citizen); // see whicj cate gory the user belong to 
    citizen->deduction[0].deductionAmount = citizen->totalDeduction; // copy the deduction amount 

    citizen->taxableIncome = citizen->annualIncome - citizen->totalDeduction; // calc the taxable income 
    if (citizen->taxableIncome < 0.0) {  // if the thing deduct to be negative the the taxable income is 0 
        citizen->taxableIncome = 0.0;
    }

    citizen->incomeTax = citizen->taxableIncome * lookupTaxRate(citizen->taxableIncome, brackets, bracket_count); // add user taxrate using lookup on the table 
    SaveUserData();
}

void SortCitizens(struct Citizen list[], int count, int sortbycategory, int sortorder) { // sorting algorthm 
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            int cmp = 0;
            switch (sortbycategory) {
            case 1: // sort by id _stricmp()
                cmp = _stricmp(list[j].citizenID, list[j + 1].citizenID);
                break;
            case 2: // sort by Name
                cmp = _stricmp(list[j].citizenName, list[j + 1].citizenName);
                break;
            case 3: // by age
                cmp = (list[j].age > list[j + 1].age) - (list[j].age < list[j + 1].age);
                break;
            case 4: // bt marital stat 
                cmp = (list[j].maritalStatus > list[j + 1].maritalStatus) - (list[j].maritalStatus < list[j + 1].maritalStatus);
                break;
            case 5: // by annual income
                cmp = (list[j].annualIncome > list[j + 1].annualIncome) - (list[j].annualIncome < list[j + 1].annualIncome);
                break;
            case 6: // by numchild
                cmp = (list[j].numberOfChildren > list[j + 1].numberOfChildren) - (list[j].numberOfChildren < list[j + 1].numberOfChildren);
                break;
            case 7: // by childunder18
                cmp = (list[j].childrenUnder18 > list[j + 1].childrenUnder18) - (list[j].childrenUnder18 < list[j + 1].childrenUnder18);
                break;
            case 8: // by health care 
                cmp = (list[j].specialHealthcare > list[j + 1].specialHealthcare) - (list[j].specialHealthcare < list[j + 1].specialHealthcare);
                break;
            case 9: //by deduction 
                cmp = (list[j].totalDeduction > list[j + 1].totalDeduction) - (list[j].totalDeduction < list[j + 1].totalDeduction);
                break;
            case 10: // by  taxable income
                cmp = (list[j].taxableIncome > list[j + 1].taxableIncome) - (list[j].taxableIncome < list[j + 1].taxableIncome);
                break;
            case 11: // the income tax 
                cmp = (list[j].incomeTax > list[j + 1].incomeTax) - (list[j].incomeTax < list[j + 1].incomeTax);
                break;
            default:
                cmp = 0; // by deduct the both function 
                break;
            }

            if ((sortorder == 0 && cmp > 0) || (sortorder == 1 && cmp < 0)) { // sort order  = 0 asending vice versa 
                struct Citizen temp = list[j]; // temp list to do the bubble sort 
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void WriteGrandTotal(FILE *stream, struct Citizen list[], int count) { // print the grand total under the table (stdout = screen, fp = file)
    double total_income = 0.0, total_deduction = 0.0, total_taxable = 0.0, total_tax = 0.0;
    for (int i = 0; i < count; i++) { // add up the amount of every citizen in the table
        total_income += list[i].annualIncome;
        total_deduction += list[i].totalDeduction;
        total_taxable += list[i].taxableIncome;
        total_tax += list[i].incomeTax;
    }
    fprintf(stream, "GRAND TOTAL (%d Citizens)\n", count);
    fprintf(stream, "Total Annual Income  : RM %.2f\n", total_income);
    fprintf(stream, "Total Deduction      : RM %.2f\n", total_deduction);
    fprintf(stream, "Total Taxable Income : RM %.2f\n", total_taxable);
    fprintf(stream, "Total Income Tax     : RM %.2f\n", total_tax);
}

void PrintCustomCitizenTableToFile(struct Citizen list[], int count, int dashboard_ntb_print[], int counter) {
    FILE *fp = fopen("Citizen_Tax_report.txt", "a"); // "a" = append, so it will not erase the summary report already in the file
    if (fp == NULL) {
        printf("[!] Error: Unable to write to 'Citizen_Tax_report.txt'!\n");
        getch();
        return;
    }

    fprintf(fp, "============================================================================================================\n"); //ultimate header 
    fprintf(fp, "                                  CUSTOM CITIZEN TAX DASHBOARD REPORT                                       \n");
    fprintf(fp, "============================================================================================================\n");//Ciallo～ (∠・ω< )⌒★

    for (int j = 0; j < counter; j++) {
        fprintf(fp, "%-18s | ", admin_category_Selection[dashboard_ntb_print[j]]); //print the category need to print for the header 
    }
    fprintf(fp, "\n------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        struct Citizen *citizen = &list[i]; // check if the category is need o be print , if yes write to file 
        for (int j = 0; j < counter; j++) {
            int category = dashboard_ntb_print[j];
            switch (category) {
            case 1:
                fprintf(fp, "%-18s | ", citizen->citizenID);
                break;
            case 2:
                fprintf(fp, "%-18.18s | ", citizen->citizenName);
                break;
            case 3:
                fprintf(fp, "%-18d | ", citizen->age);
                break;
            case 4:
                fprintf(fp, "%-18s | ", citizen->maritalStatus ? "Married" : "Single");
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
    fprintf(fp, "------------------------------------------------------------------------------------------------------------\n");
    WriteGrandTotal(fp, list, count); // grand total amount of the table
    fprintf(fp, "============================================================================================================\n\n"); //ending
    fclose(fp);
    printf("\n[+] Selected dashboard categories successfully added to 'Citizen_Tax_report.txt'!\n"); // inform the user already been print
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
        printf("%-18s | ", admin_category_Selection[dashboard_ntb_print[j]]); //PRINT category needed 
    }
    printf("\n------------------------------------------------------------------------------------------------------------\n");//Ciallo～ (∠・ω< )⌒★

    for (int i = 0; i < found; i++) {
        struct Citizen *citizen = &matched_citizens[i]; // print the category needed
        for (int j = 0; j < counter; j++) {
            int category = dashboard_ntb_print[j];
            switch (category) {
            case 1:
                printf("%-18s | ", citizen->citizenID);
                break;
            case 2:
                printf("%-18.18s | ", citizen->citizenName);
                break;
            case 3:
                printf("%-18d | ", citizen->age);
                break;
            case 4:
                printf("%-18s | ", citizen->maritalStatus ? "Married" : "Single");
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
    WriteGrandTotal(stdout, matched_citizens, found); // grand total amount of the table
    printf("------------------------------------------------------------------------------------------------------------\n");//Ciallo～ (∠・ω< )⌒★

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

int CompareDoubles(const void *a, const void *b) { // compare the double 
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}

void WriteSingleSummaryStat(FILE *stream, int stat_index, double threshold) { // calculate the summary state  
    int special_hc_count = 0, married_count = 0, single_count = 0; // declare all variable used 
    int with_children = 0, with_children_u18 = 0, above_threshold = 0;
    double total_annual_income = 0.0, total_taxable_income = 0.0;
    double total_tax_collected = 0.0, total_deductions = 0.0;
    double tax_array[MAX_CITIZENS];

    for (int i = 0; i < registered_user; i++) {
        CalculateTax(i); // clacualted tax for each user 
        if (citizen_list[i].specialHealthcare)
            special_hc_count++; //add yhe count of this stat 
        if (citizen_list[i].maritalStatus)
            married_count++; //same 
        else
            single_count++;
        if (citizen_list[i].numberOfChildren > 0)
            with_children++;
        if (citizen_list[i].childrenUnder18 > 0)
            with_children_u18++;
        if (citizen_list[i].annualIncome > threshold)
            above_threshold++; //all same 

        total_annual_income += citizen_list[i].annualIncome; // than this is add up all thing 
        total_taxable_income += citizen_list[i].taxableIncome;
        total_tax_collected += citizen_list[i].incomeTax;
        total_deductions += citizen_list[i].totalDeduction;
        tax_array[i] = citizen_list[i].incomeTax;
    }

    double avg_annual_income = (registered_user > 0) ? (total_annual_income / registered_user) : 0.0; //R&K method 
    double avg_taxable_income = (registered_user > 0) ? (total_taxable_income / registered_user) : 0.0; // prevent divide by 0
    double mean_tax = (registered_user > 0) ? (total_tax_collected / registered_user) : 0.0;

    qsort(tax_array, registered_user, sizeof(double), CompareDoubles); // soriting to cal the median 
    double median_tax = 0.0;
    if (registered_user > 0) {
        if (registered_user % 2 == 0)
            median_tax = (tax_array[(registered_user / 2) - 1] + tax_array[registered_user / 2]) / 2.0;
        else
            median_tax = tax_array[registered_user / 2]; // calculate the median 
    }

    double variance_sum = 0.0; // calc the variance 
    for (int i = 0; i < registered_user; i++) {
        variance_sum += pow(citizen_list[i].incomeTax - mean_tax, 2);
    }
    double std_dev_tax = (registered_user > 0) ? sqrt(variance_sum / registered_user) : 0.0; // std for data 

    switch (stat_index) {
    case 1:
        fprintf(stream, "%-62s : %d\n", adminSummaryDashboardChoice[1], registered_user); // print the selected category that admin whatnto see 
        break;
    case 2:
        fprintf(stream, "%-62s : %d\n", adminSummaryDashboardChoice[2], special_hc_count);
        break;
    case 3:
        fprintf(stream, "%-62s : %d\n", adminSummaryDashboardChoice[3], married_count);
        break;
    case 4:
        fprintf(stream, "%-62s : %d\n", adminSummaryDashboardChoice[4], single_count);
        break;
    case 5:
        fprintf(stream, "Citizens with Annual Income > RM %-29.2f : %d\n", threshold, above_threshold);
        break;
    case 6:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[6], avg_annual_income);
        break;
    case 7:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[7], avg_taxable_income);
        break;
    case 8:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[8], total_tax_collected);
        break;
    case 9:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[9], total_deductions);
        break;
    case 10:
        fprintf(stream, "%-62s : %d\n", adminSummaryDashboardChoice[10], with_children);
        break;
    case 11:
        fprintf(stream, "%-62s : %d\n", adminSummaryDashboardChoice[11], with_children_u18);//Ciallo～ (∠・ω< )⌒★
        break;
    case 12:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[12], mean_tax);
        break;
    case 13:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[13], median_tax);
        break;
    case 14:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[14], std_dev_tax);
        break;
    case 15:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[15], total_annual_income);
        break;
    case 16:
        fprintf(stream, "%-62s : RM %.2f\n", adminSummaryDashboardChoice[16], total_taxable_income);
        break;
    }
}


int SelectSummaryDashboardCategories(int selected_cats[], int *cat_count, double *threshold) {
    *cat_count = 0;
    *threshold = 50000.0;
    int current_selection = 0;

    while (*cat_count < 16) { // 16 category
        ClearScreen();
        printf("Select Summary Categories to include in the Dashboard:\n");
        printf("(Choose 'All Summary Categories (Default)' for all, or 'Done' when ready)\n"); // header
        printf("-------------------------------------------------------------------------------------------\n");
        printf("Selected Count: %d / 16\n", *cat_count);
        printf("-------------------------------------------------------------------------------------------\n\n");

        for (int i = 0; i < 19; i++) {
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
                current_selection = (current_selection - 1 + 19) % 19;
            if (key_pressed == 80)
                current_selection = (current_selection + 1) % 19;
        } else if (key_pressed == 13) {
            if (current_selection == 18) { // Back to Admin Menu
                return 0;
            } else if (current_selection == 0) { // Done / Show Selected
                if (*cat_count == 0) {
                    for (int c = 1; c <= 16; c++)
                        selected_cats[c - 1] = c;
                    *cat_count = 16;
                }
                return 1;
            } else if (current_selection == 17) { // All Summary Categories (Default)
                for (int c = 1; c <= 16; c++)
                    selected_cats[c - 1] = c;
                *cat_count = 16;
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

void SearchInfobyCategory(int category_choice, char *search_term) { // search function by cate 
    ClearScreen();
    // Validate numeric search terms before looping
    if (category_choice == 3 || category_choice == 6 || category_choice == 7) {
        char *endptr;
        strtol(search_term, &endptr, 10);
        if (CheckInputBlank(search_term) || *endptr != '\0') {
            printf("[!] Error: Please enter a valid whole number for this search category.\nPress any key to return...");
            getch();
            return;
        }
    } else if (category_choice == 5 || category_choice == 9 || category_choice == 10 || category_choice == 11) {
        char *endptr;
        strtod(search_term, &endptr);
        if (CheckInputBlank(search_term) || *endptr != '\0') {
            printf("[!] Error: Please enter a valid numeric amount for this search category.\nPress any key to return...");
            getch();
            return;
        }
    }
    struct Citizen matched_citizens[MAX_CITIZENS]; //open a struct to save the match citizen 
    int found = 0;

    for (int i = 0; i < registered_user; i++) {
        CalculateTax(i); //calculate the tax agina to prevent mis update 
        struct Citizen *citizen = &citizen_list[i]; 
        int match = 0; // match counter 

        switch (category_choice) { //search by 
        case 1: // ID
            if (strstr(citizen->citizenID, search_term) != NULL)
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
            if ((_stricmp(search_term, "Married") == 0 && citizen->maritalStatus) ||
                (_stricmp(search_term, "Single") == 0 && !citizen->maritalStatus))
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
            if (atoi(search_term) == citizen->childrenUnder18) //ascii to int
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
            matched_citizens[found] = *citizen;//record to the sstruct we open 
            found++;
        }
    }

    if (found == 0) {
        printf("[!] No matching citizens found.\nPress any key to return..."); //when no citizen found 
        getch();
        return;
    }

    ShowAndPrintCitizenTable(matched_citizens, found); // print the table 
}


void adminSummaryDashboard(){ // the admin Dshboard 
    if (registered_user == 0) {
        ClearScreen();
        printf("[!] No citizens registered yet.\nPress any key to return..."); // return the error of no register user 
        getch();
        return;
    }

    int selected_cats[17]; // the array to selcet which the thing need to print
    int cat_count = 0; //the total cat the menu have 
    double threshold = 50000.0; // max income

    if (!SelectSummaryDashboardCategories(selected_cats, &cat_count, &threshold)) {
        return; //selecte the cat wan to print 
    }

    ClearScreen();
    printf("-------------------------------------------------------------------------------------------\n"); // header 
    printf("------------------------ Summary of All Citizens Dashboard --------------------------------\n");
    printf("-------------------------------------------------------------------------------------------\n");//Ciallo～ (∠・ω< )⌒★
    for (int i = 0; i < cat_count; i++) {
        WriteSingleSummaryStat(stdout, selected_cats[i], threshold); // print the summary one by one category 
        printf("-------------------------------------------------------------------------------------------\n");
    }

    // Ask print dashboard after showing the summary of all citizens
    if (ConfirmAction("Do you want to print this Summary Dashboard to 'Citizen_Tax_report.txt'?")) { // as the user he need the simmary after wacthing the summmary 
        PrintadminSummaryDashboard(selected_cats, cat_count, threshold); // yes print it 
    }
    printf("Press any key to return..."); //wait to user back 
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
    FILE *fp = fopen("Citizen_Tax_report.txt", "a"); // "a" = append, so it will not erase the citizen table already in the file
    if (fp == NULL) {
        printf("[!] Error: Could not open 'Citizen_Tax_report.txt' for writing.\n");
        return;
    }

    fprintf(fp, "-------------------------------------------------------------------------------------------\n");
    fprintf(fp, "------------------------ Summary of All Citizens Dashboard --------------------------------\n");// header 
    fprintf(fp, "-------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < cat_count; i++) {
        WriteSingleSummaryStat(fp, selected_cats[i], threshold);
        fprintf(fp, "-------------------------------------------------------------------------------------------\n"); // loop the cat thhat nee to print
    }
    fprintf(fp, "\n"); // empty line to separate from the next report

    fclose(fp);
    printf("\n[+] Summary Dashboard successfully added to 'Citizen_Tax_report.txt'!\n"); // told user hey have sucessfully save
}





void PrintDashBoard(int user_index) { // this is foar dashboard for the user 
    CalculateTax(user_index);
    ClearScreen();
    struct Citizen *citizen = &citizen_list[user_index]; // point the pointer to the struct 
    int child_cat = citizen->deduction[0].category[1]; 
    double child_deduction_amt = lookupChildAllowance(citizen->numberOfChildren, citizen->childrenUnder18, childTable, child_table_count); 

    printf("-------------------------------------------------------------------------------------------------\n"); // print linde by line to have a buetiful interface 
    printf("========================================= Tax Dashboard =========================================\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Hi, %s\n", citizen->citizenName);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Citizen ID: %s\n", citizen->citizenID);
    printf("Age: %d\n", citizen->age);
    printf("Marital Status: %s\n", citizen->maritalStatus ? "Married" : "Single");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Number of Children: %d\n", citizen->numberOfChildren);
    printf("Number of Children Under 18: %d\n", citizen->childrenUnder18);
    printf("Special Healthcare Status: %s\n", citizen->specialHealthcare ? "Yes" : "No");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Annual Income: %82.2f\n", citizen->annualIncome);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Deduction Category Available:\n");
    printf("[1] Marital Status Deduction: %-5s %61.2f\n",citizen->maritalStatus ? "Yes" : "No",citizen->maritalStatus ? deductionTable[0].amount : 0.0);
    printf("[2] Children Deduction: %-42s %30.2f\n", (child_cat > 0 && child_cat <= child_table_count) ? childTable[child_cat - 1].category : "No Child Allowance (Not Eligible)", child_deduction_amt);
    printf("[3] Special Healthcare Deduction: %-5s %57.2f\n",citizen->specialHealthcare ? "Yes" : "No",citizen->specialHealthcare ? deductionTable[1].amount : 0.0);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Total Deduction: %80.2f\n", citizen->totalDeduction);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Taxable Income: %81.2f\n", citizen->taxableIncome);
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("Tax Rate: %86.2f%%\n", lookupTaxRate(citizen->taxableIncome, brackets, bracket_count) * 100);
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

void PrintCitizenInfo(int user_index) { //same as the dashbord just use fprintf(file_path, t), to print ot the txt fi;e 
    CalculateTax(user_index);
    char filename[100];
    snprintf(filename, sizeof(filename), "Citizen_%s_Info.txt", citizen_list[user_index].citizenID);
    FILE *file_path = fopen(filename, "w");
    if (file_path == NULL) {
        printf("[!] Error saving data!\n");
        return;
    }

    struct Citizen *Citizen = &citizen_list[user_index];
    int child_cat = Citizen->deduction[0].category[1];
    double child_deduction_amt = lookupChildAllowance(Citizen->numberOfChildren, Citizen->childrenUnder18, childTable, child_table_count);

    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "========================================= Tax Dashboard =========================================\n");
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Hi, %s\n", Citizen->citizenName);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Citizen ID: %s\n", Citizen->citizenID);
    fprintf(file_path, "Age: %d\n", Citizen->age);
    fprintf(file_path, "Marital Status: %s\n", Citizen->maritalStatus ? "Married" : "Single");
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Number of Children: %d\n", Citizen->numberOfChildren);
    fprintf(file_path, "Number of Children Under 18: %d\n", Citizen->childrenUnder18);
    fprintf(file_path, "Special Healthcare Status: %s\n", Citizen->specialHealthcare ? "Yes" : "No");
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Annual Income: %82.2f\n", Citizen->annualIncome);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Deduction Category Available:\n");
    fprintf(file_path, "[1] Marital Status Deduction: %-5s %61.2f\n",
            Citizen->maritalStatus ? "Yes" : "No",
            Citizen->maritalStatus ? deductionTable[0].amount : 0.0);
    fprintf(file_path, "[2] Children Deduction: %-42s %30.2f\n",
        (child_cat > 0 && child_cat <= child_table_count) ? childTable[child_cat - 1].category : "No Child Allowance (Not Eligible)",
        child_deduction_amt);
    fprintf(file_path, "[3] Special Healthcare Deduction: %-5s %57.2f\n",
            Citizen->specialHealthcare ? "Yes" : "No",
            Citizen->specialHealthcare ? deductionTable[1].amount : 0.0);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Total Deduction: %80.2f\n", Citizen->totalDeduction);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Taxable Income: %81.2f\n", Citizen->taxableIncome);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Tax Rate: %86.2f%%\n", lookupTaxRate(Citizen->taxableIncome, brackets, bracket_count) * 100);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Income Tax: %85.2f\n", Citizen->incomeTax);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fprintf(file_path, "Tax Per Month: %82.2f\n", Citizen->incomeTax / 12.0);
    fprintf(file_path, "-------------------------------------------------------------------------------------------------\n");
    fclose(file_path);
}

void ViewAllCitizens() { // the admin can see all the citizen 
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

    ShowAndPrintCitizenTable(all_citizens, registered_user); // category , the number of register user 
}

void AdminAddCitizen() { // this is the same logic as the register user but this got the default password it the id num 
    ClearScreen();
    if (registered_user >= MAX_CITIZENS) {
        printf("[!] Maximum user limit (%d) reached.\n", MAX_CITIZENS);
        getch();//Ciallo～ (∠・ω< )⌒★
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
        if (!IsValidID(temp_input)) {
            printf("[!] Error: Citizen ID must be exactly 12 digits (0-9)!\n");
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

        strcpy(new_citizen->citizenID, temp_input);
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
        if (strlen(temp_input) >= 50 || CheckInputBlank(temp_input) || !IsValidName(temp_input)) {
            printf("[!] Error: Name must be 1-49 letters/spaces only.\n");
            getch();
            ClearScreen();
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
    printf("\n[+] Citizen added! Default password set to Citizen ID (%s).\nPress any key to return...", new_citizen->citizenID);
    getch();
}

void AdminWatchCitizenPersonalReport() { // function for admin to view individual dashboard
    ClearScreen();
    char target_id[30];
    
    printf("--- Watch Citizen Personal Report ---\n");
    printf("Enter Citizen ID to view: ");
    fgets(target_id, sizeof(target_id), stdin);
    RemoveNewLine(target_id);
    
    int found_idx = -1;
    for (int i = 0; i < registered_user; i++) { // search through array for matching ID
        if (strcmp(citizen_list[i].citizenID, target_id) == 0) {
            found_idx = i;
            break;
        }
    }
    
    if (found_idx == -1) { // if ID not found, print error prompt
        printf("[!] Citizen ID '%s' not found.\nPress any key to return...", target_id);
        getch();
        return;
    }
    
    if (ConfirmAction("Confirm viewing this Citizen's Personal Report?")) { // confirmation 
        // 1. Show the dashboard on screen (this function already has a getch() pause at the end)
        PrintDashBoard(found_idx); 
        
        // 2. Ask if admin wants to print it to a text file
        ClearScreen();
        if (ConfirmAction("Do you want to print this dashboard to a text file?")) {
            PrintCitizenInfo(found_idx); // reuse the existing export function
            printf("\n[+] Citizen personal report saved to 'Citizen_%s_Info.txt' successfully.\n", citizen_list[found_idx].citizenID);
        }
        printf("Press any key to return...");
        getch();
    }
}

void SortTaxBrackets() {
    for (int i = 0; i < bracket_count - 1; i++) {
        for (int j = 0; j < bracket_count - 1 - i; j++) {
            if (brackets[j].minIncome > brackets[j + 1].minIncome) {
                struct TaxBracket temp = brackets[j];
                brackets[j] = brackets[j + 1];
                brackets[j + 1] = temp;
            }
        }
    }
}

void ManageTaxRules() {
    char rule_menu[4][40] = {
        "Manage Tax Brackets",
        "Manage Child Allowances",
        "Manage Fixed Deductions",
        "Back to Admin Menu"
    };
    char action_menu[4][40] = {
        "Add New Entry",
        "Edit Existing Entry",
        "Delete Entry",
        "Back"
    };

    while (1) {
        int type = SelectOption(rule_menu, 4);
        if (type == 3) return;

        int action = SelectOption(action_menu, 4);
        if (action == 3) continue;

        ClearScreen();

        // 1. TAX BRACKETS
        if (type == 0) {
            printf("--- Current Tax Brackets (%d) ---\n", bracket_count);
            for (int i = 0; i < bracket_count; i++) {
                printf("[%d] %.2f to %.2f | Rate: %.2f%%\n",
                       i + 1, brackets[i].minIncome, brackets[i].maxIncome, brackets[i].taxRate * 100);
            }
            printf("---------------------------------\n");

            if (action == 0) { // Add
                if (bracket_count >= MAX_BRACKETS) { printf("[!] Max limit reached!\n"); getch(); continue; }
                double min_inc = CheckValidDouble("Enter Min Income: RM ");
                double max_inc = CheckValidDoubleRange("Enter Max Income: RM ", min_inc + 0.01, 1.7976931348623157E+308);
                double rate = CheckValidDoubleRange("Enter Tax Rate (0.00 to 1.00, e.g. 0.05 for 5%): ", 0.0, 1.0);
                brackets[bracket_count].minIncome = min_inc;
                brackets[bracket_count].maxIncome = max_inc;
                brackets[bracket_count].taxRate = rate;
                bracket_count++;
                SortTaxBrackets();
                SaveSystemData();
                printf("[+] Tax bracket added and sorted!\n");
            } else if (action == 1 && bracket_count > 0) { // Edit
                int idx = CheckValidInt("Enter Bracket number to edit: ", 1, bracket_count) - 1;
                double min_inc = CheckValidDouble("New Min Income: RM ");
                double max_inc = CheckValidDoubleRange("New Max Income: RM ", min_inc + 0.01, 1.7976931348623157E+308);
                double rate = CheckValidDoubleRange("New Tax Rate (0.00 to 1.00): ", 0.0, 1.0);
                brackets[idx].minIncome = min_inc;
                brackets[idx].maxIncome = max_inc;
                brackets[idx].taxRate = rate;
                SortTaxBrackets();
                SaveSystemData();
                printf("[+] Tax bracket updated and sorted!\n");
            } else if (action == 2 && bracket_count > 0) { // Delete
                int idx = CheckValidInt("Enter Bracket number to delete: ", 1, bracket_count) - 1;
                if (ConfirmAction("Confirm delete?")) {
                    for (int i = idx; i < bracket_count - 1; i++) brackets[i] = brackets[i + 1];
                    bracket_count--;
                    SaveSystemData();
                    printf("[+] Tax bracket deleted!\n");
                }
            }
        }
        // 2. CHILD ALLOWANCES
        else if (type == 1) {
            printf("--- Current Child Allowances (%d) ---\n", child_table_count);
            for (int i = 0; i < child_table_count; i++) {
                printf("[%d] %s | %d-%d children | RM %.2f\n",i + 1, childTable[i].category, childTable[i].minChildren,childTable[i].maxChildren, childTable[i].amount);
            }
            printf("-------------------------------------\n");

            if (action == 0) { // Add
                if (child_table_count >= MAX_CHILD_ALLOWANCE) { printf("[!] Max limit reached!\n"); getch(); continue; }
                GetValidCategoryName(childTable[child_table_count].category, 25, "Enter Category Name: ");
                int min_c = CheckValidInt("Min Children (1-99): ", 1, 99);
                int max_c = CheckValidInt("Max Children: ", min_c, 99);
                double amt = CheckValidDouble("Allowance Amount: RM ");
                childTable[child_table_count].minChildren = min_c;
                childTable[child_table_count].maxChildren = max_c;
                childTable[child_table_count].amount = amt;
                child_table_count++;
                SaveSystemData();
                printf("[+] Child allowance added!\n");
            } else if (action == 1 && child_table_count > 0) { // Edit
                int idx = CheckValidInt("Enter Child Allowance number to edit: ", 1, child_table_count) - 1;
                GetValidCategoryName(childTable[idx].category, 25, "Enter New Category Name: ");
                int min_c = CheckValidInt("New Min Children (1-99): ", 1, 99);
                int max_c = CheckValidInt("New Max Children: ", min_c, 99);
                double amt = CheckValidDouble("New Allowance Amount: RM ");
                childTable[idx].minChildren = min_c;
                childTable[idx].maxChildren = max_c;
                childTable[idx].amount = amt;
                SaveSystemData();
                printf("[+] Child allowance updated!\n");
            } else if (action == 2 && child_table_count > 0) { // Delete
                int idx = CheckValidInt("Enter Child Allowance number to delete: ", 1, child_table_count) - 1;
                if (ConfirmAction("Confirm delete?")) {
                    for (int i = idx; i < child_table_count - 1; i++) childTable[i] = childTable[i + 1];
                    child_table_count--;
                    SaveSystemData();
                    printf("[+] Child allowance deleted!\n");
                }
            }
        }
        // 3. FIXED DEDUCTIONS
        else if (type == 2) {
            printf("--- Current Fixed Deductions (%d) ---\n", deduction_count);
            for (int i = 0; i < deduction_count; i++) {
                printf("[%d] %s | RM %.2f\n", i + 1, deductionTable[i].category, deductionTable[i].amount);
            }
            printf("-------------------------------------\n");

            if (action == 0) { // Add
                if (deduction_count >= MAX_DEDUCTIONS) { printf("[!] Max limit reached!\n"); getch(); continue; }
                GetValidCategoryName(deductionTable[deduction_count].category, 25, "Enter Deduction Name: ");
                deductionTable[deduction_count].amount = CheckValidDouble("Deduction Amount: RM ");
                deduction_count++;
                SaveSystemData();
                printf("[+] Deduction added!\n");
            } else if (action == 1 && deduction_count > 0) { // Edit
                int idx = CheckValidInt("Enter Deduction number to edit: ", 1, deduction_count) - 1;
                GetValidCategoryName(deductionTable[idx].category, 25, "Enter New Deduction Name: ");
                deductionTable[idx].amount = CheckValidDouble("New Deduction Amount: RM ");
                SaveSystemData();
                printf("[+] Deduction updated!\n");
            } else if (action == 2 && deduction_count > 0) { // Delete
                int idx = CheckValidInt("Enter Deduction number to delete: ", 1, deduction_count) - 1;
                if (idx == 0 || idx == 1) {
                    printf("[!] Error: Core deductions (Marital & Special Healthcare) cannot be deleted, only edited.\n");
                } else if (ConfirmAction("Confirm delete?")) {
                    for (int i = idx; i < deduction_count - 1; i++) deductionTable[i] = deductionTable[i + 1];
                    deduction_count--;
                    SaveSystemData();
                    printf("[+] Deduction deleted!\n");
                }
            }
        }
        printf("Press any key to continue...");
        getch();
    }
}

void AdminDeleteCitizen() {
    ClearScreen();
    if (registered_user == 0) {
        printf("[!] No citizens registered yet.\nPress any key to return...");
        getch();
        return;
    }

    char target_id[30];
    printf("--- Admin: Delete Citizen --- (press 0 to cancel)\n");
    printf("Enter Citizen ID to delete: ");
    fgets(target_id, sizeof(target_id), stdin);
    if (strcmp(target_id, "0\n") == 0) return;
    RemoveNewLine(target_id);

    int found_idx = -1;
    for (int i = 0; i < registered_user; i++) {
        if (strcmp(citizen_list[i].citizenID, target_id) == 0) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("[!] Citizen ID '%s' not found.\nPress any key to return...", target_id);
        getch();
        return;
    }

    printf("\nFound Citizen: %s (ID: %s)\n", citizen_list[found_idx].citizenName, citizen_list[found_idx].citizenID);
    if (ConfirmAction("Are you sure you want to permanently delete this citizen?")) {
        for (int i = found_idx; i < registered_user - 1; i++) {
            citizen_list[i] = citizen_list[i + 1];
        }
        memset(&citizen_list[registered_user - 1], 0, sizeof(struct Citizen));
        registered_user--;
        SaveUserData();
        printf("\n[+] Citizen deleted successfully!\n");
    }
    printf("Press any key to return...");
    getch();
}

void AdminMenu(int *running) {
    int admin_logged_in = 1;
    while (admin_logged_in) {
        int choice = SelectOption(admin_menu, 11);
        switch (choice) {
        case 0: // View All Citizens (Choose Categories + Print Option)
            ViewAllCitizens();
            break;
        case 1: { // Search Citizen Info (Choose Search Field -> Choose Display Categories -> Print Option)
            int cat = SelectOption(admin_category_Selection, 12);
            if (cat >= 1 && cat <= 11) {
                ClearScreen();
                char search_term[100];
                printf("Searching by %s\n", admin_category_Selection[cat]); // ask what to search 
                printf("Enter search value: ");
                fgets(search_term, sizeof(search_term), stdin); //the search term (ID : iwitt)
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
            fgets(target_id, sizeof(target_id), stdin); // get the id 
            RemoveNewLine(target_id);
            int found_idx = -1;
            for (int i = 0; i < registered_user; i++) { // loop in the struct to find the id 
                if (strcmp(citizen_list[i].citizenID, target_id) == 0) {
                    found_idx = i;
                    break;
                }
            }
            if (found_idx == -1) {
                printf("[!] Citizen ID '%s' not found.\nPress any key to return...", target_id); // error code of not finding 
                getch();
            } else {
                EditCitizenInfo(found_idx, SelectOption(edit_menu, 7)); // if find call the EditCitizenInfo
            }
            break;
        }
        case 4: AdminDeleteCitizen(); 
            break;
        case 5:
            AdminWatchCitizenPersonalReport();
            break;
        case 6: // Overall Statistics (Choose Summary Categories -> Show -> Ask to Print)
            adminSummaryDashboard();
            break;
        case 7: { // Print Overall Statistics (Choose Summary Categories or Table Categories to Print)
            int selected_cats[17];
            int cat_count = 0;
            double threshold = 50000.0;
            if (SelectSummaryDashboardCategories(selected_cats, &cat_count, &threshold)) {  //selectSummaryDashCat 
                ClearScreen();
                PrintadminSummaryDashboard(selected_cats, cat_count, threshold); // the call the print function //Ciallo～ (∠・ω< )⌒★
                printf("Press any key to return...");
                getch();
            }
            break;
        }
        case 8: // Manage Tax & Allowance Rules
            ManageTaxRules();
            break;
        case 9: // Logout
            ClearScreen();
            if (ConfirmAction("Logout ?")){ // important desision dulbel confirm 
                admin_logged_in = 0;
            }

            break;
        case 10: // Exit
            ClearScreen();
            if (ConfirmAction("Exit ?")) // important desision dulbel confirm 
            {
                admin_logged_in = 0;
                *running = 0; // change the running to exit 
            }
            break;
        }
    }
}



int main() { // the main 
    LoadUserData();
    LoadSystemData();
    int running = 1;

    while (running) {
        int choice = SelectOption(login_menu, 3); // select the option in the login menu 
        if (choice == 0) { // login 
            current_active_user = UserLogin(); // do the login 
            if (current_active_user == -2) {
                AdminMenu(&running); // the aadmin log in (go to the admin menu)
            } else if (current_active_user != -1) { // logi in sucessfull (check is the active user == -1 ot not)
                int logged_in = 1; // to let it stay log in 
                while (logged_in) {
                    if (citizen_list[current_active_user].LoginCount == 1) { // check first time log in 
                        GetCitizenInfo(current_active_user); // yes then get the info 
                        citizen_list[current_active_user].LoginCount++; //the add the log in cout to make it not first time m=any more 
                    }
                    int dash_choice = SelectOption(dash_menu, 7); // the dash menu 
                    switch (dash_choice) {
                    case 0:
                        EditCitizenInfo(current_active_user, SelectOption(edit_menu, 7)); // edit Citzen info 
                        ClearScreen();
                        break;
                    case 1: {
                        ClearScreen();
                        temp_value = CheckValidDouble("Please enter the taxable income to lookup the tax bracket: RM ");
                        double tax_rate = lookupTaxRate(temp_value, brackets, bracket_count);
                        for (int i = 0; i < bracket_count - 1; i++) {
                            printf("Bracket %d: %.2f to %.2f, Tax Rate: %.2f%%\n",
                                 i + 1, brackets[i].minIncome, brackets[i].maxIncome, brackets[i].taxRate * 100);
                        }
                        if (bracket_count > 0) {
                            printf("Bracket %d: %.2f and above, Tax Rate: %.2f%%\n",
                                 bracket_count, brackets[bracket_count - 1].minIncome, brackets[bracket_count - 1].taxRate * 100);
                        }
                        printf("\n--------------------------------------------------------------------------\n");
                        printf("Your taxable income of %.2f falls into a tax rate of %.2f%%\n", temp_value, tax_rate * 100);
                        printf("Press any key to return...");
                        getch();
                        break;
                    }
                    case 2: {
                        ClearScreen();
                        int child_count = CheckValidInt("Please enter the children below 18 count to lookup the child allowance: ", 0, 99);
                        double child_allowance = lookupChildAllowance(child_count, child_count, childTable, child_table_count);
                        for (int i = 0; i < child_table_count - 1; i++) {
                            printf("%s: %d to %d children, Amount: %.2f\n",childTable[i].category, childTable[i].minChildren, childTable[i].maxChildren, childTable[i].amount);
                        }
                        if (child_table_count > 0) {
                            printf("%s: %d or more children, Amount: %.2f\n",childTable[child_table_count - 1].category,childTable[child_table_count - 1].minChildren,childTable[child_table_count - 1].amount);
                        }
                        printf("\n--------------------------------------------------------------------------\n");
                        printf("Your children count of %d falls into a child allowance of %.2f\n", child_count, child_allowance);
                        printf("Press any key to return...");
                        getch();
                        break;
                    }
                    case 3: // print dashboard 
                        PrintDashBoard(current_active_user);
                        break;
                    case 4:
                        ClearScreen(); // save a file of the dashboard 
                        PrintCitizenInfo(current_active_user);
                        printf("Citizen information saved to file successfully.\nPress any key to return..."); // prompt 
                        getch();
                        break;
                    case 5:
                        ClearScreen(); // log out 
                        if (ConfirmAction("Logout?")) /// important desision dulbel confirm 
                        {
                            logged_in = 0;
                        }
                        break;
                    case 6: //end program 
                        ClearScreen();
                        if(ConfirmAction("Exit?")){ // important desision dulbel confirm 
                            running = 0;
                            logged_in = 0;
                        }
                        break;
                    }
                }
            }
        } else if (choice == 1) {
            RegisterUser(); // register user 
        } else if (choice == 2) {
            ClearScreen();
            if (ConfirmAction("Exit ?")){ // important desision dulbel confirm 
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