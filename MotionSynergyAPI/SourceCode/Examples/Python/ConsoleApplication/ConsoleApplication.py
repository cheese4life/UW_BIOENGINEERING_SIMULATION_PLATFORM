# The copyright to the computer program(s) herein is the property of
# Dover Motion, Inc. This file is subject to the terms and
# conditions found in the 'DoverLicense.txt' file included with this
# source code package.

import os
import sys
import argparse

def setupDllsPath():
    # Change the current directory to the root folder so the Motion Synergy API and it's associated support files can be found.
    rootFolders = ['../../../../']  # This is the path used by the deployed release package.

    # Add the paths used during development (can be removed / updated by the end user)
    if isLinux:
        rootFolders.extend(['../../../build_linux_x64_debug/bin', '../../../build_linux_x64_release/bin'])
    else:
        rootFolders.extend(['../../../build_windows_x64/bin/Debug', '../../../build_windows_x64/bin/Release'])

    for rootFolder in rootFolders:
        if os.path.isfile(os.path.join(rootFolder, 'MotionSynergyAPI.dll')):
            print(f"Found MotionSynergyAPI.dll in {rootFolder}.")
            os.chdir(rootFolder)
            break

# Add the current directory so the MotionSynergyAPINative.dll can be found
sys.path.append('.')

isLinux = sys.platform.startswith("linux")
if isLinux:
    # Use Microsoft's dotnet, rather than Mono
    from pythonnet import load
    load("coreclr")

import clr
from System.Collections import *
from System.Collections.Generic import List
from System import String

setupDllsPath()

# Add a reference to the MotionSynergyAPI .NET assembly and import the needed types.
clr.AddReference("MotionSynergyAPI")
from MotionSynergyAPI import MotionSynergyAPINative
from MotionSynergyAPI import InstrumentSettings

if __name__ == "__main__":
    print("MotionSynergyAPI Python Example!")

    # Process which example to run
    parser = argparse.ArgumentParser(description="MotionSynergyAPI Python Example")
    parser.add_argument('--performMoves', action='store_true', help='Perform moves.')
    parser.add_argument('--bodePlot', action='store_true', help='Run bode plot.')
    parser.add_argument('--amplitude', type=float, default=3, help="Excitation amplitude, used to derived each sine wave amplitude.")
    parser.add_argument('--measurementPos', type=float, default=0, help="Measurement position.")
    parser.add_argument('--startHz', type=float, default=2, help="Starting frequency to perform the sweep.")
    parser.add_argument('--endHz', type=float, default=1000, help="End frequency to perform the sweep.")
    parser.add_argument('--numPoints', type=int, default=100, help="Number of frequencies between start and end (inclusive) to sweep.")
    parser.add_argument('--samplePeriod', type=float, default=51.2, help="Sampling period of the controller.")
    parser.add_argument('--travelLength', type=float, default=100, help="Travel length of the axis.")
    args = parser.parse_args()

    # Perform moves if no option selected
    if len(sys.argv) == 1:
        args.performMoves = True

    if args.numPoints is not None and args.numPoints < 2:
        raise ValueError("Number of sine wave points must be at least 2")

    # Get a reference to the IMotionSynergyAPI interface
    motionSynergy = MotionSynergyAPINative()

    instrumentSettings = InstrumentSettings()
    instrumentSettings.ApplicationVersionString = "1.0"
    instrumentSettings.SupportFolder = "./SupportFolder"
    instrumentSettings.ProgramDataFolder = "./ProgramDataFolder"
    instrumentSettings.ConfigurationFilename = "Instrument.cfg"

    # First check the MotionSynergyGUI has been run to select the product and communications settings.
    if not os.path.isfile(os.path.join(instrumentSettings.SupportFolder, instrumentSettings.ConfigurationFilename)):
        sys.exit(f"The MotionSynergyGUI application must be run to select your product and communications settings.")

    # Configure the system using the above settings.
    result = motionSynergy.Configure(instrumentSettings).Result

    if not result.Success:
        sys.exit(f"Configuration failed: {result}")

    # Initialize will establish a connection to each axis device, initialize the hardware and
    # execute the Scripts/Initialize.lua script, which in turn will home each axis for this example.
    # Once initialize has completed successfully, individual commands on the axis may be performed
    # and/or further scripts executed.
    result = motionSynergy.Initialize().Result
    if not result.Success:
        sys.exit(f"Initialization failed: {result}")

    # Perform a series of moves, appropriate for the selected product.
    # These moves are wrapped in a try / catch block to ensure Shutdown is called prior to
    # exit, ensuring each axis is disabled on exit.
    try:
        product = motionSynergy.GetFirstProduct()
        productType = product.ProductType
        axisNames = product.AxisNames
        axes =  [axis for axisName in axisNames for axis in motionSynergy.AxisList if axis.Name == axisName]

        if args.performMoves:
            if productType == "SmartStageLinear":
                from SmartStageLinear import *
                smartStage = SmartStageLinear(axes[0], motionSynergy.Diagnostics)
                smartStage.PerformMoves()
            elif productType == "SmartStageXY":
                from SmartStageXY import *
                smartStage = SmartStageXY(axes[0], axes[1], motionSynergy.Diagnostics)
                smartStage.PerformMoves()
            elif productType == "DOF5":
                from DOF5 import *
                dof5 = DOF5(axes[0], motionSynergy.Diagnostics)
                dof5.PerformMoves()
            elif productType == "DMCM":
                from DMCM import *
                dmcm = DMCM(axes[0], motionSynergy.Diagnostics)
                dmcm.PerformMoves()
            else:
                print(
                    f"Unknown ProductType {productType} specified in configuration file {instrumentSettings.ConfigurationFilename}.")
        elif args.bodePlot:
            from MeasureBodePlot import *
            measureBodePlot = MeasureBodePlot(axes[0], motionSynergy.Diagnostics)
            measureBodePlot.Run(args)

    except Exception as e:
        print(repr(e))

    # Prior to exiting the application, Shutdown should be called on the MotionSynergyAPI library.
    # This will close all connections to the axis devices and close the log file(s).
    result = motionSynergy.Shutdown()
    if not result.Success:
        sys.exit(f"Shutdown failed: {result}")

    print("MotionSynergyAPI Python Example Complete!")
