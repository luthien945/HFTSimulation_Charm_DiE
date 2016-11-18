OBJS = mPairDst.o analysis.o
#OBJS = mTreeDst.o ehHadron.o StHelix.o StPhysicalHelix.o StThreeVector.o
EXE = analysis

#MY_VPATH = ./StmTreeCut
#vpath %.C $(MY_VPATH)
#    vpath %.cxx $(MY_VPATH)
#    vpath %.h $(MY_VPATH)

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

INCFLAGS = -I$(ROOTSYS)/include
LDFLAGS = -L$(ROOTSYS)/lib

CXX = g++ -m32
FLAGS = -Wall -g $(INCFLAGS) $(LDFLAGS)

COMPILE = $(CXX) $(FLAGS) -c 

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(ROOTFLAGS) $(ROOTLIBS)

%.o: %.cxx
	$(COMPILE)  $< 

.PHONY: clean
clean:
	rm -f ${EXE} ${OBJS}


