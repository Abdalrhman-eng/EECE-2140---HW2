/*
 * Name: Abdalrhman Sheer
 * Student ID: 002576282
 * Course: EECE 2140, Section 07, Fall 2026
 * Assignment: Homework 2 - Calendar Toolkit
 */

#include <iostream>
#include <cstdlib>
#include <cassert>

enum class Weekday { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

const int MIN_YEAR = 1900;   // earliest year the toolkit supports
const int MAX_YEAR = 2100;   // latest year the toolkit supports

bool isLeapYear(int year);
int daysInMonth(int month, int year);
bool isValidDate(int day, int month, int year);
int daysSince1900(int day, int month, int year);
Weekday dayOfWeek(int day, int month, int year);
void printWeekdayName(Weekday weekday, bool abbreviated = false);
void printDate(int day, int month, int year);
int daysBetween(int day1, int month1, int year1,
                int day2, int month2, int year2);
int randomInRange(int low, int high);
void randomDate(int& day, int& month, int& year, int minYear, int maxYear);
void printTwoDigits(int value);

int main()
{
    int choice = 0;

    std::cout << "=== EECE 2140 Calendar Toolkit ===" << std::endl;

    do
    {
        std::cout << std::endl
                  << "1) Day of the week for a date" << std::endl
                  << "2) Days between two dates" << std::endl
                  << "3) Weekday quiz" << std::endl
                  << "4) Exit" << std::endl
                  << "Enter your choice: ";

        if (!(std::cin >> choice))
        {
            choice = 4;   // end of input (or unreadable input) acts as Exit
        }

        switch (choice)
        {
            case 1:
            {
                int day = 0, month = 0, year = 0;
                std::cout << "Enter a date (day month year): ";
                std::cin >> day >> month >> year;
                if (!isValidDate(day, month, year))
                {
                    std::cout << "Invalid date." << std::endl;
                    break;
                }
                printDate(day, month, year);
                std::cout << " is a ";
                printWeekdayName(dayOfWeek(day, month, year));
                std::cout << "." << std::endl;
                break;
            }
            case 2:
            {
                int day1 = 0, month1 = 0, year1 = 0;
                int day2 = 0, month2 = 0, year2 = 0;
                std::cout << "Enter the first date (day month year): ";
                std::cin >> day1 >> month1 >> year1;
                std::cout << "Enter the second date (day month year): ";
                std::cin >> day2 >> month2 >> year2;
                if (!isValidDate(day1, month1, year1) || !isValidDate(day2, month2, year2))
                {
                    std::cout << "Invalid date." << std::endl;
                    break;
                }

                std::cout << "From ";
                printWeekdayName(dayOfWeek(day1, month1, year1), true);
                std::cout << " ";
                printDate(day1, month1, year1);
                std::cout << " to ";
                printWeekdayName(dayOfWeek(day2, month2, year2), true);
                std::cout << " ";
                printDate(day2, month2, year2);
                int difference = daysBetween(day1, month1, year1, day2, month2, year2);
                std::cout << ": " << difference
                          << ((difference == 1 || difference == -1) ? " day" : " days") << std::endl;
                break;
            }
            case 3:
            {
                unsigned int seed = 0;
                int numQuestions = 0, correct = 0;
                std::cout << "Random seed: ";
                std::cin >> seed;
                std::cout << "Number of questions: ";
                std::cin >> numQuestions;
                if (numQuestions < 1)
                {
                    std::cout << "Number of questions must be at least 1." << std::endl;
                    break;
                }

                std::srand(seed);   // seed ONCE, before any random numbers are drawn
                for (int question = 1; question <= numQuestions; ++question)
                {
                    int day = 0, month = 0, year = 0, guess = -1;
                    randomDate(day, month, year, MIN_YEAR, MAX_YEAR);

                    std::cout << "Question " << question << ": what day of the week was ";
                    printDate(day, month, year);
                    std::cout << "? (0=Sun 1=Mon 2=Tue 3=Wed 4=Thu 5=Fri 6=Sat): ";
                    std::cin >> guess;

                    Weekday answer = dayOfWeek(day, month, year);
                    if (guess == static_cast<int>(answer))
                    {
                        ++correct;
                        std::cout << "Correct!" << std::endl;
                    }
                    else
                    {
                        std::cout << "Not quite -- it was a ";
                        printWeekdayName(answer);
                        std::cout << "." << std::endl;
                    }
                }
                std::cout << "Quiz score: " << correct << " of " << numQuestions
                          << "." << std::endl;
                break;
            }
            case 4:
                std::cout << "Goodbye." << std::endl;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    } while (choice != 4);

    return 0;
}

bool isLeapYear(int year)
{
    // divisible by 400, or divisible by 4 but not by 100
    return (year % 400 == 0) ||
           (year % 4 == 0 && year % 100 != 0);
}

int daysInMonth(int month, int year)
{
    assert(month >= 1 && month <= 12);

    switch (month)
    {
        case 2:
            if (isLeapYear(year))
                return 29;
            return 28;

        case 4:
        case 6:
        case 9:
        case 11:
            return 30;

        default:
            return 31;   // Jan, Mar, May, Jul, Aug, Oct, Dec
    }
}

bool isValidDate(int day, int month, int year)
{
    if (year < MIN_YEAR || year > MAX_YEAR)
        return false;

    // check the month before calling daysInMonth so its assert can't fail
    if (month < 1 || month > 12)
        return false;

    return day >= 1 && day <= daysInMonth(month, year);
}

int daysSince1900(int day, int month, int year)
{
    assert(isValidDate(day, month, year));

    int total = 0;

    // add every full year before this one
    for (int y = MIN_YEAR; y < year; ++y)
    {
        if (isLeapYear(y))
            total += 366;
        else
            total += 365;
    }

    // add every full month before this one in the current year
    for (int m = 1; m < month; ++m)
    {
        total += daysInMonth(m, year);
    }

    // day - 1 because 1 Jan 1900 itself counts as 0
    total += day - 1;

    return total;
}

Weekday dayOfWeek(int day, int month, int year)
{
    assert(isValidDate(day, month, year));

    int days = daysSince1900(day, month, year);

    // 1 Jan 1900 was a Monday, so count forward from Monday and wrap every 7 days
    return static_cast<Weekday>((static_cast<int>(Weekday::Monday) + days) % 7);
}

void printWeekdayName(Weekday weekday, bool abbreviated)
{
    switch (weekday)
    {
        case Weekday::Sunday:
            std::cout << (abbreviated ? "Sun" : "Sunday");
            break;
        case Weekday::Monday:
            std::cout << (abbreviated ? "Mon" : "Monday");
            break;
        case Weekday::Tuesday:
            std::cout << (abbreviated ? "Tue" : "Tuesday");
            break;
        case Weekday::Wednesday:
            std::cout << (abbreviated ? "Wed" : "Wednesday");
            break;
        case Weekday::Thursday:
            std::cout << (abbreviated ? "Thu" : "Thursday");
            break;
        case Weekday::Friday:
            std::cout << (abbreviated ? "Fri" : "Friday");
            break;
        case Weekday::Saturday:
            std::cout << (abbreviated ? "Sat" : "Saturday");
            break;
    }
}

// Prints a number from 0 to 99 as two digits, adding a leading zero if needed.
void printTwoDigits(int value)
{
    if (value < 10)
        std::cout << "0";
    std::cout << value;
}

void printDate(int day, int month, int year)
{
    // years are always 4 digits (1900-2100), so only month and day need padding
    std::cout << year << "-";
    printTwoDigits(month);
    std::cout << "-";
    printTwoDigits(day);
}

int daysBetween(int day1, int month1, int year1,
                int day2, int month2, int year2)
{
    // positive if the second date is later, negative if earlier
    return daysSince1900(day2, month2, year2) - daysSince1900(day1, month1, year1);
}

int randomInRange(int low, int high)
{
    // rand() % size gives 0 to (high - low), then shift it up to start at low
    return low + std::rand() % (high - low + 1);
}

void randomDate(int& day, int& month, int& year, int minYear, int maxYear)
{
    // draw order must be year, month, day (Random Date Protocol)
    year = randomInRange(minYear, maxYear);
    month = randomInRange(1, 12);
    day = randomInRange(1, daysInMonth(month, year));
}
