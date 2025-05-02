#include <iostream>
using namespace std;

string masterPassword = "0000";

/*
    support function
*/
void headerFunction(string text){
    cout<<"- - - - - - - - - - - - - - - - - -\n";
    cout<<"\t"<< text<< "\n";
    cout<<"- - - - - - - - - - - - - - - - - -\n";
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * @brief Encrypts the input data using AES-256.
 *
 * @param data The plaintext input to encrypt.
 * @param key A 256-bit key used for encryption.
 * @return A base64-encoded encrypted string.
*/
void validateInsertVault(string website, string username, string password){
    char valid;
    
    headerFunction("Confirm Login Info");
    cout<<"Username: ", username;
    cout<<"Website:  ", website;
    cout<<"Password: ", password;
    cout<<"\n Is this valid? [ Y ] / [ N ]";
    cin>>valid;
    if (valid == 'y' || valid == 'Y'){
        // DO SQL INSERT HERE
    } else if (valid == 'n' || valid == 'N'){
        
    } else {
        cout<<"Invalid response\n";
        cout<<"Invalid response\n";

    }
}



/*
    Encrypts/decrypts the input data using AES-256.
    
    @param bool for whether the text should be encrypted (true) or decrypted (false).
    @param string The text input to encrypt or decrypt.

    @return the string of encrypted or decrypted text.
*/
string aes256(bool encrypt, string password){
    string out;
    if (encrypt){

    } else {

    }
    return out;
}

/*
    support function for creating a new login
    1) ask user for password length
    2) pull random characters from charArray
    3) 
    4) 
*/
string createNewPassword(){
    // https://www.w3schools.com/cpp/cpp_howto_random_number.asp
    char charArray[]={
        '0','1','2','3','4','5','6','7','8','9',
        '!','@','#','$','%','^','&','*','(',')',
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };
    string genPassword= "";
    int passwordLen;
    cout<<"How long should your password be?";
    cin>>passwordLen;

    for (int i = 0; i<passwordLen; i++){
        // to do -- use a RANDOM library
        genPassword += i; // update this with randomChar
    }

    return genPassword;
}
////////////////////////////////////////////////////////////////////////////////
// SQL stuff https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/

/*

SQL Data template

LOGIN
-----
+ int pwID
+ string website
+ string username
+ string password

*/

/*
    support function for creating a new login
    1) 
    2) 
    3) 
    4) 
*/
void SQLVaultWriter(string website, string username, string password){

}
/*
    support function for creating a new login
    1) 
    2) 
    3) 
    4) 
*/
void SQLVaultReader(){

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    primary function for creating a new login
    1) insert website name
    2) insert profle name / email
    3) insert createNewPassword()
    4) store to password vault through sqlVaultWriter()
*/
void createNewLogin(){
    headerFunction("Create New Login");
    string website = "";

    string username = "";

    string password = createNewPassword();

    validateInsertVault(website, username, password);
}



/*
    primary function for opening the password vault
    1) ask user to confirm their master password
    2) if true, open vault -- if false, re call readPasswordVault -- Q to quit
    3) 
    4) 
*/
void readPasswordVault(){
    headerFunction("Password Vault");
    string enteredMasterPassword = "";
    cout<< "Enter master password or quit [ Q ]:\n";
    cin >> enteredMasterPassword;
    if(enteredMasterPassword == masterPassword){
        SQLVaultReader();
    } else if (enteredMasterPassword == "q" || enteredMasterPassword == "Q") {
        // todo -- supposed to go back to the main

    } else {
        cout<< "Enter master password:\n";
        readPasswordVault();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    main menu function
*/
int main() {

    int option = 0;
    headerFunction("Main Menu");

    cout<<"Select from options below:\n";
    cout<<"[ 1 ] Create New Login\n";
    cout<<"[ 2 ] Create New Password\n";
    cout<<"[ 3 ] Unlock Vault\n\n";

    cin>>option;

    if (option == 1){
        createNewLogin();
    } else if (option == 2){
        createNewPassword();
    } else if (option == 3){
        readPasswordVault();
    } else {
        cout<<"Response rejected.\n";
    }


    return 0;
};

