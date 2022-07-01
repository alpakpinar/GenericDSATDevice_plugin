default: build

CXX=g++
DSAT_PATH?=dsat/
RUNTIME_LDPATH?=${PWD}

# Define the libraries to build
LIBRARY_DSAT = lib/libDSAT_GenericDSAT.so
LIBRARY_DSAT_SOURCES = $(wildcard src/GenericDSAT/*.cpp)
LIBRARY_DSAT_OBJECT_FILES = $(patsubst src/%.cpp,obj/%.o,${LIBRARY_DSAT_SOURCES})

LIBRARY_DSAT_DEVICE = lib/libDSAT_GenericDSATDevice.so
LIBRARY_DSAT_DEVICE_SOURCES = $(wildcard src/GenericDSATDevice/*.cpp)
LIBRARY_DSAT_DEVICE_OBJECT_FILES = $(patsubst src/%.cpp,obj/%.o,${LIBRARY_DSAT_DEVICE_SOURCES})

BUEXCEPTION_PATH=${BUTOOL_PATH}/external/BUException

# Set up include paths for compiler
RELATIVE_INCLUDE_PATH = \
	include \
	${DSAT_PATH}/include        \
	${BUTOOL_PATH}/include      \
	${BUEXCEPTION_PATH}/include

INCLUDE_PATH = $(patsubst %,-I%,$(abspath ${RELATIVE_INCLUDE_PATH}))

# Library paths for linker
RELATIVE_LIBRARY_PATH = \
			lib \
			${DSAT_PATH}/lib         \
			${BUTOOL_PATH}/lib       \
			${BUEXCEPTION_PATH}/lib    

LIBRARY_PATH = $(patsubst %,-L%,$(abspath ${RELATIVE_LIBRARY_PATH}))

LIBRARIES = 	-lToolException     \
				-lBUTool_BUTextIO   \
				-lBUTool            \
				-lBUTool_Helpers    \
				-ldsat

CXX_FLAGS = -g -O3 -rdynamic -Wall -MMD -MP -fPIC ${INCLUDE_PATH} -Werror -Wno-literal-suffix

CXX_FLAGS += -std=c++11 -fno-omit-frame-pointer -pedantic -Wno-ignored-qualifiers -Werror=return-type -Wextra -Wno-long-long -Winit-self -Wno-unused-local-typedefs  -Woverloaded-virtual

LINK_LIBRARY_FLAGS = -shared -fPIC -Wall -g -O3 -rdynamic ${LIBRARY_PATH} ${LIBRARIES} \
			-Wl,-rpath=$(RUNTIME_LDPATH)/lib

clean: _cleanall
_cleanall:
	rm -rf obj
	rm -rf bin
	rm -rf lib
	make -C ${DSAT_PATH} clean

all: _all
build: _all
_all: dsat self

butool_env:
ifdef BUTOOL_PATH
	$(info Using BUTool lib from user defined BUTOOL_PATH=${BUTOOL_PATH})
else
	$(error You must define BUTOOL_PATH through the command line (or as an env variable)!)
endif

self: butool_env ${LIBRARY_DSAT} ${LIBRARY_DSAT_DEVICE}

# -------------------
# The .so libraries
# -------------------
${LIBRARY_DSAT_DEVICE}: ${LIBRARY_DSAT_DEVICE_OBJECT_FILES} ${LIBRARY_DSAT} 
	${CXX} ${LINK_LIBRARY_FLAGS} -lDSAT_GenericDSAT ${LIBRARY_DSAT_DEVICE_OBJECT_FILES} -o $@

${LIBRARY_DSAT}: ${LIBRARY_DSAT_OBJECT_FILES}
	${CXX} ${LINK_LIBRARY_FLAGS} ${LIBRARY_DSAT_OBJECT_FILES} -o $@

# Rule to make object files
obj/%.o : src/%.cpp
	mkdir -p $(dir $@)
	mkdir -p {lib,obj}
	${CXX} ${CXX_FLAGS} -c $< -o $@

# ------------------
# The DSAT library
# ------------------
dsat:
	make -C ${DSAT_PATH}