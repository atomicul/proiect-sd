CFLAGS := $(CFLAGS) -O3 -g --std=c11 -D_POSIX_C_SOURCE=200809L -Iinclude

ALGORITHMS_DIR = algorithms
ALGORITHMS = $(notdir $(wildcard $(ALGORITHMS_DIR)/*))
CSV_FILES = $(ALGORITHMS:%=%.csv)

TEST_OBJECTS = main.o src/read_buffer.o src/check_sorted.o src/checksum.o src/sort_worker.o

.PHONY: help
help:
	@echo "make help            Displays this help screen"
	@echo "make clean           Deletes generated files"
	@echo "make {algorithm}     Compiles test executable"
	@echo "make {algorithm}.csv Creates the test results file"
	@echo "Where {algorithm} is one of:" $(ALGORITHMS)

$(CSV_FILES): %.csv: %
	./test_executable.sh $* $$(find $(ALGORITHMS_DIR)/$* -name "*.in") > $@

$(ALGORITHMS): %: $(ALGORITHMS_DIR)/%/sort.o $(TEST_OBJECTS)
	$(CC) -o $@ $^ -pthread

.PHONY: clean
clean:
	rm -f *algorithms/*/*.o $(TEST_OBJECTS) $(ALGORITHMS) $(CSV_FILES)
