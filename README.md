# COM102_Clang_Assignment

This repo is to record the process of me doing the assignment

## Year-End Personal Income Tax Management System — User Manual

A console-based C application designed to help citizens calculate and track their year-end personal income tax, and to allow administrators to manage citizen records, customize tax/allowance rules, and generate statistical reports.

## 1. Features Overview

* **Citizen Self-Service:** Account registration, profile management, interactive tax bracket and child allowance lookup, personal tax dashboard, and individual `.txt` report export.
* **Admin Management (Full CRUD):** View, search, sort, add, edit, and delete citizen profiles (up to 30 citizens).
* **Dynamic Tax & Allowance Rules:** All tax brackets, child allowance tiers, and fixed deductions are loaded from `data.txt` and can be added, edited, or deleted by the admin without recompiling the code.
* **Statistical Reporting:** Generates customizable citizen tables (with grand totals) and overall summary statistics (mean, median, standard deviation, and custom income thresholds), appended to `Citizen_Tax_report.txt`.
* **Strict Input Validation:** Rejects blank inputs, invalid ranges, non-digit Citizen IDs, non-alphabetic names, trailing letters on numbers (e.g., `25abc`), and invalid characters in rule names.

## 2. System Requirements & Setup

### Requirements

* **Operating System:** Windows (uses `<conio.h>`, `_stricmp()`, and `system("cls")`).
* **Compiler:** GCC (MinGW) or any standard Windows C compiler.
* **Python 3** *(optional)*: Only needed if you want to generate sample data using `create_userdata.py`.

### Compilation & Running

1. Clone or download this repository.
2. *(Optional)* Generate the initial `data.txt` and `userdata.bin` with 20 sample citizen records:
   ```bash
   python create_userdata.py
   ```
   > **Note:** Make sure `data.txt` is in the same folder as the compiled executable before running the program.

3. Compile the C source file:
   ```bash
   gcc Assignment_1.c -o tax_system.exe -lm
   ```
4. Run the application:
   ```bash
   tax_system.exe
   ```

## 3. Project Files

| File | Description |
| ----- | ----- |
| `Assignment_1.c` | Main C source code for the application. |
| `data.txt` | Plain-text configuration storing tax brackets, child allowance tiers, and fixed deductions. |
| `userdata.bin` | Local binary database storing registered citizen profiles (max 30 citizens). |
| `create_userdata.py` | Helper script to generate a fresh `data.txt` and a 20-citizen sample `userdata.bin`. |
| `Citizen_<ID>_Info.txt` | Exported personal tax dashboard for an individual citizen. |
| `Citizen_Tax_report.txt` | Exported admin reports (custom citizen tables and overall summary dashboards; appended so previous reports are kept). |

## 4. Controls & Navigation

* **Up / Down Arrow Keys (`↑` / `↓`):** Move the `->` cursor through menu options.
* **Enter:** Select the highlighted option.
* **Confirmation Prompts:** Press `Y` or `Enter` to confirm (`Yes`), or `N` to cancel/redo (`No`).
* **Cancel Entry:** Type `0` and press `Enter` on ID/Registration prompts to return to the previous menu.

## 5. Citizen User Guide

### Registering a New Account

1. From the main menu, select **Register New User**.
2. Enter the following details (each requires confirmation):
   * **Citizen ID:** Exactly **12 numeric digits** (`0–9`), must be unique.
   * **Username (Full Name):** **1–49 characters**, letters and spaces only.
   * **Password:** **1–19 characters**, cannot be blank.

### First-Time Login & Profile Setup

1. Select **Login** and enter your 12-digit Citizen ID and Password.
2. On your first login, the system will prompt you to enter your tax profile:
   * **Age:** `18` to `149`
   * **Marital Status:** `0` (Single) or `1` (Married)
   * **Annual Income:** Non-negative amount in RM
   * **Number of Children:** `0` to `19`
   * **Number of Children Under 18:** `0` up to your total number of children
   * **Special Healthcare Status:** `0` (No) or `1` (Yes)
     > **Rule:** Special healthcare deduction applies only to a spouse or child. Single citizens with 0 children cannot claim it.

### Citizen Dashboard Menu

| Option | Action |
| ----- | ----- |
| **Edit Citizen Info** | Update Age, Marital Status, Annual Income, Children counts, or Special Healthcare Status. Tax is automatically recalculated and saved. |
| **Lookup Tax Bracket** | Enter any taxable income to view all active tax brackets and see which rate applies. |
| **Lookup Child Allowance** | Enter a count of children under 18 (`0–99`) to view all child allowance tiers and the eligible deduction. |
| **Tax Dashboard** | View your complete personal tax breakdown (deductions, taxable income, tax rate, annual tax, and monthly tax). |
| **Print Citizen Info** | Export your personal tax dashboard to `Citizen_<YourID>_Info.txt`. |
| **Logout / Exit** | Return to the login menu or close the application (requires confirmation). |

## 6. Administrator Guide

### Admin Login

Select **Login** from the main menu and enter the default administrator credentials:

* **ID:** `admin`
* **Password:** `admin123`

### Admin Menu Options

| Option | Action |
| ----- | ----- |
| **View All Citizens** | Select columns to display, optionally sort (Ascending/Descending), view grand totals, and optionally append the table to `Citizen_Tax_report.txt`. |
| **Search Citizen Info** | Filter citizens by any of the 11 categories (ID, Name, Age, Marital Status, Income, Children, Healthcare, Deduction, Taxable Income, Income Tax). |
| **Add Citizen** | Register a new citizen directly. Their **default password is automatically set to their 12-digit Citizen ID**. |
| **Edit Citizen Info** | Enter a Citizen ID to modify any of their profile fields and recalculate their tax. |
| **Delete Citizen** | Enter a Citizen ID to permanently remove a record. Remaining records shift left automatically so no empty gaps are left in the database. |
| **Watch Citizen Personal Report** | Enter a Citizen ID to view their personal Tax Dashboard and optionally export it to `Citizen_<ID>_Info.txt`. |
| **Overall Statistics** | Select from 16 aggregate metrics (counts, averages, totals, mean, median, standard deviation, and custom income threshold), view on screen, and optionally export. |
| **Print Overall Statistics** | Select summary metrics and append them directly to `Citizen_Tax_report.txt`. |
| **Manage Tax & Allowance Rules** | Add, Edit, or Delete **Tax Brackets**, **Child Allowances**, and **Fixed Deductions**. All changes are immediately saved to `data.txt`. |
| **Logout / Exit** | Return to the login menu or close the program. |

### Rules for Managing `data.txt` via Admin Menu

* **Tax Brackets:** `Max Income` must be greater than `Min Income`, and `Tax Rate` must be between `0.00` and `1.00` (e.g., `0.05` = `5%`). Brackets are automatically sorted by `Min Income` after adding or editing.
* **Child Allowances:** Category names cannot contain double quotes (`"`). `Max Children` must be greater than or equal to `Min Children`.
* **Fixed Deductions:** Core deductions (`[1] Marital allowance` and `[2] Special healthcare`) can be **edited**, but are protected from **deletion** so tax calculations remain valid.

## 7. Default Tax & Allowance Rules (`data.txt`)

### Tax Brackets

| Bracket | Taxable Income Range (RM) | Tax Rate |
| ----- | ----- | ----- |
| 1 | `0.00` – `48,000.00` | `0%` |
| 2 | `48,000.01` – `60,000.00` | `5%` |
| 3 | `60,000.01` – `100,000.00` | `7%` |
| 4 | `100,000.01` – `200,000.00` | `9%` |
| 5 | `200,000.01` – `500,000.00` | `14%` |
| 6 | `500,000.01` – `1,000,000.00` | `19%` |
| 7 | Above `1,000,000.00` | `25%` |

### Deductions & Child Allowances

* **Marital Allowance:** `RM 5,000.00` (for married citizens)
* **Special Healthcare Allowance:** `RM 20,000.00` (for spouse or child)
* **Child Allowance** *(all children must be under 18)*:
  * **Category A (`1–2` children):** `RM 8,000.00`
  * **Category B (`3–4` children):** `RM 10,000.00`
  * **Category C (`5–6` children):** `RM 12,000.00`
  * **Category D (`7+` children):** `RM 14,000.00`

## 8. `data.txt` File Format

The system loads and saves tax brackets, child allowances, and fixed deductions from `data.txt` so rules can be updated without recompiling the program.

### Structure

1. **Header Line (Counts):**
   ```text
   <bracket_count> <child_table_count> <deduction_count>
   ```
   * Each count must be between `0` and `20`.

2. **Tax Brackets (`bracket_count` lines):**
   ```text
   <minIncome>, <maxIncome>, <taxRate>
   ```
   * `taxRate` is stored as a decimal (e.g., `0.05` for 5%).
   * The highest bracket uses `1.7976931348623157E+308` (`DBL_MAX`) as its upper bound.

3. **Child Allowances (`child_table_count` lines):**
   ```text
   "<category>", <minChildren>, <maxChildren>, <amount>
   ```
   * Category names must be enclosed in double quotes (`"`), max 24 characters, and cannot contain `"` inside the name.

4. **Fixed Deductions (`deduction_count` lines):**
   ```text
   "<category>", <amount>
   ```
   * Entry 1 is **Marital allowance** and Entry 2 is **Special healthcare** (core deductions can be edited, not deleted).

### Example `data.txt`

```text
7 4 2
0.0, 48000.0, 0.00
48000.0, 60000.0, 0.05
60000.0, 100000.0, 0.07
100000.0, 200000.0, 0.09
200000.0, 500000.0, 0.14
500000.0, 1000000.0, 0.19
1000000.0, 1.7976931348623157E+308, 0.25
"Child allowance A", 1, 2, 8000.0
"Child allowance B", 3, 4, 10000.0
"Child allowance C", 5, 6, 12000.0
"Child allowance D", 7, 99, 14000.0
"Marital allowance", 5000.0
"Special healthcare", 20000.0
```

## 9. Troubleshooting & Limitations

* **`[!] Error: data.txt not found!`**: Ensure `data.txt` is placed in the exact same directory as `tax_system.exe`, or run `python generate_userdata_bin_and_data_txt.py` to regenerate it.
* **Corrupted/Weird Citizen Data on Startup:** If you previously ran an older build of the program, delete the old `userdata.bin` and regenerate it using `create_userdata.py`.
* **Known Limitations:**
  * Supports a maximum of **30 registered citizens**.
  * Local binary storage only (`userdata.bin`); passwords are stored in plain text without encryption.
  * Console window should be maximized when viewing all 11 columns in the admin table to prevent line wrapping.
