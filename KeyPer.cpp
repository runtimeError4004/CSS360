#include <iostream>
#include "db_tables.cpp"
#include "db_insert.cpp"
// #include "sqlite3.c"
#include <limits>

using namespace std;

// need an encryption library
// need a SQL library https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
// need a random library


// whats the best way to store the masterPassword? SQL DB or plain text in this cpp file as I have written?
string masterPassword = "000";

/*
    @brief creates a header for every mode that is entered into
*/
// TODO(Caeden): i kinda wanna have the dashed lines line up perfectly with the text, definitely a polish thing though
void headerFunction(string text){
    cout<<"- - - - - - - - - - - - - - - - - -\n";
    cout<<"\t"<< text<< "\n";
    cout<<"- - - - - - - - - - - - - - - - - -\n";
}

////////////////////////////////////////////////////////////////////////////////
// SQL stuff https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/

/*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
    @brief Encrypts/decrypts the input data using AES-256.
    
    @param bool for whether the text should be encrypted (true) or decrypted (false).
    @param string The text input to encrypt or decrypt.

    @return the string of encrypted or decrypted text.
*/
string aes256(bool encrypt, string password){
    string out;
    if (encrypt){
        // use encrypt library
        // encrypt the password
        
    } else if (!encrypt) {
        // reverse encrypt / decrypt the password

    }
    // update the out string to be the new password here
    out = password;
    return out;
}

/*
    @brief support function for creating a new login
        1) ask user for password length
        2) pull random characters from charArray
        3) 
        4) 

    @return the randomly generated password

*/
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

    // while (){ please add while conditions to 
        cout<<"How long should your password be? ";
        cin>>passwordLen;

        // If Invalid Length
        if(cin.fail() || passwordLen <= 0 || passwordLen > 128) {
            cout << "Invalid password length. \n";
            cin.clear(); // Clear fail state of cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return "";
        }

        // if (){
        
        // }

    //}


    // Seed RNG
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i<passwordLen; i++){
        // to do -- use a RANDOM library
        int randIndex = rand() % charArraySize;
        genPassword += charArray[randIndex]; // update this with randomChar
    }

    headerFunction('"'+ genPassword + '"'); 
    cout << "\nDo you want to copy to clipboard? [ Y ] / [ N ]: ";
    string option;
    cin >> option;

    while (option != "Y" && option != "y" && option != "N" && option != "n" ){
        cout << "Please enter [ Y ] or [ N ]: ";
        cin >> option;
            // use copy to clipboard libary 
    }
    if (option == "Y" || option == "y") {
        cout << "[Clipboard functionality not implemented yet]\n";
    }
        // please unit test

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

        // 

        headerFunction("Confirm Login Info");
            cout << "\nUsername: " << username;  // [ 1 ]
            cout << "\nWebsite:  " << website;   // [ 2 ]
            cout << "\nPassword: " << password;  // [ 3 ]

        cout<<"\nIs this valid? [ Y ] / [ N ]: ";

        char valid;
        cin>>valid;
        if (valid == 'y' || valid == 'Y'){
            SQL_vaultWriter(website, username, password);
        } else if (valid == 'n' || valid == 'N'){
            // Create a menu here to ask the user which of the three they want to update

            
            while (true){

                // 

                cout << "What text is invalid? [ 1 ], [ 2 ], or [ 3 ]";
                cout << "\n[ 1 ] - "<< website;
                cout << "\n[ 2 ] - "<< username;
                cout << "\n[ 3 ] - "<< password;

                cin>> valid;
                if ('1'){
                    // display the current website, and let them overwrite it
                    cout << "";
                    cin >> website;
                    break;
                } else if ('2'){
                    // display the current un, and let them overwrite it
                    cout << "";
                    cin >> website;
                    break;
                } else if ('3'){
                    createNewPassword(false);
                    break;
                } else {
                    // repeat loop 
                    
                }
            }

            

            validateInsertVault(website, username, password);
        } else {
            cout<<"Invalid response\n";
            validateInsertVault(website, username, password);
        }
    }
    
    
}
/*
    @brief primary function for creating a new login
        1) insert website name
        2) insert profle name / email
        3) insert createNewPassword()
        4) store to password vault through SQL_vaultWriter()
*/
void createNewLogin(){
    headerFunction("Create New Login");

    // while menu
    // while(true){
        string website = "";
        cout << "Enter website name: ";
        cin >> website;
        cout << website<<"\n";


        string username = "";
        cout << "Enter user name: ";
        cin >> username;
        cout << username<<"\n";


        string password = createNewPassword(false);

        // Check
        if(password.empty()) {
            cout << "Password generation failed. \n";
            return;
        }

        cout << password<<"\n";


        string encrypted_pass = aes256(1, password);

        // print 

        validateInsertVault(website, username, encrypted_pass); // Insert encrypted password
        while (true){
            
            break;
        }
    // }


}

/*
    @brief primary function for opening the password vault
        1) ask user to confirm their master password
        2) if true, open vault -- if false, re call readPasswordVault -- Q to quit
        3) 
        4) 
    @return whether login accepted
*/
bool loginVault(){
    headerFunction("KeyPer - Password Manager");
    string enteredMasterPassword = "";
    cout<< "Enter master password or quit [ Q ]:\n";
    cout<< "[default password is '000']:\n\n";
    cin >> enteredMasterPassword;
    if(enteredMasterPassword == masterPassword){
        cout<< "\nAccess Granted.\n";
        SQL_attemptWriter(true);
        return true;
    } else if (enteredMasterPassword == "q" || enteredMasterPassword == "Q") {
        // todo -- quit program
        SQL_attemptWriter(false);
        exit(0);
    } else {
        headerFunction("Response rejected.");
        SQL_attemptWriter(false);
        return false;
    }
    return false;
}

/*
    @brief warns and prompts the user to confirm a factory reset before deleting DB data.

    @returns true if vault was reset, false otherwise
*/
bool resetVault() {
    headerFunction("WARNING: Factory Reset Vault");
    cout << "This will permanently erase your password vault and all stored data.\n";
    cout << "Type 'reset' to confirm or 'Q' to go back to the menu: ";

    string input;
    cin >> input;

    // Convert input to lowercase
    for (char &c : input) c = tolower(c);

    if (input == "reset") {
        deleteData();
        cout << "Vault has been reset.\n";
        return true;
    } else if (input == "q") {
        return false;
    } else {
        // Loop on invalid option
        cout << "Invalid input.\n";
        return resetVault();
    }
}

void signOut(){
    while (!loginVault()){
        loginVault();
    }
}


/*
    @brief function loops through all vault functions
    
    @param bool has previously correctly entered master password

*/
void menu(){
    int option = 0;

    while (true){
        headerFunction("Main Menu");

        cout<<"Select from options below:\n\n";
        cout<<"[ 1 ] Create New Login\n";
        cout<<"[ 2 ] Create New Password\n";
        cout<<"[ 3 ] Open Vault\n";
        cout<<"[ 4 ] View Access Log\n\n";

        cout<<"[ 9 ] Factory Reset Vault\n";
        cout<<"[ 0 ] Lock Vault\n";


        cout<<"\n";

        cin>>option;

        if (option == 1){
            createNewLogin();
            menu();
        } else if (option == 2){
            createNewPassword(true);
            menu();
        } else if (option == 3){
            headerFunction("Vault");
            SQL_vaultReader();
            menu();
        } else if (option == 4){
            headerFunction("Access Log");
            SQL_attemptReader();
            menu();
        } else if (option == 9){
            // Return to login vault loop if vault was reset; return to menu loop if canceled
            if (resetVault()) {
                main();
            }else{
                menu();
            }
        } else if (option == 0){
            signOut();
        } else {
            headerFunction("Response rejected.");
            menu();
        }
    }
        

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    main menu caller
*/
int main() {
    create_tables();

    while (!loginVault()){
        loginVault();
    }
    
    menu();
    return 0;
};

