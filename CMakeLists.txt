set(TARGET logger)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets)

add_library(${TARGET}
    STATIC
    logger.h
    logger.cpp
)
                           
target_link_libraries(${TARGET} PUBLIC Qt5::Core Qt5::Widgets)
