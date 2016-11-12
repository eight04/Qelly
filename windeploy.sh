cd bin
rm *.zip
d=qelly-`git rev-parse --short HEAD`
mkdir $d
cd $d
cp ../release/qelly.exe .
windeployqt qelly.exe
cfp () {
    cp $(which $1) $2 
}
cfp QXTCORE.DLL .
cfp QXTWIDGETS.DLL .
cd ..
zip -r $d $d
rm -r $d
