set(LIBHELLO_SRC add.c)
######### dynamic library
#add_library(add SHARED ${LIBHELLO_SRC})
######### static library
add_library(add ${LIBHELLO_SRC})



# $ cmake -DCMAKE_INSTALL_PREFIX=<prefix> ..
# default /usr/local
install(TARGETS add LIBRARY DESTINATION lib)
install(FILES ../include/add.h DESTINATION include/add)

