#include <string>
#include <map>
#include <utility>
#include <vector>
#include <fstream>


class TM {
public:
  TM(std::string, std::string);
  ~TM();
  void readTM();

private:
  void readFirstLine(std::string);
  void readTapeAlphabet(std::string);
  void readTMStates(std::string);



  std::string MachineFileName;
  std::string TapeFileName;

  std::string NameOfTM;
  int NumberOfTapes;
  int MaximumTapeLength;
  int MaximumNumberOfSteps;

  std::vector<char> TapeAlphabet;
  std::vector<std::string> TMStates;



};
