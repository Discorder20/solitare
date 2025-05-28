cmake_minimum_required(VERSION 3.16)

project(solitare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory( lib/json )

add_executable(solitare main.cpp
    page.h
    engine.h engine.cpp
    constants.h
    menupage.h menupage.cpp
    colors.h
    howtopage.h howtopage.cpp
    colors.cpp
    card.h card.cpp
    cardstack.h
    regularcardstack.h regularcardstack.cpp
    outstandingcardstack.h outstandingcardstack.cpp
    cardcontainer.h cardcontainer.cpp
    jsonhandler.h jsonhandler.cpp
    game.h game.cpp
    modepage.h modepage.cpp
    newgamepage.h newgamepage.cpp
    loadgamepage.h loadgamepage.cpp
    recordpage.h recordpage.cpp

)
target_link_libraries(solitare PRIVATE -static nlohmann_json)
include(GNUInstallDirs)
install(TARGETS solitare
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
