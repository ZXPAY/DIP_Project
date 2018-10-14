CC = g++  # 使用的編譯器
RM := rm -rf

LIBSRC = $(wildcard lib/*.cpp)
SRC = $(wildcard ./*.cpp)
LIBOBJS = $(patsubst %.cpp,%.o,$(LIBSRC))
LIBOBJS += $(patsubst %.cpp,%.o,$(SRC))
INC = ./lib/

ExecuteName := HW_1.exe
# OBJ_LIB += \
# ../HW_1.cpp\
# ../lib/*.cpp\
# # ../lib/*.c

CFLAGS := -o

%.exe: $(LIBOBJS)
	@echo MAKE EXE $(SRC)
	@echo $(LIBOBJS)
	$(CC) $(LIBOBJS) -o $@

%.o: %.c
	@echo MAKE $@
	$(CC) -c $< -o $@ -I $(INC)

clean:
	$(RM) -f *.o
