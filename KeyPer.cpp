#include <iostream>
#include "sqlite3.h"
#include "db_tables.h"
#include "db_insert.h"
#include "sha256_util.h"    // NEW: for SHA-256 hashing of the master password
#include "aes256.h"
#include "master_password.h"
#include "tests.h"
#include <limits>
using namespace std;


std::string masterPlain;  // NEW: holds verified master password during runtime
sqlite3* db = nullptr;

/*
    @brief creates a header for every mode that is entered into
    TODO(Caeden): i kinda wanna have the dashed lines line up perfectly with the text, definitely a polish thing though
*/
// 
void headerFunction(string text){
    std::cout<<"- - - - - - - - - - - - - - - - - -\n";
    std::cout<<"\t"<< text<< "\n";
    std::cout<<"- - - - - - - - - - - - - - - - - -\n";
}

// need a SQL library https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
// need a random library
constexpr int MIN_PASSWORD_LEN = 8;
constexpr int MAX_PASSWORD_LEN = 25;

bool masterPasswordSet() {
    const char* sql = "SELECT PasswordHash FROM MASTER WHERE ID = 1;";
    sqlite3_stmt* stmt = nullptr;
    bool exists = false;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* hashText = sqlite3_column_text(stmt, 0);
            if (hashText) exists = true;
        }
    } else {
        cerr << "[ERROR] Cannot check MASTER table: " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_finalize(stmt);
    return exists;
}



// Prompt user to create a new master password (3–11 chars), confirm it,
// then store its SHA-256 hex in MASTER(ID=1). Keeps plaintext in masterPlain.
void setMasterPassword() {
    string pw1, pw2;
    while (true) {
        headerFunction("New Account Initiated");
        std::cout << "Create a new master password ("
             << MIN_PASSWORD_LEN << "-" << MAX_PASSWORD_LEN << " chars): ";
        std::cin >> pw1;
        if (pw1.size() < MIN_PASSWORD_LEN || pw1.size() > MAX_PASSWORD_LEN) {
            std::cout << "Password length must be between "
                 << MIN_PASSWORD_LEN << " and " << MAX_PASSWORD_LEN << ".\n";
            continue;
        }
        std::cout << "Confirm master password: ";
        std::cin >> pw2;
        if (pw1 != pw2) {
            std::cout << "Passwords do not match. Try again.\n";
            continue;
        }
        else {
            headerFunction("Vault Created");
        }
        break;
    }

    // Compute SHA-256 hex of pw1
    string hashHex = sha256hex(pw1);
    masterPlain = pw1;  // store plaintext for this session

    // Insert or replace the single MASTER row
    const char* sql = "INSERT OR REPLACE INTO MASTER (ID, PasswordHash) VALUES (1, ?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, hashHex.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "[ERROR] Failed to write master hash: " << sqlite3_errmsg(db) << "\n";
        } else {
            // cout << "Master password has been set.\n";
        }
    } else {
        cerr << "[ERROR] Cannot prepare MASTER INSERT: " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_finalize(stmt);
}

// Prompt user for master password, hash it, compare to stored SHA-256.
// If correct, save plaintext in masterPlain and return true; else false.
bool verifyMasterPassword() {
    // headerFunction("Welcome Back");
    string attempt;
    std::cout << "Enter master password (or Q to quit): ";
    std::cin >> attempt;
    if (attempt == "Q" || attempt == "q") {
        headerFunction("Good Bye");
        sqlite3_close(db);
        exit(0);
    }
    if (attempt.size() < MIN_PASSWORD_LEN || attempt.size() > MAX_PASSWORD_LEN) {
        SQL_attemptWriter(false);
        std::cout << "Password length must be between "
             << MIN_PASSWORD_LEN << " and " << MAX_PASSWORD_LEN << ".\n";
        return false;
    }

    // Compute the hash of what the user typed
    string attemptHash = sha256hex(attempt);

    // Fetch the stored hash from MASTER
    const char* sql = "SELECT PasswordHash FROM MASTER WHERE ID = 1;";
    sqlite3_stmt* stmt = nullptr;
    bool match = false;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* storedHash = sqlite3_column_text(stmt, 0);
            if (storedHash && attemptHash == reinterpret_cast<const char*>(storedHash)) {
                match = true;
            }
        }
    } else {
        cerr << "[ERROR] Cannot prepare MASTER SELECT: " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_finalize(stmt);

    if (!match) {
        std::cout << "Incorrect master password.\n";
        SQL_attemptWriter(false);
        return false;
    }
    masterPlain = attempt;  // correct plaintext for this session
    SQL_attemptWriter(true);

    return true;
}













/*
    @brief prints dev notes for future reference
    this is called every time a dev wants other devs to notice something during compilation
*/
void devNote(string text){
    string out= "[ DEVNOTE / TO DO ] - "; 
    out+=text;

    std::cout<<"\t\t\t\t\t\t/ / / / / / / / / / / / / / / / / /\n";
    std::cout<<"\t\t\t\t\t\t"<< out<< "\n";
    std::cout<<"\t\t\t\t\t\t/ / / / / / / / / / / / / / / / / /\n";

}
////////////////////////////////////////////////////////////////////////////////
// SQL stuff https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/




string createNewPassword(bool fromMenu){
    // https://www.w3schools.com/cpp/cpp_howto_random_number.asp

    
    char charArray[]={
        '0','1','2','3','4','5','6','7','8','9',
        '!','@','#','$','%','^','&','*','(',')',
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };

    // Calculate size of charArray manually
    int charArraySize = sizeof(charArray) / sizeof(charArray[0]);

    string genPassword= "";
    int passwordLen;

    while (true){ 
    
        std::cout << "How long should your password be ("
        << MIN_PASSWORD_LEN << "-" << MAX_PASSWORD_LEN << ")? ";
        std::cin >> passwordLen;

        // If Invalid Length
        if (std::cin.fail() ||
            passwordLen < MIN_PASSWORD_LEN ||
            passwordLen > MAX_PASSWORD_LEN) {
            std::cout << "Invalid password length. Must be between "
                << MIN_PASSWORD_LEN << " and " << MAX_PASSWORD_LEN << ".\n";
            std::cin.clear();
            std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // return "";
        }
        else {
            break;
        }
    }
    // Seed RNG
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i<passwordLen; i++){
        // to do -- use a RANDOM library
        int randIndex = rand() % charArraySize;
        genPassword += charArray[randIndex]; // update this with randomChar
    }

                                                                    // headerFunction(genPassword); 
                                                                    // // cout << "\nDo you want to copy to clipboard? [ Y ] / [ N ]: ";
                                                                    // string option;
                                                                    // cin >> option;

                                                                    // while (option != "Y" && option != "y" && option != "N" && option != "n" ){
                                                                    //     cout << "Please enter [ Y ] or [ N ]: ";
                                                                    //     cin >> option;
                                                                    //         // use copy to clipboard libary 
                                                                    // }
                                                                    // if (option == "Y" || option == "y") {
                                                                    //     devNote("Clipboard functionality not implemented yet");
                                                                    // }
                                                                    //     // please unit test

    return genPassword;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    @brief
    
    @param string website 
    @param string username 
    @param string password

    @return 
*/
void validateInsertVault(string website, string username, string password){
    
    // please write the loop menu using while true
    
    while (true){

        devNote("user input validation loop"); 

        headerFunction("Confirm Login Info");

            std::cout << "\nWebsite:  " << website;   // [ 2 ]
            std::cout << "\nUsername: " << username;  // [ 1 ]
            std::cout << "\nPassword: " << password;  // [ 3 ]
            std::cout << "\n";

        std::cout<<"\nIs this valid? [ Y ] / [ N ]: ";

        char valid;
        std::cin>>valid;
        if (valid == 'y' || valid == 'Y'){
            SQL_vaultWriter(website, username, password);
            return;
        } else if (valid == 'n' || valid == 'N'){
            // Create a menu here to ask the user which of the three they want to update

            
            while (true){

                headerFunction("Update");

                std::cout << "What text is invalid? [ 1 ], [ 2 ], or [ 3 ]";
                std::cout << "\n[ 1 ] - "<< username;
                std::cout << "\n[ 2 ] - "<< website;
                std::cout << "\n[ 3 ] - "<< password << "\n";
                std::cout << "Enter 1, 2, or 3: ";

                std::cin>> valid;
                if (valid == '1'){
                    // display the current username, and let them overwrite it
                    cout << "Current Username: " << username << "\n";
                    cout << "New Username: ";
                    std::cin >> username;
                    break;
                } else if (valid == '2'){
                    // display the current website, and let them overwrite it
                    cout << "Current Website: " << website << "\n";
                    cout << "New Website: ";
                    std::cin >> website;
                    break;
                } else if (valid == '3'){
                    std::cout << "Current Password: " << password << "\n";
                    password = createNewPassword(false);
                    break;
                } else {
                    // repeat loop 
                    cout << "Invalid option. \n";
                    
                }
            }

            
        } else {

            cout<<"Invalid response. Please enter [ Y ] or [ N ]: \n";
            
        }
    }
    
    
}



string removeWhiteSpace(string in){
    // string out;
    // for (int i = 0; i++; i<in.size()){
    //     if (in[i] != ' '){
    //         out += in[i];
    //     }
    // }
    // return out;
    return in;
}

/*
    @brief primary function for creating a new login
        1) insert website name
        2) insert profle name / email
        3) insert createNewPassword()
        4) store to password vault through SQL_vaultWriter()
*/
void createNewLogin(){ 

    // while menu
    while(true){
        headerFunction("Create New Login");
        string website = "";
        std::cout << "Enter website name: ";
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer
        getline(std::cin,website);
        // website = removeWhiteSpace(website);

        // devNote("erase white space from website");

        string username = "";
        std::cout << "Enter user name: ";
        getline(std::cin, username);
        
        // username = removeWhiteSpace(username);

        // devNote("erase white space from website");


        string password = createNewPassword(false);



        validateInsertVault(website, username, password);
        break;

    }


}

void showSearchSQLMenu() {

    int choice = 0;

    do {
        cout << "[ 1 ] Search \n";
        cout << "[ 0 ] Return to Main Menu\n";
        cout << "Enter your choice: ";

        std::cin >> choice;

        if (choice == 1) {
            headerFunction("Search Vault");
            string searchKey;
            cout << "Search Term: ";  
            std::cin.ignore();  // Clear any leftover characters in the buffer
            getline(std::cin,searchKey); // Newline was still sitting in the input buffer, getline immediately sees it and thinks it's the end of input.
            SQL_vaultSearch(searchKey);
                do {
                    // cout << "[ 1 ] Update\n";
                    cout << "[ 1 ] Delete\n";
                    cout << "[ 0 ] Return to Main Menu\n";
                    cout << "Enter your choice: ";

                    std::cin >> choice;

                    // if (choice == 1) {
                    //     // headerFunction("Update Vault");
                    //     devNote("Update entry selected. Implement update SQL logic here.\n");
                    //     SQL_vaultSearchDelete(searchKey);
                    //     return;
                    // } 
                    
                    if (choice == 1) {
                        do {
                            cout << "This will permanently erase all of the searched credentials.\n";
                            cout << "Type [ Y ] to confirm or [ N ] to go back to the menu: ";

                            string input;
                            std::cin >> input;

                            // Convert input to lowercase
                            for (char &c : input) c = tolower(c);

                            if (input == "y") {
                                // devNote("Incomplete!");
                                SQL_vaultSearchDelete(searchKey);
                                return;
                            } else if (input == "n") {
                                return;
                            } else {
                                cout << "Invalid input.\n";
                            }
                        } while(true);

                        // devNote("Delete entry selected. Implement delete SQL logic here.\n");
                    } 
                    else if (choice == 0) {
                        cout << "Returning to Main Menu\n";
                        return; // Exit to Main Menu
                    } else {
                        cout <<  "Invalid choice.\n";
                        continue;
                    }
                } while(true);
            if (!SQL_vaultSearch(searchKey)){
                cout << "No entries found.\n";
            }

        } else if (choice == 0) {
            cout << "Returning to Main Menu\n";
            return; // Exit to Main Menu
        } else {
            cout <<  "Invalid choice.\n";
            continue;
        }
    } while (true);
}

/*
    @brief primary function for opening the password vault
        1) ask user to confirm their master password
        2) if true, open vault -- if false, re call readPasswordVault -- Q to quit
        3) 
        4) 
    @return whether login accepted
*/

/*
    @brief warns and prompts the user to confirm a factory reset before deleting DB data.

    @returns true if vault was reset, false otherwise
*/
bool resetVault() {
    headerFunction("WARNING: Factory Reset Vault");



    // This while loop is broken by the two "reset" and "q" conditionals inside.
    // It will only loop continuously for as long as the user continues to input invalid responses
    do {

        cout << "This will permanently erase your password vault and all stored data.\n";
        cout << "Type [ RESET ] to confirm or [ Q ] to go back to the menu: ";

        string input;
        cin >> input;

        // Convert input to lowercase
        for (char &c : input) c = tolower(c);

        if (input == "reset") {
            deleteData();
            return true;
        } else if (input == "q") {
            return false;
        } else {
            cout << "Invalid input.\n";
        }
    } while (true);

    // devNote("Reset master password as well");
    setMasterPassword();

}

void signOut(){
    // Re-authenticate by asking for master password again
    while (true) {
        // keep looping until correct or user quits inside verifyMasterPassword()
        if (verifyMasterPassword()){
            break;
        }
    }
}


/*
    @brief function loops through all vault functions
    
    @param bool has previously correctly entered master password

*/
void menu() {
    int option = 0;
    while (true) {
        headerFunction("Main Menu");
        cout << "[ 1 ] Create New Login\n"
             << "[ 2 ] Open Vault\n"
             << "[ 3 ] View Access Log\n"
             << "[ 9 ] Factory Reset Vault\n"
             << "[ 0 ] Lock Vault\n\n"
             << "Enter choice: ";
        cin >> option;

        if (option == 1) {
            createNewLogin();
        }
        else if (option == 2) {
            headerFunction("Vault");
            SQL_vaultReader();
            showSearchSQLMenu();
        }
        else if (option == 3) {
            headerFunction("Access Log");
            SQL_attemptReader();
        }
        else if (option == 9) {
            if (resetVault()) {
                headerFunction("Vault Wiped");
                // After wiping, require user to set a new master and then verify it
                setMasterPassword();
                while (!verifyMasterPassword()) {
                    // keep looping until valid
                }
            }
        }
        else if (option == 0) {
            signOut();
        } else {
            headerFunction("Response rejected.");
        }

        // PAUSE so you can read Vault/Log before the menu reprints
        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    main menu caller
*/
int main() {
    // Run tests if KEYPER_TESTS environment variable is set
    if (getenv("KEYPER_TESTS")) {
        return run_tests();
    }

    // Open (or create) the SQLite database
    if (sqlite3_open("db_data.db", &db) != SQLITE_OK) {
        cerr << "DEVNOTE - Database inaccessible! \n";
        return 1;
    }

    // Ensure CREDENTIAL, ACCESS_LOG, and MASTER tables exist
    create_tables();

    // If no master‐password hash exists yet, prompt the user to create one
    if (!masterPasswordSet()) {
        setMasterPassword();
    }

    // Prompt and verify until the user enters the correct master password (or quits)
    while (!verifyMasterPassword()) {
        // SQL_attemptWriter(false);
        // keep looping until correct
    }
    // Record a successful vault login in the ACCESS_LOG table
    // SQL_attemptWriter(true);

    // Enter the main menu loop
    menu();

    sqlite3_close(db);
    return 0;
}

