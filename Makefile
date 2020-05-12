
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
	cd $(BUILD) && cmake \
	-DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX) \
	-DCMAKE_BUILD_TYPE=Debug ../
	make -C $(BUILD)
	-ls -ls $(BUILD)/bin

build-release: clean
	@echo "*** run build-release ***"
	cd $(BUILD) && cmake \
	-DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX) \
	-DCMAKE_BUILD_TYPE=Release \
	-DENABLE_UNIT_TESTS=OFF ../
	make -C $(BUILD)
	-ls -ls $(BUILD)/bin

.PHONY: build-clang
build-clang: clean-cache create-build
	cd $(BUILD) && cmake \
	-DCMAKE_C_COMPILER=$$(which clang) \
	-DCMAKE_CXX_COMPILER=$$(which clang++) \
	-DENABLE_UNIT_TESTS=OFF \
	-DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX) \
	-DCMAKE_BUILD_TYPE=Release -G Ninja ../
	cd $(BUILD) && $$(which ninja)

install: build-clang
	@echo "*** run install ***"
	$$(which ninja) install -C $(BUILD)

.PHONY: test
test: clean
	@echo "*** run test ***"
	cd $(BUILD) && $(CMAKE) -DENABLE_UNIT_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ../
	$(MAKE) -C $(BUILD)
	$(MAKE) test -C $(BUILD)
	$(BUILD)/bin/unit_tests

.PHONY: coverage
coverage: test
	@echo "*** run coverage ***"
	$(MAKE) coverage -C $(BUILD)

.PHONY: clean
clean: create-build
	@echo "*** run clean ***"
	-$(MAKE) clean -C $(BUILD)

.PHONY: clean-cache
clean-cache:
	@echo "*** run clean-cache ***"
	-$(RM) -r $(BUILD) cmake-build-*