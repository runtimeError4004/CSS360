#include <iostream>
using namespace std;

string masterPassword = "0000";

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
    support function for creating a new login
    1) ask user for password length
    2) pull random characters from charArray
    3) 
    4) 
*/
string createNewPassword(){

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
        genPassword += i;
    }

    return genPassword;
}

/*
    primary function for opening the password vault
    1) ask user to confirm their master password
    2) if true, open vault -- if false, re call readPasswordVault
    3) 
    4) 
*/
void readPasswordVault(){
    headerFunction("Password Vault");
    string enteredMasterPassword = "";
    cout<< "Enter master password";
    cin >> enteredMasterPassword;
    if(enteredMasterPassword == masterPassword){
        SQLVaultReader();
    } else {
        readPasswordVault();
    }
}

/*
    support function for creating a new login
    1) 
    2) 
    3) 
    4) 
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
    support function for creating a new login
    1) 
    2) 
    3) 
    4) 
*/
void SQLVaultWriter(string website, string username, string password){

}
/*

*/
void SQLVaultReader(){
}

/*
    support function
*/
void headerFunction(string text){
    cout<<"- - - - - - - - - - - - - - - - -";
    cout<<"- - - - - - - - ", text, " - - - - - - - - -";
    cout<<"- - - - - - - - - - - - - - - - -";
}





/*
    main menu function
*/
int main() {


    int option = 0;


    
    cout<<"Main Menu\n";
    cout<<"Select from options below:\n";
    cout<<"[ 1 ] Create New Login";
    cout<<"[ 2 ] Create New Password";
    cout<<"[ 3 ] Unlock Vault";

    cin>>option;

    if (option == 1){
        
    } else if (option == 1){
        
    } else if (option == 2){
        
    } else if (option == 3){
        
    } else {
        cout<<"Response rejected.";
        main();
    }


    return 0;
}