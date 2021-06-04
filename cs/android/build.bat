set ANDROID_NDK=E:\Download\android-ndk-r16b
set ANDROID_CMAKE=cmake.exe
set STRIP=%ANDROID_NDK%\toolchains\aarch64-linux-android-4.9\prebuilt\windows-x86_64\bin\aarch64-linux-android-strip.exe

if exist libs (
    rd /S /Q libs
    mkdir libs
)

@REM v8a
if not exist build-v8a (
    %ANDROID_CMAKE% -B build-v8a ^
        -G "Unix Makefiles"  ^
        -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%\build\cmake\android.toolchain.cmake"  ^
        -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe"  ^
        -DANDROID_ABI=arm64-v8a  ^
        -DANDROID_NATIVE_API_LEVEL=21  ^
        -DANDROID_ARM_MODE=arm
)
    
%ANDROID_CMAKE% --build ./build-v8a --config Release 
%STRIP% --strip-unneeded ./build-v8a/libcs.so  

mkdir libs\arm64-v8a
copy build-v8a\libcs.so libs\arm64-v8a\libcs.so

@REM v7a
set STRIP=%ANDROID_NDK%\toolchains\arm-linux-androideabi-4.9\prebuilt\windows-x86_64\bin\arm-linux-androideabi-strip.exe
if not exist build-v7a (
    %ANDROID_CMAKE% -B build-v7a ^
        -G "Unix Makefiles"  ^
        -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%\build\cmake\android.toolchain.cmake"  ^
        -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe"  ^
        -DANDROID_ABI=armeabi-v7a  ^
        -DANDROID_NATIVE_API_LEVEL=21  ^
        -DANDROID_ARM_MODE=arm 
)
    
%ANDROID_CMAKE% --build ./build-v7a --config Release 
%STRIP% --strip-unneeded ./build-v7a/libcs.so  

mkdir libs\armeabi-v7a
copy build-v7a\libcs.so libs\armeabi-v7a\libcs.so

@REM x86
set STRIP=%ANDROID_NDK%\toolchains\x86-4.9\prebuilt\windows-x86_64\bin\i686-linux-android-strip.exe
if not exist build-x86 (
    %ANDROID_CMAKE% -B build-x86 ^
        -G "Unix Makefiles"  ^
        -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%\build\cmake\android.toolchain.cmake"  ^
        -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe"  ^
        -DANDROID_ABI=x86  ^
        -DANDROID_NATIVE_API_LEVEL=21  ^
        -DANDROID_ARM_MODE=arm 
)
    
%ANDROID_CMAKE% --build ./build-x86 --config Release 
%STRIP% --strip-unneeded ./build-x86/libcs.so  

mkdir libs\x86
copy build-x86\libcs.so libs\x86\libcs.so


