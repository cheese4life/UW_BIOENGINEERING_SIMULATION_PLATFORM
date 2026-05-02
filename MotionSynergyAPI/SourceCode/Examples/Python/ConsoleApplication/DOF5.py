# The copyright to the computer program(s) herein is the property of
# Dover Motion, Inc. This file is subject to the terms and
# conditions found in the 'DoverLicense.txt' file included with this
# source code package.

from MotionSynergyAPI import LogLevel

class DOF5:

    def __init__(self, axis, logger):
        self.Axis = axis
        self.Logger = logger

    def PerformMoves(self):
        
        self.Logger.LogString(LogLevel.Info, "Performing a series of moves on the DOF-5...")

        self.Axis.SelectMoveProfile(f"{self.Axis.Name}StandardMoveProfile")

        # Move to starting position
        xTask = self.Axis.MoveAbsolute(0)
        result = xTask.Result

        countX = 5
        for j in range(0,countX):
            result = self.Axis.MoveRelative(0.5).Result
