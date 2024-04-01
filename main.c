#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ROWS 3192
#define COLS 11
#define YEARROWS 256 // (ROWS - index of year 1760) / 12 months (index of 1760 is 121)

// Declare the arrays to store the data
char dates[ROWS][COLS];
char years[YEARROWS][COLS];
double LandAvrgTemp[ROWS];
double LandAvrgTempUncertainty[ROWS];
double LandMaxTemp[ROWS];
double LandMaxTempUncertainty[ROWS];
double LandMinTemp[ROWS];
double LandMinTempUncertainty[ROWS];
double LandAndOceanAvrgTemp[ROWS];
double LandAndOceanAvrgTempUncertainty[ROWS];
double YearlyLandAvrgTemp[YEARROWS]; // starts at year 1760
double MonthlyAvrgTemp[12];
char month[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char YearString[ROWS];
int YearInt[ROWS];

// Function to read data from a CSV file and assign it to various arrays
void assignArrays()
{
    // Open the CSV file for reading
    FILE *fp = fopen("GlobalTemperatures.csv", "r");
    // Check if the file was opened successfully
    if (fp == NULL)
    {
        printf("Failed to open file.\n");
        return; // Exit the function if file cannot be opened
    }

    char line[1000]; // Array to hold each line of the file

    // Loop through each row of the file
    // Start with row = -1 to skip the header of the CSV
    for (int row = -1; row < ROWS; row++)
    {
        fgets(line, 1000, fp);           // Read a line from the file
        char *token = strtok(line, ","); // Use strtok to split the line into tokens based on commas
        int col = 0;                     // Column index

        // Check if the current row is the header
        if (row == -1)
        {
            continue; // Skip the rest of the loop if it's the header
        }

        // Process each token (column) in the line
        while (token != NULL)
        {
            // Use a switch statement to assign the token to the correct array based on the column index
            switch (col)
            {
            case 0: // Date
                strcpy(dates[row], token);
                break;
            case 1: // Average Land Temperature
                LandAvrgTemp[row] = atof(token);
                break;
            case 2: // Land Average Temperature Uncertainty
                LandAvrgTempUncertainty[row] = atof(token);
                break;
            case 3: // Maximum Land Temperature
                LandMaxTemp[row] = atof(token);
                break;
            case 4: // Maximum Land Temperature Uncertainty
                LandMaxTempUncertainty[row] = atof(token);
                break;
            case 5: // Minimum Land Temperature
                LandMinTemp[row] = atof(token);
                break;
            case 6: // Minimum Land Temperature Uncertainty
                LandMinTempUncertainty[row] = atof(token);
                break;
            case 7: // Average Land and Ocean Temperature
                LandAndOceanAvrgTemp[row] = atof(token);
                break;
            case 8: // Land and Ocean Average Temperature Uncertainty
                LandAndOceanAvrgTempUncertainty[row] = atof(token);
                break;
            }
            token = strtok(NULL, ","); // Move to the next token in the line
            col++;                     // Increment the column index for the next token
        }
    }

    fclose(fp); // Close the file after reading all lines
}
void YearlyAvgCalculator(double *inputArray, double *outputArray, int size)
{
    int counter = 0;
    double yearlytotaltemp = 0;
    int noData = 0;
    int yearIndex = 0; // Index for the year in outputArray
    for (int i = 0; i < size; i++)
    {
        yearlytotaltemp += inputArray[i];
        counter++;
        if (inputArray[i] == 0)
        {
            noData++;
        }
        if (counter == 12)
        { // End of a year
            if (noData < 12)
            {
                outputArray[yearIndex] = yearlytotaltemp / (12 - noData); // Store average
            }
            else
            {
                outputArray[yearIndex] = 0; // No data for this year
            }
            counter = 0;
            noData = 0;
            yearlytotaltemp = 0;
            yearIndex++;
            if (yearIndex >= YEARROWS)
            { // Prevents writing beyond the output array's bounds
                break;
            }
        }
    }
}
void q1() // Calculate yearly average for each year between 1760 and 2015
{
    int counter = 0;
    double yearlytotaltemp = 0;
    int j = 0; // j is the tracker variable for each years[]'s element
    FILE *q6 = fopen("q6.txt", "w");
    FILE *century19th = fopen("century19th.txt", "w");
    FILE *century20th = fopen("century20th.txt", "w");

    for (int i = 120; i < ROWS; i++) // Index 120 starts at the beginning of year 1760
    {
        yearlytotaltemp += LandAvrgTemp[i];
        counter++;

        if (counter == 12) // Assigns an average yearly temperature and resets the counter after every 12 months
        {
            strncpy(years[j], dates[i], 4);
            YearlyLandAvrgTemp[j] = yearlytotaltemp / 12;
            printf("The average temperature for the year %s is %lf degrees Celsius.\n", years[j], YearlyLandAvrgTemp[j]);
            fprintf(q6, "%s %lf\n", years[j], YearlyLandAvrgTemp[j]);
            counter = 0;
            yearlytotaltemp = 0;

            YearInt[j] = atoi(years[j]); // Convert string to years

            // Assigns the average yearly temperature to the respective century file
            if (YearInt[j] >= 1800 && YearInt[j] <= 1899)
            {
                fprintf(century19th, "%d %lf\n", (YearInt[j] - 1799), YearlyLandAvrgTemp[j]); // Print x and y values for the yearly temperature for 19th century
            }
            else if (YearInt[j] >= 1900 && YearInt[j] <= 1999)
            {
                fprintf(century20th, "%d %lf\n", (YearInt[j] - 1899), YearlyLandAvrgTemp[j]); // Print x and y values for the yearly temperature for 19th century
            }

            j++; // Increment the tracker variable
        }
    }

    fclose(q6);
    fclose(century19th);
    fclose(century20th);
}

// Calculate the average land temperature for the different centuries
void q2()
{
    // Define century names for easy reference
    char century[4][5] = {"18th", "19th", "20th", "21th"};
    // Initialize total temperature sums for each century to 0
    double centuryTotals[4] = {0, 0, 0, 0};
    // Initialize counters for the number of temperature records per century
    double centuryCounter[4] = {0, 0, 0, 0};
    // Array to store the calculated average temperatures for each century
    double centuryAvrgTemp[4];

    // Loop through all rows starting from index 120 (assumed start year of 1760)
    for (int i = 120; i < ROWS; i++)
    {
        // Extract the year from the dates array and convert it to an integer
        strncpy(YearString, dates[i], 4);
        YearInt[i] = atoi(YearString);
        // Determine the century of the current year and accumulate the total temperature and count
        if (YearInt[i] >= 1760 && YearInt[i] < 1800)
        {
            centuryTotals[0] += LandAvrgTemp[i];
            centuryCounter[0]++;
        }
        else if (YearInt[i] >= 1800 && YearInt[i] < 1900)
        {
            centuryTotals[1] += LandAvrgTemp[i];
            centuryCounter[1]++;
        }
        else if (YearInt[i] >= 1900 && YearInt[i] < 2000)
        {
            centuryTotals[2] += LandAvrgTemp[i];
            centuryCounter[2]++;
        }
        else if (YearInt[i] >= 2000 && YearInt[i] < 2100)
        {
            centuryTotals[3] += LandAvrgTemp[i];
            centuryCounter[3]++;
        }
    }
    // Calculate and print the average temperature for each century
    for (int i = 0; i < 4; i++)
    {
        // Calculate the average by dividing the total by the count
        centuryAvrgTemp[i] = centuryTotals[i] / centuryCounter[i];
        // Print the calculated average temperature for the century
        printf("The average temperature for the %s century is %.5lf degrees Celsius.\n", century[i], centuryAvrgTemp[i]);
    }
}

void q3() // Calculate monthly averages for all years between 1900 and 2015
{
    double monthlytotaltemp = 0;
    int counter = 0;
    int j;

    for (int i = 0; i < 12; i++) // Outer loop iterates for each month
    {
        for (j = 1680 + i; j < ROWS; j += 12) // Index 1680 starts at the beginning of year 1900 plus the current month, inner loop assigns value to each month element
        {
            monthlytotaltemp += LandAvrgTemp[j];
            counter++;
        }
        MonthlyAvrgTemp[i] = monthlytotaltemp / counter;
        printf("The average temperature for %s is %lf degrees Celsius.\n", month[i], MonthlyAvrgTemp[i]);
        monthlytotaltemp = 0; // Counter and total monthly accumulation resets after all iterations of the specific month ends
        counter = 0;
    }
}

// What was the hottest month recorded and what was the coldest month recorded? Ignore ties
void q4()
{
    // Initialize the coldest and hottest temperatures to the first temperature in the dataset
    double coldest = LandAvrgTemp[0];
    double hottest = LandAvrgTemp[0];
    // Allocate memory for the hottest and coldest dates
    char *hottestDate = malloc(7);
    char *coldestDate = malloc(7);

    // Iterate through the dataset starting from the 120th index (assuming data relevant to the query starts here)
    for (int i = 120; i < ROWS; i++)
    {
        // Update the coldest temperature and its date if a new minimum is found
        if (coldest > LandAvrgTemp[i])
        {
            coldest = LandAvrgTemp[i];
            strncpy(coldestDate, dates[i], 7); // Copy the date of the coldest temperature
        }
        // Update the hottest temperature and its date if a new maximum is found
        if (hottest < LandAvrgTemp[i])
        {
            hottest = LandAvrgTemp[i];
            strncpy(hottestDate, dates[i], 7); // Copy the date of the hottest temperature
        }
    }

    // Print the results
    printf("The hottest month recorded is %s and its temperature is %.2lf˚\n", hottestDate, hottest);
    printf("The coldest month recorded is %s and its temperature is %.2lf˚\n", coldestDate, coldest);
}

void q5() // Determine the hottest and coldest year between 1760 and 2015
{
    double coldtemp = YearlyLandAvrgTemp[0]; // Hot and cold temperatures are intially set as the first element in the array
    double hottemp = YearlyLandAvrgTemp[0];
    char hotyear[5], coldyear[5];

    for (int i = 0; i < YEARROWS; i++) // The hot and cold temperature values update with each iteration of loop
    {
        if (hottemp < YearlyLandAvrgTemp[i])
        {
            hottemp = YearlyLandAvrgTemp[i];
            strncpy(hotyear, years[i], 5);
        }
        if (coldtemp > YearlyLandAvrgTemp[i])
        {
            coldtemp = YearlyLandAvrgTemp[i];
            strncpy(coldyear, years[i], 5);
        }
    }
    printf("The hottest year is %s and its average temperature is %lf degrees Celsius.\nThe coldest year is %s and its average temperature is %lf degrees Celsius.", hotyear, hottemp, coldyear, coldtemp);
}

void q8() // Write to GNUPlot data file and graph
{
    FILE *q8 = fopen("q8.txt", "w");
    int counter = 0;

    // for (int i = 0; i < ROWS; i++) // determining land max and min temps
    // {
    //     LandMaxTemp[i] = LandAvrgTemp[i] + LandAvrgTempUncertainty[i];
    //     LandMinTemp[i] = LandAvrgTemp[i] - LandAvrgTempUncertainty[i];
    // }

    // FIX YEAR INT SO THAT IT TAKES MORE THAN I - 120 (has to start at 120)

    // this question can only be called if the other question is called

    double YearlyLandUncertainty[ROWS];
    double yearlylanduncertaintyaverage = 0;
    double YearlyLandMaxTemp[ROWS];
    double YearlyLandMinTemp[ROWS];

    double realYearlyLandAvrgTemp[ROWS];
    double yearlylandaveragetemp = 0;

    for (int i = 0; i < ROWS; i++)
    {
        yearlylanduncertaintyaverage += LandAvrgTempUncertainty[i];
        yearlylandaveragetemp += LandAvrgTemp[i];

        counter++;
        if (counter == 12)
        {
            realYearlyLandAvrgTemp[i] = yearlylandaveragetemp / 12;
            YearlyLandUncertainty[i] = yearlylanduncertaintyaverage / 12;

            YearlyLandMaxTemp[i] = realYearlyLandAvrgTemp[i] + YearlyLandUncertainty[i];
            YearlyLandMinTemp[i] = realYearlyLandAvrgTemp[i] - YearlyLandUncertainty[i];

            if (i >= 1200)
            {
                fprintf(q8, "%d\t%lf\t%lf\t%lf\n", YearInt[i], realYearlyLandAvrgTemp[i], YearlyLandMaxTemp[i], YearlyLandMinTemp[i]);
            }

            yearlylandaveragetemp = 0;
            yearlylanduncertaintyaverage = 0;
            counter = 0;
            // printf("%lf\t%lf\t%lf\n", YearlyLandUncertainty[i], YearlyLandMaxTemp[i], YearlyLandMinTemp[i]);
        }
    }

    // for (int i = 1200; i < ROWS; i += 12)
    // {
    //     fprintf(q8, "%d\t%lf\t%lf\t%lf\n", YearInt[i], YearlyLandAvrgTemp[90 + j], YearlyLandMaxTemp[0], YearlyLandMinTemp[0]); // index at 90 represents the year 1850, adding j moves to the next year's average temp
    //     j++;
    // }

    fclose(q8);
}
void q9()
{
    double q9_Temp[YEARROWS];
    double q9_MaxTemp[YEARROWS];
    double q9_MinTemp[YEARROWS];

    // Assuming YEARROWS represents the total years and ROWS is the correct size of input data
    YearlyAvgCalculator(LandAvrgTemp, q9_Temp, ROWS);
    YearlyAvgCalculator(LandMaxTemp, q9_MaxTemp, ROWS);
    YearlyAvgCalculator(LandMinTemp, q9_MinTemp, ROWS);
    printf("The average temperature for the year 1850 is %lf\n", q9_Temp[0]);
    FILE *q9_LandTemp = fopen("q9_LandTemp.txt", "w");
    FILE *q9_LandMaxTemp = fopen("q9_LandMaxTemp.txt", "w");
    FILE *q9_LandMinTemp = fopen("q9_LandMinTemp.txt", "w");

    // Corrected loop to iterate over YEARROWS and calculate the correct year
    // for (int i = 0; i < YEARROWS; i++)
    // {
    //     int year = 1850 + i;
    //     if (q9_Temp[i] != 0)
    //     { // Only write if there's data
    //         fprintf(q9_LandTemp, "%d\t%lf\n", year, q9_Temp[i]);
    //         fprintf(q9_LandMaxTemp, "%d\t%lf\n", year, q9_MaxTemp[i]);
    //         fprintf(q9_LandMinTemp, "%d\t%lf\n", year, q9_MinTemp[i]);
    //     }
    // }

    fclose(q9_LandTemp);
    fclose(q9_LandMaxTemp);
    fclose(q9_LandMinTemp);
}
int main(void)
{
    assignArrays();
    // q1();
    // q2();
    // q3();
    // q4();
    // q5();
    // q8();
    // YearlyAvgCalculator(LandAvrgTemp, ROWS);
    q9();

    return (0);
}