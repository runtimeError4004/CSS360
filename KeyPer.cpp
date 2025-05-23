#include <iostream>
#include "db_tables.cpp"
#include "db_insert.cpp"
#include "sqlite3.c"
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
    @brief
    
    @param string website 
    @param string username 
    @param string password

    @return 
*/
void validateInsertVault(string website, string username, string password){
    char valid;
    
    headerFunction("Confirm Login Info");
    cout<<"Username: ", username;       // [ 1 ]
    cout<<"Website:  ", website;        // [ 2 ]
    cout<<"Password: ", password;       // [ 3 ]
    cout<<"\n Is this valid? [ Y ] / [ N ]";
    cin>>valid;
    if (valid == 'y' || valid == 'Y'){
        SQL_vaultWriter(website, username, password);
    } else if (valid == 'n' || valid == 'N'){
        // Create a menu here to ask the user which of the three they want to update
        cout << "update [ 1 ],[ 2 ],[ 3 ]";
        cout << "[ 1 ]  \n";
        cout << "[ 2 ]  \n";
        cout << "[ 3 ]  \n";

        cin>> valid;
        if ('1'){
            //website 

        } else if ('2'){
            //username 

        } else if ('2'){
            //password 

        } else {
            // what happens if they enter something that isnt 1,2,3

        }
    } else {
        cout<<"Invalid response\n";
        validateInsertVault(website, username, password);
    }
}

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
    out = "";
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
string createNewPassword(){
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

    cout<<"How long should your password be?";
    cin>>passwordLen;

    // If Invalid Length
    if(cin.fail() || passwordLen <= 0 || passwordLen > 128) {
        cout << "Invalid password length. Aborting.\n";
        cin.clear(); // Clear fail state of cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return "";
    }

    // Seed RNG
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i<passwordLen; i++){
        // to do -- use a RANDOM library
        int randIndex = rand() % charArraySize;
        genPassword += charArray[randIndex]; // update this with randomChar
    }

    cout << "Generated password: " << genPassword << "\nDo you want to copy to clipboard? [ Y ] / [ N ]: ";
    string option;
    cin >> option;

    while (option != "Y" && option != "y" && option != "N" && option != "n" ){
        cout << "Please enter Y or N: ";
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
    @brief primary function for creating a new login
        1) insert website name
        2) insert profle name / email
        3) insert createNewPassword()
        4) store to password vault through SQL_vaultWriter()
*/
void createNewLogin(){
    headerFunction("Create New Login");

    string website = "";
    cout << "Enter website name";
    cin >> website;

    string username = "";
    cout << "Enter user name";
    cin >> username;

    string password = createNewPassword();

    // Check
    if(password.empty()) {
        cout << "Password generation failed. Aborting. \n";
        return;
    }

    string encrypted_pass = aes256(1, password);

    validateInsertVault(website, username, encrypted_pass); // Insert encrypted password
}

/*
    @brief primary function for opening the password vault
        1) ask user to confirm their master password
        2) if true, open vault -- if false, re call readPasswordVault -- Q to quit
        3) 
        4) 
*/
bool loginVault(){
    headerFunction("KeyPer - Password Manager");
    string enteredMasterPassword = "";
    cout<< "Enter master password or quit [ Q ]:\n";
    cout<< "[default password is '000']:\n";
    cin >> enteredMasterPassword;
    if(enteredMasterPassword == masterPassword){
        cout<< "\nAccess Granted.\n\n";
        SQL_attemptWriter(true);
        return true;
    } else if (enteredMasterPassword == "q" || enteredMasterPassword == "Q") {
        // todo -- quit program
        SQL_attemptWriter(false);
    } else {
        cout<< "Answer rejected.\n";
        SQL_attemptWriter(false);
        return false;
    }
}

/*
    @brief function loops through all vault functions
    
    @param bool has previously correctly entered master password

*/
void vault(){
    int option = 0;

    headerFunction("Main Menu");

    cout<<"Select from options below:\n\n";
    cout<<"[ 1 ] Create New Login\n";
    cout<<"[ 2 ] Create New Password\n";
    cout<<"[ 3 ] Unlock Vault\n";
    cout<<"[ 4 ] View Access Log\n";
    cout<<"[ 5 ] Factory Reset Vault\n";

    cout<<"\n";

    cin>>option;

    if (option == 1){
        createNewLogin();
        vault();
    } else if (option == 2){
        createNewPassword();
        vault();
    } else if (option == 3){
        SQL_vaultReader();
        vault();
    } else if (option == 4){
        SQL_attemptReader();
        vault();
    } else if (option == 5){
        // no function yet
        vault();
    } else {
        cout<<"\nResponse rejected.\n\n";
        vault();
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    main menu caller
*/
int main() {
    while (!loginVault()){
        loginVault();
    }
    vault();
    return 0;
};

