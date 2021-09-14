# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

project ("BsuirForGeysV5")

# Включите подпроекты.
add_subdirectory ("IGPM2")
add_subdirectory ("IGPM3")
add_subdirectory ("IGPM4")
add_subdirectory ("IGPM5")
add_subdirectory ("IGPM6")
