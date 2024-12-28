#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

time_t start = 0;
char command;

void mainMenu();


void timeSessionOptions();

void startTimer(time_t* start);

void stopTimer(time_t start);

void logTime();

void generateReportOptions();

void yearlyReport();

void monthlyReport();

void saveToFile(int minutes);

int isWholeNumber(char* str);

void printLastReported();

void clearBuffer();






int main(int argc, char* argv[]) {

    int minutes = 0;
    

    if (argc == 2) {

        if (strcmp(argv[1], "-h") == 0) {
            fprintf(stderr, "Usage: %s <minutes worked> (must be whole number)\n", argv[0]);
            printf("%s", "Or open application executable and navigate within menu\n");
            return 1;
        }
        else if (isWholeNumber(argv[1])) {

            printLastReported();
            minutes = atoi(argv[1]);
            printf("Received time worked: %d minutes\n", minutes);
            saveToFile(minutes);
        }
        else {
            printf("%s", "Invalid argument. Please provide a whole number.\n");
            return 1;
        }
    }
    else if (argc == 1) {
        printLastReported();
        mainMenu();
    }
    else  {
        printf("%s", "Invalid number of arguments. '-h' for usage\n");
        return 1;
    }

        return 0;
}

/***
    End of main
*/

void mainMenu() {
    int quitflag = 0;
    while (!quitflag) {
        printf("%s", "\nWorktime Logger\n");
        printf("%s", "1. Time session\n");
        printf("%s", "2: Log time\n");
        printf("%s", "3: Generate report\n");
        printf("%s", "q: Quit\n");
        printf("%s", "Enter command: ");
        scanf_s(" %c", &command, 1);

        switch (command) {
        case '1':
            timeSessionOptions();
            break;
        case '2':
            logTime();
            break;

        case '3':
            generateReportOptions();
            break;
        case 'q':
            printf("Exiting...\n");
            quitflag = 1;
            break;
        default:
            printf("Invalid command!\n");
            break;
        }
    }
}

void timeSessionOptions() {
    int quitflag = 0;
    while (!quitflag) {
        printf("%s", "\nTime Session \n");
        printf("%s", "1. Start timer\n");
        printf("%s", "2: Stop timer\n");
        printf("%s", "3: Go back\n");
        scanf_s(" %c", &command, 1);

        switch (command) {
        case '1':
            if (start != 0) {
                printf("%s", "Error: Timer already ongoing!");
            }
            else {
                startTimer(&start);
            }

            break;
        case '2':
            if (start == 0) {
                printf("%s", "Error: Timer has not been started!\n");
            }
            else {
                stopTimer(start);
                start = 0;
            }
            break;
        case '3':
            printf("%s", "Returning...\n");
            quitflag = 1;
            break;
        default:
            printf("%s", "Invalid command!\n");
        }
    }
}



void startTimer(time_t* start) {
    *start = time(NULL);

    char start_time_str[26];
    ctime_s(start_time_str, sizeof(start_time_str), start);
    printf("Worktime started at %s", start_time_str);
}

void stopTimer(time_t start) {
    time_t end = time(NULL);
    double elapsed = difftime(end, start);

    if (elapsed >= 60) {
        
        int minutes = ((int)elapsed / 60);
        fprintf("Timer stopped. Time elapsed = %d\n", minutes);
        saveToFile(minutes);
    }
    else {
        printf("%s", "Timer stopped. Elapsed time less than 60 seconds. Session discarded");
    }
}

void logTime() {
    int minutes;
    printf("Enter the time worked in minutes: ");
    scanf_s("%d", &minutes);

    if (minutes <= 0) {
        printf("%s", "Invalid input. Please enter a positive integer.\n");
        return;
    }

    saveToFile(minutes);
    printf("Time of %d minutes has been logged for today.\n", minutes);
    printf("%s", "Press Enter to return to the main menu...");
    getchar(); 
    getchar(); 
    
}

void generateReportOptions() {
    int quitflag = 0;
    while (!quitflag) {
        printf("%s", "\nGenerate report from: \n");
        printf("%s", "1. Year\n");
        printf("%s", "2: Month\n");
        printf("%s", "3: Go back\n");
        scanf_s(" %c", &command, 1);

        switch (command) {
        case '1':
            yearlyReport();
            break;
        case '2':
            monthlyReport();
            break;
        case '3':
            printf("%s", "Returning...\n");
            quitflag = 1;
            clearBuffer();
            break;
        default:
            printf("%s", "Invalid command!\n");
        }
    }
 
}


void yearlyReport() {
    int year;

    
    printf("%s", "Enter the year for the report: ");
    scanf_s("%d", &year);

    
    FILE* file = fopen("worktime_data.bin", "rb");
    if (file == NULL) {
        printf("%s", "Error opening file for reading.\n");
        return;
    }

  
    int file_year, month, day, minutes;
    int total_minutes_per_month[12] = { 0 }; 
    
    while (fread(&file_year, sizeof(int), 1, file) == 1) {
        fread(&month, sizeof(int), 1, file);
        fread(&day, sizeof(int), 1, file);
        fread(&minutes, sizeof(int), 1, file);

        if (file_year == year) {
            total_minutes_per_month[month - 1] += minutes; 
        }
    }

    
    fclose(file);

    
    printf("\nYearly Report for %d\n", year);
    printf("%s", "Month\t\tHours\tMinutes\n");
    for (int i = 0; i < 12; i++) {
        printf("%d\t\t%d\t%d\n", i + 1, total_minutes_per_month[i] / 60, total_minutes_per_month[i] % 60);
    }

    
    printf("%s", "\nPress Enter to return...");
    clearBuffer();
    getchar(); 
}

void monthlyReport() {
    int year;
    char month_str[4];

   
    printf("%s", "Enter the year for the report: ");
    scanf_s("%d", &year);

    
    printf("%s", "Enter the month (first 3 letters): ");
    scanf_s("%s", month_str);

    
    for (int i = 0; month_str[i]; i++) {
        month_str[i] = tolower(month_str[i]);
    }

    
    int month;
    if (strcmp(month_str, "jan") == 0) month = 1;
    else if (strcmp(month_str, "feb") == 0) month = 2;
    else if (strcmp(month_str, "mar") == 0) month = 3;
    else if (strcmp(month_str, "apr") == 0) month = 4;
    else if (strcmp(month_str, "may") == 0) month = 5;
    else if (strcmp(month_str, "jun") == 0) month = 6;
    else if (strcmp(month_str, "jul") == 0) month = 7;
    else if (strcmp(month_str, "aug") == 0) month = 8;
    else if (strcmp(month_str, "sep") == 0) month = 9;
    else if (strcmp(month_str, "oct") == 0) month = 10;
    else if (strcmp(month_str, "nov") == 0) month = 11;
    else if (strcmp(month_str, "dec") == 0) month = 12;
    else {
        printf("%s", "Invalid month input.\n");
        return;
    }

    
    FILE* file = fopen("worktime_data.bin", "rb");
    if (file == NULL) {
        printf("%s", "Error opening file for reading.\n");
        return;
    }

    
    int file_year, file_month, day, minutes;
    int total_minutes = 0;

    
    printf("\nMonthly Report for %s %d\n", month_str, year);
    printf("%s", "Day\t\tHours\tMinutes\n");
    while (fread(&file_year, sizeof(int), 1, file) == 1) {
        fread(&file_month, sizeof(int), 1, file);
        fread(&day, sizeof(int), 1, file);
        fread(&minutes, sizeof(int), 1, file);

        if (file_year == year && file_month == month) {
            printf("%d\t\t%d\t%d\n", day, minutes / 60, minutes % 60);
            total_minutes += minutes;
        }
    }

    
    fclose(file);

    
    printf("\nTotal for %s %d: %d hours %d minutes\n", month_str, year, total_minutes / 60, total_minutes % 60);

    
    printf("%s", "\nPress Enter to return...");
    clearBuffer();
    getchar(); 
    
}


void saveToFile(int minutes) {
    
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    
    int year = timeinfo->tm_year + 1900; 
    int month = timeinfo->tm_mon + 1; 
    int day = timeinfo->tm_mday;

    
    FILE* file = fopen("worktime_data.bin", "ab");
    if (file == NULL) {
        
        printf("%s", "Error opening file for writing.\n");
        return;
    }

    
    fwrite(&year, sizeof(int), 1, file);
    fwrite(&month, sizeof(int), 1, file);
    fwrite(&day, sizeof(int), 1, file);
    fwrite(&minutes, sizeof(int), 1, file);
    
    fclose(file);

    printf("%s", "Data saved to file.\n");
}

int isWholeNumber(char* str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0; 
        }
        str++;
        }
        return 1; 
    }

void printLastReported() {
    
    FILE* file = fopen("worktime_data.bin", "rb");
    if (file == NULL) {
        printf("%s", "Error opening file for reading.\n");
        return;
    }

    
    fseek(file, 0, SEEK_END);

    
    long pos = ftell(file);
    long record_size = sizeof(int) * 4; 
    long last_record_pos = pos - record_size;

    
    if (last_record_pos < 0) {
        printf("%s", "No records found.\n");
        fclose(file);
        return;
    }

    
    fseek(file, last_record_pos, SEEK_SET);

    
    int year, month, day, minutes;
    fread(&year, sizeof(int), 1, file);
    fread(&month, sizeof(int), 1, file);
    fread(&day, sizeof(int), 1, file);
    fread(&minutes, sizeof(int), 1, file);

    
    fclose(file);

    
    printf("%s", "\nLast reported worktime:\n");
    printf("Date: %d-%02d-%02d\n", year, month, day);
    printf("Minutes worked: %d\n", minutes);
}

void clearBuffer() {
int c;
while ((c = getchar()) != '\n' && c != EOF) {}

}
