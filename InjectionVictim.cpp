// Жертва dll-инъекции
#include <iostream>
#include <windows.h>
#include <cstring>

#define STRING_SIZE 1024

int main()
{
    const char* leftId = "Left";
    const char* rightId = "Right";
    int number_of_voters = 0;;
    int leftVotes = 0;
    int rightVotes = 0;
    char* right = (char*)calloc(STRING_SIZE, sizeof(char));
    char* left = (char*)calloc(STRING_SIZE, sizeof(char));
    strcpy_s(right, STRING_SIZE, leftId);
    strcpy_s(left, STRING_SIZE, rightId);
   
    std::wcout << "Choose your path!" << std::endl;

    while (true)
    {
        int choice = 0;
        bool isVoted = false;
        std::wcout << "It's time to choose:\n";
        std::wcout << "1." << leftId << std::endl;
        std::wcout << "2." << rightId << std::endl;
        std::wcout << "0." << "Finish" << std::endl;
        std::wcin >> choice;
        if (choice == 1)
        {
            rightVotes++;
            isVoted = true;
        }
        if (choice == 2)
        {
            leftVotes++;
            isVoted = true;
        }
        if (choice == 0)
        {
            std::wcout << "It's to announce results\n";
            break;
        }
        if (isVoted)
            std::wcout << "Thanks for choosing!\n";
    }

    std::wcout << "And the path is...";
    Sleep(2000);
    if (leftVotes > rightVotes)
        std::wcout << left;
    else if (leftVotes < rightVotes)
        std::wcout << right;
    else
        std::wcout << "Noone! It's draw";
    std::wcout << "!!!" << std::endl;
    Sleep(500);
    std::wcout << leftId << " - " << leftVotes << " votes\n";
    Sleep(500);
    std::wcout << rightId << " - " << rightVotes << " votes\n";

    int i;
    std::wcin >> i;
}
