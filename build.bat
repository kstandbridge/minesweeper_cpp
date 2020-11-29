@echo off

if not exist tmp mkdir tmp
pushd tmp

cmake --build .

popd