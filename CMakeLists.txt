CMAKE_MINIMUM_REQUIRED(VERSION 3.9.5)

PROJECT(libwn)

ADD_LIBRARY(wn STATIC 
	wn_db.cpp
	binsrch.c
	search.c
	wnhelp.c
	wnutil.c
	morph.c
	wnglobal.c
	wnrtl.c
)

INCLUDE_DIRECTORIES("${CMAKE_INSTALL_PREFIX}/include")

set_target_properties(wn PROPERTIES PUBLIC_HEADER "wn_db.h")

install(TARGETS wn
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)
