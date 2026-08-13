#include <iostream>
#include <fstream>
#include <sstream>
#include <sqlite3.h>

using namespace std;


// Execute one SQL command
bool executeSQL(sqlite3* database, const string& sql)
{
    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        database,
        sql.c_str(),
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        cout << "SQL Error: "
             << errorMessage
             << endl;

        sqlite3_free(errorMessage);

        return false;
    }

    return true;
}


int main()
{
    sqlite3* database = nullptr;

    cout << "========================================" << endl;
    cout << " Multi-Vendor E-Commerce Marketplace" << endl;
    cout << " C++ Backend + SQLite" << endl;
    cout << "========================================" << endl;


    // ------------------------------------
    // 1. Open SQLite database
    // ------------------------------------

    int result = sqlite3_open(
        "marketplace.db",
        &database
    );


    if (result != SQLITE_OK)
    {
        cout << "Database connection failed!" << endl;

        cout << "Error: "
             << sqlite3_errmsg(database)
             << endl;

        sqlite3_close(database);

        return 1;
    }


    cout << "SQLite database connected successfully!"
         << endl;


    // ------------------------------------
    // 2. Enable foreign keys
    // ------------------------------------

    if (!executeSQL(
            database,
            "PRAGMA foreign_keys = ON;"
        ))
    {
        sqlite3_close(database);
        return 1;
    }


    // ------------------------------------
    // 3. Create USERS table
    // ------------------------------------

    string usersTable = R"(

        CREATE TABLE IF NOT EXISTS users (

            user_id INTEGER PRIMARY KEY AUTOINCREMENT,

            name TEXT NOT NULL,

            email TEXT NOT NULL UNIQUE,

            password TEXT NOT NULL,

            role TEXT NOT NULL
                CHECK (role IN ('buyer', 'seller', 'admin')),

            created_at DATETIME
                DEFAULT CURRENT_TIMESTAMP
        );

    )";


    // ------------------------------------
    // 4. Create PRODUCTS table
    // ------------------------------------

    string productsTable = R"(

        CREATE TABLE IF NOT EXISTS products (

            product_id INTEGER PRIMARY KEY AUTOINCREMENT,

            seller_id INTEGER NOT NULL,

            name TEXT NOT NULL,

            description TEXT,

            price REAL NOT NULL
                CHECK (price >= 0),

            category TEXT NOT NULL,

            stock INTEGER NOT NULL DEFAULT 0
                CHECK (stock >= 0),

            image_url TEXT,

            created_at DATETIME
                DEFAULT CURRENT_TIMESTAMP,

            updated_at DATETIME
                DEFAULT CURRENT_TIMESTAMP,

            FOREIGN KEY (seller_id)
                REFERENCES users(user_id)
                ON DELETE CASCADE
        );

    )";


    // ------------------------------------
    // 5. Create CART table
    // ------------------------------------

    string cartTable = R"(

        CREATE TABLE IF NOT EXISTS cart (

            cart_id INTEGER PRIMARY KEY AUTOINCREMENT,

            buyer_id INTEGER NOT NULL,

            product_id INTEGER NOT NULL,

            quantity INTEGER NOT NULL DEFAULT 1
                CHECK (quantity > 0),

            FOREIGN KEY (buyer_id)
                REFERENCES users(user_id)
                ON DELETE CASCADE,

            FOREIGN KEY (product_id)
                REFERENCES products(product_id)
                ON DELETE CASCADE,

            UNIQUE (buyer_id, product_id)
        );

    )";


    // ------------------------------------
    // 6. Create ORDERS table
    // ------------------------------------

    string ordersTable = R"(

        CREATE TABLE IF NOT EXISTS orders (

            order_id INTEGER PRIMARY KEY AUTOINCREMENT,

            buyer_id INTEGER NOT NULL,

            customer_name TEXT NOT NULL,

            email TEXT NOT NULL,

            phone TEXT NOT NULL,

            address TEXT NOT NULL,

            city TEXT NOT NULL,

            pincode TEXT NOT NULL,

            payment_method TEXT NOT NULL,

            total_amount REAL NOT NULL
                CHECK (total_amount >= 0),

            status TEXT NOT NULL
                DEFAULT 'Processing',

            created_at DATETIME
                DEFAULT CURRENT_TIMESTAMP,

            FOREIGN KEY (buyer_id)
                REFERENCES users(user_id)
        );

    )";


    // ------------------------------------
    // 7. Create ORDER ITEMS table
    // ------------------------------------

    string orderItemsTable = R"(

        CREATE TABLE IF NOT EXISTS order_items (

            order_item_id INTEGER PRIMARY KEY AUTOINCREMENT,

            order_id INTEGER NOT NULL,

            product_id INTEGER NOT NULL,

            product_name TEXT NOT NULL,

            quantity INTEGER NOT NULL
                CHECK (quantity > 0),

            price REAL NOT NULL
                CHECK (price >= 0),

            FOREIGN KEY (order_id)
                REFERENCES orders(order_id)
                ON DELETE CASCADE,

            FOREIGN KEY (product_id)
                REFERENCES products(product_id)
        );

    )";


    // ------------------------------------
    // 8. Create REVIEWS table
    // ------------------------------------

    string reviewsTable = R"(

        CREATE TABLE IF NOT EXISTS reviews (

            review_id INTEGER PRIMARY KEY AUTOINCREMENT,

            product_id INTEGER NOT NULL,

            buyer_id INTEGER NOT NULL,

            rating INTEGER NOT NULL
                CHECK (rating BETWEEN 1 AND 5),

            comment TEXT,

            created_at DATETIME
                DEFAULT CURRENT_TIMESTAMP,

            FOREIGN KEY (product_id)
                REFERENCES products(product_id)
                ON DELETE CASCADE,

            FOREIGN KEY (buyer_id)
                REFERENCES users(user_id)
                ON DELETE CASCADE,

            UNIQUE (product_id, buyer_id)
        );

    )";


    // ------------------------------------
    // 9. Execute table creation
    // ------------------------------------

    cout << endl;
    cout << "Creating database tables..." << endl;


    if (!executeSQL(database, usersTable))
    {
        sqlite3_close(database);
        return 1;
    }

    cout << "✓ Users table ready" << endl;


    if (!executeSQL(database, productsTable))
    {
        sqlite3_close(database);
        return 1;
    }

    cout << "✓ Products table ready" << endl;


    if (!executeSQL(database, cartTable))
    {
        sqlite3_close(database);
        return 1;
    }

    cout << "✓ Cart table ready" << endl;


    if (!executeSQL(database, ordersTable))
    {
        sqlite3_close(database);
        return 1;
    }

    cout << "✓ Orders table ready" << endl;


    if (!executeSQL(database, orderItemsTable))
    {
        sqlite3_close(database);
        return 1;
    }

    cout << "✓ Order items table ready" << endl;


    if (!executeSQL(database, reviewsTable))
    {
        sqlite3_close(database);
        return 1;
    }

    cout << "✓ Reviews table ready" << endl;


    // ------------------------------------
    // 10. Close database
    // ------------------------------------

    sqlite3_close(database);


    cout << endl;
    cout << "========================================" << endl;
    cout << "Database setup completed successfully!" << endl;
    cout << "marketplace.db is ready." << endl;
    cout << "========================================" << endl;


    return 0;
}
