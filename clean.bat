@echo off

if exist tmp rd /s /q tmp

mkdir tmp
pushd tmp

cmake ../

popd