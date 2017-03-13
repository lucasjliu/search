CPPFLAGS=-g -Wall -Wextra -std=c++11 -iquote src/ -Wno-sign-compare
CPPFLAGS+=$(PLATFORM_CXXFLAGS) $(PLATFORM_LDFLAGS)
CXX=g++
SRC_DIR=src/search
BIN_DIR=bin
OBJ_DIR=obj
BIN=search
LDFLAGS=
LIBS=-lhandy

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

include src/include/config.mk

all: $(OBJ_DIR) $(BIN_DIR) test

test: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LIBS) $(OBJS) -o $(BIN_DIR)/$(BIN)$

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(LIBS) -c $< -o $@

clean:
	rm $(BIN_DIR)/* $(OBJ_DIR)/*.o