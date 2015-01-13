## GNU makefile, C source test driver
PROG=test-issue-20853

.PHONY: default
default: $(PROG)

.PHONY: testrun
testrun: $(PROG)
	( pass=y ; \
		./$(PROG)   ; if [ $$? -eq 0 ] ; then echo "OK" ; else echo "NG" ; pass=n ; fi ; \
		./$(PROG) e ; if [ $$? -eq 0 ] ; then echo "OK" ; else echo "NG" ; pass=n ; fi ; \
		./$(PROG) n ; if [ $$? -eq 0 ] ; then echo "OK" ; else echo "NG" ; pass=n ; fi ; \
		if [ "$$pass" = "y" ] ; then exit 0 ; else exit 1 ; fi )

## run tester on local rustc bin
LOCAL_RUSTC_BIN := $(PWD)/../../x86_64-unknown-linux-gnu/stage2/bin/rustc
.PHONY: testrunlocal
testrunlocal: export RUSTC_BIN=$(LOCAL_RUSTC_BIN)
testrunlocal: export LD_LIBRARY_PATH=$(PWD)/../../x86_64-unknown-linux-gnu/stage2/lib
testrunlocal: $(LOCAL_RUSTC_BIN) testrun

## show environment variables
.PHONY: testrunenvverbose testrunlocalenvverbose
testrunenvverbose: export ENV_VERBOSE=1
testrunenvverbose: testrun
testrunlocalenvverbose: export ENV_VERBOSE=1
testrunlocalenvverbose: testrunlocal

$(PROG): main.c Makefile
	$(CC) -Wall $< -o $(PROG)

.PHONY: clean
clean:
	rm $(PROG)
