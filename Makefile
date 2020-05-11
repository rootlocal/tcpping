
GIT = $$(which git)
CMAKE = $$(which cmake)
MAKE = $$(which make)
RM ?= rm -f
CMAKE_INSTALL_PREFIX ?= /usr/local
BUILD = build

.PHONY: all create-build install build-debug build-release

all: build-release
	@echo "*** run all ***"

create-build:
	@echo "*** run build ***"
	-mkdir -p $(BUILD)

build-debug: clean
	@echo "*** run build-debug ***"
	cd $(BUILD) && cmake -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX) -DCMAKE_BUILD_TYPE=Debug ../
	make -C $(BUILD)
	-ls -ls $(BUILD)/bin

build-release: clean
	@echo "*** run build-release ***"
	cd $(BUILD) && cmake -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX) -DCMAKE_BUILD_TYPE=Release -DENABLE_UNIT_TESTS=OFF ../
	make -C $(BUILD)
	-ls -ls $(BUILD)/bin

install: build-release
	@echo "*** run install ***"
	$(MAKE) install -C $(BUILD)

.PHONY: test
test: clean
	@echo "*** run test ***"
	cd $(BUILD) && $(CMAKE) -DENABLE_UNIT_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ../
	make -C $(BUILD)
	$(MAKE) test -C $(BUILD)
	$(BUILD)/bin/unit_tests

.PHONY: clean
clean: create-build
	@echo "*** run clean ***"
	-$(MAKE) clean -C $(BUILD)

.PHONY: clean-cache
clean-cache:
	@echo "*** run clean-cache ***"
	-$(RM) -r $(BUILD) cmake-build-*