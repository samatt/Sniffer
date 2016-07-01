
BUILD=`uname -s`
OS="linux"
LIB_OSX="https://dl.dropboxusercontent.com/u/1108171/WifiWhisperer-Moogfest/libtins-osx/lib.zip"
LIB_LINUX="https://dl.dropboxusercontent.com/u/1108171/WifiWhisperer-Moogfest/libtins-linux/lib.zip"
LIB=""
LIB_DIR="lib"

if [  $BUILD = "Darwin" ];
then
  echo 'Im a Mac!'
  OS="osx"
  LIB=$LIB_OSX
 else
  echo 'Im a Pi!'
  LIB=$LIB_LINUX
fi

get_lib ()
{
  echo 'Downloading '$OS' build...'
  wget -O temp.zip $LIB
  unzip temp.zip -d  ./

  if [ -d "__MACOSX" ]; then
    rm -rf __MACOSX
    echo 'Deleting garbage'
  fi
}

if [ -d "$LIB_DIR" ]; then
    echo 'lib already exists.'
else
  echo 'lib doesnt exists. Downloading...'
  get_lib
fi

if [ ! -d "build" ]; then
    mkdir build
fi

TINS_LIB_PATH="lib/libtins/"$OS"/lib"
TINS_INCLUDE_PATH="lib/libtins/"$OS"/include/"
APP_NAME="build/tinsSniffer"
SRC=""

# NOTE: Make sure you have libpcap installed on your machine
# If using a pi: sudo apt-get install libpcap-dev
g++ -std=c++11 $SRC"main.cpp" -o $APP_NAME -L$TINS_LIB_PATH  -I$TINS_INCLUDE_PATH -lpthread -ltins -lpcap

