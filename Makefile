PREFIX ?= /usr/local
BUILD_DIR = build
INSTALL_LIB = $(PREFIX)/lib
INSTALL_INCLUDE = $(PREFIX)/include/libshell

CFLAGS = -fPIC -Wall -Wextra -std=c99
CXXFLAGS = -fPIC -Wall -Wextra -std=c++17
LDFLAGS =

HAS_X11 := $(shell pkg-config --exists x11 && echo 1 || echo 0)
ifeq ($(HAS_X11),1)
    X11_CFLAGS = $(shell pkg-config --cflags x11)
    X11_LIBS = $(shell pkg-config --libs x11)
    WATCHER_LIB = libshell-watcher.so
else
    $(warning X11 not found; WindowWatcher library will not be built)
    WATCHER_LIB =
endif

CORE_C_SOURCES = launch/launch.c
CORE_CXX_SOURCES = audio/audio.cpp logger/logger.cpp notifications/notifications.cpp resource/resource.cpp settings/settings.cpp

CORE_C_OBJECTS = $(CORE_C_SOURCES:%.c=$(BUILD_DIR)/%.o)
CORE_CXX_OBJECTS = $(CORE_CXX_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
CORE_OBJECTS = $(CORE_C_OBJECTS) $(CORE_CXX_OBJECTS)

WATCHER_C_SOURCES = WindowWatcher/EventQueue.c WindowWatcher/Receiver.c WindowWatcher/WindowWatcher.c
WATCHER_C_OBJECTS = $(WATCHER_C_SOURCES:%.c=$(BUILD_DIR)/%.o)

CORE_LIB = libshell.so
ALL_LIBS = $(CORE_LIB) $(WATCHER_LIB)

.PHONY: all install uninstall clean

all: $(ALL_LIBS)

$(CORE_LIB): $(CORE_OBJECTS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)
	@echo "Built $(CORE_LIB)"

$(WATCHER_LIB): $(WATCHER_C_OBJECTS)
	$(CC) -shared -o $@ $^ $(X11_LIBS) $(LDFLAGS)
	@echo "Built $(WATCHER_LIB)"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(X11_CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: all
	@echo "Installing to $(PREFIX)..."
	install -d $(INSTALL_LIB) $(INSTALL_INCLUDE)
	install -m 644 $(CORE_LIB) $(INSTALL_LIB)/
ifneq ($(WATCHER_LIB),)
	install -m 644 $(WATCHER_LIB) $(INSTALL_LIB)/
endif
	@echo "Installing headers..."
	install -d $(INSTALL_INCLUDE)/audio
	install -m 644 audio/audio.hpp $(INSTALL_INCLUDE)/audio/
	install -d $(INSTALL_INCLUDE)/launch
	install -m 644 launch/launch.h $(INSTALL_INCLUDE)/launch/
	install -d $(INSTALL_INCLUDE)/logger
	install -m 644 logger/logger.hpp $(INSTALL_INCLUDE)/logger/
	install -d $(INSTALL_INCLUDE)/notifications
	install -m 644 notifications/notifications.hpp $(INSTALL_INCLUDE)/notifications/
	install -d $(INSTALL_INCLUDE)/resource
	install -m 644 resource/resource.hpp $(INSTALL_INCLUDE)/resource/
	install -d $(INSTALL_INCLUDE)/settings
	install -m 644 settings/settings.hpp settings/ini.h $(INSTALL_INCLUDE)/settings/
	install -d $(INSTALL_INCLUDE)/WindowWatcher
	install -m 644 WindowWatcher/WindowWatcher.h WindowWatcher/EventQueue.h $(INSTALL_INCLUDE)/WindowWatcher/
	ldconfig
	@echo "Installation complete!"

uninstall:
	@echo "Uninstalling from $(PREFIX)..."
	rm -f $(INSTALL_LIB)/libshell.so $(INSTALL_LIB)/libshell-watcher.so
	rm -rf $(INSTALL_INCLUDE)
	ldconfig
	@echo "Uninstall complete!"

clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR) $(ALL_LIBS)
	@echo "Clean complete!"
