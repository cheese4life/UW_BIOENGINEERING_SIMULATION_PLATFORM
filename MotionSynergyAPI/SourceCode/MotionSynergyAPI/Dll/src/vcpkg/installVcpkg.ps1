#!powershell
#   The copyright to the computer program(s) herein is the property of
#   Dover Motion, Inc. This file is subject to the terms and
#   conditions found in the 'DoverLicense.txt' file included with this
#   source code package.

# This script will download and build vcpkg if it is not already installed.

# The current vcpkg release being used. This can be updated to access newer package releases. Remember to also update the corresponding baseline git hash in vcpkg-configuration.json.
$vcpkgReleaseTag = "2025.10.17"

$ErrorActionPreference = "Stop"

if($IsLinux)
{
    $vcPkgParentPath = "~/"
    $bootstrapScript = "./vcpkg/bootstrap-vcpkg.sh"
}
else 
{
    $vcPkgParentPath = $env:PROGRAMDATA
    $bootstrapScript = ".\vcpkg\bootstrap-vcpkg.bat"
}

$vcPkgPath = Join-Path -Path $vcPkgParentPath -ChildPath "vcpkg"
$vcPkgExePath = Join-Path -Path $vcPkgPath -ChildPath "/vcpkg"
$vcPkgConfigPath = Join-Path -Path $vcPkgPath -ChildPath ".git/config"

$isInstallRequired = $true

if (Get-Command $vcPkgExePath -ErrorAction SilentlyContinue)
{
    # Check the correct release is installed
    $config = Get-Content $vcPkgConfigPath -Raw
    $isInstallRequired = $config -notmatch "\+refs/tags/${vcpkgReleaseTag}"
    if($isInstallRequired)
    {
        Write-Host "vcpkg release at $vcPkgPath does not match version $vcpkgReleaseTag"
    }
    else 
    {
        Write-Host "vcpkg release $vcpkgReleaseTag found at $vcPkgPath"
    }
}

if($isInstallRequired)
{
    Write-Host "vcpkg release $vcpkgReleaseTag does not exist at path $vcPkgPath. Installing..."
    if (Test-Path $vcPkgPath)
    {
        Remove-Item -Recurse -Force -Path $vcPkgPath
    }

    New-Item -ItemType Directory -Force -Path $vcPkgParentPath | Out-Null
    Push-Location -Path $vcPkgParentPath | Out-Null
    git clone --depth 1 --branch $vcpkgReleaseTag https://github.com/microsoft/vcpkg.git
    . $bootstrapScript
    Pop-Location | Out-Null
} 

