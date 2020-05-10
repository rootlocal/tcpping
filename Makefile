
GIT = $$(which git)
CMAKE = $$(which cmake)
MAKE = $$(which make)
RM ?= rm -f
CMAKE_INSTALL_PREFIX ?= /usr/local
BUILD = build

.PHONY: all create-build make install

all: make
	@echo "*** run all ***"

create-build:
	@echo "*** run build ***"
	-mkdir -p $(BUILD)

make: clean
	@echo "*** run make ***"
	cd $(BUILD) && cmake -D CMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX) ../
	make -C $(BUILD)
	-ls -ls $(BUILD)/bin

install: make
	@echo "*** run install ***"
	$(MAKE) install -C $(BUILD)


.PHONY: test
test: clean
	@echo "*** run test ***"
	cd $(BUILD) && $(CMAKE) -D ENABLE_UNIT_TESTS=ON ../
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