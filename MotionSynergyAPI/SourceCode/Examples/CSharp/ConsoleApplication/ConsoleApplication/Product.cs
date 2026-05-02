/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

using System.Threading.Tasks;

namespace ConsoleApplication
{
    /// <summary>
    /// A base class used to demonstrate how to use the IAxisDevice interface to perform various operations such as
    /// Initialization, Homing and moving for each product type.
    /// </summary>
    public abstract class Product
    {
        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="productName">Name of the product</param>
        protected Product(string productName)
        {
            Name = productName;
        }

        /// <summary>
        /// Name of the product.
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// Initialize the axis. This will connect to each axis, perform analog calibration and commutation.
        /// Initialize must be called before homing.
        /// </summary>
        public void Initialize()
        {
            OnInitialize().Wait();
        }

        /// <summary>
        /// Perform a series of moves on each axis.
        /// Each axis must be Initialized and Homed before general move operations can be performed.
        /// </summary>
        public void PerformMoves()
        {
            OnPerformMoves().Wait();
        }

        /// <summary>
        /// Start monitoring the digital input device associated with an axis.
        /// Each axis has up to two general purpose inputs which can be monitored.
        /// </summary>
        public abstract void StartMonitoringDigitalInputs();

        /// <summary>
        /// Stop monitoring all digital input devices (<see cref="StartMonitoringDigitalInputs"/>.
        /// </summary>
        public abstract void StopMonitoringDigitalInputs();

        /// <summary>
        /// Derived classes will provide an implementation to initialize and home the axis/axes which make up the selected product type.
        /// </summary>
        /// <returns>A task which can be Waited on.</returns>
        public abstract Task OnInitialize();

        /// <summary>
        /// Derived classes will provide an implementation to perform a series of moves on the axis/axes which make up the selected product type.
        /// </summary>
        /// <returns>A task which can be Waited on.</returns>
        public abstract Task OnPerformMoves();
    }
}
