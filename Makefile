CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
LDFLAGS = -pthread

SERVER_SRC = Servers/Server.cpp 
CLIENT_SRC = Clients/Client.cpp
RESET_SRC  = Functions/ResetData.cpp

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)
RESET_OBJ  = $(RESET_SRC:.cpp=.o)

all: server client reset

server: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

client: $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

reset: $(RESET_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f server client reset $(SERVER_OBJ) $(CLIENT_OBJ) $(RESET_OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(SERVER_OBJ:.o=.d) $(CLIENT_OBJ:.o=.d) $(RESET_OBJ:.o=.d)


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