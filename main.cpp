#include <cctype>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

random_device rd;

class HangTheMan {
public:
  int lives = 6;
  vector<string> words;

  HangTheMan() { words = {"test", "test1"}; }

  HangTheMan(vector<string> words) {
    for (int i = 0; i < words.size(); i++) {
      this->words.push_back(words.at(i));
    }
  }

  // function for printing out the hangman board
  void print(HangTheMan hangman, string incorrectGuesses) {
    cout << "  +----+" << endl;
    cout << "  |    |   Incorrect Guesses:" << endl;

    // shorthand if statement for outputting the head
    // if they have 5 lives or less
    cout << (lives < 6 ? "  o    |" : "       |");

    // outputs the incorrect guessed letters next to the hangman
    cout << "   ";
    for (int i = 0; i < incorrectGuesses.length(); i++) {
      cout << incorrectGuesses[i] << " ";
    }
    cout << endl;

    // determines how many limbs to output (body and arm(s)) depending on how
    // many lives are left. 4 lives = body, 3 lives = body and left arm,
    // 2 lives = body and both arms
    if (lives < 5) {
      if (lives < 4)
        if (lives < 3)
          cout << " /|\\   |";
        else
          cout << " /|    |";
      else
        cout << "  |    |";
    } else
      cout << "       |";

    cout << "   Lives: " << hangman.lives << endl;

    // determines how many legs to output depending on how many lives
    // are left. 1 life = left leg, 0 lives = both legs (end of the round).
    if (lives < 2) {
      if (lives < 1)
        cout << " / \\   |";
      else
        cout << " /     |";
    } else
      cout << "       |";
    cout << endl;

    cout << "       |" << endl;
    cout << "=========" << endl;
  }
};

// function for selecting the word to be used for the round
void selectWord(HangTheMan *hangman, string *word, string *activeWord) {
  // pick random index in the word list to select a word
  // to be used for the round
  mt19937 seed(rd());
  uniform_int_distribution<> distr(0, hangman->words.size() - 1);
  int pos = distr(seed);

  // only assign a word from the word list in the hangman
  // class if there are words left
  if (hangman->words.size() != 0) {
    // set word var to selected word from list
    *word = hangman->words.at(pos);

    // append underscores to activeWord for each char in the selected word so it
    // can be used for gameplay
    *activeWord = "";
    for (char c : hangman->words.at(pos)) {
      if (isalpha(c))
        *activeWord += "_";
      else
        *activeWord += c;
    }

    // remove selected word from the word list so it isn't selected again
    hangman->words.erase(hangman->words.begin() + pos);
  }
}

// function for checking whether a guess has already been guessed
// and whether that guess if correct or not
int checkGuess(string word, string activeWord, string incorrectGuesses,
               char guess) {
  // return 0 if the guess has already been guessed
  for (char c : activeWord) {
    if (c == tolower(guess))
      return 0;
  }
  for (char c : incorrectGuesses) {
    if (c == tolower(guess))
      return 0;
  }

  // return 1 if the guess is correct
  for (int i = 0; i < word.length(); i++) {
    if (tolower(guess) == tolower(word[i])) {
      return 1;
    }
  }

  // return -1 if the guess is incorrect
  return -1;
}

int main() {
  int poi = 0; // poi = pregenerated or input
  cout << "Welcome to Hangman!" << endl << endl;

  // make sure input for selection menu is valid
  while (poi == 0) {
    // selection menu
    cout << "Select an option:" << endl;
    cout << "1. use pregenerated words." << endl;
    cout << "2. input your own words." << endl;
    cout << "3. quit" << endl << endl;
    cin >> poi;

    // check input to make sure it's 1, 2, or 3
    if (poi != 1) {
      if (poi != 2) {
        if (poi != 3) {
          // output that selection is invalid and reset poi to 0
          // so the loop repeats
          cout << "\033[2J\033[1;1H"; // clear console
          cout << "Invalid choice\n" << endl;
          poi = 0;
        }
      }
    }

    // if selection is 3 exit the program
    if (poi == 3) {
      cout << "\033[2J\033[1;1H"; // clear console
      cout << "Thanks for playing!" << endl << endl;

      return 0; // return 0 to end program because the user selected quit
    }
  }
  cout << "\033[2J\033[1;1H"; // clear console

  // create hangman object
  HangTheMan hangman;

  // check if user selected to input their own words
  if (poi == 2) {
    // vector for holding inputted words
    vector<string> words;
    int end;

    // determine how many words the user would like to input
    // and store it in end
    cout << "How many words would you like to enter?" << endl;
    cin >> end;

    cout << "\033[2J\033[1;1H"; // clear console

    // get user inputted words
    cout << "Enter your words:" << endl << endl;
    string word;
    for (int i = 0; i < end; i++) {
      cin >> word;
      words.push_back(word);
    }

    hangman = HangTheMan(words);
  }

  /***********************/
  /*      MAIN GAME      */
  /***********************/

  cout << "\033[2J\033[1;1H"; // clear console

  // initialize vars for the selected word, the string used for gameplay output,
  // the string holding the incorrect guesses, the users guess, and the loop
  // control/user continuation input
  string word, activeWord, incorrectGuesses;
  char guess, playAgain = 'y';

  while (tolower(playAgain) == 'y') {
    // reset vars to blanks/defaults at the start of each round
    word = "", activeWord = "", incorrectGuesses = "";
    hangman.lives = 6;

    // call function for selecting the word to be used for the round
    selectWord(&hangman, &word, &activeWord);

    // main loop for the round; exit when activeWord is the same as word meaning
    // the user solved the word or when they have 0 lives left meaning they ran
    // out of lives.
    while (word != activeWord && hangman.lives > 0) {

      // output the hangmanboard and activeWord
      hangman.print(hangman, incorrectGuesses);
      cout << activeWord << endl;

      // ask the user to guess a letter
      cout << "Guess a letter: ";
      cin >> guess;

      // make sure guess is actually a letter, if not ask user
      // to guess a letter again
      while (!isalpha(guess)) {
        cout << "\nInvalid guess\nGuess a letter: ";
        cin >> guess;
      }

      cout << "\033[2J\033[1;1H"; // clear console

      // check if the guess is incorrect, if so, append the incorrect guess to
      // the incorrectGuesses string and take away a life
      if (checkGuess(word, activeWord, incorrectGuesses, guess) == -1) {
        cout << "Incorrect Guess :(" << endl;
        hangman.lives--;
        incorrectGuesses += tolower(guess);
      }

      // check if the guess is correct, if so, check for each occurance in word
      // and change each corresponding underscore in activeWord to the letter
      else if (checkGuess(word, activeWord, incorrectGuesses, guess) == 1) {
        cout << "Correct Guess!" << endl;
        for (int i = 0; i < word.length(); i++) {
          if (tolower(guess) == tolower(word[i]))
            activeWord[i] = word[i];
        }
      }

      // check if the user has already guessed that letter
      else if (checkGuess(word, activeWord, incorrectGuesses, guess) == 0) {
        cout << "You've already guessed that letter." << endl;
      }
    }

    // output the hangmanboard and activeWord
    hangman.print(hangman, incorrectGuesses);
    cout << activeWord << endl;

    // check whether ther user guessed the word or not
    if (hangman.lives == 0) {
      cout << "You didn't guess the word :(" << endl;
      hangman.words.push_back(word);
    } else {
      cout << "You guessed the word! The word was \"" << word << "\"." << endl;
    }

    // check if there are any words left in the list, if not, exit game loop
    if (hangman.words.size() == 0)
      break;

    // loop to make sure input for playing again is valid
    while (1) {
      cout << "\nWould you like to play again? (\"y\" for yes and \"n\" for no)"
           << endl;
      cin >> playAgain;

      // if input is valid exit the loop
      if (tolower(playAgain) == 'y' || tolower(playAgain) == 'n') {
        cout << "\033[2J\033[1;1H"; // clear console
        break;
      }

      cout << "\nInvalid choice" << endl;
    }
  }

  // check to see if word list is empty so user can be congradualted for
  // guessing
  if (hangman.words.size() == 0) {
    cout << "\nCongratulations, you've guessed all the words!" << endl;
  }

  cout << "\nThanks for playing!" << endl;

  return 0;
}
