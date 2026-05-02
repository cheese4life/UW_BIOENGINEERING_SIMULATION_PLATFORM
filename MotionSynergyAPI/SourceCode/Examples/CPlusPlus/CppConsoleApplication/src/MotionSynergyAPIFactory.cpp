/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "MotionSynergyAPIFactory.h"

#ifdef MOTIONSYNERGYAPI_DLL
    #include "MotionSynergyAPISingleton.h"

namespace MotionSynergyAPI
{
IMotionSynergySPtr
MotionSynergyAPIFactory::Create()
{
    return MotionSynergyAPISingleton::Get();
}

void
MotionSynergyAPIFactory::Destroy()
{
    MotionSynergyAPISingleton::Destroy();
}

} // namespace MotionSynergyAPI

#else

// Build directly from source files

    #include "MotionSynergyAPI.h"

namespace MotionSynergyAPI
{

IMotionSynergySPtr
MotionSynergyAPIFactory::Create()
{
    return std::make_shared<MotionSynergyAPI>();
}
void
MotionSynergyAPIFactory::Destroy()
{
    // Nothing to do - the shared pointer will clean up the instance.
}

} // namespace MotionSynergyAPI
#endif
