add_subdirectory(PWL)
target_sources(poly_fem PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/discretizations.h
        ${CMAKE_CURRENT_LIST_DIR}/discretizations_addview.cc
        )