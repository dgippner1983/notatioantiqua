# Install script for directory: /Users/davidgippner/Programmierung/Qt/notatioantiqua

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

set(CMAKE_BINARY_DIR "/Users/davidgippner/Programmierung/Qt/notatioantiqua")

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/davidgippner/Programmierung/Qt/notatioantiqua/Debug/notatioantiqua.app" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/davidgippner/Qt/6.7.1/macos/lib"
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/davidgippner/Programmierung/Qt/notatioantiqua/Release/notatioantiqua.app" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/davidgippner/Qt/6.7.1/macos/lib"
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/davidgippner/Programmierung/Qt/notatioantiqua/MinSizeRel/notatioantiqua.app" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/davidgippner/Qt/6.7.1/macos/lib"
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
    endif()
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/davidgippner/Programmierung/Qt/notatioantiqua/RelWithDebInfo/notatioantiqua.app" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/davidgippner/Qt/6.7.1/macos/lib"
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/./notatioantiqua.app/Contents/MacOS/notatioantiqua")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    include("/Users/davidgippner/Programmierung/Qt/notatioantiqua/.qt/deploy_notatioantiqua_bf85a46c7d-Debug.cmake")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    include("/Users/davidgippner/Programmierung/Qt/notatioantiqua/.qt/deploy_notatioantiqua_bf85a46c7d-Release.cmake")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    include("/Users/davidgippner/Programmierung/Qt/notatioantiqua/.qt/deploy_notatioantiqua_bf85a46c7d-MinSizeRel.cmake")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    include("/Users/davidgippner/Programmierung/Qt/notatioantiqua/.qt/deploy_notatioantiqua_bf85a46c7d-RelWithDebInfo.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/davidgippner/Programmierung/Qt/notatioantiqua/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
