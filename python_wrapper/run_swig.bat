echo off

set outdir="./out"
set cppdev=".."


swig -c++ -python -modern -py3 -I%cppdev%/my_model -I%cppdev%/cs -I%cppdev%/tools -I%cppdev%/snappy -o mycube_wrap.cpp myAA.i