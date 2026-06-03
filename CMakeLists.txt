cmake_minimum_required(VERSION 3.10)
project(SeniorShop)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(JNI REQUIRED)

find_package(SQLite3 REQUIRED)

include_directories(
    include
    ${JNI_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
)

add_executable(SeniorShop
    src/Main.cpp
    src/AuthSystemUser.cpp
    src/Captcha.cpp
    src/Cart.cpp
    src/Check.cpp
    src/DatabaseStorage.cpp
    src/Logger.cpp
    src/LoginSystem.cpp
    src/MethodSuperAdmin.cpp
    src/Product.cpp
    src/SaleControl.cpp
    src/SaleService.cpp
    src/SellActiv.cpp
    src/Storage.cpp
    src/SupplierManager.cpp
    src/Supply.cpp
    src/SupplyManager.cpp
    src/User.cpp
    jni/android_jni_bridge.cpp
)

target_link_libraries(SeniorShop
    ${JNI_LIBRARIES}
    ${SQLite3_LIBRARIES}
    pthread
    dl
)

include(GNUInstallDirs)
install(TARGETS SeniorShop
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
