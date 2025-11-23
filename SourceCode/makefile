OUT := game
CC := g++

CXXFLAGS := -Wall -std=c++17 -O2
SOURCE := $(wildcard *.cpp */*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(notdir $(SOURCE)))
RM_OBJ := 
RM_OUT := 

# ==========================================
# Windows 設定 (保留原始設定)
# ==========================================
ifeq ($(OS), Windows_NT) 
    ALLEGRO_PATH := ../allegro
    export Path := ../MinGW/bin;$(Path)

    ALLEGRO_FLAGS_RELEASE := -I$(ALLEGRO_PATH)/include -L$(ALLEGRO_PATH)/lib/liballegro_monolith.dll.a
    ALLEGRO_DLL_PATH_RELEASE := $(ALLEGRO_PATH)/lib/liballegro_monolith.dll.a
    ALLEGRO_FLAGS_DEBUG := -I$(ALLEGRO_PATH)/include -L$(ALLEGRO_PATH)/lib/liballegro_monolith-debug.dll.a
    ALLEGRO_DLL_PATH_DEBUG := $(ALLEGRO_PATH)/lib/liballegro_monolith-debug.dll.a

    RM_OBJ := $(foreach name, $(OBJ), del $(name) & )
    ifeq ($(suffix $(OUT)),)
        RM_OUT := del $(OUT).exe
    else
        RM_OUT := del $(OUT)
    endif

# ==========================================
# Mac / Linux 設定 (已修正為自動偵測路徑)
# ==========================================
else 
    UNAME_S := $(shell uname -s)
    
    # 使用 pkg-config 自動抓取所有需要的 Allegro 模組
    # 這樣可以兼容 Intel Mac (/usr/local) 和 Apple Silicon (/opt/homebrew)
    ALLEGRO_LIBRARIES := allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5
    
    # 自動產生正確的 Include 和 Lib 參數
    ALLEGRO_FLAGS_RELEASE := $(shell pkg-config --cflags --libs $(ALLEGRO_LIBRARIES))
    
    # Mac 下 Debug 和 Release flags 通常共用 pkg-config 的結果
    ALLEGRO_DLL_PATH_RELEASE := 
    ALLEGRO_FLAGS_DEBUG := $(ALLEGRO_FLAGS_RELEASE)
    ALLEGRO_DLL_PATH_DEBUG := 

    RM_OBJ := rm -f $(OBJ)
    RM_OUT := rm -f $(OUT)
endif

# ==========================================
# 建置規則
# ==========================================

# 預設編譯 Release
all: release

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