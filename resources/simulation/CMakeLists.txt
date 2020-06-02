cmake_minimum_required(VERSION 3.5)
project(CrawlerSim
        VERSION 0.1.0
        LANGUAGES CXX)

SET(SKETCH crawler_sim)

SET(MODEL_FILES
        Crawler.h
        Crawler.cpp
        Crawler_vehicle.h
        Crawler_vehicle.cpp
        ChCrawlerVehicle.h
        ChCrawlerVehicle.cpp
        HydraulicSteering.cpp
        HydraulicSteering.h
        ChHydraulicSteering.cpp
        ChHydraulicSteering.h
        Crawler_SimpleFluidPowertrain.cpp
        Crawler_SimpleFluidPowertrain.h
        Crawler_ArchmidesTire.cpp
        Crawler_ArchmidesTire.h
        Crawler_Chassis.cpp
        Crawler_Chassis.h
        Crawler_Driveline.cpp
        Crawler_Driveline.h)

# TODO use find_package(Boost)
include_directories(Boost_INCLUDE_DIRS)
find_package(Chrono
             COMPONENTS Vehicle Postprocessing Irrlicht
             CONFIG)

MESSAGE(STATUS "... add ${SKETCH}")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})

ADD_EXECUTABLE(${SKETCH} ${SKETCH}.cpp ${MODEL_FILES})
SOURCE_GROUP("" FILES ${SKETCH}.cpp)

SET_TARGET_PROPERTIES(${SKETCH} PROPERTIES
        COMPILE_FLAGS "${CH_CXX_FLAGS}"
        LINK_FLAGS "${CH_LINKERFLAG_EXE}")

#TARGET_LINK_LIBRARIES(${SKETCH}
#        ChronoEngine
#        ChronoEngine_vehicle
#        ChronoModels_vehicle
#        ChronoEngine_postprocess)
#ADD_DEPENDENCIES(${SKETCH}
#        ChronoEngine
#        ChronoEngine_postprocess)
#INSTALL(TARGETS ${SKETCH} DESTINATION ${CH_INSTALL_DEMO})
