cmake_minimum_required(VERSION 3.16)

project(SeniorShop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SeniorShop src/Main.cpp
    include/Storage.h src/Storage.cpp
    include/Supply.h src/Supply.cpp
    include/SupplyManager.h src/SupplyManager.cpp
    include/AuthSystemUser.h src/AuthSystemUser.cpp
    include/Logger.h src/Logger.cpp
    include/SellActiv.h src/SellActiv.cpp
    include/Product.h src/Product.cpp
    include/User.h src/User.cpp
    include/LoginSystem.h src/LoginSystem.cpp
    include/Captcha.h src/Captcha.cpp
    include/MethodSuperAdmin.h src/MethodSuperAdmin.cpp
    include/Cart.h src/Cart.cpp
    include/SaleControl.h src/SaleControl.cpp
    include/SaleService.h src/SaleService.cpp
    include/SupplierManager.h
    include/Check.h src/Check.cpp
    src/SupplierManager.cpp
)

include(GNUInstallDirs)
install(TARGETS SeniorShop
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
