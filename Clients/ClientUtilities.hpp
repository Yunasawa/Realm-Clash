#ifndef CLIENT_UTILITIES 
#define CLIENT_UTILITIES 

void WriteLineAt(int row, const string& text)
{
    cout << "\033[s";                 // Save cursor
    cout << "\033[" << row << ";1H";  // Move to row, col 1
    cout << "\033[2K";                // Clear whole line
    cout << text;                     // Write new content
    cout << "\033[u";                 // Restore cursor
    cout.flush();
}

#endif
