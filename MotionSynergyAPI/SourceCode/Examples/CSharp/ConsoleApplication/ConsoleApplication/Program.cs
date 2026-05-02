/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

using System;
using System.IO;
using System.Linq;
using MotionSynergyAPI;

namespace ConsoleApplication
{
    /// <summary>
    /// A simple console application demonstrating how to initialize the system,
    /// execute a script and perform commands on individual axis.
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("MotionSynergyAPI C# Example!");

            // The basic lifecycle when using the MotionSynergyAPI library is:
            //
            // 1. Get a reference to the IMotionSynergyAPI interface - this creates the instance if not already created.
            // 2. Configure the library.
            // 3. Initialize - this also runs the initialization script (Scripts/Initialize.lua).
            // 4. Execute further scripts and / or execute commands on individual axis.
            // 5. Shutdown the MotionSynergyAPI library.
            // 6. Destroy the MotionSynergyAPI instance (supports repeating from step 1 which may be useful for unit testing, etc).


            // Get a reference to the IMotionSynergyAPI interface.
            using var motionSynergy = new MotionSynergyAPINative();

            var instrumentSettings = new InstrumentSettings
            {
                ApplicationVersionString =
                    "1.0", // This version is passed to MotionSynergyAPI and logged alongside the MotionSynergyAPI version information.
                SupportFolder =
                    "./", // The 'SupportFolder' contains read-only files deployed with an application (e.g. configuration, scripts).
                ProgramDataFolder =
                    "./", // The 'ProgramDataFolder' contains device-specific files (e.g. log files, calibration settings, etc).
                ConvertRelativePathsToAbsolute = false, // Only required if the exe is not located alongside the MotionSynergyAPI dll.
                ConfigurationFilename =
                    "Instrument.cfg" // This configuration file defines each axis present in a system and related settings such as named positions, move profiles, etc.
            };

            var configurationPath = Path.Combine(instrumentSettings.SupportFolder, instrumentSettings.ConfigurationFilename);

            // First check the MotionSynergyGUI has been run to select the product and communications settings.
            if (!File.Exists(configurationPath))
            {
                Console.WriteLine("The MotionSynergyGUI application must be run to select your product and communications settings.");
                Console.WriteLine("You must then rebuild this solution.");
                return;
            }

            // Configure the system, using the current output directory for both support files and program data files (for simplicity).
            var result = motionSynergy.Configure(instrumentSettings).Result;

            // Long-running operations such as Configure / Initialize / ExecuteScript run as async tasks.
            // For this example, the application blocks waiting for the InstrumentResult, turning these into synchronous calls.
            if (!result.Success)
            {
                // InstrumentResult's ToString() method will return details of the error (Alert) in the case of a failure.
                Console.WriteLine($"Configuration failed: {result}");
                return;
            }

            // Initialize will establish a connection to each axis device.
            // Once initialize has completed successfully, individual commands on the axis may be performed
            // and/or further scripts executed.
            const bool runInitializeScript = false; // Do not run the initialize lua script - the equivalent commands will be issued from C#.
            result = motionSynergy.Initialize(runInitializeScript, null).Result;
            if (!result.Success)
            {
                Console.WriteLine($"Initialization failed: {result}");
                return;
            }

            // Returns a list of axis names in the order they appear in the configuration file and the product type for the first axis.
            var firstProduct = motionSynergy.GetFirstProduct();
            Product product = null;

            // Look up each axis by name.
            var axes = firstProduct.AxisNames.Select(axisName => motionSynergy.AxisList.First(axis => axis.Name == axisName)).ToList();

            // Look up associated digital inputs - these may not exist as they are disabled by default in the configuration
            var digitalInputs = firstProduct.AxisNames.Select(axisName => motionSynergy.DigitalInputList.FirstOrDefault(digitalInput => digitalInput.Name == $"{axisName}DigitalInput")).ToList();

            if (firstProduct.ProductType == "SmartStageLinear")
            {
                product = new SmartStageLinear(firstProduct.GetName(), axes[0], digitalInputs[0], motionSynergy.Diagnostics);
            }
            else if (firstProduct.ProductType == "SmartStageXY")
            {
                product = new SmartStageXY(firstProduct.GetName(), axes[0], axes[1], digitalInputs[0], digitalInputs[1], motionSynergy.Diagnostics);
            }
            else if (firstProduct.ProductType == "DOF5")
            {
                product = new DOF5(firstProduct.GetName(), axes[0], digitalInputs[0], motionSynergy.Diagnostics);
            }
            else if (firstProduct.ProductType == "DMCM")
            {
                product = new DMCM(firstProduct.GetName(), axes[0], digitalInputs[0], motionSynergy.Diagnostics);
            }
            else
            {
                Console.WriteLine($"Unknown ProductType {firstProduct.ProductType} specified in configuration file.");
            }

            if (product != null)
            {
                try
                {
                    // Initialize the axis - this connects to the axis, performs analog calibration, commutation and homes the axis.
                    product.Initialize();

                    // Start monitoring digital inputs
                    product.StartMonitoringDigitalInputs();

                    // Move the axis, loop for a number of times.
                    product.PerformMoves();

                    // Stop monitoring digital inputs
                    product.StopMonitoringDigitalInputs();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
            }

            // Prior to exiting the application, Shutdown should be called on the MotionSynergyAPI library.
            // This will close all connections to the axis devices and close the log file(s).
            result = motionSynergy.Shutdown();
            if (!result.Success)
            {
                Console.WriteLine($"Shutdown failed: {result}");
            }
        }
    }
}
