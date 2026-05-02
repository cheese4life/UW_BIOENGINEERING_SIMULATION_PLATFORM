# The copyright to the computer program(s) herein is the property of
# Dover Motion, Inc. This file is subject to the terms and
# conditions found in the 'DoverLicense.txt' file included with this
# source code package.

from MotionSynergyAPI import LogLevel

import clr

# Assume we are at the correct directory, with these dlls.
clr.AddReference("ProductSetupLibrary")
clr.AddReference("MotionWorkflowLibrary")

from ProductSetupLibrary import SineWaveTraceInputs, SineWaveSweepInputs, LinearUnits, BodePlotData
from MotionWorkflowLibrary import AxisSineWaveWorkflows

class MeasureBodePlot:
    def __init__(self, axis, logger):
        self.Axis = axis
        self.Logger = logger

    def Run(self, bodePlotArgs):
        self.Logger.LogString(LogLevel.Info, f"Performing a bode plot actions on the {self.Axis.Name}...")

        # Setup parameters for bode plot measurement
        traceInputs = SineWaveTraceInputs()
        traceInputs.Amplitude = bodePlotArgs.amplitude
        traceInputs.MeasurementPos = bodePlotArgs.measurementPos

        startingHz = bodePlotArgs.startHz
        endingHz = bodePlotArgs.endHz
        numPoints = bodePlotArgs.numPoints
        sweepInputs = SineWaveSweepInputs(startingHz, endingHz, numPoints)

        dataOutputPath = "ProgramDataFolder"
        controllerSamplingPeriodUs = 51.2
        travelLength = bodePlotArgs.travelLength
        linearUnits = LinearUnits.Millimetres

        # Do measurement and get results
        bodeTask = AxisSineWaveWorkflows.BodePlotMeasurement(self.Axis, traceInputs, sweepInputs, \
                                                               controllerSamplingPeriodUs, travelLength, linearUnits, dataOutputPath, None)
        results = bodeTask.Result

        logStr = [f"Results written to {results.FilePath}",
                    f"Gain margin: {results.GainMargin:.6f}",
                    f"Phase margin: {results.PhaseMargin:.6f}",
                    f"Frequency_Negative3dBCutOff: {results.Frequency_Negative3dBCutOff:.6f}",
                    f"Frequency_Negative90DegPhase: {results.Frequency_Negative90DegPhase:.6f}",
                    f"Frequency_PhaseMargin: {results.Frequency_PhaseMargin:.6f}",
                    f"Frequency_PhaseCross: {results.Frequency_PhaseCross:.6f}"]
        self.Logger.LogString(LogLevel.Info, "\n".join(logStr))

        MeasureBodePlot.__Plot(results.ClosedLoopData, results.OpenLoopData)

    def __Plot(closedLoopData, openLoopData):

        import math
        try:
            import matplotlib.pyplot as plt
        except ImportError:
            print("Matplotlib module is not installed. Skipping plot ...")
            exit()

        plt.figure()
        plt.subplot(2, 1, 1)
        plt.semilogx(closedLoopData.FrequencyHz, closedLoopData.Magnitude, label="Closed Loop")
        plt.semilogx(openLoopData.FrequencyHz, openLoopData.Magnitude, label="Open Loop")
        plt.ylabel('Magnitude (dB)')
        plt.title('Bode Plot')
        plt.legend()

        plt.subplot(2, 1, 2)
        plt.semilogx(closedLoopData.FrequencyHz, [phase*180/math.pi for phase in closedLoopData.Phase], label="Closed Loop")
        plt.semilogx(openLoopData.FrequencyHz, [phase*180/math.pi for phase in openLoopData.Phase], label="Open Loop")
        plt.ylabel('Phase (deg)')
        plt.xlabel('Frequency (Hz)')

        plt.tight_layout()
        plt.show()