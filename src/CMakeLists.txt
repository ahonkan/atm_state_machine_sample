cmake_minimum_required(VERSION 3.10)                                            
                                                                                
set(PROJECT_ROOT_DIR ../)                                                               
include(${PROJECT_ROOT_DIR}/cmake/cmake_common_include)                                            

# libs                                                                                
add_subdirectory(hal)                                                         

# executable
add_subdirectory(controller)                                                    
