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

## Sources
* [Open Motor](https://github.com/reilleya/openMotor)
* [Richard Nakka](https://www.nakka-rocketry.net/th_pres.html)
* [Parameters Affecting the Erosive Burning of Solid Rocket Motor](https://www.matec-conferences.org/articles/matecconf/pdf/2018/12/matecconf_icmme2018_03001.pdf)
* [SOLID ROCKET MOTORS INTERNAL BALLISTIC MODEL WITH EROSIVE AND CONDENSED PHASE CONSIDERATIONS](https://amme.journals.ekb.eg/article_34725_63cd48d03bd8dda10d9a74a114ec8b51.pdf)
