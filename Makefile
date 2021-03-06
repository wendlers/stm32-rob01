##
# Toplevel Makefile RobotRemCtrlFW
#
# Stefan Wendler, sw@kaltpost.de 
##

BASEDIR 	= .
SRCDIR  	= src
BINDIR		= bin
TESTBINDIR	= test-bin
FIRMWARE	= firmware.elf
OOCD_IF		= interface/openocd-usb.cfg 
OOCD_BOARD	= board/stm32100b_eval.cfg
TESTSRCDIR 	= test-src
DEPLOYDIR	= deploy
TMPDIR		= /tmp
VERSION		= 0.1
TARGET		= RobotControlFW_v$(VERSION)

all: target

world: target target-test gen-docs

target: 
	make -C $(SRCDIR)

target-test: target
	make -C $(TESTSRCDIR)

gen-docs: target
	cd $(SRCDIR) && make gen-docs 

style:
	cd $(SRCDIR) && make style
	cd $(TESTSRCDIR) && make style

check: 
	make -C $(SRCDIR) check
# We do not check the tests	
#	make -C $(TESTSRCDIR) check 

flash-target: target
	openocd -f $(OOCD_IF) -f $(OOCD_BOARD) \
		-c init -c targets -c "halt" \
		-c "flash write_image erase $(BINDIR)/$(FIRMWARE)" \
		-c "verify_image $(BINDIR)/$(FIRMWARE)" \
		-c "reset run" -c shutdown

flash-target-test: target-test
	openocd -f $(OOCD_IF) -f $(OOCD_BOARD) \
		-c init -c targets -c "halt" \
		-c "flash write_image erase $(TESTBINDIR)/firmware.elf" \
		-c "verify_image $(TESTBINDIR)/$(FIRMWARE)" \
		-c "reset run" -c shutdown

deploy-src: target-test check style clean gen-docs
	rm -fr $(TMPDIR)/$(TARGET)_src
	mkdir $(TMPDIR)/$(TARGET)_src
	cp -a ./* $(TMPDIR)/$(TARGET)_src/.
	rm -fr $(TMPDIR)/$(TARGET)_src/deploy
	rm -fr $(TMPDIR)/$(TARGET)_src/tools
	mkdir $(TMPDIR)/$(TARGET)_src/deploy
	(cd $(TMPDIR); tar --exclude=".svn" -jcvf $(TARGET)_src.tar.bz2  $(TARGET)_src)
	mv $(TMPDIR)/$(TARGET)_src.tar.bz2 $(DEPLOYDIR)/. 

deploy-bin: clean target-test 
	rm -fr $(TMPDIR)/$(TARGET)_bin
	mkdir $(TMPDIR)/$(TARGET)_bin
	cp CHANGELOG LICENSE README $(TMPDIR)/$(TARGET)_bin/.
	cp -a ./bin $(TMPDIR)/$(TARGET)_bin/.
	cp -a ./doc $(TMPDIR)/$(TARGET)_bin/.
	cp -a ./test-bin $(TMPDIR)/$(TARGET)_bin/.
	(cd $(TMPDIR); tar --exclude=".svn" -jcvf $(TARGET)_bin.tar.bz2  $(TARGET)_bin)
	mv $(TMPDIR)/$(TARGET)_bin.tar.bz2 $(DEPLOYDIR)/. 
	
deploy: deploy-src deploy-bin

tags: target
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q $(SRCDIR) 

clean: 
	make -C $(SRCDIR) clean
	make -C $(TESTSRCDIR) clean
	rm -fr doc/gen	
	rm -f bin/*
	rm -f test-bin/*
