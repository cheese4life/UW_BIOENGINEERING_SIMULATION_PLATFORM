# Package Management Using the vcpkg Package Manager

The MotionSynergyAPI uses [vcpkg](https://github.com/microsoft/vcpkg) for package management of 3rd-party C++ libraries (SOUP).  

The vcpkg package manager was chosen (in October 2025) for the following reasons:  

  * Works with C++ libraries.
  * Integrates well with CMake (really just extends CMake).
  * Has packages for the current set of 3rd-party C++ libraries (SOUP) used by MotionSynergyAPI.
  * Supports Windows and Linux.
  * Builds from source but supports binary caching to reduce build times.
  * Relatively low learning curve (leverages existing knowledge of git and CMake).

The Conan package manager was considered but rejected for the following reasons:  

  * Higher learning curve - many comments on the web indicate it should be easy to use but people run into problems.
    * https://www.reddit.com/r/cpp/comments/11xj8np/conan_package_manager_completely_broken_after_20/
    * https://www.reddit.com/r/cpp_questions/comments/pe5zbv/why_is_conan_broken_so_often_on_my_system/
    * https://decovar.dev/blog/2022/10/30/cpp-dependencies-with-vcpkg/
  * Conan 2.0 is relatively new and while it has some nice features, it has introduced instability with existing packages (see above).
  * Tightly coupled to Artifactory for caching of binary packages.

## How vcpkg is used by MotionSynergyAPI

Vcpkg has been seamlessly integrated into the current developer workflow and build system. There is no need for any manual installation steps.

  1. The bootstrapping scripts [createWindows.ps1](../createWindows.ps1) and [createLinux.sh](../createLinux.sh) will automatically download and install vckpg
  by calling [installVcpkg.ps1](installVcpkg.ps1).
  1. The [installVcpkg.ps1](installVcpkg.ps1) script:
      1. Checks to see if the specified release of vckpg is already installed. If the correct version is installed, no further action is taken.
      1. Downloads vcpkg from the [vcpkg github repo](https://github.com/microsoft/vcpkg) and runs the bootstrapping script to build / download the vcpkg command line tool. Note that the vcpkg repo contains a list of packages ([ports](https://learn.microsoft.com/en-us/vcpkg/concepts/ports)) and the source for the vcpkg command line tool.
  1. The Vcpkg installation is located at `%PROGRAMDATA%\vcpkg` on Windows and `~/vckpg` on Linux.
  1. The CMake toolchain files (e.g. [Toolchain_WindowsVS2022_x64.cmake](../CMake/Toolchain_WindowsVS2022_x64.cmake) used to build MotionSynergyAPI integrate vcpkg into the CMake build system as follows:
      1. Define the vcpkg [Triplet](https://learn.microsoft.com/en-us/vcpkg/concepts/triplets) configuration set to use for the build (e.g. x64/Windows/static linking).
      1. Includes (invokes) the vcpkg cmake script `vcpkg/scripts/buildsystems/vcpkg.cmake`. This will download and install the 3rd-party C++ libraries used by MotionSynergyAPI.
  1. The 3rd-party package dependencies used by MotionSynergyAPI are specified in [vcpkg.json](vcpkg.json).
  1. The list of vcpkg [registries](https://learn.microsoft.com/en-us/vcpkg/concepts/registries) and their associated baselines used by vcpkg to locate packages is specified in [vcpkg-configuration.json](vcpkg-configuration.json)
  1. There is a [local registry](./local-registry/) that can be used to store customised 3rd-party ports and MotionSynergyAPI ports. 

## Vcpkg version management

In a modern development environment, it's important to carefully manage the versions of all 3rd-party libraries (i.e. COTS). All COTS libraries are pinned to a specific version and only
updated using an explicit software change that's reviewed using the standard change control process. This pinning of versions is achieved in two ways:

  1. A vcpkg `baseline` is specified in [vcpkg-configuration.json](vcpkg-configuration.json). The baseline specifies a git hash of a specific release of the vcpkg github repository. This effectively snapshots the set of package versions
     available from this repository. It prevents newer package versions being automatically available as they are released.
  1. Each package listed in [vcpkg.json](vcpkg.json) specifies a pinned version in the `overrides` section. This ensures the exact version of a package is used.

Note that the CMake `find_package()` function allows an `EXACT VERSION` to be specified. This feature is not used because the version matching implementation is up to each package and many packages either don't implement version matching at all
or do not fail the build if an exact version is not available.

### Updating a package to a new version

There are a number of steps required to update a vcpkg to a newer version.

  1. Update to the latest vcpkg release to make newer package versions available. This involves:
     1. Update the `$vcpkgReleaseTag` in [installVcpkg.ps1](installVcpkg.ps1).
     1. Update the `baseline` git hash in [vcpkg-configuration.json](vcpkg-configuration.json) to the corresponding release.
  1. Update the individual pinned version for the package in [vcpkg.json](vcpkg.json)
  1. Update the MotionSynergyAPI source code to remain compatible with the new package version (if required).
  1. Carry out the necessary regression testing.
