tmp=${VCINSTALLDIR//\\//}
vcbase=${tmp//c://c}
vcbase=${vcbase//C://c}
echo $vcbase
export PATH=$vcbase/bin:$PATH
echo $PATH
cmd /c build_vcexpress2008