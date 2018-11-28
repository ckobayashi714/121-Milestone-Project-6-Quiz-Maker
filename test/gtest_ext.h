// This file contains implementations that extend the functionality of the
// google test framework
// Version: 0.1.1-beta

#include <gtest/gtest.h>
#include <random>
#include <string>
#include <iomanip>
#include <future>
#include <chrono>

// Run and retrieves the output of an executable program from
// the command line.
//
// @param prog_name name of the executable file
// @param input     keyboard input sent to the program
//
// @return output of the program
std::string main_output(std::string prog_name, std::string input)
{
  FILE *fp = popen(("echo \""+ input +"\" | ./" + prog_name).c_str(), "r");
  char buf[1024];
  std::string output = "";
  while (fgets(buf, 1024, fp)) {
    output += buf;
  }
  pclose(fp);
  return output;
}

// Converts a double value to a formatted string
//
// @param val   value to be formatted
// @param prec  decimal precision
//
// @return string representation of the double value
std::string to_string_double(double val, const int prec = 2)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(prec) << val;
    return out.str();
}

// Generate a string with random values from an alphanumeric character set
//
// @param max_length  length of string to generate
//
// @return            randomly generated string
std::string generate_string(int max_length){
  std::string possible_characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<> dist(0, possible_characters.size()-1);
  std::string ret = "";
  for(int i = 0; i < max_length; i++){
      int random_index = dist(engine); //get index between 0 and possible_characters.size()-1
      ret += possible_characters[random_index];
  }
  return ret;
}

// This macro is used to simulate the standard input (cin) for a code block
//
// @param input       simulated input
// @param statement   lambda code block to run
#define SIMULATE_CIN(input, statement) { \
  std::stringstream input_ss, output_ss; \
  auto old_output_buf = std::cout.rdbuf(output_ss.rdbuf()); \
  auto old_input_buf = std::cin.rdbuf(input_ss.rdbuf()); \
  input_ss.str(input); \
  statement; \
  std::cin.rdbuf(old_input_buf); \
  std::cout.rdbuf(old_output_buf); \
}

// This macro checks if the output of an executable program matches an expected
// output.
//
// @param prog_name name of the executable file
// @param input     keyboard input sent to the program
// @param output    expected output of the program
#define ASSERT_EXECIO_EQ(prog_name, input, output) { \
  if ( access( prog_name, F_OK ) == -1 ) { \
    GTEST_FATAL_FAILURE_("      cannot test '" prog_name "': no such file"); \
  } \
  ASSERT_EQ(main_output(prog_name, input), output) << "   Input: " << input; \
}

// Version of ASSERT_EXECIO_EQ that uses google mock's matchers
//
// @param prog_name name of the executable file
// @param input     keyboard input sent to the program
// @param output    expected output of the program
#define ASSERT_EXECIO_THAT(prog_name, input, matcher) {\
  if ( access( prog_name, F_OK ) == -1 ) { \
    GTEST_FATAL_FAILURE_("      cannot test '" prog_name "': no such file"); \
  } \
  ASSERT_THAT(main_output(prog_name, input), matcher) << "   Input: " << input; \
}

// This macro asserts that the result of performing the statement
// is equal to the expected value in the standard output (cout)
//
// @param expected  expected string value
// @param stmt      statement(s) performed
#define ASSERT_SIO_EQ(input, expected, stmt) { \
  std::stringstream input_ss, output_ss; \
  auto old_inputbuf = std::cin.rdbuf(input_ss.rdbuf()); \
  auto old_outputbuf = std::cout.rdbuf(output_ss.rdbuf()); \
  input_ss.str(input); \
  stmt; \
  std::cin.rdbuf(old_inputbuf); \
  std::cout.rdbuf(old_outputbuf); \
  std::string output = output_ss.str(); \
  ASSERT_EQ(output, expected); \
}

// Version of ASSERT_SIO_EQ that uses google mock's matchers
//
// @param expected  expected string value
// @param stmt      statement(s) performed
#define ASSERT_SIO_THAT(input, expected, stmt) { \
  std::stringstream input_ss, output_ss; \
  auto old_inputbuf = std::cin.rdbuf(input_ss.rdbuf()); \
  auto old_outputbuf = std::cout.rdbuf(output_ss.rdbuf()); \
  input_ss.str(input); \
  stmt; \
  std::cin.rdbuf(old_inputbuf); \
  std::cout.rdbuf(old_outputbuf); \
  std::string output = ss.str(); \
  ASSERT_THAT(output, expected); \
}

// This macro checks whether a function executes within a given time
//
// A thread is created to run the statement and update the status of a promise
// object. A future object is created from the promise object to check whether
// the promise  object's value was updated within the specified duration. If the
// promise's value is not changed in time, the function is considered to have
// timed out.
//
// Code based on: http://antonlipov.blogspot.com/2015/08/how-to-timeout-tests-in-gtest.html
//
// @param secs  seconds to wait before statement is considered to have
//              timed out
// @param stmt  statement to be tested
#define ASSERT_DURATION_LE(secs, stmt) { \
  std::promise<bool> completed; \
  auto stmt_future = completed.get_future(); \
  std::thread([&](std::promise<bool>& completed) { \
    stmt; \
    completed.set_value(true); \
  }, std::ref(completed)).detach(); \
  if(stmt_future.wait_for(std::chrono::seconds(secs)) == std::future_status::timeout) \
    GTEST_FATAL_FAILURE_("       timed out (> " #secs \
    " seconds). Check code for infinite loops"); \
}
