# Makefle for evoc lib
# change 'y' or 'n' the following line to disable / enable debugging
#
DEBUG = n

# The executable file name.  
# If not specified, current directory name or `a.out' will be used.
PROGRAM := libfacedetection.so
OUTDIR := ./
INCLUDES :=
SUBDIRS := 
SRCEXTS := .c .cpp .cxx .cc .C .CPP
HDREXTS := .h .hh
LDFLAGS :=
CXXFLAGS := -fPIC -c -std=c++11 -mavx -mfma
CFLAGS :=
LIBS := -shared -fPIC


ifeq ($(DEBUG),y)
	CXXFLAGS += -DDEBUG -g
	CFLAGS += -DDEBUG -g
else
	CXXFLAGS += -O3
	CFLAGS += -O3
endif
LIBS += -lm -Wall
SUBDIRS += .
SOURCES := $(foreach f, $(SUBDIRS), $(wildcard $(addprefix $(f)/*, $(SRCEXTS))))
SOURCES := $(filter-out %.c, $(SOURCES))
ifeq ($(SOURCES),)  
	CC := gcc
else
	CC := g++
endif
HEADERS := $(foreach f, $(SUBDIRS), $(wildcard $(addprefix $(f)/*, $(HDREXTS))))
OBJS := $(addsuffix .o, $(basename $(SOURCES)))
ifeq ($(PROGRAM),)  
	PROGRAM = a.out  
endif
ifeq ($(OUTDIR),)  
	OUTDIR = .
endif
TARGET_LIB := $(CURDIR)/$(OUTDIR)/$(PROGRAM)

${TARGET_LIB}:${OBJS}
ifeq ($(SOURCES),)  
	$(CC) $(INCLUDES) $(CXXFLAGS) $(LIBS) -o ${TARGET_LIB} ${OBJS}
else
	$(CC) $(INCLUDES) $(CFLAGS) $(LIBS) -o ${TARGET_LIB} ${OBJS}
endif

	#chcon -t texrel_shlib_t $(TARGET_LIB)

%.o:%.cpp
	@echo Compiling $< ...
	$(CC) -c $(INCLUDES) $(CXXFLAGS) $< -o $*.o

.PHONY:clean

clean:
#	$(foreach f, $(SUBDIRS), $(RM) $(wildcard $(addprefix $(f)/*, $(DEPEND))))
	$(RM) $(OBJS)
	$(RM) $(TARGET_LIB)
