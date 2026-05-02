## Public Interface
### MotionSynergyAPISingleton Class
```c++
#include <MotionSynergyAPISingleton.h>

class MotionSynergyAPISingleton 
```
Singleton responsible for loading the MotionSynergyAPI (DLL) on demand.   
  

#### Public Methods  

##### Destroy()  

```c++
static void Destroy()
```
Prepares for shutdown, releases the IMotionSynergy and unloads the DLL.   
  

##### Get()  

```c++
static IMotionSynergySPtr Get()
```
Retrieve the singleton IMotionSynergy instance. This will load the DLL if it's not already loaded.   
  


Returns:  

   Pointer to the created IMotionSynergy.   


Exceptions:  

   std::runtime error if the DLL cannot be loaded   

---
