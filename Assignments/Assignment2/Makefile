CXX       = g++
CXXFLAGS  = -g -O2 -Wall -c
LDFLAGS   = -g -O2 -Wall -lGL -lglut #-L/usr/X11R6/lib -lGL -lglut
RM        = rm -f
EXEC      = ray_tracer



all: $(EXEC)
debug: CXXFLAGS += -DDEBUG
debug: all

$(EXEC): ray_tracer.o main.o
	$(CXX)  $^ -o $@ $(LDFLAGS)

ray_tracer.o: ray_tracer.cpp ray_tracer.h
	$(CXX) $(CXXFLAGS) $< -o $@

main.o: main.cpp ray_tracer.h
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	$(RM) *~ *.bak *.o $(EXEC)
