ROOT=`pwd`
PROJ_DIR=$ROOT/projects/xcode_ios
BUILD_DIR=$ROOT/builds/xcode_ios
LIB_DIR=$ROOT/builds/xcode_ios/libs
BIN_DIR=$ROOT/builds/xcode_ios/bins
rm -rf $PROJ_DIR
mkdir -p $PROJ_DIR
mkdir -p $LIB_DIR
mkdir -p $BIN_DIR
pushd $PROJ_DIR
cmake \
-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=$LIB_DIR \
-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=$LIB_DIR \
-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$BIN_DIR \
-DNIMBLE_TARGET_IOS=1 \
-DNIMBLE_OPENGLES_3_0=1 \
-DXCODE=1 \
-G "Xcode" $ROOT
popd