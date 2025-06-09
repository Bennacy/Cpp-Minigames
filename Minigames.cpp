#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <time.h>

using namespace std;

#define Helpers
#define HangmanArt
#define Snake
#define Hangman
#define TicTacToe
#define Dinosaur
#define Connect4

#if defined Helpers //Helper Print functions because "std::cout << " is a pain in the ass
void print(string toWrite){
   cout << toWrite << endl;
}

void print(int toWrite){
   cout << toWrite << endl;
}

void print(float toWrite){
   cout << toWrite << endl;
}

void print(char toWrite){
   cout << toWrite << endl;
}

void clear(){
   system("CLS");
}

void SetTextColor(int colorCode){
   /*
   Important Colors:
      - Red BG + white text: 79
      - Yellow BG + white text: 111
      - Black BG + white text = 15
   */
   HANDLE hConsole;
   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

   SetConsoleTextAttribute(hConsole, colorCode);
}
#endif

#if defined HangmanArt //Awful hardcoded text art
const string lives6 = "\t    _____\n\t    |   |\n\t    |\n\t    |\n\t    |\n\t    |\n\t____|____";
const string lives5 = "\t    _____\n\t    |   |\n\t    |   O\n\t    |\n\t    |\n\t    |\n\t____|____";
const string lives4 = "\t    _____\n\t    |   |\n\t    |   O\n\t    |   |\n\t    |\n\t    |\n\t____|____";
const string lives3 = "\t    _____\n\t    |   |\n\t    |   O\n\t    |  /|\n\t    |\n\t    |\n\t____|____";
const string lives2 = "\t    _____\n\t    |   |\n\t    |   O\n\t    |  /|\\\n\t    |\n\t    |\n\t____|____";
const string lives1 = "\t    _____\n\t    |   |\n\t    |   O\n\t    |  /|\\\n\t    |  /\n\t    |\n\t____|____";
const string lives0 = "\t    _____\n\t    |   |\n\t    |   O\n\t    |  /|\\\n\t    |  / \\\n\t    |\n\t____|____";
#endif

#if defined Hangman
string easyWords[64];
string medWords[64];
string hardWords[64];

string word;
string revealed;
string usedLetters;

int lives = 6;
bool won;

void LoadWords(){

   ifstream easyFile("easyWords.txt");
   ifstream medFile("mediumWords.txt");
   ifstream hardFile("hardWords.txt");
   for (int c = 0; c < 3; c++){
      string temp;
      int pos1 = 0;
      int pos2 = 0;
      int endPos;
      switch (c){
      case 0:
         getline(easyFile, temp);
         endPos = temp.find(':', 0);

         for (int x = 0; x < 64; x++){
            pos2 = temp.find("|", pos1);
            easyWords[x] = temp.substr(pos1, (pos2 - pos1));
            pos1 = pos2 + 1;
            if (pos1 >= endPos)
               break;
         }
         break;
      case 1:
         getline(medFile, temp);
         endPos = temp.find(':', 0);

         for (int x = 0; x < 64; x++){
            pos2 = temp.find("|", pos1);
            medWords[x] = temp.substr(pos1, (pos2 - pos1));
            pos1 = pos2 + 1;
            if (pos1 >= endPos)
               break;
         }
         break;
      case 2:
         getline(hardFile, temp);
         endPos = temp.find(':', 0);

         for (int x = 0; x < 64; x++){
            pos2 = temp.find("|", pos1);
            hardWords[x] = temp.substr(pos1, (pos2 - pos1));
            pos1 = pos2 + 1;
            if (pos1 >= endPos)
               break;
         }
         break;
      }
   }
}

void PickWord(int diff){
   srand(time(NULL));
   int random = rand() % 64;

   switch (diff){
   case 1:
      while (true){
         word = easyWords[random];
         if (!word.empty()){
            break;
         }
      }
      break;
   case 2:
      word = medWords[random];
      if (!word.empty()){
         break;
      }
      break;
   case 3:
      word = hardWords[random];
      if (!word.empty()){
         break;
      }
      break;
   }
}

void DrawHangman(){
   switch (lives){
   case 0:
      print(lives0);
      break;
   case 1:
      print(lives1);
      break;
   case 2:
      print(lives2);
      break;
   case 3:
      print(lives3);
      break;
   case 4:
      print(lives4);
      break;
   case 5:
      print(lives5);
      break;
   case 6:
      print(lives6);
      break;
   default:
      print(lives0);
      break;
   }
}

void PlayHangman(){
   lives = 6;
   usedLetters = "";
   bool lost = false;;
   LoadWords();

   while (true){
      int diff = 0;
      while (true){
         print("\n\tChoose difficulty:\n\t(1-easy) - (2-medium) - (3-hard)");
         cout << "\t";
         cin >> diff;
         if (diff > 0 && diff < 4){
            clear();
            break;
         }
         clear();
         print("\tPlease enter a valid difficulty!");
      }
      PickWord(diff);

      revealed = "";
      for (char character : word){
         revealed += "_ ";
      }

      while (true){
         clear();
         DrawHangman();

         if (lives <= 0){
            lost = true;
            break;
         }
         won = true;
         for (int i = 0; i < word.length(); i++){
            if (word[i] != revealed[2 * i]){
               won = false;
            }
         }
         if (won){
            break;
         }

         print("\tUsed letters: " + usedLetters);
         print("\t" + revealed);
         cout << endl << "\t";

         char input;
         cin.get(input);
         bool miss = true;
         if (input != '\n'){
            for (int i = 0; i < usedLetters.length() + 1; i++){
               cout << "A";
               if (usedLetters[i] == input){
                  miss = false;
                  break;
               }
            }
            if (miss){
               usedLetters += input;
            }

            for (int i = 0; i < word.length(); i++){
               if (word[i] == input){
                  revealed[2 * i] = input;
                  miss = false;
               }
            }
            if (miss)
               lives--;
         }
      }

      clear();
      DrawHangman();
      if (won){
         print("\tYou Won!");
      }
      else{
         print("\tYou Lost!");
      }
      print("\tThe word was: " + word + "\n");

      // print("\n\tPlay again?\n\ty - yes\ty - no");
      // char again;
      // scanf_s(" %c", &again);
      //
      // if (again != 'y' && again != 'Y'){
      //    break;
      // }
      break;
   }
   system("pause");
}
#endif

#if defined Snake

class Vector2Int{
public:
   int x;
   int y;

   Vector2Int(int newX, int newY){
      x = newX;
      y = newY;
   }

   Vector2Int(){

   }

   void Overwrite(int newX, int newY){
      x = newX;
      y = newY;

   }
};

class SnakeSegment{
public:
   Vector2Int position;
   Vector2Int prevPosition;
   bool head;
   SnakeSegment* next;

   SnakeSegment(Vector2Int pos, bool isHead, SnakeSegment* nex){
      position.x = pos.x;
      position.y = pos.y;
      head = isHead;
      next = nex;
   }

   void Advance(){
      prevPosition = position;
      position = next->position;
   }
};

Vector2Int foodPos(0, 0);
Vector2Int screenSize(100, 25);
float timeCount;

void NewFood(SnakeSegment* received){
   srand(time(NULL));

   int xPos = rand() % screenSize.x;
   int yPos = rand() % screenSize.y;
   bool overlap = false;
   
   do{
      SnakeSegment* traverseSnake = received;
      while (traverseSnake != NULL){
         if (xPos == traverseSnake->position.x && xPos == traverseSnake->position.y){
            overlap = true;
         }
         traverseSnake = traverseSnake->next;
      }
   }while(overlap);

   foodPos.Overwrite(xPos, yPos);
}


void PlaySnake(){
   timeCount = 0;
   Vector2Int snakeDir(1, 0);
   SnakeSegment head(Vector2Int(2, 3), true, NULL);
   SnakeSegment* lastSeg = &head;
   SnakeSegment body(Vector2Int(2, 2), false, lastSeg);
   lastSeg = &body;
   SnakeSegment body2(Vector2Int(2, 1), false, lastSeg);
   lastSeg = &body2;
   SnakeSegment body3(Vector2Int(2, 0), false, lastSeg);
   lastSeg = &body3;
   SnakeSegment* traverseSnake = lastSeg;
   
   NewFood(lastSeg);

   while (true){
      timeCount++;
      if (_kbhit()){
         char pressed = _getch();
         switch (pressed){
         case 'w':
            if (snakeDir.y != 1){
               snakeDir.y = -1;
               snakeDir.x = 0;
            }
            break;
         case 'a':
            if (snakeDir.x != 1){
               snakeDir.y = 0;
               snakeDir.x = -1;
            }
            break;
         case 's':
            if (snakeDir.y != -1){
               snakeDir.y = 1;
               snakeDir.x = 0;
            }
            break;
         case 'd':
            if (snakeDir.x != -1){
               snakeDir.y = 0;
               snakeDir.x = 1;
            }
            break;
         default:
            break;
         }
      }

      clear();
      string spaces = "|";
      
      for(int i = 0; i < screenSize.x; i++){
         spaces += "-";
      }
      spaces += "|";
      cout << spaces << endl;
      spaces = "";
      
      for (int j = 0; j < screenSize.y; ++j){
         cout << "|";
         for (int i = 0; i < screenSize.x; ++i){
            bool occupied = false;

            while (!traverseSnake->head){
               if (traverseSnake->position.x == i && traverseSnake->position.y == j){
                  occupied = true;
                  spaces += "o";
               }
               traverseSnake = traverseSnake->next;
            }

            if (head.position.x == i && head.position.y == j){
               occupied = true;
               spaces += "0";
            }
            if (head.position.x == foodPos.x && head.position.y == foodPos.y){
               timeCount = 0;
               SnakeSegment* newSnake = new SnakeSegment(Vector2Int(lastSeg->prevPosition.x, lastSeg->prevPosition.y),
                                                         false, lastSeg);
               lastSeg = newSnake;
               NewFood(lastSeg);
            }
            traverseSnake = lastSeg;

            if (foodPos.x == i && foodPos.y == j && !occupied){
               occupied = true;
               spaces += "#";
            }
            if (!occupied){
                  spaces += " ";
            }
         }
         cout << spaces;
         cout << "|\n";
         spaces = "";
      }
      
      spaces = "|";
      for(int i = 0; i < screenSize.x; i++){
         spaces += "-";
      }
      spaces += "|\n";
      cout << spaces;

      while (!traverseSnake->head){
         traverseSnake->Advance();
         traverseSnake = traverseSnake->next;
      }
      head.position.x += snakeDir.x;
      head.position.y += snakeDir.y;

      traverseSnake = lastSeg;
      bool died = false;
      while (!traverseSnake->head){
         if (
               head.position.x == traverseSnake->position.x && head.position.y == traverseSnake->position.y ||
               head.position.x < 0 || head.position.x > screenSize.x - 1 ||
               head.position.y < 0 || head.position.y > screenSize.y - 1
            ){
            died = true;
         }
         traverseSnake = traverseSnake->next;
      }

      if (died){
         break;
      }

      Sleep(50);
   }
   print("You died!\n");
   system("pause");
}
#endif

#if defined TicTacToe

char placement[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

void PrintRow(int row, int choosingX, int choosingY){
   if (choosingY != row || placement[choosingX][choosingY] != ' '){
      cout << "\t     |     |     " << endl;
      cout << "\t  " << placement[0][row] << "  |  " << placement[1][row] << "  |  " << placement[2][row] << endl;
      if (row != 2){
         cout << "\t_____|_____|_____" << endl;
      }
      else{
         cout << "\t     |     |     " << endl;
      }
   }
   else{
      switch (choosingX){
      case 0:
         cout << "\t ___ |     |     " << endl;
         cout << "\t|___||  " << placement[1][row] << "  |  " << placement[2][row] << endl;
         if (row != 2){
            cout << "\t_____|_____|_____" << endl;
         }
         else{
            cout << "\t     |     |     " << endl;
         }
         break;

      case 1:
         cout << "\t     | ___ |     " << endl;
         cout << "\t  " << placement[0][row] << "  ||___||  " << placement[2][row] << endl;
         if (row != 2){
            cout << "\t_____|_____|_____" << endl;
         }
         else{
            cout << "\t     |     |     " << endl;
         }
         break;

      case 2:
         cout << "\t     |     | ___ " << endl;
         cout << "\t  " << placement[0][row] << "  |  " << placement[1][row] << "  ||___|" << endl;
         if (row != 2){
            cout << "\t_____|_____|_____" << endl;
         }
         else{
            cout << "\t     |     |     " << endl;
         }
         break;
      }
   }
}

void PlayTicTacToe(){
   for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
         placement[i][j] = ' ';
      }
   }
   char currChar = 'O';
   int choosingX = 0;
   int choosingY = 0;
   while (true){
      clear();
      for (int j = 0; j < 3; ++j){
         PrintRow(j, choosingX, choosingY);
      }

      cout << "\n\n\tCurrently placing: \"" << currChar << "\"" << endl;

      int whileCount = 0;
      switch (_getch()){
      case 'w':
         do{
            choosingY--;
            if (choosingY < 0){
               choosingY = 2;
               choosingX--;
               if (choosingX < 0){
                  choosingX = 2;
               }
            }
         }
         while (placement[choosingX][choosingY] != ' ');
         break;
      case 'a':
         do{
            choosingX--;
            if (choosingX < 0){
               choosingX = 2;
               choosingY--;
               if (choosingY < 0){
                  choosingY = 2;
               }
            }
         }
         while (placement[choosingX][choosingY] != ' ');
         break;
      case 's':
         do{
            choosingY++;
            if (choosingY > 2){
               choosingY = 0;
               choosingX++;
               if (choosingX > 2){
                  choosingX = 0;
               }
            }
         }
         while (placement[choosingX][choosingY] != ' ');
         break;
      case 'd':
         do{
            choosingX++;
            if (choosingX > 2){
               choosingX = 0;
               choosingY++;
               if (choosingY > 2){
                  choosingY = 0;
               }
            }
         }
         while (placement[choosingX][choosingY] != ' ');
         break;

      case '\r':
         placement[choosingX][choosingY] = currChar;
         if (currChar == 'O'){
            currChar = 'X';
         }
         else if (currChar == 'X'){
            currChar = 'O';
         }
         do{
            choosingX++;
            if (choosingX > 2){
               choosingX = 0;
               choosingY++;
               if (choosingY > 2){
                  choosingY = 0;
               }
            }
            whileCount++;
         }
         while (placement[choosingX][choosingY] != ' ' && whileCount < 10);
         break;
      case 'c':
         return;
      }

      bool tie = true;
      for (int i = 0; i < 3; i++){
         for (int j = 0; j < 3; j++){
            if (placement[i][j] == ' '){
               tie = false;
            }
         }
      }

      bool won = false;
      char wonChar;
      if (!tie){
         if (placement[0][0] == placement[1][1] && placement[0][0] == placement[2][2] && placement[0][0] != ' '){
            won = true;
            wonChar = placement[0][0];
         }
         if (placement[0][2] == placement[1][1] && placement[0][2] == placement[2][0] && placement[0][2] != ' '){
            won = true;
            wonChar = placement[0][2];
         }
         if (!won){
            for (int i = 0; i < 3; i++){
               if (placement[i][0] == placement[i][1] && placement[i][0] == placement[i][2] && placement[i][0] != ' '){
                  won = true;
                  wonChar = placement[i][0];
                  break;
               }
               if (placement[0][i] == placement[1][i] && placement[0][i] == placement[2][i] && placement[0][i] != ' '){
                  won = true;
                  wonChar = placement[0][i];
                  break;
               }
            }
         }
      }


      if (tie){
         clear();
         for (int i = 0; i < 3; i++){
            PrintRow(i, 0, 0);
         }
         print("\n\tThe game ended in a tie!");
         system("pause");
         return;
      }
      if (won){
         clear();
         for (int i = 0; i < 3; i++){
            PrintRow(i, 0, 0);
         }
         cout << "\n\tPlayer \"" << wonChar << "\" won!" << endl;
         system("pause");
         return;
      }
      Sleep(50);
   }
}

#endif

#if defined Dinosaur

const string obstacles[2] = {"/\\", "/\\"};
string map = "__________________________";
const int jumpFrames = 10;
const string guyString = "ROPO";

void PlayDinosaur(){
   char guy = 'O';
   int guyIndex = 0;
   bool lost;
   int points = 0;
   int firstIndex = 0;
   int jumped = 0;
   int frameCount = 0;
   srand(time(NULL));

   while (true){
      frameCount++;
      if (frameCount > 1){
         frameCount = 0;
         guyIndex++;
         if (guyIndex >= guyString.length()){
            guyIndex = 0;
         }
         guy = guyString[guyIndex];
      }

      clear();
      lost = false;
      cout << "\n\t\tScore: " << points << "\n\n";
      if (jumped > 0){
         cout << "\t  " << guy;
      }
      cout << endl << "\t";

      for (int i = 0; i < 1000; i++){
         map += '_';
         if (rand() % 10 >= 9){
            map += obstacles[0];
         }
      }

      int tempCount = 0;
      for (int i = firstIndex; i < firstIndex + 25; i++, tempCount++){
         if (tempCount == 2 && jumped < 0){
            cout << guy;
            if (map[i] != '_'){
               lost = true;
            }
         }
         else{
            cout << map[i];
         }
      }
      if (lost){
         break;
      }

      firstIndex++;
      if (firstIndex >= 1000){
         break;
      }

      print("");
      if (_kbhit()){
         char hit = _getch();
         switch (hit){
         case 'w':
            if (jumped <= -1){
               jumped = jumpFrames + 1;
            }
            break;
         case 's':
            if (jumped > 0){
               jumped = 0;
            }
            break;
         default:
            break;
         }
      }
      else{
      }

      jumped--;

      points += 10;
      Sleep(50);
   }
   cout << endl;
   system("pause");
   clear();
   cout << "\n\tYou lost!\n\tFinal score: " << points << endl;
   system("pause");
}

#endif

#if defined Connect4

class Connect4Cell{
public:
   int color;
   bool occupied;

   Connect4Cell(){
      color = 15;
      occupied = false;
   }

   void Assign(int newCol){
      color = newCol;
      occupied = true;
   }
};

vector<vector<Connect4Cell>> board;
int colSelection = 0;
int currentColor = 111;

void DrawChosenCol(){
   int tabCount = 0;
   while (tabCount < colSelection){
      SetTextColor(15);
      cout << "    ";
      tabCount++;
   }
   tabCount = 0;
   cout << " ___" << endl;
   while (tabCount < colSelection){
      SetTextColor(15);
      cout << "    ";
      tabCount++;
   }
   tabCount = 0;
   cout << "|";
   SetTextColor(currentColor);
   cout << "   ";
   SetTextColor(15);
   cout << "|" << endl;
   while (tabCount < colSelection){
      cout << "    ";
      tabCount++;
   }
   cout << "|";
   SetTextColor(currentColor);
   cout << "___";
   SetTextColor(15);
   cout << "|" << endl;
}

void DrawConnect4Board(){
   for (int i = 0; i < 7; i++){
      cout << " ___";
   }
   cout << endl;
   for (int j = board.size() - 1; j > -1; j--){
      for (int i = 0; i < board[j].size(); i++){
         SetTextColor(15);
         cout << "|";
         SetTextColor(board[j][i].color);
         cout << "   ";
      }
      SetTextColor(15);
      cout << "|" << endl;
      for (int i = 0; i < 7; i++){
         SetTextColor(15);
         cout << "|";
         SetTextColor(board[j][i].color);
         cout << "___";
      }
      SetTextColor(15);
      cout << "|" << endl;
   }
}

int Connect4Win(){
   int lastColor = 15;
   int counter = 0;

   //Horizontal check
   for (int j = 0; j < board.size(); j++){
      for (int i = 0; i < board[j].size(); i++){
         if(board[j][i].occupied){
            if(board[j][i].color == lastColor){
               counter++;
            }else{
               lastColor = board[j][i].color;
               counter = 1;
            }

            if(counter == 4){
               return lastColor;
            }
         }else{
            lastColor = 15;
            counter = 0;
         }
      }
      counter = 0;
      lastColor = 15;
   }
   counter = 0;
   lastColor = 15;

   //Vertical check
   for(int i = 0; i < board[0].size(); i++){
      for(int j = 0; j < board.size(); j++){
         if(board[j][i].occupied){
            if(board[j][i].color == lastColor){
               counter++;
            }else{
               lastColor = board[j][i].color;
               counter = 1;
            }

            if(counter == 4){
               return lastColor;
            }
         }else{
            lastColor = 15;
            counter = 0;
         }
      }
      counter = 0;
      lastColor = 15;
   }
   counter = 0;
   lastColor = 15;

   //Diagonal check (Top -> Down)
   for(int j = 0; j < board.size(); j++){
      int checkingJ = j;
      for(int i = 0; i < board[j].size(); i++){
         int checkingI = i;
         if(checkingJ >= board.size()){
            break;
         }
         if(board[checkingJ][checkingI].occupied){
            if(board[checkingI][checkingJ].color == lastColor){
               counter++;
            }else{
               lastColor = board[checkingJ][checkingI].color;
               counter = 1;
            }

            if(counter == 4){
               return lastColor;
            }
         }else{
            lastColor = 15;
            counter = 0;
         }
         checkingJ++;
      }
      counter = 0;
      lastColor = 15;
   }

   //Diagonal check (Bottom -> Up)
   for(int j = board.size() - 1; j >= 0; j--){
      int checkingJ = j;
      for(int i = 0; i < board[j].size(); i++){
         int checkingI = i;
         if(checkingJ < 0){
            break;
         }
         if(board[checkingJ][checkingI].occupied){
            if(board[checkingI][checkingJ].color == lastColor){
               counter++;
            }else{
               lastColor = board[checkingJ][checkingI].color;
               counter = 1;
            }

            if(counter == 4){
               return lastColor;
            }
         }else{
            lastColor = 15;
            counter = 0;
         }
         checkingJ--;
      }
      counter = 0;
      lastColor = 15;
   }
   
   return -1;
}

int PlaceConnect4Cell(){
   int placeRow = 0;

   for (int j = 0; j < board.size(); j++){
      if (board[j][colSelection].occupied){
         placeRow = j + 1;
      }
      else{
         break;
      }
   }
   if (placeRow >= board.size()){
      return -1;
   }
   board[placeRow][colSelection].Assign(currentColor);
   switch (currentColor){
   case 111:
      currentColor = 79;
      break;
   case 79:
      currentColor = 111;
      break;
   }

   return Connect4Win();
}

void PlayConnect4(){
   board.clear();
   int finalResult = 0;
   colSelection = 0;
   currentColor = 111;

   for (int j = 0; j < 6; j++){
      vector<Connect4Cell> test;
      board.push_back(test);
      for (int i = 0; i < 7; i++){
         Connect4Cell temp;
         board[j].push_back(temp);
      }
   }

   // for(int c = 0; c < 255; c++){
   //    SetTextColor(c);
   //    cout << "Color:    " << c << endl;
   // }

   while (true){
      finalResult = -1;

      cout << endl << endl;
      DrawChosenCol();
      DrawConnect4Board();

      switch (_getch()){
      case 'd':
         colSelection++;
         if (colSelection >= board[0].size()){
            colSelection = board[0].size() - 1;
         }
         break;
      case 'a':
         colSelection--;
         if (colSelection < 0){
            colSelection = 0;
         }
         break;
      case 's':
         finalResult = PlaceConnect4Cell();
         break;
      case '\r':
         finalResult = 0;
         break;
      default:
         break;
      }

      clear();
      if (finalResult != -1){
         break;
      }
   }

   cout << endl << endl << endl << endl << endl;
   DrawConnect4Board();
   SetTextColor(15);
   switch(finalResult){
   case 111:
      cout << "\tYellow won!\n";
      break;
   case 79:
      cout << "\tRed won!\n";
      break;
   default:
      cout << "\tGame forfeited!\n";
   }
   system("pause");
   clear();
}

#endif

int main(int argc, char* argv[]){
   while (true){
      int choice;
      print(
         "\tChoose what game to play\n\t 1 - Snake;\n\t 2 - Hangman;\n\t 3 - Tic Tac Toe;\n\t 4 - Dinosaur game;\n\t 5 - Connect 4");
      cin >> choice;
      switch (choice){
      case 1:
         clear();
         PlaySnake();
         break;
      case 2:
         clear();
         PlayHangman();
         break;
      case 3:
         clear();
         PlayTicTacToe();
         break;
      case 4:
         clear();
         PlayDinosaur();
         break;
      case 5:
         clear();
         PlayConnect4();
         break;
      }

      clear();
      while (true){
         char again;
         print("Do you want to play again?\n(Y - yes; N - no)");
         cin >> again;
         if (again == 'Y' || again == 'y'){
            clear();
            break;
         }
         if (again == 'N' || again == 'n'){
            return 0;
         }
         clear();
         print("Please insert a valid character!\n");
      }
   }
}
