# SK PMT

This is the optical framework for simulating light transmission and interaction in the SK PMT. 
Also see the [MetaPMT sim repository](https://github.com/BenSmithers/). 

# Build Instructions 

You will need Geant4 and root - this version here was built and tested against Geant4-v11.2.0 and Root v6-30-04. 
Similar version numbers will likely work as well, although Geant4-v10 and Rootv5 are known to fail. 

You will also need cmake, that can be downloaded from your machine's standard package manager (homebrew, apt, yum, etc.). 

Installation should be as easy as making a folder named `build` in the folder containing `CMakeLists.txt`, navigating to that build folder, and then running 
```
    cmake ../
    make 
```
Make sure you set up your environment as necessary for Root and Geant before calling CMake. 
You can enter the Geant visualizer by simply running the built executable `skPMT`. 
Alternatively, you can prepare a macro file to automate simulation. 

# Datafiles

Datafiles used to create the SK PMT polycones are included in the `datafiles` folder. 
We also include a datafile following a 3D scan of the surface of a 20" PMT.