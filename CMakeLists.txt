# CMakeList.txt : CMake project for KoiKoi_v2 include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_VERBOSE_MAKEFILE on)
cmake_policy(SET CMP0167 NEW)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:DebugRelWithDebInfo>:ProgramDatabase>>")
endif()

project ("KoiKoi_v2")

# Add source to this project's executable.
add_executable (KoiKoi_v2 "src/main.cpp"  "src/Hanafuda_Card.h" "src/Hanafuda_Deck.h" "src/KoiKoi_AI_Player.h" "src/KoiKoi_Display.h" "src/KoiKoi_Game.h" "src/KoiKoi_Game_Handler.h" "src/KoiKoi_Player.h" "src/KoiKoi_Probability_Eval.h" "src/Hanafuda_Card.cpp" "src/Hanafuda_Deck.cpp" "src/KoiKoi_Display.cpp" "src/KoiKoi_Game.cpp" "src/KoiKoi_Game_Handler.cpp" "src/KoiKoi_Player.cpp" "src/KoiKoi_Probability_Eval.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET KoiKoi_v2 PROPERTY CXX_STANDARD 20)
endif()

# Add compiler configurations
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")   

# Add Hanafuda_Card_Data file to build dir
find_path(HANAFUDA_CARD_PATH PATHS "src/Hanafuda_Card_Data" NAMES "CardInfo.txt")
configure_file("${HANAFUDA_CARD_PATH}/CardInfo.txt" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/CardInfo.txt" COPYONLY)

# Add Hanafuda_Card_Data textures to build dir
configure_file("${HANAFUDA_CARD_PATH}/backCard.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/backCard.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/highlight.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/highlight.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M0_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M0_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M0_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M0_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M0_S2.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M0_S2.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/crane_and_sun.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/crane_and_sun.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M1_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M1_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M1_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M1_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M1_S2.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M1_S2.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/warbling_white-eye.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/warbling_white-eye.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M2_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M2_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M2_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M2_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M2_S2.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M2_S2.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/curtain.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/curtain.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M3_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M3_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M3_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M3_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M3_S1.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M3_S1.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/cuckoo.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/cuckoo.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M4_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M4_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M4_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M4_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M4_S1.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M4_S1.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/eight-plank_bridge.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/eight-plank_bridge.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M5_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M5_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M5_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M5_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M5_S3.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M5_S3.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/butterfly.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/butterfly.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M6_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M6_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M6_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M6_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M6_S1.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M6_S1.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/boar.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/boar.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M7_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M7_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M7_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M7_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/geese.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/geese.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/full_moon.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/full_moon.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M8_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M8_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M8_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M8_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M8_S3.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M8_S3.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/sake_cup.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/sake_cup.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M9_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M9_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M9_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M9_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M9_S3.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M9_S3.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/deer.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/deer.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/lighting.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/lighting.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M10_S1.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M10_S1.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/swallow.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/swallow.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/ono_no_michikaze.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/ono_no_michikaze.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M11_S0_A.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M11_S0_A.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M11_S0_B.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M11_S0_B.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/M11_S0_C.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/M11_S0_C.png" COPYONLY)
configure_file("${HANAFUDA_CARD_PATH}/chinese_phoenix.png" "${CMAKE_CURRENT_BINARY_DIR}/Hanafuda_Card_Data/chinese_phoenix.png" COPYONLY)

configure_file("src/missing_texture.png" "${CMAKE_CURRENT_BINARY_DIR}/missing_texture.png" COPYONLY)
# Add seed_log to build dir for debugging purposes
configure_file("bin/seed_log.txt" "${CMAKE_CURRENT_BINARY_DIR}/bin/seed_log.txt" APPEND)

# Finding vcpkg packages
find_package(raylib REQUIRED)
find_package(Boost REQUIRED COMPONENTS math algorithm)

# Linking dependancies
target_link_libraries(KoiKoi_v2 PRIVATE raylib)
target_link_libraries(KoiKoi_v2 PRIVATE Boost::math Boost::algorithm)
