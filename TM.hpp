#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>


class TM {
public:
  TM(std::string, std::string);
  ~TM();
  void runTM();

private:
  bool readTM();
  void runTapeFile();

  bool readFirstLine(std::string);
  void readInputAlphabet(std::string);
  void readTMStates(std::string);
  void readStartRejectState(std::string);
  void readTapeAlphabet(std::string);
  void readTransition(std::string, int);
  bool readTape(std::ifstream&);
  void moveLeftOnTape();
  void moveRightOnTape();
  void writeOnTape(char);
  bool inInputAlphabet(char);
  bool inTMStates(std::string);
  bool inTapeAlphabet(char);
  void runSimulation();




  std::string MachineFileName;
  std::string TapeFileName;

  std::string NameOfTM;
  unsigned int NumberOfTapes;
  unsigned int MaximumTapeLength;
  unsigned int MaximumNumberOfSteps;

  std::vector<char> InputAlphabet;
  std::vector<std::string> TMStates;
  std::string StartState;
  std::string AcceptingState;
  std::string RejectState;
  std::vector<char> TapeAlphabet;
  std::map<std::pair<std::string,char>,std::pair<std::string,int>> TransitionFunction;
  std::vector<std::string> Rules;

  std::string Tape;
  unsigned int TapeIndex;
  std::string CurrentState;
  unsigned int SimulationSteps;
  unsigned int TapeNumber;



};
