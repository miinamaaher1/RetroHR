#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define ENTER 13
#define ESC 27
#define BS 8
#define TAB 9

#define SPACE 32
#define DOT 46
#define DASH 45
#define LOWERM 109
#define LOWERF 102

#define SPECIAL -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define HOME 71
#define END 79
#define DEL 83

#define RESET      "\033[0m"

#define FG_BLACK      "\033[30m"
#define FG_RED        "\033[31m"
#define FG_GREEN      "\033[32m"
#define FG_YELLOW     "\033[33m"
#define FG_BLUE       "\033[34m"
#define FG_MAGENTA    "\033[35m"
#define FG_CYAN       "\033[36m"
#define FG_WHITE      "\033[37m"

#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"


typedef struct {
    int id, age;
    char sex, fName[20], lName[20], department[20], phone[13];
    float salary, overtime, deduction;
} Employee;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool isFloat(char ch) {
    return isdigit(ch) || ch == DOT;
}

bool isSex(char ch) {
    return ch == LOWERF || ch == LOWERM;
}

bool isText(char ch) {
    return isalpha(ch) || ch == SPACE || ch == DASH;
}

bool isPhone(char ch) {
    return isdigit(ch) || ch == DASH;
}

void viewEmployee(Employee* empArray, int* empTotal, int idx) {
    if (*empTotal == 0) return;

    char fullName[40] = "\0";
    strcat(fullName, empArray[idx].fName);
    strcat(fullName, " ");
    strcat(fullName, empArray[idx].lName);

    float netSalary = empArray[idx].salary + empArray[idx].overtime - empArray[idx].deduction;

    system("cls");
    gotoxy(10, 4);
    printf("Info:");
    gotoxy(10, 8);
    printf("Full Name: ");
    printf(FG_YELLOW "%s" RESET, fullName);
    gotoxy(50, 8);
    printf("Gender: ");
    printf(FG_YELLOW "%c" RESET, empArray[idx].sex);
    gotoxy(10, 10);
    printf("Age: ");
    printf(FG_YELLOW "%i" RESET, empArray[idx].age);
    gotoxy(50, 10);
    printf("Phone: ");
    printf(FG_YELLOW "%s" RESET, empArray[idx].phone);
    gotoxy(10, 12);
    printf("Department: ");
    printf(FG_YELLOW "%s" RESET, empArray[idx].department);
    gotoxy(50, 12);
    printf("Net Salary: ");
    printf(FG_YELLOW "%.2lf" RESET, netSalary);
    gotoxy(10, 14);
    system("pause");
}

void deleteEmployee(Employee* empArray, int* empTotal, int idx) {
    if (*empTotal == 0) return;
    char ch;

    char fullName[40] = "\0";
    strcat(fullName, empArray[idx].fName);
    strcat(fullName, " ");
    strcat(fullName, empArray[idx].lName);

    system("cls");
    gotoxy(10, 4);
    printf("Alert:");
    gotoxy(10, 8);
    printf("Are you sure you want to delete " FG_YELLOW "%s" RESET "?", fullName);
    gotoxy(10, 10);
    printf("Press " FG_YELLOW "y" RESET " to confirm or " FG_YELLOW "n" RESET " to cancel.");
    
    do {
        ch = getch();
        if (ch == 'n') return;
        if (ch == 'y') {
            for (int i = idx; i < *empTotal - 1; i++) {
                empArray[i] = empArray[i + 1];
            }
            (*empTotal)--;
        }
    } while (ch != 'y');
}

void selectEmployee(Employee* empArray, int* empTotal) {
    int exitFlag = 0, current = 0, temp = 0;
    int totalPages = (*empTotal + 10 - 1) / 10; // Total number of pages
    int currentPage = 0;
    char ch;

    do {
        // Display header
        system("cls");
        gotoxy(10, 4);
        printf("Select an employee:");

        // Display current page
        int pageStart = currentPage * 10;
        int pageEnd = pageStart + 10 < *empTotal ? pageStart + 10 : *empTotal;
        for (int i = pageStart; i < pageEnd; i++) {
            if (i == current) {
                printf(FG_BLACK BG_WHITE);
            }
            gotoxy((10 + 40 * ((i - pageStart)/ 5)), (8 + 2 * ((i - pageStart) % 5)));
            printf("%i\t%s", empArray[i].id, empArray[i].fName);
            printf(RESET);
        }

        // Display navigation instructions
        gotoxy(10, 18);
        printf("Press " FG_YELLOW "ENTER" RESET " to select, " FG_YELLOW "DEL" RESET " to delete, or " FG_YELLOW "ESC" RESET " to exit.");

        gotoxy((10 + 40 * ((current - pageStart) / 5)), (8 + 2 * ((current - pageStart) % 5)));
        ch = getch();
        switch (ch) {
        case SPECIAL:
            ch = getch();
            switch (ch) {
            case UP:
                current = (current > 0) ? current - 1 : *empTotal - 1;
                currentPage = current / 10;
                break;

            case DOWN:
                current = (current < *empTotal - 1) ? current + 1 : 0;
                currentPage = current / 10;
                break;
            
            case LEFT:
                if (current >= (*empTotal < 10? *empTotal: 10)) {
                    current -= 10;
                } else {
                    current += (*empTotal - (*empTotal % 10 == 0? 10: *empTotal % 10));
                    if (current >= *empTotal) current = *empTotal - 1;
                }
                currentPage = current / 10;
                break;

            case RIGHT:
                if (current < *empTotal - (*empTotal % 10 == 0? 10: *empTotal % 10)) {
                    current += 10;
                    if (current >= *empTotal) current = *empTotal - 1;
                } else {
                    current -= (*empTotal - (*empTotal % 10 == 0? 10: *empTotal % 10));
                }
                currentPage = current / 10;
                break;

            case HOME:
                currentPage = 0;
                current = 0;
                break;

            case END:
                currentPage = totalPages - 1;
                current = *empTotal - 1;
                break;

            case DEL:
                deleteEmployee(empArray, empTotal, current);
                if (current == *empTotal) {
                    current--;
                    currentPage = current / 10;
                }
                break;
            }
            break;

        case ENTER:
            viewEmployee(empArray, empTotal, current);
            break;

        case ESC:
            exitFlag = 1;
            break;
        }
    } while (!exitFlag);
}

void drawForm(int current) {
    char form[10][13] = {"First Name:", "Last Name:", "Gender:", "Age:", "Phone:", "Department:", "Salary:", "Overtime:", "Deduction:"};
    gotoxy(10, 4);    
    printf("Enter empolyee info:");
    for (int i = 0; i < 10; i++) {
        if (i == current) {
            printf(FG_BLACK BG_WHITE);
        } 
        gotoxy((10 + 40 * (i / 5)), (8 + 2 * (i % 5)));
        cprintf("%s\n", form[i]);
        printf(RESET);
    }
    gotoxy(10, 18);    
    printf("Press " FG_YELLOW "ENTER" RESET " to save or " FG_YELLOW "ESC" RESET " to cancel.");
}

char** multiLine(int count, char* type, int* len, int originX, int originY) {
    int currentX = 0, currentY = 0, *end, *pointFlag;
    char **field, ch;

    field = (char**) malloc(count * sizeof(char*));
    for (int i = 0; i < count; i++)
    {
        field[i] = (char*) malloc((len[i] + 1) * sizeof(char));
        for (int j = 0; j < (len[i] + 1); j++)
        {
            field[i][j] = ' ';
        } 
    }

    end = (int*) malloc(count * sizeof(int));
    for (int i = 0; i < count; i++)
        {
            end[i] = 0;
        }

    pointFlag = (int*) malloc(count * sizeof(int));
    for (int i = 0; i < count; i++)
        {
            pointFlag[i] = 0;
        }

    do {
        system("cls");
        drawForm(currentY);
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < len[i]; j++)
            {
                if (i == currentY && j == currentX) {
                    printf(FG_BLACK BG_YELLOW);
                } else {printf(FG_YELLOW);}
                gotoxy((originX + 40 * (i / 5) + j), (originY + 2 * (i % 5)));
                printf("%c", field[i][j]);
                printf(RESET);
            }
            
        }

        gotoxy((originX + 40 * (currentY / 5) + currentX), (originY + 2 * (currentY % 5)));
        ch = getch();

        switch (ch) {
        case SPECIAL:
        ch = getch();
            switch (ch) {  
            case UP:
                currentY--;
                if (currentY == -1) currentY = count - 1;
                currentX = end[currentY];
                if (currentX == len[currentY]) currentX--;
                break;

            case DOWN:
                currentY++;
                if (currentY == count) currentY = 0;
                currentX = end[currentY];
                if (currentX == len[currentY]) currentX--;
                break;

            case LEFT:
                currentX--;
                if (currentX == -1) {
                    currentX = end[currentY];
                    if (currentX == len[currentY]) currentX--;
                }
                break;
            
            case RIGHT:
                currentX++;
                if (currentX == end[currentY] + 1) currentX = 0;
                if (currentX == len[currentY]) currentX = 0;
                break;

            case HOME:
                currentX = 0;
                break;

            case END:
                currentX = end[currentY];
                if (currentX == len[currentY]) currentX--;
                break;
            }
            break;

        case ENTER:
            bool incompleteData = false;
            for (int i = 0; i < count; i++) {
                if (end[i] == 0) {
                    incompleteData = true;
                    break;
                    }
            }
            if (incompleteData) break;
            for (int i = 0; i < count; i++)
            {
                for (int j = len[i]; j >= 0; j--) {
                    if (field[i][j] != ' ') break;
                    field[i][j] = '\0';
                }
            }
            free(end);
            free(pointFlag);
            return field;
            break;

        case ESC:
            // cancels the operation
            for (int i = 0; i < count; i++) {
                free(field[i]);
            }
            free(field);
            free(end);
            free(pointFlag);
            return NULL;
            break;

        case TAB:
            currentY++;
            if (currentY == count) currentY = 0;
            currentX = end[currentY];
            if (currentX == len[currentY]) currentX--;
            break;

        case BS:
            if (end[currentY] > 0) {
                if (field[currentY][end[currentY] - 1] == DOT) pointFlag[currentY] = 0;
                field[currentY][end[currentY]-- - 1] = ' ';
                currentX = end[currentY];
            }
            break;
        
        default:
            if (type[currentY] == 't' && isText(ch)) {
                // text validation
            } else if (type[currentY] == 'f' && isFloat(ch)) {
                // number validation
                if (ch == DOT && pointFlag[currentY]) break;
                if (field[currentY][currentX] == DOT) pointFlag[currentY] = 0;
                if (ch == DOT) pointFlag[currentY] = 1;
            } else if (type[currentY] == 's' && isSex(ch)) {
                // sex validation
            } else if (type[currentY] == 'd' && isdigit(ch)) {
                // digit validation
            } else if (type[currentY] == 'p' && isPhone(ch)) {
                // phone validation
            } else {
                // skip invalid
                break;
            }
            field[currentY][currentX++] = ch;
            if (currentX > end[currentY]) end[currentY]++;
            if (currentX == len[currentY]) currentX--;
            break;
        }
    } while (true);
}

void addEmployee(Employee** empArray, int* empTotal, int* maxID, int* capacity) {
    // Attempt to reallocate memory for one additional employee
    if (*empTotal == *capacity) {
        (*capacity) *= 2;
        Employee* temp = realloc(*empArray, (*capacity) * sizeof(Employee));
        if (temp == NULL) {
            // Reallocation failed
            system("cls");
            gotoxy(10, 4);
            printf("Alert:");
            gotoxy(10, 8);
            printf("No enough memory.");
            gotoxy(10, 10);
            system("pause");
            return;
        }
        // Update the array pointer
        *empArray = temp;
    }

    // Collect employee data
    int num = 9;
    char types[] = {'t', 't', 's', 'd', 'p', 't', 'f', 'f', 'f'};
    int lens[] = {20, 20, 1, 2, 12, 20, 10, 10, 10};
    char** data = multiLine(num, types, lens, 24, 8);

    if (data == NULL) {
        // Operation was canceled
        system("cls");
        gotoxy(10, 4);
        printf("Alert:");
        gotoxy(10, 8);
        printf("Operation was canceled.");
        gotoxy(10, 10);
        system("pause");
        return;
    }

    // Add the employee
    int idx = (*empTotal)++;
    (*empArray)[idx].id = ++(*maxID);
    strcpy((*empArray)[idx].fName, data[0]);
    strcpy((*empArray)[idx].lName, data[1]);
    (*empArray)[idx].sex = data[2][0];
    (*empArray)[idx].age = atoi(data[3]);
    strcpy((*empArray)[idx].phone, data[4]);
    strcpy((*empArray)[idx].department, data[5]);
    (*empArray)[idx].salary = atof(data[6]);
    (*empArray)[idx].overtime = atof(data[7]);
    (*empArray)[idx].deduction = atof(data[8]);

    // Free temporary input data
    for (int i = 0; i < num; i++) {
        free(data[i]);
    }
    free(data);

    system("cls");
    gotoxy(10, 4);
    printf("Alert:");
    gotoxy(10, 8);
    printf("Employee was added successfully.");
    gotoxy(10, 10);
    system("pause");
    return;
}

void saveState(Employee* empArray,int empTotal,int maxID) {
    FILE *file = fopen("RetroDB.csv", "w");
    if (file == NULL) {
        system("cls");
        gotoxy(10, 4);
        printf("Alert:");
        gotoxy(10, 8);
        printf("couldn't sava data! Thanks for using RetroHR v4.0.\n");
        gotoxy(10, 10);
        printf("Copyright (c) mina inc. 2000-2024.\n");
        gotoxy(10, 12);
        system("pause");
        return;
    }

    fprintf(file, "%i,%i\n", empTotal, maxID);
    fprintf(file, "id,fName,lName,phone,age,sex,department,salary,overtime,deduction\n");

    for (int i = 0; i < empTotal; i++) {
        fprintf(file, "%i,%s,%s,%s,%i,%c,%s,%.2f,%.2f,%.2f\n",
            empArray[i].id,
            empArray[i].fName,
            empArray[i].lName,
            empArray[i].phone,
            empArray[i].age,
            empArray[i].sex,
            empArray[i].department,
            empArray[i].salary,
            empArray[i].overtime,
            empArray[i].deduction);
    }

    fclose(file);
    system("cls");
    gotoxy(10, 4);
    printf("Alert:");
    gotoxy(10, 8);
    printf("Data saved successfully! Thanks for using RetroHR v4.0.\n");
    gotoxy(10, 10);
    printf("Copyright (c) mina inc. 2000-2024.\n");
    gotoxy(10, 12);
    system("pause");
    return;
}

void mainMenu(Employee** empArray, int* empTotal, int *maxID, int* capacity) {
    char menu[3][5] = {"VIEW", "ADD", "EXIT"}, ch;
    int current = 0, exitFlag = 0;

    do {
        system("cls");
        gotoxy(10, 4);
        printf("Welcome to RetroHR v4.0 select an option to begin:");

        for (int i = 0; i < 3; i++) {
            if (i == current) {
                printf(FG_BLACK BG_WHITE);
            } 
            gotoxy(10, 8 + 2 * i);    
            cprintf("%i\t%s\n", i+1, menu[i]);
            printf(RESET);
        }

        gotoxy(10, 8 + 2 * (current));
        ch = getch();
        switch (ch) {
        case SPECIAL:
        ch = getch();
            switch (ch) {
                
            case UP:
                (current)--;
                if (current == -1) current = 2;
                break;
            
            case DOWN:
                (current)++;
                if (current == 3) current = 0;
                break;

            case HOME:
                current = 0;
                break;

            case END:
                exitFlag = 1;
                break;
            }
            break;

        case ENTER:
            switch (current) {
            case 0:
                selectEmployee(*empArray, empTotal);
                break;
            
            case 1:
                addEmployee(empArray, empTotal, maxID, capacity);
                break;

            case 2:
                exitFlag = 1;
                break;
            }
            break;

        case ESC:
            exitFlag = 1;
            break;
        }
    } while (!exitFlag);
}

Employee* loadState(int* empTotal, int* maxID, int* capacity) {
    FILE* file = fopen("RetroDB.csv", "r");
    if (file == NULL) {
        // File not found, initialize values and return NULL
        return NULL;
    }

    // Read the first line: number of employees and maxID
    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) {
        // File is empty or reading failed
        fclose(file);
        return NULL;
    }

    // Parse the first line
    char* token = strtok(line, ",");
    *empTotal = atoi(token); // Number of employees
    *capacity = 2 * (*empTotal);

    token = strtok(NULL, ",");
    *maxID = atoi(token); // Maximum ID

    if (*capacity == 0) {
        *capacity = 1;
    }

    // Allocate memory for the employee array
    Employee* empArray = (Employee*)malloc((*capacity) * sizeof(Employee));
    if (empArray == NULL) {
        system("cls");
        gotoxy(10, 4);
        printf("Alert:");
        gotoxy(10, 8);  
        perror("No enough memory.");
        gotoxy(10, 10);
        system("pause");
        exit(-1);
    }

    // Skip the header line
    fgets(line, sizeof(line), file);

    // Read and parse each employee record
    for (int i = 0; i < *empTotal; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            *empTotal = i; // Update with the actual number of records read
            break;
        }

        token = strtok(line, ",");
        empArray[i].id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(empArray[i].fName, token);

        token = strtok(NULL, ",");
        strcpy(empArray[i].lName, token);

        token = strtok(NULL, ",");
        strcpy(empArray[i].phone, token);

        token = strtok(NULL, ",");
        empArray[i].age = atoi(token);

        token = strtok(NULL, ",");
        empArray[i].sex = token[0];

        token = strtok(NULL, ",");
        strcpy(empArray[i].department, token);

        token = strtok(NULL, ",");
        empArray[i].salary = atof(token);

        token = strtok(NULL, ",");
        empArray[i].overtime = atof(token);

        token = strtok(NULL, ",");
        empArray[i].deduction = atof(token);
    }

    fclose(file);
    return empArray;
}

int main() {
    int empTotal = 0, maxID = 0, capacity = 0;
    Employee *empArray = loadState(&empTotal, &maxID, &capacity); // Load employees from file
    // No employees loaded; allocate space for one
    if (empArray == NULL) {
        system("cls");
        gotoxy(10, 4);
        printf("Alert:");
        gotoxy(10, 8); 
        printf("No data found.\n");
        gotoxy(10, 10);
        system("pause");
        capacity = 1;
        empArray = (Employee *)malloc(capacity * sizeof(Employee));
        if (empArray == NULL) {
            system("cls");
            gotoxy(10, 4);
            printf("Alert:");
            gotoxy(10, 8); 
            perror("No enough memory.");
            gotoxy(10, 10);
            system("pause");
            exit(-1);
        }
    }
    // start the program
    mainMenu(&empArray, &empTotal, &maxID, &capacity);
    // save data before exiit
    saveState(empArray, empTotal, maxID);
    // free memory
    free(empArray);
    return 0;
}