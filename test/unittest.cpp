#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "gtest_ext.h"
#include "../quiz.hpp"
using ::testing::HasSubstr;
using ::testing::StartsWith;
using ::testing::MatchesRegex;
using ::testing::ContainsRegex;

TEST(question, PublicMethodsPresent) {
  question q;
  ASSERT_NO_THROW({
    q.text();
    q.answer();
    q.set_text("a");
    q.set_answer("b");
    q.as_save_text();
    q.check("c");
  });
}

TEST(question, DefaultConstructor) {
  question q;
  ASSERT_EQ(q.text(), "");
  ASSERT_EQ(q.answer(), "");
}

TEST(question, TextAnswerConstructor) {
  std::string unittest_text, unittest_answer;
  unittest_text = generate_string(10);
  unittest_answer = generate_string(10);
  question q(unittest_text, unittest_answer);
  ASSERT_EQ(q.text(), unittest_text);
  ASSERT_EQ(q.answer(), unittest_answer);
}

TEST(question, MutatorsAndAccessors) {
  question q;
  std::string text, answer;
  text = generate_string(10);
  answer = generate_string(10);
  ASSERT_NO_THROW({
    q.set_text(text);
    q.set_answer(answer);
  });
  ASSERT_EQ(q.text(), text);
  ASSERT_EQ(q.answer(), answer);
}

TEST(question, as_save_text) {
  question q;
  std::string text, answer;
  text = generate_string(10);
  answer = generate_string(10);
  q.set_text(text);
  q.set_answer(answer);
  std::string unittest_output = "[SQ]\n" + text + "\n" + answer + "\n";
  ASSERT_EQ(q.as_save_text(), unittest_output);
}

TEST(question, check) {
  question q;
  std::string correct_answer, incorrect_answer;
  correct_answer = generate_string(10);
  incorrect_answer = generate_string(10);
  q.set_answer(correct_answer);
  ASSERT_TRUE(q.check(correct_answer));
  ASSERT_FALSE(q.check(incorrect_answer));
}

TEST(tf_question, PublicMethodsPresent) {
  tf_question t;
  ASSERT_NO_THROW({
    t.text();
    t.answer();
    t.set_text("a");
    t.set_answer("T");
    t.as_save_text();
    t.check("T");
  });
}

TEST(tf_question, DefaultConstructor) {
tf_question t;
  ASSERT_EQ(t.text(), "");
  ASSERT_EQ(t.answer(), "T");
}

TEST(tf_question, TextAnswerConstructor) {
  std::string unittest_text, unittest_answer;
  unittest_text = generate_string(10);
  unittest_answer = rand() % 2 == 1 ? "T" : "F";
  tf_question q(unittest_text, unittest_answer);
  ASSERT_EQ(q.text(), unittest_text);
  ASSERT_EQ(q.answer(), unittest_answer);
}

TEST(tf_question, MutatorsAndAccessors) {
  question q;
  std::string text, answer;
  text = generate_string(10);
  answer = rand() % 2 == 1 ? "T" : "F";
  ASSERT_NO_THROW({
    q.set_text(text);
    q.set_answer(answer);
  });
  ASSERT_EQ(q.text(), text);
  ASSERT_EQ(q.answer(), answer);
}

TEST(tf_question, InvalidAnswer) {
  std::string text, answer;
  text = generate_string(10);
  answer =  "ABC";
  ASSERT_SIO_EQ("", "Invalid answer. Setting answer to True.\n", {
    tf_question t(text, answer);
  });
  tf_question t2;
  ASSERT_SIO_EQ("", "Invalid answer. Setting answer to True.\n", {
    t2.set_answer("ABC");
  });
}

TEST(tf_question, as_save_text) {
  tf_question t;
  std::string text, answer;
  text = generate_string(10);
  answer = rand() % 2 == 1 ? "T" : "F";
  t.set_text(text);
  t.set_answer(answer);
  std::string unittest_output = "[TFQ]\n" + text + "\n" + answer + "\n";
  ASSERT_EQ(t.as_save_text(), unittest_output);
}

TEST(tf_question, check) {
  tf_question t;
  std::string correct_answer, incorrect_answer;
  correct_answer = "T";
  incorrect_answer = "F";
  t.set_answer(correct_answer);
  ASSERT_TRUE(t.check(correct_answer));
  ASSERT_FALSE(t.check(incorrect_answer));
  ASSERT_FALSE(t.check("ABC"));
}

TEST(quiz, DefaultConstructor) {
  quiz q;
  ASSERT_EQ(q.size(), 0);
  ASSERT_EQ(q.capacity(), 100);
}

TEST(quiz, CapacityConstructor) {
  int unittest_capacity = rand() % 100 + 1;
  quiz q(unittest_capacity);
  ASSERT_EQ(q.size(), 0);
  ASSERT_EQ(q.capacity(), unittest_capacity);
}

TEST(quiz, add_question) {
  quiz q(3);
  for (int i = 0; i < 3; i++) {
    q.add_question(new question());
    int unittest_changed_size = i + 1;
    ASSERT_EQ(q.size(), unittest_changed_size);
  }
  ASSERT_SIO_EQ("", "Sorry, can't create any more questions.\n", {
    q.add_question(new question());
  });
}

TEST(quiz, add_tf_question) {
  quiz q(3);
  for (int i = 0; i < 3; i++) {
    q.add_question(new tf_question());
    int unittest_changed_size = i + 1;
    ASSERT_EQ(q.size(), unittest_changed_size);
  }
  ASSERT_SIO_EQ("", "Sorry, can't create any more questions.\n", {
    q.add_question(new tf_question());
  });
}

TEST(quiz, get_question) {
  int capacity = rand() % 15 + 4;
  quiz q(capacity);
  question** questions = new question*[capacity];

  for (int i = 0; i < capacity; i++) {
    std::string text, answer;
    question* temp;
    if (i % 2 == 0) {
      text = generate_string(10);
      answer = generate_string(10);
      temp = new question(text, answer);
    } else {
      text = generate_string(10);
      answer = rand() % 2 == 1 ? "T" : "F";
      temp = new tf_question(text, answer);
    }
    questions[i] = temp;
    q.add_question(temp);
  }

  for (int i = 0; i < capacity; i++) {
    question* unittest_question_object = questions[i];
    ASSERT_EQ(q.get_question(i)->text(), unittest_question_object->text());
    ASSERT_EQ(q.get_question(i)->answer(), unittest_question_object->answer());
  }
}

TEST(quiz, display) {
  int capacity = rand() % 10 + 1;
  quiz q(capacity);
  std::ostringstream unittest_display;
  unittest_display << "Question and Answer list\n";

  for (int i = 0; i < capacity; i++) {
    std::string text, answer;
    question* temp;
    if (i % 2 == 0) {
      text = generate_string(10);
      answer = generate_string(10);
      temp = new question(text, answer);
    } else {
      text = generate_string(10);
      answer = rand() % 2 == 1 ? "T" : "F";
      temp = new tf_question(text, answer);
    }

    unittest_display << (i+1) << ". " << text << "\n" << "Answer: " << answer
                     << "\n";
    q.add_question(temp);
  }
  unittest_display << "\n";
  ASSERT_SIO_EQ("", unittest_display.str(), {
    q.display();
  });
}

TEST(quiz, save) {
  for(int i = 0; i < 10; i++) {
    int repetitions = rand() % 10 + 1;
    quiz q(repetitions);

    question questions[repetitions];
    std::string expected_file_contents = std::to_string(repetitions) + "\n";
    for (int j = 0; j < repetitions; j++) {
      std::string text, answer;
      question* temp;
      if (i % 2 == 0) {
        text = generate_string(10);
        answer = generate_string(10);
        temp = new question(text, answer);
        expected_file_contents += "[SQ]\n";
        expected_file_contents += text + "\n";
        expected_file_contents += answer + "\n";
      } else {
        text = generate_string(10);
        answer = rand() % 2 == 1 ? "T" : "F";
        temp = new tf_question(text, answer);
        expected_file_contents += "[TFQ]\n";
        expected_file_contents += text + "\n";
        expected_file_contents += answer + "\n";
      }

      q.add_question(temp);
    }

    std::string filename = generate_string(5) + ".txt";
    ASSERT_DURATION_LE(3, {
      ASSERT_SIO_EQ("", "File saved successfully!\n\n", {
        q.save(filename);
      });
    });

    ifstream save_file(filename);
    ASSERT_TRUE(save_file.good()) << "Save file was not created";
    std::string actual_file_contents, temp_holder;
    while(save_file >> temp_holder) {
      actual_file_contents += temp_holder + "\n";
    }
    save_file.close();
    remove(filename.c_str());

    ASSERT_EQ(actual_file_contents, expected_file_contents);
  }
}

TEST(quiz, load) {
  for(int i = 0; i < 10; i++) {
    int file_questions_size = rand() % 10 + 1;
    question** file_questions = new question*[file_questions_size];
    std::string expected_file_contents = std::to_string(file_questions_size) + "\n";
    for (int j = 0; j < file_questions_size; j++) {
      std::string text, answer;
      question* temp;
      if (i % 2 == 0) {
        text = generate_string(10);
        answer = generate_string(10);
        temp = new question(text, answer);
        expected_file_contents += "[SQ]\n";
        expected_file_contents += text + "\n";
        expected_file_contents += answer + "\n";
      } else {
        text = generate_string(10);
        answer = rand() % 2 == 1 ? "T" : "F";
        temp = new tf_question(text, answer);
        expected_file_contents += "[TFQ]\n";
        expected_file_contents += text + "\n";
        expected_file_contents += answer + "\n";
      }
      file_questions[j] = temp;
    }

    std::string filename = generate_string(5) + ".txt";
    ofstream out_file(filename);
    out_file << expected_file_contents;
    out_file.close();

    quiz q;
    ASSERT_DURATION_LE(3, {
      ASSERT_SIO_EQ("", "File loaded successfully!\n\n", {
        q.load(filename);
      });
    });

    ASSERT_EQ(q.size(), file_questions_size);
    ASSERT_EQ(q.capacity(), file_questions_size + 30);
    for (int j = 0; j < file_questions_size; j++) {
      ASSERT_EQ(q.get_question(j)->text(), file_questions[j]->text());
      ASSERT_EQ(q.get_question(j)->answer(), file_questions[j]->answer());
      delete file_questions[j];
    }
    delete[] file_questions;
    remove(filename.c_str());
  }
}
//

TEST(Global, CreateQuestion) {
  for(int i = 0; i < 10; i++) {
    std::string text, answer;
    text = generate_string(10);
    answer = generate_string(10);
    question *temp, *actual;
    temp = new question(text, answer);
    std::string input = text+"\n"+answer;
    SIMULATE_CIN(input, {
      actual = create_question();
    });
    ASSERT_EQ(temp->text(), actual->text());
    ASSERT_EQ(temp->answer(), actual->answer());
  }
}

TEST(Global, CreateTFQuestion) {
  for(int i = 0; i < 10; i++) {
    std::string text, answer;
    text = generate_string(10);
    answer = rand() % 2 == 1 ? "T" : "F";
    tf_question *temp, *actual;
    temp = new tf_question(text, answer);
    std::string input = text+"\n"+answer;
    SIMULATE_CIN(input, {
      actual = create_tf_question();
    });
    ASSERT_EQ(temp->text(), actual->text());
    ASSERT_EQ(temp->answer(), actual->answer());
  }
}



TEST(main, CreateQuestionChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nPlease enter your question: Please enter the "
                         "answer: Question added!\n\nWhat would you like to do?"
                         "\na. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\nd. Save questions\n"
                         "e. Load questions\nf. Quit\nChoice: \nThank you for"
                         " using QuizMaker!\n";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", "a\nX\nX\nf", expected);
  });
}

TEST(main, CreateTFQuestionChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nPlease enter your True/False question: "
                         "Please enter the answer (T/F): "
                         "True/False question added!\n\nWhat would you like to do?"
                         "\na. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\nd. Save questions\n"
                         "e. Load questions\nf. Quit\nChoice: \nThank you for"
                         " using QuizMaker!\n";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", "b\nX\nT\nf", expected);
  });
}

TEST(main, FiniteChoiceLoop) {
  srand (time(NULL));
  ASSERT_DURATION_LE(3, {
    main_output("quizgen", "a\nX\nX\nf");
  });
}

TEST(main, DisplayQuestionsChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nPlease enter your question: Please enter the "
                         "answer: Question added!\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nQuestion and Answer list\n1. X\nAnswer: X"
                         "\n\nWhat would you like to do?\na. Create a question"
                         "\nb. Create a True/False question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nThank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", "a\nX\nX\nc\nf", expected);
  });
}

TEST(main, SaveQuestionsChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nWhat filename would you like to use? File "
                         "saved successfully!\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nThank you for using QuizMaker!\n";
  string filename = generate_string(5) + ".txt";
  string input = "d\n" + filename + "\nf";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", input, expected);
  });
  std::remove(filename.c_str());
}

TEST(main, LoadQuestionsChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nWhat file would you like to load? File "
                         "loaded successfully!\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nThank you for using QuizMaker!\n";
  string filename = generate_string(5) + ".txt";
  string input = "e\n" + filename + "\nf";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", input, expected);
  });
  std::remove(filename.c_str());
}

TEST(main, QuitChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nThank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", "f", expected);
  });
}

TEST(main, InvalidOptionChoice) {
  std::string expected = "Welcome to QuizMaker\n\nWhat would you like to do?\n"
                         "a. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nInvalid option\n\nWhat would you like to "
                         "do?\na. Create a question\nb. Create a True/False "
                         "question\nc. Display questions\n"
                         "d. Save questions\ne. Load questions\nf. Quit\n"
                         "Choice: \nThank you for using QuizMaker!\n";
  ASSERT_DURATION_LE(3, {
    ASSERT_EXECIO_EQ("quizgen", "x\nf",expected);
  });
}

TEST(main, ChoiceLoopLowercase) {
  int repetitions = rand() % 3 + 1;
  for (int i = 0; i < repetitions; i++) {
    std::string input = "";
    std::string question_list = "Question and Answer list\n";
    std::string expected = "Welcome to QuizMaker\n\n";
    std::vector<std::string> filenames;
    std::string load_file_name = generate_string(10) + ".txt";
    ofstream out_file(load_file_name);
    out_file << 2 << "\n" << "[SQ]\nq\na\n[TFQ]\nq\nT\n";
    out_file.close();
    int question_ctr = 1;
    char choice;
    do {
      choice = rand() % 6 + 97;

      expected += "What would you like to do?\na. Create a question\nb. Create a True/False "
                  "question\nc. Display questions\nd. Save questions\n"
                  "e. Load questions\nf. Quit\nChoice: \n";
      std::string s_choice(1, choice);
      input += s_choice + "\\n";
      switch (choice) {
        case 'a':
          expected += "Please enter your question: Please enter the answer: "
                      "Question added!\n\n";
          input += "q\\na\\n";
          question_list += std::to_string(question_ctr) +". q\nAnswer: a\n";
          question_ctr++;
          break;
        case 'b':
          expected += "Please enter your True/False question: Please enter the answer (T/F): "
                      "True/False question added!\n\n";
          input += "q\\nT\\n";
          question_list += std::to_string(question_ctr) +". q\nAnswer: T\n";
          question_ctr++;
          break;
        case 'c':
          expected += question_list + "\n";
          break;
        case 'd':
          {
            expected += "What filename would you like to use? File saved successfully!\n\n";
            std::string filename = generate_string(5) + ".txt";
            filenames.push_back(filename);
            input += filename+"\n";
            break;
          }
        case 'e':
          {
            expected += "What file would you like to load? File loaded successfully!\n\n";
            input += load_file_name+"\n";
            question_list = "Question and Answer list\n1. q\nAnswer: a\n2. q\nAnswer: T\n";
            question_ctr = 3;
            break;
          }
        case 'f':
          expected += "Thank you for using QuizMaker!\n";
          break;
        default:
          expected += "Invalid option\n\n";
      }
    } while(choice!= 'f');
    ASSERT_DURATION_LE(3, {
      ASSERT_EXECIO_EQ("quizgen", input, expected);
    });
    remove(load_file_name.c_str());
    for(std::string fn : filenames) {
      remove(fn.c_str());
    }
  }
}

TEST(main, ChoiceLoopUppercase) {
  int repetitions = rand() % 10 + 1;
  for (int i = 0; i < repetitions; i++) {
    std::string input = "";
    std::string question_list = "Question and Answer list\n";
    std::string expected = "Welcome to QuizMaker\n\n";
    std::vector<std::string> filenames;
    std::string load_file_name = generate_string(10) + ".txt";
    ofstream out_file(load_file_name);
    out_file << 2 << "\n" << "[SQ]\nq\na\n[TFQ]\nq\nT\n";
    out_file.close();
    int question_ctr = 1;
    char choice;
    do {
      choice = rand() % 6 + 65;

      expected += "What would you like to do?\na. Create a question\nb. Create a True/False "
                  "question\nc. Display questions\nd. Save questions\n"
                  "e. Load questions\nf. Quit\nChoice: \n";
      std::string s_choice(1, choice);
      input += s_choice + "\\n";
      switch (choice) {
        case 'A':
          expected += "Please enter your question: Please enter the answer: "
                      "Question added!\n\n";
          input += "q\\na\\n";
          question_list += std::to_string(question_ctr) +". q\nAnswer: a\n";
          question_ctr++;
          break;
        case 'B':
          expected += "Please enter your True/False question: Please enter the answer (T/F): "
                      "True/False question added!\n\n";
          input += "q\\nT\\n";
          question_list += std::to_string(question_ctr) +". q\nAnswer: T\n";
          question_ctr++;
          break;
        case 'C':
          expected += question_list + "\n";
          break;
        case 'D':
          {
            expected += "What filename would you like to use? File saved successfully!\n\n";
            std::string filename = generate_string(5) + ".txt";
            filenames.push_back(filename);
            input += filename+"\n";
            break;
          }
        case 'E':
          {
            expected += "What file would you like to load? File loaded successfully!\n\n";
            input += load_file_name+"\n";
            question_list = "Question and Answer list\n1. q\nAnswer: a\n2. q\nAnswer: T\n";
            question_ctr = 3;
            break;
          }
        case 'F':
          expected += "Thank you for using QuizMaker!\n";
          break;
        default:
          expected += "Invalid option\n\n";
      }
    } while(choice!= 'F');
    ASSERT_DURATION_LE(3, {
      ASSERT_EXECIO_EQ("quizgen", input, expected);
    });
    remove(load_file_name.c_str());
    for(std::string fn : filenames) {
      remove(fn.c_str());
    }
  }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
