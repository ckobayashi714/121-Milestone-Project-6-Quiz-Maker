#pragma once
#include <iostream>
#include <string>
using namespace std;

class question {
private:
  string _text;
  string _answer;

public:
  question(string text, string answer) {
    _text = text;
    _answer = answer;
  }
  question() : question("", "") {}

  void set_text(string text) { _text = text; }
  virtual void set_answer(string answer) { _answer = answer; }
  string text() { return _text; }
  string answer() { return _answer; }
  bool check(string answer) { return answer == _answer; }
  virtual string as_save_text() {
    return "[SQ]\n" + _text + "\n" + _answer + "\n";
  }
};

class quiz {
private:
  question **_questions;
  int _size;
  int _capacity;

public:
  quiz(int capacity) {
    _size = 0;
    _capacity = capacity;
    _questions = new question *[capacity];
  }
  quiz() : quiz(100) {}
  ~quiz() {
    for (int i = 0; i < _capacity; i++) {
      delete (*(_questions + i));
      //(*(_questions + i)) = nullptr;
    }
    delete[] _questions;
  }

  void set_size(int size) { _size = size; }
  void set_capacity(int capacity) { _capacity = capacity; }
  int size() { return _size; }
  int capacity() { return _capacity; }
  void add_question(question *q);
  question *get_question(int index) { return _questions[index]; }
  void display();
  void save(string filename);
  void load(string filename);
};
question *create_question();

class tf_question : public question {
public:
  tf_question() {
    set_text("");
    set_answer("T");
  }
  tf_question(string text, string answer) {
    set_text(text);
    set_answer(answer);
  }
  virtual void set_answer(string answer) {
    if ((answer != "T") && (answer != "F")) {
      cout << "Invalid answer. Setting answer to True.\n";
      question::set_answer("T");
    } else {
      question::set_answer(answer);
    }
  }
  virtual string as_save_text() {
    return "[TFQ]\n" + text() + "\n" + answer() + "\n";
  }
};
tf_question *create_tf_question();
