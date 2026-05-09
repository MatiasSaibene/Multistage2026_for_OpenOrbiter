# Multistage2026_for_OpenOrbiter
By fred18. Multistage2026 is the renewed Multistage module, now for the new Orbiter 2024. Inspired by the module made by Vinka, Multistage2015 allows to develop multistage launchers even for those who are not willing or capable to code, or for those who simply wants to exploits all the built in features of this module. 

# How-to compile on Windows

Follow this guide to download CMake for Windows and MS Build from Microsoft:
https://www.orbiter-forum.com/threads/preparing-cmake-to-compile-orbiter-add-ons-my-experience.41842/

Get Git for Windows:
https://git-scm.com/install/windows

Open the Git Bash Terminal and type:

>git clone https://github.com/MatiasSaibene/Multistage2026_for_OpenOrbiter

Move the folder for Windows "Multistage2026_for_Windows" inside the Orbitersdk folder.

Open a "Developer PowerShell" Terminal inside Multistage2026_for_Windows and run:

```
$env:ORBITER_ROOT="PATH\TO\YOUR\ORBITER\INSTALLATION"
```

```
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
```

```
cmake --build build --config Release
```

Then move the resulting .dlls inside Multistage2026_BIN folder to Modules folder inside your Orbiter installation.



# How-to compile on Linux

First you will need CMake.
Download https://www.orbiter-forum.com/resources/multistage2015-for-orbiter-2016.398/ Unzip and copy the files to ~/orbiter_test/Orbiter.
Remember to change Multistage2015 to Multistage2026 in the modules name in scenario files.

Before:
>> BEGIN_SHIPS
   SLS:Multistage2015
   
After:
>> BEGIN_SHIPS
   SLS:Multistage2026
   
And then do:

>git clone https://github.com/MatiasSaibene/Multistage2026_for_OpenOrbiter

Open the folder ~/orbiter_test/Orbiter/Orbitersdk/ move the folder for Linux "Multistage2026_for_Linux" inside the Orbitersdk folder.

```
cd ~/orbiter_test/Orbiter/Orbitersdk/Multistage2026_for_Linux
```

```

mkdir build && cd build
```

```
cmake .. && make -j$(nproc)
```

```
cd ..
```

```
cd Multistage2026_BIN
```

```
mv libMultistage2026_MFD.so ~/orbiter_test/Orbiter/Modules/Plugin/
```

```
mv *.so ~/orbiter_test/Orbiter/Modules
``` 
This will install the Multistage2026 modules on Orbiter Linux.
