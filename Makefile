OBJS = imagesaver.cpp 

RESULT = build/imagesaver.dll

COMPILER = x86_64-w64-mingw32-g++

FLAGS = -D IMAGESAVER_EXPORTS -shared -Wl,--subsystem,windows,--out-implib,build/imagesaver.dll.a

#begin with I
INCLUDES =  -IC:/msys64/mingw64/include

#begin with L
LIBRARIES = -LC:/msys64/mingw64/lib

#begin with l
LINKER_FLAGS = -llibz -lpng16

all :
	$(COMPILER) $(OBJS) $(FLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_FLAGS) -o $(RESULT)

# OBJS = test.cpp imagesaver.cpp

# RESULT = build/test

# COMPILER = x86_64-w64-mingw32-g++

# FLAGS = -g -Wall 

# #begin with I
# INCLUDES =  -IC:/msys64/mingw64/include 

# #begin with L
# LIBRARIES = -LC:/msys64/mingw64/lib 

# #begin with l
# LINKER_FLAGS = -llibz -lpng16

# all :
# 	$(COMPILER) $(OBJS) $(FLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_FLAGS) -o $(RESULT)