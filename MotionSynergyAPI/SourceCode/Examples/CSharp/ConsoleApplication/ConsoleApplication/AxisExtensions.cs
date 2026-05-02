/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

using MotionSynergyAPI;

namespace ConsoleApplication
{
    /// <summary>
    /// Class used to defined extension methods to the <see cref="IAxis"/> interface.
    /// </summary>
    public static class AxisExtensions
    {
        /// <summary>
        /// Generate a profile name for a specific axis (i.e. Axis name + profile name).
        /// </summary>
        /// <param name="axis">Axis to generate the profile name for</param>
        /// <param name="profileName">Profile name not specific to a particular axis.</param>
        /// <returns>A Profile name specific to a particular axis.</returns>
        public static string ProfileName(this IAxis axis, string profileName) => $"{axis.Name}{profileName}";
    }
}
