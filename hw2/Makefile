LIB_CODE_BASE=../openGLHelper

HW2_CXX_SRC=$(wildcard src/*.cpp)
HW2_HEADER=
HW2_OBJ=$(addprefix build/, $(notdir $(patsubst %.cpp,%.o,$(HW2_CXX_SRC))))

LIB_CODE_CXX_SRC=$(wildcard $(LIB_CODE_BASE)/*.cpp)
LIB_CODE_HEADER=$(wildcard $(LIB_CODE_BASE)/*.h)
LIB_CODE_OBJ=$(addprefix build/, $(notdir $(patsubst %.cpp,%.o,$(LIB_CODE_CXX_SRC))))

IMAGE_LIB_SRC=$(wildcard ../external/imageIO/*.cpp)
IMAGE_LIB_HEADER=$(wildcard ../external/imageIO/*.h)
IMAGE_LIB_OBJ=$(addprefix build/, $(notdir $(patsubst %.cpp,%.o,$(IMAGE_LIB_SRC))))

HEADER=$(HW2_HEADER) $(LIB_CODE_HEADER) $(IMAGE_LIB_HEADER)
CXX_OBJ=$(HW2_OBJ) $(LIB_CODE_OBJ) $(IMAGE_LIB_OBJ)

CXX=g++
TARGET=hw2
CXXFLAGS=-DGLM_FORCE_RADIANS -std=c++11
OPT=-O3

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Linux)
  PLATFORM=Linux
  INCLUDE=-I../external/glm/ -I$(LIB_CODE_BASE) -I../external/imageIO
  LIB=-lGLEW -lGL -lglut -ljpeg
  LDFLAGS=
else
  PLATFORM=Mac OS
  INCLUDE=-I../external/glm/ -I$(LIB_CODE_BASE) -I../external/imageIO -I../external/jpeg-9a-mac/include
  LIB=-framework OpenGL -framework GLUT ../external/jpeg-9a-mac/lib/libjpeg.a -lstdc++
  CXXFLAGS+= -Wno-deprecated-declarations
  LDFLAGS=-Wl,-w
endif

all: build_dir $(TARGET)

build_dir:
	mkdir -p build

$(TARGET): $(CXX_OBJ)
	$(CXX) $(LDFLAGS) $^ $(OPT) $(LIB) -o $@

build/%.o: src/%.cpp $(HEADER)
	$(CXX) -c $(CXXFLAGS) $(OPT) $(INCLUDE) $< -o $@

build/%.o: $(LIB_CODE_BASE)/%.cpp $(LIB_CODE_HEADER)
	$(CXX) -c $(CXXFLAGS) $(OPT) $(INCLUDE) $< -o $@

build/%.o: ../external/imageIO/%.cpp $(IMAGE_LIB_HEADER)
	$(CXX) -c $(CXXFLAGS) $(OPT) $(INCLUDE) $< -o $@

clean:
	rm -rf build $(TARGET)