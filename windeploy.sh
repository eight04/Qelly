cd bin
gfind . -maxdepth 1 -type f -name *.zip -delete
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
