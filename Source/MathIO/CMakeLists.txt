add_subdirectory(Discretizations)
add_subdirectory(Quadrature)
target_sources(poly_fem PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/mathio.h
        )