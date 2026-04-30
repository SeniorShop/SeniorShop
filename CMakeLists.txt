cmake_minimum_required(VERSION 3.16)

project(SeniorShop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SeniorShop Main.cpp
    AuthSystemUser.h AuthSystemUser.cpp
    Logger.h
    Product.h
    User.h User.cpp
    Logger.cpp
    LoginSystem.h LoginSystem.cpp
    Captcha.h Captcha.cpp
    Storage.cpp
    Storage.h
    Product.cpp
)

include(GNUInstallDirs)
install(TARGETS SeniorShop
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

