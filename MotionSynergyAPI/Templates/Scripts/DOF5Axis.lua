--[[
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
--]]

--!
--! $PackageVersion$
--!
--! @brief Home the X Axis by finding the left limit and then moving to the center flag.
--!

require("ScriptPrelude").Run()
local Utilities = require("Utilities")
local Log = require("Utilities.LogWrapper").New("HomeXAxis")

function InitializeXAxisInput()

    -- Initialize the digital inputs associated with the X Axis if it's been configured (see Instrument.cfg)
    if XAxisDigitalInput ~= nil then
        Log:info("Initializing the X Axis digital input")
        XAxisDigitalInput.Initialize()
        -- Enable polling of the inputs at a frequency defined by DigitalInputPollingPeriodMs (see Instrument.cfg)
        XAxisDigitalInput.EnableAsynchChangeUpdates() 
        XAxisDigitalInput.Wait()
    end

end

function InitializeXAxis()
    Log:info("Initializing X Axis")

    XAxis.Initialize()

    InitializeXAxisInput()

    Log:info("X Axis initialization complete")
end

function HomeXAxisToCenter()

    XAxis.EnterHomingMode()
    XAxis.Wait()

    -- configure the homing profile
    XAxis.SelectMoveProfile("XAxisHomingProfile")
    XAxis.Wait()

    if XAxis.GetFlagStatus("HomePositiveHalfFlag") == true  then
            Log:info("HomePositiveHalfFlag == true")
            XAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "HomeNegativeHalfFlag")
            XAxis.Wait()
            XAxis.NMoveRelative("MoveToCenterOffset")
            XAxis.Wait()

    elseif XAxis.GetFlagStatus("HomeNegativeHalfFlag") == true  then
                Log:info("HomeNegativeHalfFlag == true")

    end

    assert(XAxis.GetFlagStatus("HomeNegativeHalfFlag") == true, "Expected to be in the negative half of the stage.")

    XAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "HomePositiveHalfFlag")
    XAxis.Wait()

    XAxis.NResetPosition("Home")
    XAxis.Wait()
    
    XAxis.ExitHomingMode()
    XAxis.Wait()
end

function HomeXAxis()
    Log:info("Homing X Axis")

    HomeXAxisToCenter()

end

if Utilities.IsScriptExecuting() then
    InitializeXAxis()
    HomeXAxis()
end

