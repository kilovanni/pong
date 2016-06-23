CXX = g++
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -I/path/to/SFML/headers
CXXFLAGS = -std=c++11 -g

main:
	$(CXX) $(CXXFLAGS) main.cpp -o main $(LDLIBS)
