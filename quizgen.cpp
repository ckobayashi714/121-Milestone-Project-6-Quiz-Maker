#include "quiz.hpp"
#include <iostream>
#include <string>
using namespace std;

int main() {
  /* This will create an array of 100 question objects*/
  // quiz q;
  // question q;
  // question question;
  quiz quizmaker(50);
  /* This program will reapteadly ask the user to input a question or exit. If
  the user inputs something other than a or b (capital letters too), they will
  be notified with an invalid message, but will then be asked again what option
  they would like to choose.*/
  char choice;
  string filename;

  cout << "Welcome to QuizMaker\n\n";

  do {
    cout << "What would you like to do?\n"
         << "a. Create a question\n"
         << "b. Create a True/False question\n"
         << "c. Display questions\n"
         << "d. Save questions\n"
         << "e. Load questions\n"
         << "f. Quit\n"
         << "Choice: ";
    cin >> choice;
    cin.ignore();
    cout << endl;
    switch (choice) {
    case 'a':
    case 'A':
      quizmaker.add_question(create_question());
      break;
    case 'b':
    case 'B':
      quizmaker.add_question(create_tf_question());
      break;
    case 'c':
    case 'C':
      quizmaker.display();
      break;
    case 'd':
    case 'D':
      cout << "What filename would you like to use? ";
      getline(cin, filename);
      quizmaker.save(filename);
      break;
    case 'e':
    case 'E':
      cout << "What file would you like to load? ";
      getline(cin, filename);
      quizmaker.load(filename);
      break;
    case 'f':
    case 'F':
      break;
    default:
      cout << "Invalid option\n\n";
    }
  } while (choice != 'f' && choice != 'F');
  cout << "Thank you for using QuizMaker!\n";
  return 0;
}
