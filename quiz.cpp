#include "quiz.hpp"
#include <fstream>
#include <iostream>
using namespace std;

question *create_question() {
  question *q;
  q = new question();
  string text, answer;
  cout << "Please enter your question: ";
  getline(cin, text);
  cout << "Please enter the answer: ";
  getline(cin, answer);
  q->set_text(text);
  q->set_answer(answer);
  cout << "Question added!\n\n";
  return q;
}
tf_question *create_tf_question() {
  tf_question *tf;
  tf = new tf_question();
  string text, answer;
  cout << "Please enter your True/False question: ";
  getline(cin, text);
  cout << "Please enter the answer (T/F): ";
  getline(cin, answer);
  tf->set_text(text);
  tf->set_answer(answer);
  cout << "True/False question added!\n\n";
  return tf;
}

void quiz::add_question(question *q) {
  if (_size == _capacity) {
    cout << "Sorry, can't create any more questions.\n";
  } else {
    _questions[_size++] = q;
  }
}

void quiz::display() {
  cout << "Question and Answer list\n";
  for (int i = 0; i < _size; i++) {
    cout << (i + 1) << ". " << (*(_questions + i))->text() << "\n"
         << "Answer: " << (*(_questions + i))->answer() << "\n";
  }
  cout << "\n";
}
void quiz::save(string filename) {
  ofstream outfile;
  outfile.open(filename);
  outfile << _size << "\n";
  for (int i = 0; i < _size; i++) {
    outfile << (*(_questions + i))->as_save_text();
  }
  outfile.close();
  cout << "File saved successfully!\n\n";
}

void quiz::load(string filename) {
  ifstream load_file;
  load_file.open(filename);
  // Retrieves the number of questions from the file
  load_file >> _size;
  // Remove \n after the integer value
  load_file.ignore();
  // Deletes old question array then create a new array whose capacity is
  // 30 elements more than the number of questions in the loaded file
  delete[] _questions;
  _capacity = _size + 30;
  _questions = new question *[_capacity];

  // Read all questions and assign question values to the array
  string temp;
  for (int i = 0; i < _size; i++) {
    getline(load_file, temp);
    if (temp == "[SQ]") {
      _questions[i] = new question();
    } else {
      _questions[i] = new tf_question();
    }
    getline(load_file, temp);
    _questions[i]->set_text(temp);
    getline(load_file, temp);
    _questions[i]->set_answer(temp);
  }
  load_file.close();
  cout << "File loaded successfully!\n\n";
}
