#!/bin/bash
mkdir -p build_win32
cd build_win32
echo "Running qmake..."
export PATH=$MXE/usr/bin:$PATH
$MXE/usr/i686-w64-mingw32.shared.posix/qt5/bin/qmake ..
echo "Starting full elve build using $MXE..."
make -j4
echo "copying all dll from build..."
cp $MXE/usr/i686-w64-mingw32.shared.posix/bin/*.dll Elve/
cp $MXE/usr/i686-w64-mingw32.shared.posix/qt5/bin/*.dll Elve/
cp -r $MXE/usr/i686-w64-mingw32.shared.posix/qt5/plugins/platforms Elve/
echo "copying elve core dll..."
cp Core/*.dll Elve
echo "cleaning build..."
cd Elve
rm -f *.h *.o *.cpp
cd ..
echo "zipping"
zip -r ElveWin32.zip Elve
echo "Done"

