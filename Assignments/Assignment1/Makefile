CXX       = g++
CXXFLAGS  = -g -O2 -Wall -c
LDFLAGS   = -g -O2 -Wall -lGL -lGLU -lglut 
RM        = rm -f
EXEC      = minigl

all: $(EXEC)

$(EXEC): minigl.o main.o
	$(CXX) $(LDFLAGS) $^ -o $@

minigl.o: minigl.cpp minigl.h
	$(CXX) $(CXXFLAGS) $< -o $@

main.o: main.cpp minigl.h
	$(CXX) $(CXXFLAGS) $< -o $@

long: 
	g++ -g  -O2 main.cpp minigl.cpp $(LDFLAGS) -o $(EXEC)

clean:
	$(RM) *~ *.bak *.o $(EXEC)
