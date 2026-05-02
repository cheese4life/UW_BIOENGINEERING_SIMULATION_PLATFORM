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
    public class DMCM : Product
    {
        public DMCM(string productName, IAxis axis, IDigitalInput axisDigitalInput, IDiagnostics logger)
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

            await Home();

            Logger.LogString(LogLevel.Info, $"{Name} is ready for use.");
        }

        public async Task Home()
        {
            await Axis.EnterHomingMode();

            Axis.SelectMoveProfile(Axis.ProfileName("HomingProfile"));

            //If Positive move negative direction until it changes
            bool isAtNegativeLimit = Axis.GetFlagStatus("NegativeLimit").Value;
            if (!isAtNegativeLimit)
            {
                Logger.LogString(LogLevel.Info, "Moving to the Negative Limit.");
                await Axis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeLimit"); //move to negative limit
            }
            // Move to Index
            Logger.LogString(LogLevel.Info, "At Negative Limit.");
            Logger.LogString(LogLevel.Info, "Moving to Encoder Index Flag.");
            await Axis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "EncoderIndex");

            Logger.LogString(LogLevel.Info, "At Index Flag.");
            await Axis.NResetPosition("Home");

            await Axis.ExitHomingMode();
        }

        public override async Task OnPerformMoves()
        {
            Logger.LogString(LogLevel.Info, $"Performing a series of moves on the {Name}...");

            Axis.SelectMoveProfile(Axis.ProfileName("StandardMoveProfile"));

            // Move to starting position
            await Axis.NMoveAbsolute("Home");

            const int NumberOfMoves = 5;

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
            Logger.LogString(LogLevel.Info, $"{Axis.Name} digital input state change: {message.CurrentRegisterValue}");
        }


        private IAxis Axis { get; }
        private IDigitalInput AxisDigitalInput { get; }

        private IDiagnostics Logger { get; }
    }
}
