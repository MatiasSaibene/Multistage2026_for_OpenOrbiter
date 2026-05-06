# Multistage2026_for_OpenOrbiter
By fred18. Multistage2026 is the renewed Multistage module, now for the new Orbiter 2024. Inspired by the module made by Vinka, Multistage2015 allows to develop multistage launchers even for those who are not willing or capable to code, or for those who simply wants to exploits all the built in features of this module. 


# How-to compile on Linux

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
