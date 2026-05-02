/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

using System;
using System.Threading;
using System.Threading.Tasks;
using MotionSynergyAPI;

namespace ConsoleApplication
{
    public class DOF5 : Product
    {
        public DOF5(string productName, IAxis axis, IDigitalInput axisDigitalInput, IDiagnostics logger)
            : base(productName)
        {
            Axis = axis;
            AxisDigitalInput = axisDigitalInput;
            Logger = logger;
        }

        public override async Task OnInitialize()
        {
            Logger.LogString(LogLevel.Info, $"Initializing {Name}...");

            var result = await Axis.Initialize();
            // Check the result for success and throw an exception if Initialization failed.
            // Note that all commands should be checked for success. For readability this example
            // does not check every command result.
            if (!result.Success)
            {
                throw new Exception($"Initialization failed for {Axis.Name}: {result}");
            }
            Logger.LogString(LogLevel.Info, $"Homing {Name}...");

            await HomeZ();

            Logger.LogString(LogLevel.Info, $"{Name} is ready for use.");
        }

        public async Task HomeZ()
        {
            await Axis.EnterHomingMode();

            // get HalfTravelFlag and assign it to a constant
            bool isAtPositiveHalfFlag = Axis.GetFlagStatus("HomePositiveHalfFlag").Value;

            // Configure the Homing Move Profile for the axis
            Axis.SelectMoveProfile(Axis.ProfileName("HomingProfile"));

            //Check the status of the Half Travel Flag to determine where you are
            //If Positive move negative direction until it changes
            if (isAtPositiveHalfFlag)
            {
                Logger.LogString(LogLevel.Info, $"{Name} is in Positive Side.");
                await Axis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "HomeNegativeHalfFlag");

                //Move away from the flag to approach it from same direction
                await Axis.NMoveRelative("MoveToCenterOffset");
            }
            // else move positive until it changes
            else
            {
                Logger.LogString(LogLevel.Info, $"{Name} is in Positive Side.");
            }
            await Axis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "HomePositiveHalfFlag");
            await Axis.NResetPosition("Home");

            await Axis.ExitHomingMode();
        }

        public override async Task OnPerformMoves()
        {
            Logger.LogString(LogLevel.Info, $"Performing a series of moves on the {Name}...");

            Axis.SelectMoveProfile(Axis.ProfileName("StandardMoveProfile"));

            await Axis.NMoveAbsolute("Home");

            int NumberOfMoves = 5;

            for (int i = 0; i < NumberOfMoves; i++)
            {
                await Axis.MoveRelative(0.5);
            }

            Thread.Sleep(4000);

            for (int i = NumberOfMoves; i >= 0; i--)
            {
                await Axis.MoveRelative(-0.5);
            }
        }

        public override void StartMonitoringDigitalInputs()
        {
            if (AxisDigitalInput != null)
            {
                Logger.LogString(LogLevel.Info, $"Monitoring {Axis.Name} digital input");
                AxisDigitalInput.DigitalInputChanged += OnDigitalInputChanged;
                AxisDigitalInput.EnableAsynchChangeUpdates();
            }
        }

        public override void StopMonitoringDigitalInputs()
        {
            if (AxisDigitalInput != null)
            {
                AxisDigitalInput.DigitalInputChanged -= OnDigitalInputChanged;
            }
        }

        private void OnDigitalInputChanged(object sender, DigitalInputEventArgs message)
        {
            Logger.LogString(LogLevel.Info, $"Z axis digital input state change: {message.CurrentRegisterValue}");
        }


        private IAxis Axis { get; }
        private IDigitalInput AxisDigitalInput { get; }

        private IDiagnostics Logger { get; }
    }
}
