/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>
#include <vector>

namespace MotionSynergyAPI
{

/*!
 *  @brief  Structure containing information about a product defined in the configuration.
 */
class ProductInfo
{
public:
    /*!
     *  @brief  Default constructor initializes members to default values.
     */
    ProductInfo()
        : ProductType( "Undefined" )
        , ProductVariant( "None" )
    {
    }

    /*!
     * @brief  Type of product (e.g. SmartStageXY, DOF5, etc).
     */
    std::string ProductType;

    /*!
     * @brief  Variant of the product (e.g. DOF10, SmartStageMonolithic, SmartStageOF, SmartStageShuttle, etc).
     */
    std::string ProductVariant;

    /*!
     * @brief  List of axis names associated with this product.
     */
    std::vector<std::string> AxisNames;
};

} // namespace MotionSynergyAPI
