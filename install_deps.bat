echo on
REM Downloading boost_1_60_0 precompiled libs for windows
wget.exe http://iweb.dl.sourceforge.net/project/boost/boost-binaries/1.60.0/boost_1_60_0-msvc-12.0-32.exe -P build/downloads
REM Installing boost_1_60_0 precompiled libs for windows
build\boost_1_60_0-msvc-12.0-32.exe /VERYSILENT /NORESTART
::set envyromet variable for boost
setx BOOST_ROOT C:\local\boost_1_60_0
REM boost_1_60_0 instaled correctly at C:\local\boost_1_60_0

::Ahora instalamos mysql-connector-c++-1.1.7-win32
wget.exe dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-1.1.7-win32.msi -P build/downloads
build\mysql-connector-c++-1.1.7-win32.msi /quiet /NORESTART
::restart sytem to apply system varibles
shutdown -r
