# Project:     
#### KeyPer - Password Manager 

# Date:        
May 2nd, 2025

# Brief:       
#### KeyPer is a password manager application.  

> Team FamJam intends to deliver a password manager application to create, store, and retrieve user login information. All relevant data, such as website names, usernames, passwords, and successful or unsuccessful password bank login attempts, as well as the metadata, such as date and time associated with the creation or retrieval of each of these events, will be stored in a SQL database. Passwords will be encrypted and decrypted using Advanced Encryption Standard (AES). The primary goal of this project is to provide our users with a secure method for storing and retrieving sensitive information, particularly in an era where malicious actors have more and more methods to access it.

# Creators:    
Justin Chambers

Caeden Statia

Yasin Maow

Shagufta Jahan

Abel Alemayehu

# Resources
[Markdown Formatting](https://www.markdownguide.org/basic-syntax/) 

A) [C++ SQL functions](https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/) 

B) [Random Number Generator](https://www.w3schools.com/cpp/cpp_howto_random_number.asp) 

# Compiling From Source

## WindowsOS Build:
- Requires a C++ compiler and [CMake](https://cmake.org/download/)

## MacOS Build:
- Install HomeBrew 
  - Enter the following command into VSC terminal `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
  - followed by `echo >> /Users/user/.zprofile`,
    then `echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> /Users/user/.zprofile`,
    and `eval "$(/opt/homebrew/bin/brew shellenv)"`
  - Finally, enter `brew install cmake`
  - Verify with `cmake --version`

# From a terminal:

- Navigate to the "build" subdirectory: `cd ./build`
  - **Create this directory if it does not exist.** `mkdir build`
  - **It is recommended to clear the contents of this directory before every new build.**
  - The contents of this directory is gitignored and will not be added to source control.
- Build the CMake project: `cmake ..`
- Compile and link the project: `cmake --build .`
- Navigate to the output directory and run the program:
  - `cd ./debug`
  - `./KeyPer.exe` (Windows)
  - `./KeyPer` (macOS, Linux)