PREFIX ?= /usr/local

BUILD_DIR := build

INSTALL_LIB := $(PREFIX)/lib
INSTALL_INCLUDE := $(PREFIX)/include/libshell
INSTALL_PKG_CONFIG := $(PREFIX)/lib/pkgconfig
INSTALL_BIN := $(PREFIX)/bin

CFLAGS := -fPIC -Wall -Wextra -std=c99
CXXFLAGS := -fPIC -Wall -Wextra -std=c++17
LDFLAGS := -Wl,-rpath,$(INSTALL_LIB)

CORE_LIB := libshell.so
WATCHER_LIB := libshell-watcher.so
WATCHER_APP := windowwatcher

HAS_X11 := $(shell pkg-config --exists x11 && echo 1 || echo 0)

ifeq ($(HAS_X11),1)
X11_CFLAGS := $(shell pkg-config --cflags x11)
X11_LIBS := $(shell pkg-config --libs x11)
BUILD_WATCHER := 1
else
$(warning X11 not found; WindowWatcher components will not be built)
BUILD_WATCHER := 0
endif


###############################################################################
# Sources
###############################################################################

CORE_C_SOURCES := \
    launch/launch.c

CORE_CXX_SOURCES := \
    audio/audio.cpp \
    logger/logger.cpp \
    notifications/notifications.cpp \
    resource/resource.cpp \
    settings/settings.cpp

WATCHER_C_SOURCES := \
    WindowWatcher/EventQueue.c \
    WindowWatcher/Receiver.c \
    WindowWatcher/WindowWatcher.c


###############################################################################
# Objects
###############################################################################

CORE_C_OBJECTS := $(CORE_C_SOURCES:%.c=$(BUILD_DIR)/%.o)
CORE_CXX_OBJECTS := $(CORE_CXX_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
CORE_OBJECTS := $(CORE_C_OBJECTS) $(CORE_CXX_OBJECTS)

WATCHER_C_OBJECTS := $(WATCHER_C_SOURCES:%.c=$(BUILD_DIR)/%.o)


###############################################################################
# Targets
###############################################################################

.PHONY: all install uninstall clean

all: $(CORE_LIB)

ifeq ($(BUILD_WATCHER),1)
all: $(WATCHER_LIB) $(BUILD_DIR)/$(WATCHER_APP)
endif


###############################################################################
# Libraries
###############################################################################

$(CORE_LIB): $(CORE_OBJECTS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)
	@echo "Built $@"

$(WATCHER_LIB): $(WATCHER_C_OBJECTS)
	$(CC) -shared -o $@ $^ $(X11_LIBS) $(LDFLAGS)
	@echo "Built $@"


###############################################################################
# Executable
###############################################################################

$(BUILD_DIR)/$(WATCHER_APP): $(WATCHER_C_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(X11_LIBS)
	@echo "Built $@"


###############################################################################
# Compilation rules
###############################################################################

$(BUILD_DIR)/launch/%.o: launch/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/WindowWatcher/%.o: WindowWatcher/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(X11_CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


###############################################################################
# Install
###############################################################################

install: all
	@echo "Installing to $(PREFIX)..."

	install -d \
		$(INSTALL_LIB) \
		$(INSTALL_INCLUDE) \
		$(INSTALL_BIN) \
		$(INSTALL_PKG_CONFIG)

	install -m755 $(CORE_LIB) $(INSTALL_LIB)/

ifeq ($(BUILD_WATCHER),1)
	install -m755 $(WATCHER_LIB) $(INSTALL_LIB)/
	install -m755 $(BUILD_DIR)/$(WATCHER_APP) $(INSTALL_BIN)/
endif

	install -d $(INSTALL_INCLUDE)/audio
	install -m644 audio/audio.hpp $(INSTALL_INCLUDE)/audio/

	install -d $(INSTALL_INCLUDE)/launch
	install -m644 launch/launch.h $(INSTALL_INCLUDE)/launch/

	install -d $(INSTALL_INCLUDE)/logger
	install -m644 logger/logger.hpp $(INSTALL_INCLUDE)/logger/

	install -d $(INSTALL_INCLUDE)/notifications
	install -m644 notifications/notifications.hpp $(INSTALL_INCLUDE)/notifications/

	install -d $(INSTALL_INCLUDE)/resource
	install -m644 resource/resource.hpp $(INSTALL_INCLUDE)/resource/

	install -d $(INSTALL_INCLUDE)/settings
	install -m644 settings/settings.hpp helpers/ini.h \
		$(INSTALL_INCLUDE)/settings/

	install -d $(INSTALL_INCLUDE)/WindowWatcher
	install -m644 \
		WindowWatcher/WindowWatcher.h \
		WindowWatcher/EventQueue.h \
		$(INSTALL_INCLUDE)/WindowWatcher/

	@echo 'prefix=$(PREFIX)' > $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'exec_prefix=$${prefix}' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'libdir=$${exec_prefix}/lib' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'includedir=$${prefix}/include' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'Name: libshell' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'Description: Shell utilities library' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'Version: 1.0.0' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'Libs: -L$${libdir} -lshell' >> $(INSTALL_PKG_CONFIG)/libshell.pc
	@echo 'Cflags: -I$${includedir}' >> $(INSTALL_PKG_CONFIG)/libshell.pc

ifeq ($(BUILD_WATCHER),1)
	@echo 'prefix=$(PREFIX)' > $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'exec_prefix=$${prefix}' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'libdir=$${exec_prefix}/lib' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'includedir=$${prefix}/include' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'Name: libshell-watcher' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'Description: X11 window watcher library' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'Version: 1.0.0' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'Libs: -L$${libdir} -lshell-watcher $(X11_LIBS)' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	@echo 'Cflags: -I$${includedir} $(X11_CFLAGS)' >> $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
endif

	ldconfig
	@echo "Installation complete."


###############################################################################
# Uninstall
###############################################################################

uninstall:
	rm -f $(INSTALL_LIB)/$(CORE_LIB)
	rm -f $(INSTALL_LIB)/$(WATCHER_LIB)
	rm -f $(INSTALL_BIN)/$(WATCHER_APP)
	rm -rf $(INSTALL_INCLUDE)
	rm -f $(INSTALL_PKG_CONFIG)/libshell.pc
	rm -f $(INSTALL_PKG_CONFIG)/libshell-watcher.pc
	ldconfig
	@echo "Uninstalled."


###############################################################################
# Clean
###############################################################################

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(CORE_LIB)
	rm -f $(WATCHER_LIB)
