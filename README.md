# Banana_Ballistics - internal ballistics silmulator for solid rocket motors

## Installing
To install this software you mus install CMake
Go to source folder and run 
```shell
cmake
cd build
make
```

## Usage
To use simulation you need to enter motor configuratio in configuration.json file. Then just run Banana_Ballistics executable.

## Results
Results will appear in .csv format as output.csv file, typical size depending on timestep is between 40-300MB.
I included simple python plotter to analyze result data. To use it just type:
```shell
python3 Plotter.py
```
Be aware that plotter requires csv an matplotlib to run.
