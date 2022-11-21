add_subdirectory(MathIO)
add_subdirectory(MeshIO)
add_subdirectory(TextIO)
add_subdirectory(PhysicsIO)

target_sources(poly_fem PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.h
        ${CMAKE_CURRENT_LIST_DIR}/main.cc
        )
