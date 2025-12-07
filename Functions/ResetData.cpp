#include <fstream>
#include <iostream>
#include "Servers/ServerDefinition.hpp"

void resetFiles() 
{
    {
        std::ofstream statusFile(STATUS_FILE_PATH, std::ios::trunc);
        if (!statusFile) 
        {
            std::cerr << "Failed to open " << STATUS_FILE_PATH << "\n";
        } 
        else 
        {
            statusFile << "1\n";
        }
    }

    {
        std::ofstream accountFile(ACCOUNT_FILE_PATH, std::ios::trunc);
        if (!accountFile) 
        {
            std::cerr << "Failed to open " << ACCOUNT_FILE_PATH << "\n";
        }
    }
}

int main() 
{
    resetFiles();
    std::cout << "Files reset successfully.\n";
    return 0;
}
