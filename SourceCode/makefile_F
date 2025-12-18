OUT := game


CXXFLAGS := -Wall -std=c++17 -O2
SOURCE := $(wildcard *.cpp */*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(notdir $(SOURCE)))
RM_OBJ := 
RM_OUT := 

ifeq ($(OS), Windows_NT) 
    # Makefile 在 SourceCode，allegro/MinGW 在上一層
    ALLEGRO_PATH := ..
    # 使用 MinGW/Bin 裡面的 G++
    CC := $(ALLEGRO_PATH)/MinGW/bin/g++
    # 將 MinGW/Bin 加入 PATH，讓它能找到 DLLs
    export Path := $(ALLEGRO_PATH)/MinGW/bin;$(Path)

    # Allegro 連結旗標 (MinGW 不需要 -lallegro_monolith，只需 Allegro 的路徑)
    ALLEGRO_FLAGS_RELEASE := -I$(ALLEGRO_PATH)/allegro/include -L$(ALLEGRO_PATH)/allegro/lib -lallegro_monolith
    
    ALLEGRO_DLL_PATH_RELEASE := 
    ALLEGRO_FLAGS_DEBUG := $(ALLEGRO_FLAGS_RELEASE)
    ALLEGRO_DLL_PATH_DEBUG := 

    # 【關鍵修正】: 刪除指令統一使用 rm -f (Linux/MinGW 語法)
    RM_OBJ := -del /Q $(OBJ) 2>nul
    RM_OUT := -del /Q $(OUT).exe 2>nul

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
