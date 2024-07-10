# Jacob Lowe
# Dev setup script

# Install Chocolatey
# if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
#     Set-ExecutionPolicy Bypass -Scope Process -Force
#     [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
#     iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
# }

# Install make using Chocolatey
#choco install make -y

# Installing Conan for imgui
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
Write-Host "Installing Conan for imgui..." -ForegroundColor Green
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
pip install conan
# Set the path to the MSYS2 installation directory
$msys2Path = "C:\msys64"

Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
Write-Host "Installing packages..." -ForegroundColor Green
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
Write-Host "Once the packages are installed in the msys2 terminal"
# Launch the MSYS2 terminal and run the script
cd $msys2Path
# Search for packages with pacman -Ss (package name), -Qs (search in system)
# mingw-w64-x86_64-(package)
.\msys2_shell.cmd -mingw64 -c "pacman -Syuu && pacman -Sy mingw-w64-x86_64-SDL make mingw-w64-x86_64-boost mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-jsoncpp mingw-w64-x86_64-make mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf"
Pause
cd $PSScriptRoot
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
Write-Host "Compiling program for the first time..." -ForegroundColor Green
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
make
Write-Host
Write-Host 
Write-Host  
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
Write-Host "Install complete! Press any key to close the window." -ForegroundColor Green
Write-Host "You can now compile program by using the command" -ForegroundColor Yellow
Write-Host "> make"
Write-Host "You make also delete all compiled files by typing" -ForegroundColor Yellow
Write-Host "> make cleanall"
Write-Host "Launch the game by typing the following or just click the exe in the directory" -ForegroundColor Yellow
Write-Host "> .\TheOne.exe"
Write-Host "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" -ForegroundColor Magenta
Pause
exit
