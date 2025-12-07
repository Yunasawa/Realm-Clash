CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
LDFLAGS = -pthread

SERVER_SRC = Servers/Server.cpp
CLIENT_SRC = Clients/Client.cpp
RESET_SRC = Functions/ResetData.cpp

all: server client reset

server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o server $(LDFLAGS)

client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o client $(LDFLAGS)

reset: $(RESET_SRC)
	$(CXX) $(CXXFLAGS) $(RESET_SRC) -o reset $(LDFLAGS)

clean: 
	rm -f server client

# all: oldserver oldclient

# oldserver: oldServer.cpp
# 	$(CXX) $(CXXFLAGS) oldServer.cpp -o oldserver $(LDFLAGS)

# oldclient: oldClient.cpp
# 	$(CXX) $(CXXFLAGS) oldClient.cpp -o oldclient $(LDFLAGS)

# SRC_DIR = Tests
# PROGRAMS = room

# all: $(PROGRAMS)

# %: $(SRC_DIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# clean: 
# 	rm -f oldserver oldclient