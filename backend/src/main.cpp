#include <iostream>
#include <sqlite3.h>

using namespace std;

int main()
{
    sqlite3* database;
    int result;

    // Open SQLite database
    result = sqlite3_open("marketplace.db", &database);

    if (result != SQLITE_OK)
    {
        cout << "Database connection failed!" << endl;
        cout << "Error: " << sqlite3_errmsg(database) << endl;

        sqlite3_close(database);

        return 1;
    }

    cout << "====================================" << endl;
    cout << " Multi-Vendor Marketplace Backend" << endl;
    cout << "====================================" << endl;

    cout << "SQLite database connected successfully!" << endl;

    // Close database
    sqlite3_close(database);

    cout << "Database connection closed." << endl;

    return 0;
}
