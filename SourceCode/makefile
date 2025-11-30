OUT := game


CXXFLAGS := -Wall -std=c++17 -O2
SOURCE := $(wildcard *.cpp */*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(notdir $(SOURCE)))
RM_OBJ := 
RM_OUT := 

ifeq ($(OS), Windows_NT) 
    # Makefile 在 SourceCode，allegro/MinGW 在上一層
    ALLEGRO_PATH := ..
    CC := ../MinGW/bin/g++
    export Path := ../MinGW/bin;$(Path)

    # -I 指 include 目錄，-L 指 lib 目錄，-l 指連結哪個 library
    ALLEGRO_FLAGS_RELEASE := -I../allegro/include -L../allegro/lib -lallegro_monolith
    ALLEGRO_DLL_PATH_RELEASE := 
    ALLEGRO_FLAGS_DEBUG := $(ALLEGRO_FLAGS_RELEASE)
    ALLEGRO_DLL_PATH_DEBUG := 

    RM_OBJ := $(foreach name, $(OBJ), del $(name) & )
    ifeq ($(suffix $(OUT)),)
        RM_OUT := del $(OUT).exe
    else
        RM_OUT := del $(OUT)
    endif

else 
    UNAME_S := $(shell uname -s)
    CC := g++
    
    ALLEGRO_LIBRARIES := allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5
    ALLEGRO_FLAGS_RELEASE := $(shell pkg-config --cflags --libs $(ALLEGRO_LIBRARIES))
    
    ALLEGRO_DLL_PATH_RELEASE := 
    ALLEGRO_FLAGS_DEBUG := $(ALLEGRO_FLAGS_RELEASE)
    ALLEGRO_DLL_PATH_DEBUG := 

    RM_OBJ := rm -f $(OBJ)
    RM_OUT := rm -f $(OUT)
endif

all: debug 

debug:
	$(CC) -c -g $(CXXFLAGS) $(SOURCE) $(ALLEGRO_FLAGS_DEBUG) -D DEBUG
	$(CC) $(CXXFLAGS) -o $(OUT) $(OBJ) $(ALLEGRO_FLAGS_DEBUG) $(ALLEGRO_DLL_PATH_DEBUG)
	$(RM_OBJ)

release:
	$(CC) -c $(CXXFLAGS) $(SOURCE) $(ALLEGRO_FLAGS_RELEASE)
	$(CC) $(CXXFLAGS) -o $(OUT) $(OBJ) $(ALLEGRO_FLAGS_RELEASE) $(ALLEGRO_DLL_PATH_RELEASE)
	$(RM_OBJ)

clean:
	$(RM_OUT)
	$(RM_OBJ)
