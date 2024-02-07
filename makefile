CC = g++
CFLAGS = -lSDL2 -MMD

BUILD_DIR = build

SOURCE_DIR = .
SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.cpp )

OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))

TARGET = $(BUILD_DIR)/Bouncing_ball

.PHONY: all run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

-include $(OBJECTS:.o=.d)

clean:
	@rm -vr $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)


