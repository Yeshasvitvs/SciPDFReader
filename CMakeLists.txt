cmake_minimum_required(VERSION 2.8)

project(SciPDFReader)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

# Populate a CMake variable with the headers and sources
set(HEADERS include/mainwindow.h
            include/PDFManager.h
)

set(SOURCES src/main.cpp
            src/mainwindow.cpp
            src/PDFManager.cpp
)

qt5_add_resources(RESOURCES resources/resource.qrc)
add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${RESOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include -I /usr/include/poppler/qt5 -I /usr/include/poppler)

# Use the Widgets module from Qt 5
target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${QT_LIBRARIES} -lpoppler-qt5)
