/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

using MotionSynergyAPI;
using System;
using System.Threading.Tasks;

namespace ConsoleApplication
{
    public class SmartStageLinear : Product
    {
        public SmartStageLinear(string productName, IAxis axis, IDigitalInput xAxisDigitalInput, IDiagnostics logger)
        : base(productName)
        {
            Axis = axis;
            AxisDigitalInput = xAxisDigitalInput;
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

            await HomeX_Center();

            Logger.LogString(LogLevel.Info, $"{Name} is ready for use.");
        }

        public async Task HomeX_Center()
        {
            await Axis.EnterHomingMode();

            Axis.SelectMoveProfile(Axis.ProfileName("HomingProfile"));

            bool isAtPositiveHalf = Axis.GetFlagStatus("PositiveHalf").Value;
            bool isAtPositiveLimit = Axis.GetFlagStatus("PositiveLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtPositiveHalf || isAtPositiveLimit)
            {
                await Axis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeHalf");
                await Axis.NMoveRelative("MoveToCenterOffset");
            }

            await Axis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveHalf");

            await Axis.NResetPosition("Home");

            await Axis.ExitHomingMode();
        }

        public async Task HomeX_NegativeLimit()
        {
            await Axis.EnterHomingMode();

            Axis.SelectMoveProfile(Axis.ProfileName("HomingProfile"));

            bool isAtNegativeLimit = Axis.GetFlagStatus("NegativeLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtNegativeLimit)
            {
                await Axis.NMoveRelative("OffLimitMove");
            }

            await Axis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeLimit");

            await Axis.NMoveRelative("NegativeLimitOffset");

            await Axis.NResetPosition("Home");

            await Axis.ExitHomingMode();
        }

        public async Task HomeX_PositiveLimit()
        {
            await Axis.EnterHomingMode();

            Axis.SelectMoveProfile(Axis.ProfileName("HomingProfile"));

            bool isAtPositiveLimit = Axis.GetFlagStatus("PositiveLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtPositiveLimit)
            {
                await Axis.NMoveRelative("OffLimitMove");
            }

            await Axis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveLimit");

            await Axis.NMoveRelative("PositiveLimitOffset");

            await Axis.NResetPosition("Home");

            await Axis.ExitHomingMode();
        }

        public override async Task OnPerformMoves()
        {
            Logger.LogString(LogLevel.Info, "Performing a series of moves on the ${Name}...");

            Axis.SelectMoveProfile(Axis.ProfileName("StandardMoveProfile"));

            // Move to starting position
            var xTask = Axis.MoveAbsolute(-45);
            await xTask;

            int countX = 8;
            for (int j = 0; j < countX; j++)
            {

                await Axis.MoveRelative(5);
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
