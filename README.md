# 🚗 Smart Parking Lot Management System
### C-Based Command-Line Application | Team 4Loops (FPT University HCMC)

A completely self-contained command-line utility for automated parking space tracking, multi-role authentication (Admin/Staff), real-time ticket billing, database persistence, and automated invoicing.

---

## 📝 Project Description

The **Smart Parking Lot Management System** is a standard C (ANSI C) application designed to automate day-to-day operations in a commercial parking facility. It operates in a secure sandbox, storing active records and accounts locally. 

### Key Features
*   **Dual-Role Access Control**: Secure logins for **Admin** (unrestricted configuration access) and **Staff** (check-in/check-out operators).
*   **Entry/Exit Automation**: Standardized check-ins with automatic format validation, duplicate prevention, and check-out logs.
*   **Intelligent Fare Engine**: Automated billing based on elapsed hours, including night-time surcharges (23:00 to 03:00) and overtime penalties (stays exceeding 72 hours).
*   **Data Persistence**: Flat-file database storage under the `data/` directory (`vehicles.dat` and `accounts.dat`), autosaving after every transaction.
*   **Comprehensive Reporting**: Live terminal dashboards and `.txt` file reports generated automatically with real-time timestamps.
*   **Terminal Styling**: ANSI escape-code coloring for a premium, highly readable terminal interface.

---

## 📂 Project Directory Structure

```text
📦 FCode-Parking_Lot_Management_System
 ┣ 📂 data        # Flat-file database backups and exported daily revenue logs (.txt)
 ┣ 📂 include     # C headers declaring common configurations and function signatures
 ┃ ┣ 📜 types.h   # Core data structures (Vehicle, PriceConfig, Accounts...)
 ┃ ┗ 📜 ...       # Specific service header interfaces
 ┣ 📂 src         # Implementations of core service files
 ┃ ┣ 📜 parking.c # Check-in, check-out, vehicle search, and admin deletion logic
 ┃ ┣ 📜 billing.c # Surcharge loops, rate configuration, and invoicing engine
 ┃ ┣ 📜 report.c  # Daily revenue calculation and file exporter
 ┃ ┗ 📜 ...       # Utilities, standard file IO handlers, and auth checks
 ┣ 📜 main.c      # Standard entry point and application main menu loop
 ┗ 📜 Parking_Lot_Management_System.dev  # Dev-C++ project metadata
```

---

## 🛠️ How to Compile

The codebase is compiled using the standard **GCC Compiler**. Make sure your MinGW/GCC environment path is configured.

**Compilation Command:**
Run this command from your terminal inside the root directory of the project to compile all modules and links:

```bash
gcc main.c src/billing.c src/parking.c src/utils.c src/auth.c src/fileio.c src/report.c -o Parking_Lot_Management_System.exe -I"include" -static-libgcc
```

*   `-o Parking_Lot_Management_System.exe`: Output executable filename.
*   `-I"include"`: Includes header lookup directories.
*   `-static-libgcc`: Ensures the runtime includes static links to avoid missing MinGW DLL errors.

---

## 🚀 How to Run

After compiling, you can execute the compiled binary directly:

```bash
.\Parking_Lot_Management_System.exe
```

---

## 📋 Default Credentials

Upon fresh execution, the database automatically initializes a superuser admin account:
*   **Username**: `admin`
*   **Password**: `Admin1234`

---

## 💻 Sample Commands & Workflows

Here are standard operational commands and sample input workflows for the CLI:

### 1. User Authentication (Login)
```text
================================== LOGIN ==================================

Enter username: admin
Enter password: *********
Login successfully!
```

### 2. Vehicle Entry (Check-In)
*   **Action**: Select Option `1` in the main menu.
*   **Input Sequence**:
```text
===================== ADD VEHICLE =====================
Enter license plate: 51A-12345
Vehicle type (0: motorbike, 1: car, 2: bus): 1

Vehicle added successfully!
Entry time: Sun May 17 23:45:00 2026
```

### 3. Vehicle Exit & Billing (Check-Out)
*   **Action**: Select Option `2` in the main menu.
*   **Input Sequence**:
```text
--- CHECK OUT VEHICLE ---
Current vehicles in yard:
No    | LICENSE PLATE   | VEHICLE TYPE
--------------------------------------------------
1     | 51A-12345       | Car
--------------------------------------------------
Enter license plate: 51A-12345

=================================================
                    PARKING BILL                
=================================================
Plate                 : 51A-12345
Vehicle Type          : Car
Entry Time            : Sun May 17 10:00:00 2026
Exit Time             : Sun May 17 23:50:00 2026
-------------------------------------------------
Parking Hours         : 14 hours 
Base Fee              : 70.000
-------------------------------------------------
Night Surcharge Hours : 1 hours (23h - 3h)
Night Surcharge Fee   : 5.000
-------------------------------------------------
Overtime Surcharge    : 0
=================================================
TOTAL BILL            : 75.000
=================================================
```

### 4. Vehicle Search Query
*   **Action**: Select Option `4` in the main menu.
*   **Input Sequence**:
```text
Enter license plate to search: 51A

================================ VEHICLE LIST ================================

STT   | LICENSE PLATE   | VEHICLE TYPE   | STATUS     | ENTRY TIME               
------------------------------------------------------------------------------
1     | 51A-12345       | Car            | In Yard    | Sun May 17 10:00:00 2026
```

### 5. Export Daily Revenue Report
*   **Action**: Select Option `9` in the main menu (Admin Role only).
*   **Expected Console Output**:
```text
============================================================================
  Report exported successfully!
  File: data/report_17052026_235012.txt
  Vehicles: 1 | Revenue: 75000 VND
============================================================================
```
