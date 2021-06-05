cmake -B build -G "Xcode" \
    -DCMAKE_TOOLCHAIN_FILE=../ios-cmake/ios.toolchain.cmake \
    -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/ \
    -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk \
    -DENABLE_BITCODE=FALSE -DPLATFORM=OS

cmake --build "build" --config Release
