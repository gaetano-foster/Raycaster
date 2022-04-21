SOURCES := $(shell find . -type f -iname "*.cc")
CCFLAGS := -g -fexpensive-optimizations -flto -Ofast -fpermissive -Wno-narrowing -Wno-deprecated -DUNICODE -lstdc++fs -std=c++17
LIBS	:= -L. -lm -lpthread -lGL -lX11 -lpng

all:
	@g++ $(SOURCES) $(CCFLAGS) $(LIBS)