tmp=${VCINSTALLDIR//\\//}
vcbase=${tmp//c://c}
vcbase=${vcbase//C://c}
echo $vcbase
export PATH=$vcbase/bin:$PATH
echo $PATH
cmd /c buildwin 90 build shared both Win32 samples vcexpress
