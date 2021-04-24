.PHONY: all tidy format ci makefile-exe makefile-lib checkmakefile

all: makefile-exe
	$(MAKE) -C quisp -j

run-module-test:
	cd module_tests && ./runtest

run-unit-test: lib
	$(MAKE) -C quisp run-unit-test

exe: makefile-exe
	$(MAKE) -C quisp -j

lib: makefile-lib
	$(MAKE) -C quisp -j

ci: quisp/Makefile
	$(MAKE) -C quisp ci

format: quisp/Makefile
	$(MAKE) -C quisp format

tidy: quisp/Makefile
	$(MAKE) -C quisp format

googletest:
	git submoudle update --init

makefile-exe:
	cd quisp && opp_makemake -f --deep -O out -i ./makefrag

makefile-lib:
	cd quisp && opp_makemake -f --deep -O out -i ./makefrag -M debug  --make-so

clean:
	$(RM) quisp/out quisp/Makefile quisp/quisp quisp/quisp_dbg quisp/run_unit_test quisp/libquisp*

checkmakefile:
	@if [ ! -f $(QUISP_MAKEFILE) ]; then \
	echo; \
	echo '===================================================================================================='; \
	echo 'quisp/Makefile does not exist. Please use "make makefile-exe" or "make makefile-lib" to generate it!'; \
	echo '===================================================================================================='; \
	echo; \
	exit 1; \
	fi

