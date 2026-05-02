/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

using System;
using System.Threading.Tasks;
using MotionSynergyAPI;

namespace ConsoleApplication
{
    public class SmartStageXY : Product
    {
        public SmartStageXY(string productName, IAxis xAxis, IAxis yAxis, IDigitalInput xAxisDigitalInput, IDigitalInput yAxisDigitalInput, IDiagnostics logger)
            : base(productName)
        {
            XAxis = xAxis;
            YAxis = yAxis;
            XAxisDigitalInput = xAxisDigitalInput;
            YAxisDigitalInput = yAxisDigitalInput;
            Logger = logger;
        }

        public override async Task OnInitialize()
        {
            Logger.LogString(LogLevel.Info, $"Initializing {Name}...");

            var result = await XAxis.Initialize();
            // Check the result for success and throw an exception if Initialization failed.
            // Note that all commands should be checked for success. For readability this example
            // does not check every command result.
            if (!result.Success)
            {
                throw new Exception($"Initialization failed for {XAxis.Name}: {result}");
            }
            result = await YAxis.Initialize();
            if (!result.Success)
            {
                throw new Exception($"Initialization failed for {YAxis.Name}: {result}");
            }

            Logger.LogString(LogLevel.Info, $"Homing {Name}...");

            await HomeX_Center();
            await HomeY_Center();

            Logger.LogString(LogLevel.Info, $"{Name} is ready for use.");
        }

        public async Task HomeX_Center()
        {
            await XAxis.EnterHomingMode();

            XAxis.SelectMoveProfile(XAxis.ProfileName("HomingProfile"));

            bool isAtPositiveHalf = XAxis.GetFlagStatus("PositiveHalf").Value;
            bool isAtPositiveLimit = XAxis.GetFlagStatus("PositiveLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtPositiveHalf || isAtPositiveLimit)
            {
                await XAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeHalf");
                await XAxis.NMoveRelative("MoveToCenterOffset");
            }

            await XAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveHalf");

            await XAxis.NResetPosition("Home");

            await XAxis.ExitHomingMode();
        }

        public async Task HomeY_Center()
        {
            await YAxis.EnterHomingMode();

            YAxis.SelectMoveProfile(YAxis.ProfileName("HomingProfile"));

            bool isAtPositiveHalf = YAxis.GetFlagStatus("PositiveHalf").Value;
            bool isAtPositiveLimit = YAxis.GetFlagStatus("PositiveLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtPositiveHalf || isAtPositiveLimit)
            {
                await YAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeHalf");
                await YAxis.NMoveRelative("MoveToCenterOffset");
            }

            await YAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveHalf");

            await YAxis.NResetPosition("Home");

            await YAxis.ExitHomingMode();
        }

        public async Task HomeX_NegativeLimit()
        {
            await XAxis.EnterHomingMode();

            XAxis.SelectMoveProfile(XAxis.ProfileName("HomingProfile"));

            bool isAtNegativeLimit = XAxis.GetFlagStatus("NegativeLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtNegativeLimit)
            {
                await XAxis.NMoveRelative("OffLimitMove");
            }

            await XAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeLimit");

            await XAxis.NMoveRelative("NegativeLimitOffset");

            await XAxis.NResetPosition("Home");


            await XAxis.ExitHomingMode();
        }

        public async Task HomeX_PositiveLimit()
        {
            await XAxis.EnterHomingMode();

            XAxis.SelectMoveProfile(XAxis.ProfileName("HomingProfile"));

            bool isAtPositiveLimit = XAxis.GetFlagStatus("PositiveLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtPositiveLimit)
            {
                await XAxis.NMoveRelative("OffLimitMove");
            }

            await XAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveLimit");

            await XAxis.NMoveRelative("PositiveLimitOffset");

            await XAxis.NResetPosition("Home");

            await XAxis.ExitHomingMode();
        }

        public async Task HomeY_NegativeLimit()
        {
            await YAxis.EnterHomingMode();

            YAxis.SelectMoveProfile(YAxis.ProfileName("HomingProfile"));

            bool isAtNegativeLimit = YAxis.GetFlagStatus("NegativeLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtNegativeLimit)
            {
                await YAxis.NMoveRelative("OffLimitMove");
            }

            await YAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeLimit");

            await YAxis.NMoveRelative("NegativeLimitOffset");

            await YAxis.NResetPosition("Home");

            await YAxis.ExitHomingMode();
        }

        public async Task HomeY_PositiveLimit()
        {
            await YAxis.EnterHomingMode();

            YAxis.SelectMoveProfile(YAxis.ProfileName("HomingProfile"));

            bool isAtPositiveLimit = YAxis.GetFlagStatus("PositiveLimit").Value;

            // If already on negative limit, move off the negative limit
            if (isAtPositiveLimit)
            {
                await YAxis.NMoveRelative("OffLimitMove");
            }

            await YAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveLimit");

            await YAxis.NMoveRelative("PositiveLimitOffset");

            await YAxis.NResetPosition("Home");

            await YAxis.ExitHomingMode();
        }

        public override async Task OnPerformMoves()
        {
            Logger.LogString(LogLevel.Info, $"Performing a series of moves on the {Name}...");

            XAxis.SelectMoveProfile(XAxis.ProfileName("StandardMoveProfile"));
            YAxis.SelectMoveProfile(YAxis.ProfileName("StandardMoveProfile"));

            // Move to starting position
            var xTask = XAxis.MoveAbsolute(-45);
            var yTask = YAxis.MoveAbsolute(-45);

            Task.WaitAll(xTask, yTask);

            int countX = 8;
            for (int j = 0; j < countX; j++)
            {

                int countY = 8;
                for (int i = 0; i < countY; i++)
                {
                    // Move Y axis Positive
                    await YAxis.MoveRelative(5);
                }

                await XAxis.MoveRelative(5);

                for (int i = countY; i > 0; i--)
                {
                    // Move Y axis Negative
                    await YAxis.MoveRelative(-5);
                }

                await XAxis.MoveRelative(5);
            }
        }

        public override void StartMonitoringDigitalInputs()
        {
            if (XAxisDigitalInput != null)
            {
                Logger.LogString(LogLevel.Info, $"Monitoring {XAxis.Name} digital input");
                XAxisDigitalInput.DigitalInputChanged += OnDigitalInputChanged;
                XAxisDigitalInput.EnableAsynchChangeUpdates();
            }
            if (YAxisDigitalInput != null)
            {
                Logger.LogString(LogLevel.Info, $"Monitoring {YAxis.Name} digital input");
                YAxisDigitalInput.DigitalInputChanged += OnDigitalInputChanged;
                YAxisDigitalInput.EnableAsynchChangeUpdates();
            }
        }

        public override void StopMonitoringDigitalInputs()
        {
            if (XAxisDigitalInput != null)
            {
                XAxisDigitalInput.DigitalInputChanged -= OnDigitalInputChanged;
            }
            if (YAxisDigitalInput != null)
            {
                YAxisDigitalInput.DigitalInputChanged -= OnDigitalInputChanged;
            }
        }

        private void OnDigitalInputChanged(object sender, DigitalInputEventArgs message)
        {
            var source = sender == XAxisDigitalInput ? XAxis.Name : YAxis.Name;
            Logger.LogString(LogLevel.Info, $"{source} digital input state change: {message.CurrentRegisterValue}");
        }

        private IAxis XAxis { get; }
        private IAxis YAxis { get; }
        private IDigitalInput XAxisDigitalInput { get; }
        private IDigitalInput YAxisDigitalInput { get; }

        private IDiagnostics Logger { get; }

    }
}
