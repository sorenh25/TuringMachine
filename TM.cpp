#include "TM.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>


using namespace std;

TM::TM(string Machine, string Tape) {
  MachineFileName = Machine;
  TapeFileName = Tape;
}

TM::~TM() {

}

void TM::readTM() {
  ifstream TMFile;
  TMFile.open(MachineFileName.c_str());
  if (!TMFile.is_open()) {
    cerr << "Turing Machine File could not be opened" << endl;
  }
  string line;
  getline(TMFile, line);
  readFirstLine(line);

  getline(TMFile, line);
  readTapeAlphabet(line);

  getline(TMFile, line);
  readTMStates(line);




}

void TM::readFirstLine(string line) {



  size_t pos1 = 0;
  size_t pos2 = line.find_first_of(',',pos1);
  NameOfTM = line.substr(pos1, pos2);
  pos1 = pos2 + 1;

  pos2 = line.find_first_of(',',pos1);
  NumberOfTapes = atoi(line.substr(pos1,pos2 - pos1).c_str());
  pos1 = pos2 + 1;

  pos2 = line.find_first_of(',',pos1);
  MaximumTapeLength = atoi(line.substr(pos1,pos2 - pos1).c_str());
  pos1 = pos2 + 1;

  pos2 = line.find_first_of(',',pos1);
  MaximumNumberOfSteps = atoi(line.substr(pos1,pos2 - pos1).c_str());


  //cout << NameOfTM << ' ' << NumberOfTapes << ' ' << MaximumTapeLength << ' ' << MaximumNumberOfSteps << endl;
}

void TM::readTapeAlphabet(string line) {
  if (line[line.length() - 1] == '\r') {
    line.pop_back();
  }
  for (unsigned int i = 0; i < line.length(); i++) {
    if (line[i] == ',') {
      continue;
    }
    else {
      TapeAlphabet.push_back(line[i]);
    }
  }

  /*
  for (unsigned int i = 0; i < TapeAlphabet.size(); i++) {
    cout << TapeAlphabet[i] << ',';
  }
  cout << endl;

  */
}

void TM::readTMStates(string line) {
  if (line[line.length() - 1] == '\r') {
    line.pop_back();
  }
  size_t pos1 = 0;
  size_t pos2 = line.find_first_of(',', pos1);
  while(pos2 != string::npos) {
    TMStates.push_back(line.substr(pos1,pos2 - pos1));
    pos1 = pos2 + 1;
    pos2 = line.find_first_of(',',pos1);
  }
  line = line.substr(pos1, line.length() - pos1);
  //line.pop_back();
  TMStates.push_back(line);

  for (unsigned int i = 0; i < TMStates.size(); i++) {
    cout << TMStates[i] << ',';
  }
  cout << endl;
}
