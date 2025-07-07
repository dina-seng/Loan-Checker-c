#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_CUSTOMERS 100
#define MAX_NAME 50
#define MAX_ID 20
#define FILE_NAME "customers.csv"
#define APPROVED_FILE "approved_customers.csv"
#define REJECTED_FILE "rejected_customers.csv"


struct Customer {
    char name[MAX_NAME];
    char id[MAX_ID];
    char jobType[20]; // Permanent or Temporary
    float incomePerMonth;
    int yearsOfEmployment;
    int creditScore;
    float loanAmount;
    int loanStatus; // 0: Not evaluated, 1: Approved, 2: Rejected
};

struct Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

// Function declarations
void displayMenu();
void loadCustomers();
void saveCustomers();
void addCustomer();
void removeCustomer();
void viewCustomer();
void evaluateLoan();
void viewLoanSummary();
void exportApproved();
void exportRejected();
void removeNewline(char *str);
void normalizeJobType(char *jobType);
void login();

int main() {
    login();
    loadCustomers();

    int choice;
    while (1) {
        displayMenu();
        printf("Enter your choice (1-8): ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: removeCustomer(); break;
            case 3: evaluateLoan(); break;
            case 4: viewCustomer(); break;
            case 5: viewLoanSummary(); break;
            case 6: exportApproved(); break;
            case 7: exportRejected(); break;
            case 8: saveCustomers(); 
            printf("Goodbye...!\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

// Simple admin login
void login() {
    char username[20], password[20];
    printf("=== Loan System Login ===\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    while (getchar() != '\n');

    if (strcmp(username, "admin") != 0 || strcmp(password, "1234") != 0) {
        printf("Access denied. Exiting...\n");
        exit(1);
    }
    printf("Login successful.\n");
}

void removeNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

void normalizeJobType(char *jobType) {
    for (int i = 0; jobType[i]; i++) {
        jobType[i] = tolower(jobType[i]);
    }
    if (strcmp(jobType, "permanent") == 0) strcpy(jobType, "Permanent");
    else strcpy(jobType, "Temporary");
}

// Menu
void displayMenu() {
    printf("\n\033[1;34m========== Loan Checker Menu ==========\033[0m\n");
    printf("1. Add New Customer\n");
    printf("2. Remove Customer\n");
    printf("3. Evaluate Loan\n");
    printf("4. View Customer Info\n");
    printf("5. View Loan Summary\n");
    printf("6. Export Approved Customers\n");
    printf("7. Export Rejected Customers\n");
    printf("8. Save & Exit\n");
    printf("======================================\n");
}

// Load customers from CSV
void loadCustomers() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    char line[256];
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%49[^,],%19[^,],%19[^,],%f,%d,%d,%f,%d",
            customers[customerCount].name,
            customers[customerCount].id,
            customers[customerCount].jobType,
            &customers[customerCount].incomePerMonth,
            &customers[customerCount].yearsOfEmployment,
            &customers[customerCount].creditScore,
            &customers[customerCount].loanAmount,
            &customers[customerCount].loanStatus) == 8) {
            customerCount++;
            if (customerCount >= MAX_CUSTOMERS) break;
        }
    }
    fclose(file);
}

// Save customers to CSV
void saveCustomers() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error saving data!\n");
        return;
    }

    fprintf(file, "Name | ID | JobType | IncomePerMonth | YearsOfEmployment | CreditScore | LoanAmount | LoanStatus |\n");
    for (int i = 0; i < customerCount; i++) {
        fprintf(file, "%s | %s | %s | %.2f | %d | %d |  %.2f | %d | \n",
            customers[i].name,
            customers[i].id,
            customers[i].jobType,
            customers[i].incomePerMonth,
            customers[i].yearsOfEmployment,
            customers[i].creditScore,
            customers[i].loanAmount,
            customers[i].loanStatus);
    }

    fclose(file);
}

// Add new customer
void addCustomer() {
    if (customerCount >= MAX_CUSTOMERS) {
        printf("Max customers reached!\n");
        return;
    }

    struct Customer *c = &customers[customerCount];

    printf("Enter Customer's Name: ");
    fgets(c->name, sizeof(c->name), stdin); 
    removeNewline(c->name);

    printf("Enter National-ID : ");
    fgets(c->id, sizeof(c->id), stdin); 
    removeNewline(c->id);

    printf("Enter job type (Permanent/Temporary): ");
    fgets(c->jobType, sizeof(c->jobType), stdin);
    removeNewline(c->jobType); 
    normalizeJobType(c->jobType);

    printf("Enter income per month: ");
    scanf("%f", &c->incomePerMonth); 
    while (getchar() != '\n');

    if (strcmp(c->jobType, "Permanent") == 0) {
        c->yearsOfEmployment = -1;
    } else {
        printf("Enter years of employment: ");
        scanf("%d", &c->yearsOfEmployment);
        while (getchar() != '\n');
    }

    printf("Enter credit score (-1 if none): ");
    scanf("%d", &c->creditScore); 
    while (getchar() != '\n');

    printf("Enter loan amount: ");
    scanf("%f", &c->loanAmount); 
    while (getchar() != '\n');

    c->loanStatus = 0;
    printf("Customer added!\n");
    customerCount++;
}
//Remove Customer
void removeCustomer() {
    if (customerCount == 0) {
        printf("No customers to remove.\n");
        return;
    }

    char searchId[MAX_ID];
    printf("Enter the Customer ID to remove: ");
    fgets(searchId, sizeof(searchId), stdin);
    removeNewline(searchId);

    int found = -1;
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].id, searchId) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf(" Customer with ID '%s' not found.\n", searchId);
        return;
    }

    printf(" Are you sure you want to remove customer '%s'? (y/n): ", customers[found].name);
    char confirm;
    scanf(" %c", &confirm);
    while (getchar() != '\n');

    if (confirm == 'y' || confirm == 'Y') {
        for (int i = found; i < customerCount - 1; i++) {
            customers[i] = customers[i + 1]; 
        }
        customerCount--;
        printf(" Customer removed successfully!\n");
    } else {
        printf(" Cancelled. Customer not removed.\n");
    }
}


// Evaluate loan
void evaluateLoan() {
    char id[MAX_ID];
    printf("Enter customer ID to evaluate: ");
    fgets(id, sizeof(id), stdin); removeNewline(id);

    int found = -1;
    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].id, id) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Customer not found!\n");
        return;
    }

    struct Customer *c = &customers[found];
    float rate; int term;

    printf("Enter monthly interest rate (e.g. 0.05 for 5%%): ");
    scanf("%f", &rate);
    printf("Enter loan term (in months): ");
    scanf("%d", &term);
    while (getchar() != '\n');

    float p = c->loanAmount;
    float mp = p * (rate / (1 - pow(1 + rate, -term)));

    if (isnan(mp) || isinf(mp)) {
        printf("Invalid calculation!\n");
        return;
    }

    int eligible = 0;
    if ((strcmp(c->jobType, "Permanent") == 0 || strcmp(c->jobType, "Temporary") == 0) &&
        (c->creditScore == -1 || c->creditScore >= 500) &&
        (c->incomePerMonth * 12 >= mp * term * 1.2)) {
        eligible = 1;
    }

    if (eligible) {
        printf("\033[0;32mLoan Approved!\033[0m Monthly Payment: %.2f\n", mp);
        c->loanStatus = 1;
    } else {
        printf("\033[0;31mLoan Rejected.\033[0m Monthly Payment: %.2f\n", mp);
        c->loanStatus = 2;
    }
}

// View customers
void viewCustomer() {
    char id[MAX_ID];
    printf("Enter ID to search (or press Enter to view all): ");
    fgets(id, sizeof(id), stdin); removeNewline(id);

    int found = 0;
    for (int i = 0; i < customerCount; i++) {
        if (strlen(id) == 0 || strcmp(customers[i].id, id) == 0) {
            found = 1;
            struct Customer *c = &customers[i];

            printf("\n\033[1;33mCustomer #%d:\033[0m\n", i + 1);
            printf("Name: %s ", c->name);
            switch (c->loanStatus) {
                case 1: printf("[\033[0;32mApproved\033[0m]"); break;
                case 2: printf("[\033[0;31mRejected\033[0m]"); break;
                default: printf("[Not Evaluated]"); break;
            }
            printf("\nID: %s\n", c->id);
            printf("Job Type: %s\n", c->jobType);
            printf("Income: %.2f\n", c->incomePerMonth);
            printf("Employment: %s\n", (c->yearsOfEmployment == -1) ? "N/A" : "Available");
            printf("Credit Score: %d\n", c->creditScore);
            printf("Loan Request: %.2f\n", c->loanAmount);
        }
    }
    if (!found && strlen(id) > 0) {
        printf("No customer found with ID: %s\n", id);
    }
}

// Loan summary
void viewLoanSummary() {
    int approved = 0, rejected = 0, pending = 0;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].loanStatus == 1) approved++;
        else if (customers[i].loanStatus == 2) rejected++;
        else pending++;
    }

    printf("\n\033[1;36m======= Loan Summary =======\033[0m\n");
    printf("Total Customers: %d\n", customerCount);
    printf("Approved: %d\n", approved);
    printf("Rejected: %d\n", rejected);
    printf("Pending: %d\n", pending);
}

// Export Approved Customers
void exportApproved() {
    FILE *file = fopen(APPROVED_FILE, "w");
    if (!file) {
        printf("Error exporting approved customers.\n");
        return;
    }

    fprintf(file, "Name | ID | JobType | Income | LoanAmount | \n");
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].loanStatus == 1) {
            fprintf(file, "%s | %s | %s | %.2f | %.2f | \n",
                customers[i].name, customers[i].id,
                customers[i].jobType, customers[i].incomePerMonth,
                customers[i].loanAmount);
        }
    }
    fclose(file);
    printf("Approved customers exported to '%s'\n", APPROVED_FILE);
}

//Export Rejected Customers
void exportRejected() {
    FILE *file = fopen(REJECTED_FILE, "w");
    if (!file) {
        printf("Error exporting rejected customers.\n");
        return;
    }

    fprintf(file, "Name | ID | JobType | Income | LoanAmount | \n");
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].loanStatus == 2) {
            fprintf(file, "%s | %s | %s | %.2f | %.2f | \n",
                customers[i].name,
                customers[i].id,
                customers[i].jobType,
                customers[i].incomePerMonth,
                customers[i].loanAmount);
        }
    }

    fclose(file);
    printf("Rejected customers exported to '%s'\n", REJECTED_FILE);
}
