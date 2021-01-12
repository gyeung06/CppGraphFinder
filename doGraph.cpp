// Gladys Monagan, Graph Assignment #8
// CPSC 2150
// March 26, 2019
// Use for the assignment but do not post anywhere
#include <fstream>
#include <string>
#include <iostream>
#include "Graph.h"

using std::cout;
using std::cin;
using std::ifstream;
using std::istream;
using std::string;

// the commands allowed
enum Command 
{
   INPUT,
   OUTPUT,
   CONNECTED,
   FIND_CYCLE,
   PRINT_COMPONENTS,
   TWIN,
   DOCUMENT,
   QUIT,
   INVALID
};

// output the commands available
void printCommandMenu() 
{
   cout << "\n ------------------ command list ----------------------\n";
   cout << "(i) - input the file name that contains the graph \n";
   cout << "(o) - output the graph\n";
   cout << "(c) - determines whether the graph is connected or not\n";
   cout << "(f) - find (out whether the graph has) at least one cycle\n";
   cout << "(p) - prints the connected components (one set of vertices per line)\n";
   cout << "(t) - twin the graph by making a copy\n";
   cout << "(d) - document, make a comment, the subsequent line is ignored\n";
   cout << "(q) - quit the program altogether\n";
   cout << " --------------------------------------------------------\n"; 
}

// read from standard input a command and obtain the first letter of that 
// command to match it to a corresponding value from the enum type Command
// if notInteractive, the command read is echoed to standard output
// postcondition:
//    returns a command from Command (including INVALID)
Command getCommand(bool notInteractive)
{
   Command command;
   cout << "\ncommand: ";
   string commandStr;
   cin >> commandStr;
   if (notInteractive) cout << commandStr << "\n";  // echo what was read

   char cmd = commandStr[0];
   if ('A' <= cmd && cmd <= 'Z') { // if upper case,
      cmd = (cmd - 'A') + 'a'; // translate to lower case
   }
   switch (cmd) {
      case 'i':
         command = INPUT;
         break;
      case 'o':
         command = OUTPUT;
         break;
      case 'c':
         command = CONNECTED;
         break;
      case 'f':
         command = FIND_CYCLE;
         break;
      case 'p':
         command = PRINT_COMPONENTS;
         break;
      case 't':
         command = TWIN;
         break;
      case 'd':
         command = DOCUMENT;
         break;
      case 'q':
         command = QUIT;
         break;
      default:
         command = INVALID;
         break;
   } 
   string ignoreEndOfLine;
   std::getline(cin, ignoreEndOfLine); // consume the-end-of-line
   return command;
}

// reset the input stream cin (out of its fail state) 
// and clear the buffer
// postcondition:
//    cin is ready to read again
void cinReset()
{
   static const int MAX_CHARACTERS_IN_BUFFER = 250;
   cin.clear();
   cin.ignore(MAX_CHARACTERS_IN_BUFFER, '\n');
}

// read from standard input a line 
// and output it to standard output if notInteractive
void getNextLine(bool notInteractive)
{
   string line;
   std::getline(cin, line); 
   if (notInteractive) cout << line << "\n";
}

// read from standard input the name of the file that has the undirected simple 
// graph if the file can be opened, read the graph from that file
// precondition:
//    the graph file has an integer n for the number of vertices in the graph
//    followed by edges as pairs (i, j) such that 0 <= i < n and 0 <= i < n   
// postcondition: 
//    the graph with the data if reading was successful
//    the input stream cin is cleared in case of having read non-numeric input
//    return true if the reading was successful, false otherwise
bool inputGraph(Graph& graph, bool notInteractive)
{
   bool success = true;
   string fileName;
   cout << "enter the file name that has the graph: ";
   cin >> fileName;
   if (notInteractive) cout << fileName << "\n";
   ifstream fin;
   fin.open(fileName);
   if (!cin.good() || (!fin.good()) ) {
      cout << "ERROR in reading/opening the graph file \"" << fileName << "\"\n";
      cinReset();
      success = false;
   }
   else {
      fin >> graph;
      fin.close();
   }
   return success;
}

// read from standard input a single word and check if the first
// letter corresponds to 'y' or 'Y' (implying a yes)
// return true if the string read from cin starts with 'y' or 'Y', false otherwise
// if notInteractive, the answer read is echoed onto standard output
bool yesAnswer(bool notInteractive = false) 
{
   string answerStr;
   cin >> answerStr;
   if (notInteractive) cout << answerStr << "\n";
   return (answerStr[0] == 'Y' || answerStr[0] == 'y');
}

// process a command one at a time
// return false if QUIT is called
bool processCommand(Command command, bool notInteractive, Graph& graph);

// make a copy of graph using the copy constructor
// test that some properties are the same and call a few commands on it
// the destructor is implicitly called once other goes out of scope
void makeTwinNCommand(Graph& graph, bool notInteractive) {
   Graph other(graph);
   bool done = (other.hasCycle() != graph.hasCycle()) ||
               (other.isConnected() != graph.isConnected());
   if (done) {
      cout << "ERROR: twin is not identical to graph\n";
   }
   else {
      while (!done) 
      {
         cout << "\nfor twin graph ";
         Command cmd = getCommand(notInteractive);
         done = processCommand(cmd, notInteractive, other);
      }
   }
}

bool processCommand(Command command, bool notInteractive, Graph& graph)
{   
   bool doneProcessing = false;
   Graph other;
   switch (command)
   {
      case INPUT:
         if (!inputGraph(graph, notInteractive)) cout << "input the file again!\n";
         break;
      case OUTPUT:
         cout << graph;
         break;
      case CONNECTED:
         cout << "the graph is ";
         if (!graph.isConnected()) cout << "NOT ";
         cout << "connected\n";
         break;
      case FIND_CYCLE:
         if (graph.hasCycle()) 
            cout << "the graph has at least one cycle\n";
         else
            cout << "no cycles\n";
         break;
      case PRINT_COMPONENTS:
         graph.printComponents(cout);
         break;
      case TWIN:
         makeTwinNCommand(graph, notInteractive);
         break;
      case DOCUMENT:
         getNextLine(notInteractive);
         break;
      case QUIT:
         // the task is done
         doneProcessing = true;
         break;
      case INVALID:
         cout << "invalid command, please input again!\n";
         break;
      default:
         std::cerr << "**** ERROR in the switch of processCommand ***\n";
         break;
   }
   return doneProcessing;
}

// determine if the "-batch" option was entered meaning that the
// session is NOT interactive
// return true if the string matches -batch, false otherwise
bool notInteractiveOption(string option) {
   return option == "-batch";
} 

// if the session is interactive, echo the input
// process the commands until QUIT
int main(int argc, char* argv[]) {
   bool notInteractive = false;
   if (argc > 1) notInteractive = notInteractiveOption(argv[1]);

   Command cmd;
   do 
   {
      if (!notInteractive) printCommandMenu();
      bool done = false;
      Graph graph; 
      do 
      {
         cmd = getCommand(notInteractive);
         done = processCommand(cmd, notInteractive, graph);
      } while (!done); 
   } while (cmd != QUIT); 
   return 0;
}

