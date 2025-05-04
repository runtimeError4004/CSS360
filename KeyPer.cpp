#include <iostream>
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

Only TWO SQL database tables needed

LOGIN
-----
+ int pwID // is it needed?
+ string Website
+ string Username
+ string Password


VAULT ENTRY
-----
+ int pwID // is it needed?
+ string Date
+ string Time
+ bool Valid

*/

/*
    @brief writes the user's website, username and password to the SQL database
    
    @param string website name
    @param string username
    @param string password

*/
void SQL_vaultWriter(string website, string username, string password){

}
/*
    @brief reads the password vault database
    
*/
void SQL_vaultReader(){

}
/*
    @brief inserts date, time, and valid entry for every attempt to enter the vault
    
    @param bool on whether the masterpassword enetered was valid

    @return null
*/
void SQL_attemptWriter(bool accessGranted){
    string date; // get date
    string time; // get date

    // database function. Insert date, time, accessGranted

    cout<<"function incomplete";
}
/*
    @brief reads the list of all vault master password attempts from SQL DB
    
*/
void SQL_attemptReader(){


    cout<<"function incomplete";

}
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
        
    } else {
        // reverse encrypt / decrypt the password

    }
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
    string genPassword= "";

    cout<<"How long should your password be?";
    int passwordLen;
    cin>>passwordLen;

    for (int i = 0; i<passwordLen; i++){
        // to do -- use a RANDOM library
        genPassword += i; // update this with randomChar
    }

    cout<< "Generated password: "<< genPassword << "\nDo you want to copy to copy to clipboard? [ Y ] / [ N ]\n";
    string option = "";
    cin>>option;

    while (option != "Y" || option != "y" || option != "N" || option != "n" ){
        if(option == "Y" || option == "y" ){
            // use copy to clipboard libary 
        }
        else if(option == "N" || option == "n" ){
            
        }
        // please unit test 
    }


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
    // cout
    cin >> website;
    string username = "";
    // cout
    cin >> username;
    string password = createNewPassword();
    // cout
    // cin



    validateInsertVault(website, username, password);
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
    cout<<"[ 5 ] Update Login Info\n";

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
        // SQL_attemptReader(); // if we choose to, we can have a master password login through the DB rather than plain text. Would be smart but not a P0
        vault();
    } else {
        cout<<"\nResponse rejected.\n\n";
        vault();
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    main menu function
*/
int main() {
    while (!loginVault()){
        loginVault();
    }
    vault();
    return 0;
};

