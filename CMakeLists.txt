cmake_minimum_required(VERSION 3.16)

project(SeniorShop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SeniorShop Main.cpp
    Storage.h Storage.cpp
    Supply.h Supply.cpp
    SupplyManager.h SupplyManager.cpp
    AuthSystemUser.h AuthSystemUser.cpp
    Logger.h Logger.cpp
    SellActiv.h SellActiv.cpp
    Product.h Product.cpp
    User.h User.cpp
    LoginSystem.h LoginSystem.cpp
    Captcha.h Captcha.cpp
    MethodSuperAdmin.h MethodSuperAdmin.cpp
    Cart.h Cart.cpp
    SaleControl.h SaleControl.cpp
    SaleService.h SaleService.cpp
    Check.h Check.cpp

)

include(GNUInstallDirs)
install(TARGETS SeniorShop
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
