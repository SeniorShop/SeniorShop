cmake_minimum_required(VERSION 3.10)
project(SeniorShop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(SQLite3 REQUIRED)
find_package(JNI REQUIRED)

set(SOURCES
    src/Storage.cpp
    src/Product.cpp
    src/SupplyManager.cpp
    src/SupplierManager.cpp
    src/Check.cpp
    src/User.cpp
    src/AuthSystemUser.cpp
    src/Captcha.cpp
    src/SellActiv.cpp
    src/Logger.cpp
    src/Cart.cpp
    src/SaleControl.cpp
    src/SaleService.cpp
    src/Supply.cpp
    src/MethodSuperAdmin.cpp
    src/DatabaseStorage.cpp
    jni/android_jni_bridge.cpp
)

add_library(native SHARED ${SOURCES})

target_include_directories(native PRIVATE 
    include 
    ${SQLite3_INCLUDE_DIRS}
    ${JNI_INCLUDE_DIRS}
)

target_link_libraries(native 
    ${SQLite3_LIBRARIES}
    ${JNI_LIBRARIES}
    pthread
    dl
)

install(TARGETS native LIBRARY DESTINATION libs)
