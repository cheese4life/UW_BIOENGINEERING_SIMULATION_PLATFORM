# The copyright to the computer program(s) herein is the property of
# Dover Motion, Inc. This file is subject to the terms and
# conditions found in the 'DoverLicense.txt' file included with this
# source code package.

from MotionSynergyAPI import LogLevel

class SmartStageXY:

    def __init__(self, xAxis, yAxis, logger):
        self.XAxis = xAxis
        self.YAxis = yAxis
        self.Logger = logger

    def PerformMoves(self):
        
        self.Logger.LogString(LogLevel.Info, "Performing a series of moves on the SmartStageXY...")

        self.XAxis.SelectMoveProfile(f"{self.XAxis.Name}StandardMoveProfile")
        self.YAxis.SelectMoveProfile(f"{self.YAxis.Name}StandardMoveProfile")

        # Move to starting position
        xTask = self.XAxis.MoveAbsolute(-45)
        yTask = self.YAxis.MoveAbsolute(-45)

        result = xTask.Result
        result = yTask.Result

        countX = 8
        for j in range(0,countX):
            countY = 8
            for i in range(0,countY):
                # Move Y axis Positive
                result = self.YAxis.MoveRelative(5).Result

            result = self.XAxis.MoveRelative(5).Result

            for i in range(countY, 0, -1):
                # Move Y axis Negative
                result = self.YAxis.MoveRelative(-5).Result

            result = self.XAxis.MoveRelative(5).Result
