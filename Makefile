CXX=g++
CXXFLAGS=-std=c++11 -Wall -Iinclude -Isrc
LIBS=-lgmp -lgmpxx -lssl -lcrypto -lsecp256k1

SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(SRC:.cpp=.o)

TARGET=2ecdsa_dummy_communication

MAIN_DIR = .

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) $(MAIN_DIR)/2ecdsa.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(MAIN_DIR)/main.o: $(MAIN_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET) $(MAIN_DIR)/2ecdsa.o


