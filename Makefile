CXX = g++
CXXFLAGS = -Iinclude -Wall
LIBS = -Llib -lglfw3 -lopengl32 -lgdi32

LAB ?= Lab-4
SRC ?=

GLAD = $(LAB)/src/glad.c
EXE_NAME = $(basename $(notdir $(SRC)))
OUT = $(LAB)/$(EXE_NAME).exe

all:
	$(CXX) $(SRC) $(GLAD) $(CXXFLAGS) $(LIBS) -o $(OUT)
