# vendor folder
If there is no possible way to install the dependencies using vcpkg, you can copy the include folder of the dependencies into this folder. This is not recommended, but it is a workaround if you are unable to install the dependencies using vcpkg.

# CMakeLists.txt configuration
You should add the following lines to your CMakeLists.txt file to include the dependencies from the vendor folder:

```cmake
target_include_directories(CropIQ 
  PUBLIC 
    ...
    ${VENDOR_DIR}/dependency_name
)