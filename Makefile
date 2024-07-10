# Makefile for The One
# Jacob Lowe

CXX=g++


EXE = engine
IMGUI_DIR = ./imgui
# VCPKG Source using the current user profile
MSYS2_ROOT := C:/msys32
MSYS2_TARGET_TRIPLET := mingw64

#Icon
# Application Icon
ICON = icon.ico
# Resource Script
RC = icon.rc
# Icon Object
RES = icon.o

# Im GUI Compilation
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_sdlrenderer2.cpp
# Game Compilation
SOURCES += Application.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))


# Swap between Windows and Unix installs
ifeq ($(OS),Windows_NT)
    RM = del /Q
	ECHO_MESSAGE = "MinGW"
	CXXFLAGS = -std=c++11 -I$(MSYS2_ROOT)/$(MSYS2_TARGET_TRIPLET)/include  -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
	LDFLAGS := -L$(MSYS2_ROOT)/$(MSYS2_TARGET_TRIPLET)/lib 
	CXXFLAGS += -g -Wall -Wformat
	LIB_LIST = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -ljsoncpp -static-libgcc -static-libstdc++ -lgdi32 -lopengl32 -limm32
else
    RM = rm -f
	LDFLAGS= -Lusr/include
	LIB_LIST = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -ljsoncpp
endif

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(RES): $(RC)
	windres -O coff -i $(RC) -o $(RES)

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)
$(EXE): $(OBJS) $(RES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIB_LIST)

# Swap between Windows and Unix installs
ifeq ($(OS),Windows_NT)
    RM = del /Q
else
    RM = rm -f
endif

# Clean all object files
cleanall:
	$(RM) *.o

# Clean specific object file, used more if a header file is edited and not detected as a change
clean:
	$(RM) $(addsuffix .o,$(filter-out $@,$(MAKECMDGOALS)))