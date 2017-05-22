# file-merge
Merge two files and sort a portion of text area.

# Pre-requisite to run on Windows
<li>MinGW installed under C:\MinGW

<li>PATH variable updated with addition of:
<blockquote>
  > C:\MinGW\bin
  
  > C:\MinGW\mingw32\bin
  
  > C:\MinGW\msys\1.0\bin
</blockquote>

<li>Project cloned locally:
<blockquote>
  > cd C:\my_projects
  
  > mkdir file-merge
  
  > git clone https://github.com/marcpascal/file-merge.git
</blockquote>

# How to build in Windows DOS console
<blockquote>
  > make -n      # To list what will be executed
  
  > make [clean]      # Will clean the previous build items (*.o and *.exe)
  
  > make all          # Rebuild and generate the output
</blockquote>

# Execution
<blockquote>
> cd C:\my_projects\file-merge
  <blockquote>
  Edit the file data\liste.in and add the tags to be appended to the output files<br>
  Drop the original files into the folder data. I.e.: file.in as delivered
  </blockquote>
> build_patch.exe data\liste.in data\file.in file.out
  <blockquote>
  The output file will be generated under C:\my_projects\file-merge\file.out
  </blockquote>
</blockquote>

For multiple files to be processed you will need to create your own suite of commands.
