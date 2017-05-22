# file-merge
Merge two files and sort a portion of text area.

# Pre-requisite to run on Windows
1 - MinGW installed under C:\MinGW
2 - PATH variable updated with addition of:
    C:\MinGW\bin
    C:\MinGW\mingw32\bin
    C:\MinGW\msys\1.0\bin
3 - Project cloned locally:
    > cd C:\my_projects
    > mkdir file-merge
    > git clone https://github.com/marcpascal/file-merge.git

# How to build in Windows DOS console
> make -n      # To list what will be executed
> make [clean] # Will clean the previous build items (*.o and *.exe)
> make all     # Rebuild and generate the output

# Execution
> cd C:\my_projects\file-merge
  Edit the file data\liste.in and add the tags to be appended to the output files
  Drop the original files into the folder data. I.e.: file.in as delivered
> build_patch.exe data\liste.in data\file.in file.out
  The output file will be generated under C:\my_projects\file-merge\file.out

For multiple files to be processed you will need to create your own suite of commands.