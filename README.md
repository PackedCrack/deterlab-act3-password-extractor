https://www.isi.deterlab.net/file.php?file=/share/shared/Computerforensics/index.html#act3


## Compiler Support
Built and tested with __GCC 13.2__.

# Build
### Pre steps
* /cppcheck.sh must be executable -> sudo chmod +x cppcheck.sh
* Install cppcheck -> sudo apt install cppcheck
* Install cmake -> sudo apt install cmake

## Building with Ninja
* sudo apt install ninja-build
* cd root
* mkdir build
* cd build
#### Debug Configuration
* cmake .. -G Ninja -D CMAKE_BUILD_TYPE=Debug
#### ReleaseWithDebugInfo Configuration
* cmake .. -G Ninja -D CMAKE_BUILD_TYPE=RelWithDebInfo
#### MinSizeRelease Configuration
* cmake .. -G Ninja -D CMAKE_BUILD_TYPE=MinSizeRel
#### Release Configuration
* cmake .. -G Ninja -D CMAKE_BUILD_TYPE=Release
#### Compile and Link
ninja

## Removing Cppcheck as build dependency
Cppcheck can be removed as a custom build target in case it causes troubles (cppcheck sometimes complains about missing headers when pch is used). Or in case you just don't want to install it..
* open root/CMakeLists.txt
* Remove line 29 - "add_dependencies(${MAIN_PROJECT} cppcheck)"


## Running
* cd root/cmake-build-\<_configuration_>/bin/
* ./MAIN \<args...>

### Arguments
* *&lt;filepath-to-data-folder*&gt;

#### Data folder layout
data\
├── act3-cpy.img\
├── swiss_keys\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── swisskey1.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey2.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey3.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey4.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey5.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey6.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey7.gpg\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── swisskey8.gpg\
