# Directory containing test files
TEST_PATH     := test
# Space-separated list of header files (e.g., algebra.hpp)
HEADERS       := quiz.hpp
# Space-separated list of implementation files (e.g., algebra.cpp)
IMPLEMS       := quiz.cpp
# File containing main
DRIVER        := quizgen.cpp
# Expected name of executable file
EXECFILE      := quizgen
FILES         := $(DRIVER) $(IMPLEMS) $(HEADERS)
SHELL         := /bin/bash
HAS_CLANGTDY  := $(shell command -v clang-tidy 2> /dev/null)
HAS_CLANGFMT  := $(shell command -v clang-format 2> /dev/null)
CLANGTDY_CHKS := *,-google-build-using-namespace,-fuchsia-default-arguments,-llvm-header-guard,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-hicpp-no-array-decay,-cppcoreguidelines-pro-bounds-constant-array-index,-performance-unnecessary-value-param,-google-global-names-in-headers,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-readability-non-const-parameter,-cppcoreguidelines-owning-memory,-google-explicit-constructor,-cppcoreguidelines-special-member-functions,-hicpp-explicit-conversions,-[hicpp-special-member-functions,-hicpp-special-member-functions,-readability-redundant-member-init,-hicpp-use-override,-modernize-use-override,-clang-analyzer-optin.cplusplus.VirtualCall

.PHONY: test stylecheck formatcheck all clean

$(TEST_PATH)/unittest: $(TEST_PATH)/unittest.cpp $(IMPLEMS) $(HEADERS)
	@g++ -std=c++17 $(IMPLEMS) $(TEST_PATH)/unittest.cpp -o $(TEST_PATH)/unittest -pthread -lgtest

/usr/lib/libgtest.a:
	@echo -e "google test library not installed\n"
	@echo -e "Installing libgtest-dev. Please provide the password when asked\n"
	@sudo apt-get -y install libgtest-dev cmake
	@sudo apt-get install cmake # install cmake
	@echo -e "\nConfiguring libgtest-dev\n"
	@cd /usr/src/gtest; sudo cmake CMakeLists.txt; sudo make; sudo cp *.a /usr/lib
	@echo -e "Finished installing google test library\n"

test: /usr/lib/libgtest.a $(TEST_PATH)/unittest
	@echo -e "\n========================\nRunning unit test\n========================\n"
	@./$(TEST_PATH)/unittest --gtest_output="xml:test/unittest.xml"
	@echo -e "\n========================\nUnit test complete\n========================\n"

$(TEST_PATH)/compile_commands.json :
	@bash test/gen_ccjs.sh $(EXECFILE) $(DRIVER) "$(IMPLEMS)" "$(HEADERS)"
ifndef HAS_CLANGTDY
	@echo -e "clang-tidy not installed\n"
	@echo -e "Installing clang-tidy. Please provide password when asked\n"
	@sudo apt-get -y install clang-tidy
	@echo -e "Finished installing clang-tidy\n"
endif

stylecheck: $(TEST_PATH)/compile_commands.json
	@echo -e "========================\nRunning style checker\n========================\n"
	@clang-tidy -p=$(TEST_PATH) -quiet -checks=$(CLANGTDY_CHKS) -header-filter=.* -export-fixes=test/style.yaml $(IMPLEMS) $(HEADERS) $(DRIVER)
	@echo -e "========================\nStyle checker complete\n========================\n"

formatcheck:
ifndef HAS_CLANGFMT
	@echo -e "clang-format not installed.\n"
	@echo -e "Installing clang-format. Please provide the password when asked\n"
	@sudo apt-get -y install clang-format
	@echo -e "Finished installing clang-format\n"
endif
	@echo -e "========================\nRunning format checker\n========================"
	@bash $(TEST_PATH)/diff_format.sh $(FILES)
	@clang-format $(FILES) -output-replacements-xml > test/format.xml
	@echo -e "========================\nFormat checking complete\n========================\n"

all:	test stylecheck formatcheck

clean:
	@rm -f $(TEST_PATH)/unittest.xml
	@rm -f $(TEST_PATH)/style.yaml
	@rm -f $(TEST_PATH)/format.xml
	@rm -f $(TEST_PATH)/compile_commands.json
	@rm -f $(TEST_PATH)/unittest
