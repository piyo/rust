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

$(PROG): main.c Makefile
	$(CC) -Wall $< -o $(PROG)

clean:
	rm $(PROG)
