#include <fstream>
#include <iostream>
#include "../Servers/ServerDefinition.hpp"

void ResetFiles() 
{
    // Status.ynl
    {
        std::ofstream statusFile(STATUS_PATH, std::ios::trunc);
        if (!statusFile) 
        {
            std::cerr << "Failed to open " << STATUS_PATH << "\n";
        } 
        else 
        {
            statusFile << "1\n";
        }
    }

    // Account.ynl
    {
        std::ofstream accountFile(ACCOUNT_PATH, std::ios::trunc);
        if (!accountFile) 
        {
            std::cerr << "Failed to open " << ACCOUNT_PATH << "\n";
        }
    }

    {
        std::ofstream logFile(LOG_PATH, std::ios::trunc);
        if (!logFile)
        {
            std::cerr << "Failed to open " << LOG_PATH << "\n";
        }
    }
}

int main() 
{
    ResetFiles();
    std::cout << "Files reset successfully.\n";
    return 0;
}
