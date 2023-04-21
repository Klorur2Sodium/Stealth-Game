CC = gcc
CFLAGS = -Wall -ansi -pedantic
LDFLAGS = -lMLV -lm

TARGET_EXEC = stealth_game

INCLUDE_DIR = ./include/
SRC_DIR = ./src/
BUILD_DIR = ./bin/

$(BUILD_DIR)$(TARGET_EXEC): $(BUILD_DIR)Main.o $(BUILD_DIR)Graphics.o \
$(BUILD_DIR)Guardian.o $(BUILD_DIR)Hitbox.o $(BUILD_DIR)Player.o \
$(BUILD_DIR)Relic.o $(BUILD_DIR)Terrain.o $(BUILD_DIR)Ranking.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)Main.o: $(SRC_DIR)Main.c $(INCLUDE_DIR)Terrain.h\
$(INCLUDE_DIR)Player.h $(INCLUDE_DIR)Hitbox.h $(INCLUDE_DIR)Ranking.h\
$(INCLUDE_DIR)Graphics.h $(INCLUDE_DIR)Relic.h $(INCLUDE_DIR)Guardian.h

$(BUILD_DIR)Graphics.o: $(SRC_DIR)Graphics.c $(INCLUDE_DIR)Graphics.h\
$(INCLUDE_DIR)Terrain.h $(INCLUDE_DIR)Guardian.h $(INCLUDE_DIR)Player.h\
$(INCLUDE_DIR)Ranking.h

$(BUILD_DIR)Guardian.o: $(SRC_DIR)Guardian.c $(INCLUDE_DIR)Guardian.h\
$(INCLUDE_DIR)Terrain.h $(INCLUDE_DIR)Player.h $(INCLUDE_DIR)Hitbox.h

$(BUILD_DIR)Hitbox.o: $(SRC_DIR)Hitbox.c $(INCLUDE_DIR)Hitbox.h\
$(INCLUDE_DIR)Terrain.h

$(BUILD_DIR)Player.o: $(SRC_DIR)Player.c $(INCLUDE_DIR)Player.h\
$(INCLUDE_DIR)Terrain.h

$(BUILD_DIR)Relic.o: $(SRC_DIR)Relic.c $(INCLUDE_DIR)Relic.h\
$(INCLUDE_DIR)Terrain.h $(INCLUDE_DIR)Guardian.h

$(BUILD_DIR)Terrain.o: $(SRC_DIR)Terrain.c $(INCLUDE_DIR)Terrain.h

$(BUILD_DIR)Ranking.o: $(SRC_DIR)Ranking.c $(INCLUDE_DIR)Ranking.h

$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	$(CC) -o $@ $(CFLAGS) -c $<


.PHONY: clean
clean:
	rm -rfv $(BUILD_DIR)*