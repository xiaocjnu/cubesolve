#!/bin/bash
LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"
DESTDIR="."

if [ -d "./libs" ]
then
    rm -rf ./libs
    mkdir ./libs
fi

# arm64
cmake -B build -G "Xcode" \
    -DCMAKE_TOOLCHAIN_FILE=../ios-cmake/ios.toolchain.cmake \
    -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/ \
    -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk \
    -DENABLE_BITCODE=FALSE -DPLATFORM=OS -DARCHS=arm64

cmake --build "build" --config Release

cp ./build/Release-iphoneos/libcs.a ./libs/libcs-arm64.a

# armv7
cmake -B build -G "Xcode" \
    -DCMAKE_TOOLCHAIN_FILE=../ios-cmake/ios.toolchain.cmake \
    -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/ \
    -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk \
    -DENABLE_BITCODE=FALSE -DPLATFORM=OS -DARCHS=armv7

cmake --build "build" --config Release

cp ./build/Release-iphoneos/libcs.a ./libs/libcs-armv7.a

$LIPO -create libs/libcs-*.a -output "$DESTDIR"/libcs.a
$STRIP -S "$DESTDIR"/libcs.a
