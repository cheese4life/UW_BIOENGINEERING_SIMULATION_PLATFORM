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

    -- Home to the center of the stage

    -- If in the positive half of the stage move to the negative half flag
    -- and then move a further small offset so homing to the center is always performed
    -- from the same direction (i.e in the positive direction).
    if XAxis.GetFlagStatus("PositiveHalf") == true or XAxis.GetFlagStatus("PositiveLimit") == true then
            XAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeHalf")
            XAxis.Wait()
            XAxis.NMoveRelative("MoveToCenterOffset")
            XAxis.Wait()
        end

    assert(XAxis.GetFlagStatus("NegativeHalf") == true or XAxis.GetFlagStatus("NegativeLimit") == true, "Expected to be in the negative half of the stage.")

    XAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveHalf")
    XAxis.Wait()

    XAxis.NResetPosition("Home")
    XAxis.Wait()
    
    XAxis.ExitHomingMode()
    XAxis.Wait()
end

function HomeXAxisToNegativeLimit()
    Log:info("Homing X axis")

    XAxis.EnterHomingMode()
    XAxis.Wait()

    -- configure the homing profile
    XAxis.SelectMoveProfile("XAxisHomingProfile")
    XAxis.Wait()

    -- If already in the negative limit, move out 10mm and rehome to the limit.
    if XAxis.GetFlagStatus("NegativeLimit") == true then
            XAxis.MoveRelative(10)
            XAxis.Wait()
        end

    XAxis.NMoveRelativeToFlag("MaxNegativeHomingDistance", "NegativeLimit")
    XAxis.Wait()

    assert(XAxis.GetFlagStatus("NegativeLimit") == true, "Expected to be in the negative limit.")

    XAxis.NResetPosition("Home")
    XAxis.Wait()
    
    XAxis.ExitHomingMode()
    XAxis.Wait()
end

function HomeXAxisToPositiveLimit()
    Log:info("Homing X axis")

    XAxis.EnterHomingMode()
    XAxis.Wait()

    -- configure the homing profile
    XAxis.SelectMoveProfile("XAxisHomingProfile")
    XAxis.Wait()

    -- If already in the negative limit, move out 10mm and rehome to the limit.
    if XAxis.GetFlagStatus("PositiveLimit") == true then
            XAxis.MoveRelative(-10)
            XAxis.Wait()
        end

    XAxis.NMoveRelativeToFlag("MaxPositiveHomingDistance", "PositiveLimit")
    XAxis.Wait()

    assert(XAxis.GetFlagStatus("PositiveLimit") == true, "Expected to be in the positive limit.")

    XAxis.NResetPosition("Home")
    XAxis.Wait()
    
    XAxis.ExitHomingMode()
    XAxis.Wait()
end

function HomeXAxis()
    Log:info("Homing X Axis")

    -- Default is to home the axis to the center
    HomeXAxisToCenter()

    -- Alternatively, the axis can be homed to the negative or positive limit
    -- HomeXAxisToNegativeLimit()
    -- HomeXAxisToPositiveLimit()

end

if Utilities.IsScriptExecuting() then
    InitializeXAxis()
    HomeXAxis()
end

