CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
LDFLAGS = -pthread

SERVER_SRC = Servers/Server.cpp
CLIENT_SRC = Clients/Client.cpp

all: server client

server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o server $(LDFLAGS)

client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o client $(LDFLAGS)

SRC_DIR = Tests
PROGRAMS = room

all: $(PROGRAMS)

%: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f server client