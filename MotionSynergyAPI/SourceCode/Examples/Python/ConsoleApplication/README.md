# Python Console Application

This folder contains example Python console applications for interacting with Dover Motion hardware and APIs.

## Pre-requisites

See the User Guide for software pre-requisites.

Before running the application, appropriate Instrument.cfg should be created for your axis/axes.
It should live in the SupportFolder, in the same directory where MotionSynergyAPI library file is.

For the bode plot option, Matplotlib should first be installed.

```
pip install matplotlib
```

## Usage

Run the application by using the command

```
python ConsoleApplication.py [option]
```

Options can be: `--performMoves`, `--bodePlot`

If no argument is supplied, the application will run perform moves option.

## Options

### Perform moves 

This option will move the axis/axes depending on its product type.

### Bode plot

This option will perform a bode plot data measurement and a simple bode plot using Matplotlib.

The user may need to change the parameters in `MeasureBodePlot.py` to suit their measurement need.