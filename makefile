GTEST_DIR = /usr/src/gtest
USER_DIR = .
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

CXX = clang++

CXXFLAGS += -Wall -Wextra -std=c++14
GTESTFLAGS += -isystem $(GTEST_DIR)/include -pthread

DEBUG_FLAGS = -O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls
RELEASE_FLAGS = -O3

BIN_DIR = ./bin
OBJ_DIR = ./obj
PROF_DIR = ./profiling

GTEST_HEADERS = /usr/include/gtest/*.h \
                /usr/include/gtest/internal/*.h

all: all_tests

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
		-c $(GTEST_DIR)/src/gtest-all.cc -o $(OBJ_DIR)/$@

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) \
		-c $(GTEST_DIR)/src/gtest_main.cc -o $(OBJ_DIR)/$@

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $(OBJ_DIR)/$@ $(^:%=$(OBJ_DIR)/%)

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $(OBJ_DIR)/$@ $(^:%=$(OBJ_DIR)/%)

#################

all_tests: dirs unittest

run_tests: all_tests
	$(BIN_DIR)/unittest

unittest.o: $(USER_DIR)/unittest.cpp $(USER_DIR)/HeapList.hpp $(GTEST_HEADERS)
	$(CXX) $(GTESTFLAGS) $(CXXFLAGS) $(DEBUG_FLAGS) \
		-c $< -o $(OBJ_DIR)/$@

unittest: unittest.o gtest_main.a
	$(CXX) $(GTESTFLAGS) $(CXXFLAGS) $(DEBUG_FLAGS) -lpthread \
		$(^:%=$(OBJ_DIR)/%) -o $(BIN_DIR)/$@

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(PROF_DIR)

clean:
	rm -f $(BIN_DIR)/* $(OBJ_DIR)/*
