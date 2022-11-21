target_sources(poly_fem PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/diffusion2d.h
        ${CMAKE_CURRENT_LIST_DIR}/diffusion2d_constrdestr.cc
        ${CMAKE_CURRENT_LIST_DIR}/diffusion2d_initialize.cc
        ${CMAKE_CURRENT_LIST_DIR}/diffusion2d_initializeitems.cc
        ${CMAKE_CURRENT_LIST_DIR}/diffusion2d_execute.cc
        ${CMAKE_CURRENT_LIST_DIR}/diffusion2d_cell_loop.cc
        )