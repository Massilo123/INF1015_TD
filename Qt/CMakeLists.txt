cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
# La raison pour laquelle on fait une variable d'environnement VCPKG_ROOT.
if (DEFINED ENV{VCPKG_ROOT})
	message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
	message(STATUS "       platform: ${CMAKE_GENERATOR_PLATFORM}")
	message(STATUS "    system name: ${CMAKE_SYSTEM_NAME}")
	message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
	if (DEFINED ENV{VCPKG_TARGET_TRIPLET})
		set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_TARGET_TRIPLET}")
	endif()
	include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
	message(STATUS "VCPKG triplet: ${VCPKG_TARGET_TRIPLET}")
endif()

# Le nom du projet.
project(ProjetTest)

# On met les fichiers sources (incluant les entêtes)
set(ALL_FILES
	"main.cpp"
	"Calc.cpp"
	"Calc.hpp"
	"TestCalc.cpp"
	"CalcWindow.cpp"
	"CalcWindow.hpp"
	"bibliotheque_cours/bibliotheque_cours.cpp"
	"bibliotheque_cours/verification_allocation.cpp"
	"googletest/src/gtest-all.cc"
)

add_executable(${PROJECT_NAME} ${ALL_FILES})

# Pour Qt (basé sur https://doc.qt.io/qt-5/cmake-get-started.html )
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets)

# Les flags de compilation. Ceux-ci reproduisent un peu le comportement de MSVC avec /W4.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fsigned-char -Wall -Wextra -Wpedantic -Woverloaded-virtual -Wno-unknown-pragmas -Wno-enum-compare --coverage")

# On ajoute le dossier include/ fourni, comme des fichiers d'inclusion du système.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "include")

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "googletest" PRIVATE "googletest/include")
add_compile_definitions(_POSIX_C_SOURCE=200809L)  # Requis pour compiler GoogleTest sous Cygiwin, voir https://github.com/google/googletest/issues/813

# Si on trouve gcov, on fait une cible avec nom ..._couveture pour exécuter gcov et obtenir la couverture de code.
find_program(GCOV_PATH gcov)
if (GCOV_PATH)
	add_custom_target(${PROJECT_NAME}_couverture
		# Vous devez exécuter le programme avant de "build" cette cible, ou décommenter la ligne suivante qui l'exécute mais ne fonctionne pas si le programme a besoin d'interagir avec l'usager:
		# COMMAND ${PROJECT_NAME}
	 
		# Analyse le fichier de couverture:
		COMMAND ${GCOV_PATH} -s "${ProjetTest_SOURCE_DIR}" -r -f -m "${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}.dir/*.gcda"

		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		DEPENDS ${PROJECT_NAME}
	)
endif()


# Tous les package Vcpkg. Pour savoir quoi mettre, on fait "vcpkg install <le package>", puis on met ce qu'ils disent de mettre dans le CMakeLists.
# find_package(cppitertools CONFIG REQUIRED)  # On pourrait prendre le cppitertools de vcpkg au lieu de celui fourni dans include/
# find_package(Microsoft.GSL CONFIG  REQUIRED)  # Et GSL, noter que son nom de package est "ms-gsl" (le package "gsl" dans vcpkg est le "GNU Scientific Library" et non le "Guidelines Support Library").
