/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "CppConsoleApplication.h"
#include "CmdLineArgumentParser.h"
#include "DMCM.h"
#include "DOF5.h"
#include "IMotionSynergy.h"
#include "MotionSynergyAPIFactory.h"
#include "SmartStageLinear.h"
#include "SmartStageXY.h"
#include "SmartStageXYZ.h"

#include <iostream>


using namespace MotionSynergyAPI;

/*!
 * @brief   Example of a C++ application using the DoverMotion C++ source code to
 *          control the SmartStage XY, DOF-5 and DMCM products.
 */
int
main( int argc, char** argv )
{
    bool wasSuccessful = true;

    // If an error occurs at any stage an exception will be thrown. Catch the error and
    // write it to the console. Note that all infrastructure will be cleaned up correctly
    // when the exception is thrown.
    try
    {
        // Default paths
        std::string instrumentConfigPathname( "[SupportFolder]/Instrument.cfg" );
        std::string logConfigPathname( "[SupportFolder]/Logger.cfg" );
        const std::string logFolderPathname = "[ProgramDataFolder]/logs";

        if ( !CmdLineArgumentParser::Parse( argc, argv, instrumentConfigPathname, logConfigPathname ) )
        {
            return -1;
        }

        // Create an instance of MotionSynergyAPI, passing in various paths
        auto pMotionSynergyAPI = MotionSynergyAPIFactory::Create();

        // Configure the MotionSynergyAPI infrastructure ready for use, which includes opening the log file(s).
        pMotionSynergyAPI->Configure( instrumentConfigPathname, logConfigPathname, logFolderPathname );

        // Get a list of products/axes specified in the configuration file.
        const auto productList = pMotionSynergyAPI->GetProductList();
        if ( productList.empty() )
        {
            std::cerr << "No products/axes specified in configuration file " << instrumentConfigPathname << std::endl;
            return -1;
        }

        // Cycle through each product in turn.

        for ( const auto& productInfo : productList )
        {
            const auto productType = productInfo.ProductType;
            const auto productVariant = productInfo.ProductVariant;
            const auto productName = !productVariant.empty() && productVariant != "None" ? productVariant : productType;
            const auto& axisNames = productInfo.AxisNames;

            std::shared_ptr<Product> pProduct;

            if ( productType == "SmartStageLinear" )
            {
                // Create an instance of the SmartStageLinear class which demonstrates how to use the API to control the stage.
                pProduct =
                    std::make_shared<SmartStageLinear>( productName, pMotionSynergyAPI->GetAxis( axisNames[ 0 ] ) );
            }
            else if ( productType == "SmartStageXY" )
            {
                // Create an instance of the SmartStageXY class which demonstrates how to use the API to control the stage.
                pProduct = std::make_shared<SmartStageXY>(
                    productName,
                    pMotionSynergyAPI->GetAxis( axisNames[ 0 ] ),
                    pMotionSynergyAPI->GetAxis( axisNames[ 1 ] ) );
            }
            else if ( productType == "SmartStageXYZ" )
            {
                // Create an instance of the SmartStageXYZ class which demonstrates how to use the API to control an XY stage and a DOF-5.
                pProduct = std::make_shared<SmartStageXYZ>(
                    productName,
                    "DOF-5",
                    pMotionSynergyAPI->GetAxis( axisNames[ 0 ] ),
                    pMotionSynergyAPI->GetAxis( axisNames[ 1 ] ),
                    pMotionSynergyAPI->GetAxis( axisNames[ 2 ] ) );
            }
            else if ( productType == "DOF5" )
            {
                // Create an instance of the DOF5 class which demonstrates how to use the API to control the DOF-5.
                pProduct = std::make_shared<DOF5>( productName, pMotionSynergyAPI->GetAxis( axisNames[ 0 ] ) );
            }
            else if ( productType == "DMCM" )
            {
                // Create an instance of the DOF5 class which demonstrates how to use the API to control the DMCM.
                pProduct = std::make_shared<DMCM>( productName, pMotionSynergyAPI->GetAxis( axisNames[ 0 ] ) );
            }
            else
            {
                std::cerr << "Unknown ProductType '" << productType << "' specified in configuration file "
                          << instrumentConfigPathname << std::endl;
                return -1;
            }

            // Initialize each axis - this connects to each axis, performs analog calibration, commutation and homes each axis.
            pProduct->Initialize();

            // Start monitoring the associated digital input(s) if enabled.
            for ( auto& axisName : axisNames )
            {
                const auto digitalInputName = axisName + "DigitalInput";
                if ( pMotionSynergyAPI->IsDigitalInputEnabled( digitalInputName ) )
                {
                    pProduct->StartMonitoringDigitalInput(
                        pMotionSynergyAPI->GetDispatcher(),
                        pMotionSynergyAPI->GetDigitalInput( digitalInputName ) );
                }
            }

            // Perform a series of axis moves
            pProduct->PerformMoves();

            // Stop monitoring the associated digital input(s).
            pProduct->StopMonitoringDigitalInputs();

            // Free the example product object.
            pProduct.reset();
        }


        // Shutdown MotionSynergyAPI - this will close the connection to each axis, destroy the MotionSynergyAPI infrastructure and close the logs.
        pMotionSynergyAPI->Shutdown();
        pMotionSynergyAPI.reset();
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        wasSuccessful = false;
    }

    MotionSynergyAPIFactory::Destroy();

    return wasSuccessful ? 0 : -1;
}
