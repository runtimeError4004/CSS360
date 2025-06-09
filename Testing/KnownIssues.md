# Add a file called KnownIssues.txt.  
## Briefly list all known issues.

## Known Issue: `Error: could not load cache` when running `cmake --build`
### Description  
This error occurs when you try to build the project from a directory that doesn't contain the required CMake build files—specifically, the `CMakeCache.txt` file. It typically happens if you accidentally run the configuration command (`cmake ..`) inside a nested subfolder (like `build/debug`) instead of the main `build/` directory.
### How to Fix

1. **Clean your build environment**  
   Delete the entire `build/` directory to remove any nested or conflicting cache files:
   ```powershell
   Remove-Item -Recurse -Force build
   ```

2. **Create and configure the build directory properly**  
   From the project root:
   ```powershell
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. **Build the project from the correct build directory**
   ```powershell
   cmake --build .
   ```

4. **Locate and run the executable**
   The compiled executable will usually be in the `Debug/` subfolder:
   ```powershell
   cd Debug
   .\KeyPer.exe
   ```


