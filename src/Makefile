IDIR1 = ../include
IDIR2 = ../../common/third-party/glm-master
CXX = g++
CXXFLAGS = -I$(IDIR1) -I$(IDIR2) -O2 -Wall -Wpedantic -Wdeprecated -Wextra -Werror=return-type -MP -MMD -lSDL2 -ldl 
TARGET = main
ODIR = obj
BDIR = ../bin

# set the .cpp files as .o files
_CPP_OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
CPP_OBJ = $(patsubst %, $(ODIR)/%, $(_CPP_OBJ))

# set the .c files as .o files
_C_OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
C_OBJ = $(patsubst %, $(ODIR)/%, $(_C_OBJ))

# set the .d files for both .cpp and .c files respectively
DEPS_CXX = $(patsubst %.cpp, %.d, $(wildcard *.cpp))
DEPS_C = $(patsubst %.c, %.d, $(wildcard *.c))

# get all of the .o files and .d files we set above into two varibales
ALL_OBJS = $(CPP_OBJ) $(C_OBJ)
ALL_DEPS = $(DEPS_CXX) $(DEPS_C)

# compile our .cpp files
$(ODIR)/%.o : %.cpp
	@$(CXX) -c -o $@ $< $(CXXFLAGS) 

# compile our .c files
$(ODIR)/%.o : %.c 
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

# link all our objects
$(TARGET) : $(ALL_OBJS) 
	@$(CXX) -o $(BDIR)/$@ $^ $(CXXFLAGS) 

-include $(ALL_DEPS)

# phony target
.phony : clean

clean :
	@rm -f $(ODIR)/*.o
	@rm -f $(ODIR)/*.d


