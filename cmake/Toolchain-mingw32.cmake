# Toolchain configuration for SUSE/Fedora like MinGW32 setup

# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)
#SET(CMAKE_SYSTEM_VERSION 1)
SET(COMPILER_PREFIX x86_64-w64-mingw32)
#SET (CMAKE_C_COMPILER_WORKS   1)
SET (CMAKE_CXX_COMPILER_WORKS 1)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER ${COMPILER_PREFIX}-gcc)
SET(CMAKE_RC_COMPILER ${COMPILER_PREFIX}-windres)
SET(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}-g++)
SET(CMAKE_AR ${COMPILER_PREFIX}-ar)
SET(CMAKE_LD ${COMPILER_PREFIX}-ld)
SET(CMAKE_NM ${COMPILER_PREFIX}-nm)
SET(CMAKE_OBJCOPY ${COMPILER_PREFIX}-objcopy)
SET(CMAKE_OBJDUMP ${COMPILER_PREFIX}-objdump)
SET(CMAKE_RANLIB ${COMPILER_PREFIX}-ranlib)
SET(CMAKE_STRIP ${COMPILER_PREFIX}-strip)
SET(CMAKE_MC ${COMPILER_PREFIX}-windmc)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /home/jan.hoelscher/devel/mingw-64 )

#SET(CMAKE_RC_COMPILE_OBJECT
#    "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -O coff -o <OBJECT> <SOURCE>")

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
# Windows libraries names
#set(WIN_LIB_ICONV) # builtin
#set(WIN_LIB_INTL libintl-8.dll)
#set(WIN_LIB_CURL libcurl-4.dll libidn-11.dll libnspr4.dll nss3.dll libssh2-1.dll ssl3.dll zlib1.dll nssutil3.dll libplc4.dll libplds4.dll libgcrypt-11.dll libgpg-error-0.dll)
#set(WIN_LIB_MYSQL libmysql.dll)
#set(WIN_LIB_PGSQL libpq.dll)
#set(WIN_LIB_GLIB libglib-2.0-0.dll libgobject-2.0-0.dll libiconv-2.dll libgthread-2.0-0.dll)

# Disable pkg-config lookups
#set(PKG_CONFIG_EXECUTABLE /bin/false)
