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

void TM::runTM() {
  if (readTM()) {
    runTapeFile();
  }
}

bool TM::readTM() {
  ifstream TMFile;
  TMFile.open(MachineFileName.c_str());
  if (!TMFile.is_open()) {
    cerr << "Turing Machine File could not be opened" << endl;
  }
  string line;
  getline(TMFile, line);
  if (!readFirstLine(line)) {
    return false;
  }

  getline(TMFile, line);
  readInputAlphabet(line);

  getline(TMFile, line);
  readTMStates(line);

  getline(TMFile, StartState);
  if (StartState[StartState.length() - 1] == '\r') {
    StartState.pop_back();
  }

  getline(TMFile, line);
  readStartRejectState(line);

  getline(TMFile, line);
  readTapeAlphabet(line);

  int ruleNumber = 0;
  while(!TMFile.eof()) {
    getline(TMFile, line);
    readTransition(line, ruleNumber);
    ruleNumber++;
  }

  TMFile.close();
  return true;
}

bool TM::readFirstLine(string line) {
  if (line[line.length() - 1] == '\r') {
    line.pop_back();
  }
  size_t pos1 = 0;
  size_t pos2 = line.find_first_of(',',pos1);
  NameOfTM = line.substr(pos1, pos2);
  pos1 = pos2 + 1;

  pos2 = line.find_first_of(',',pos1);
  NumberOfTapes = atoi(line.substr(pos1,pos2 - pos1).c_str());
  pos1 = pos2 + 1;

  if (NumberOfTapes != 1) {
    cout << "This Turing Machine will only run with one tape" << endl;
    return false;
  }

  pos2 = line.find_first_of(',',pos1);
  MaximumTapeLength = atoi(line.substr(pos1,pos2 - pos1).c_str());
  pos1 = pos2 + 1;

  pos2 = line.find_first_of(',',pos1);
  MaximumNumberOfSteps = atoi(line.substr(pos1,pos2 - pos1).c_str());

  return true;
}

void TM::readInputAlphabet(string line) {
  if (line[line.length() - 1] == '\r') {
    line.pop_back();
  }
  for (unsigned int i = 0; i < line.length(); i++) {
    if (line[i] == ',') {
      continue;
    }
    else {
      InputAlphabet.push_back(line[i]);
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

  /*
  for (unsigned int i = 0; i < TMStates.size(); i++) {
    cout << TMStates[i] << ',';
  }
  cout << endl;
  */
}

void TM::readStartRejectState(string line) {
  if (line[line.length() - 1] == '\r') {
    line.pop_back();
  }
  size_t pos1 = 0;
  size_t pos2 = line.find_first_of(',',pos1);
  AcceptingState = line.substr(pos1, pos2);
  pos1 = pos2 + 1;

  pos2 = line.find_first_of(',',pos1);
  RejectState = line.substr(pos1,pos2 - pos1);

  //cout << AcceptingState << ',' << RejectState << endl;
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

  if (!inTapeAlphabet('_')) {
    TapeAlphabet.push_back('_');
  }

 /*
  for (unsigned int i = 0; i < TapeAlphabet.size(); i++) {
    cout << TapeAlphabet[i] << ',';
  }
  cout << endl;
  */


}

void TM::readTransition(string line, int ruleNumber) {
  if (line.empty()) {
    cout << "Read in an empty line, something is wrong" << endl;
    return;
  }
  if (line[line.length() - 1] == '\r') {
    line.pop_back();
  }

  Rules.push_back(line);

  size_t pos1 = 0;
  size_t pos2 = line.find_first_of(',', pos1);
  string beginState = line.substr(pos1, pos2);
  if (!inTMStates(beginState)) {
    return;
  }

  char inputCharacter = line[pos2 + 1];
  if (!inTapeAlphabet(inputCharacter)) {
    return;
  }

  pos1 = pos2 + 3;
  pos2 = line.find_first_of(',', pos1);
  string destinationState = line.substr(pos1, pos2 - pos1);
  if (!inTMStates(destinationState)) {
    return;
  }

  pos1 = pos2 + 1;
  char newCharacter = line[pos1];
  if (!inTapeAlphabet(newCharacter)) {
    return;
  }

  pos1 = pos1 + 2;
  char direction = line[pos1];
  if ( !((direction == 'R') | (direction == 'L') | (direction == 'S')) ) {
    cout << direction << " is not R or S or L" << endl;
  }

  //cout << beginState << ' ' << inputCharacter << " - " << destinationState << ' ' << newCharacter << ' ' << direction << ' ' << ruleNumber << endl;
  destinationState.push_back(newCharacter);
  destinationState.push_back(direction);


  TransitionFunction[make_pair(beginState,inputCharacter)] = make_pair(destinationState, ruleNumber);
}

bool TM::readTape(std::ifstream& TapeFile) {
  if (TapeFile.eof()) {
    return false;
  }
  getline(TapeFile, Tape);
  if (Tape.empty()) {
    return false;
  }
  if (Tape[Tape.length() - 1] == '\r') {
    Tape.pop_back();
  }

  Tape.resize(MaximumTapeLength, '_');
  TapeIndex = 0;

  /* this check they want done when the tape is simulated
  for (unsigned int i = 0; i < Tape.size(); i++) {
    if (!inTapeAlphabet(Tape[i])) {
      return false;
    }
  }
  */

  return true;
}

void TM::moveLeftOnTape() {
  if (TapeIndex == 0) {
    return;
  }
  TapeIndex--;
}

void TM::moveRightOnTape() {
  if (TapeIndex >= MaximumTapeLength) {
    return;
  }
  TapeIndex++;
}

void TM::writeOnTape(char character) {
  Tape[TapeIndex] = character;
}

bool TM::inInputAlphabet(char input) {
  for (unsigned int i = 0; i < InputAlphabet.size(); i++) {
    if (input == InputAlphabet[i]) {
      return true;
    }
  }
  //cout << input << " not in Input Alphabet" << endl;
  return false;
}

bool TM::inTMStates(string state) {
  for (unsigned int i = 0; i < TMStates.size(); i++) {
    if (state == TMStates[i]) {
      return true;
    }
  }
  //cout << state << " not in TM States" << endl;
  return false;
}

bool TM::inTapeAlphabet(char character) {
  for (unsigned int i = 0; i < TapeAlphabet.size(); i++) {
    if (character == TapeAlphabet[i]) {
      return true;
    }
  }
  if (character != '_') {
    //cout << character << " not in Tape Alphabet" << endl;
  }
  return false;
}

void TM::runTapeFile() {
  ifstream TapeFile;
  TapeFile.open(TapeFileName.c_str());
  if (!TapeFile.is_open()) {
    cerr << "Tape File could not be opened" << endl;
  }
  TapeNumber = 0;
  while (readTape(TapeFile)) {
    runSimulation();
    TapeNumber++;
  }
}

void TM::runSimulation() {
  CurrentState = StartState;
  SimulationSteps = 0;
  pair<string, int> DestinationPair;
  char Direction, NewCharacter;
  string NewState;

  cout << endl << "Tape " << '1' << ": " << Tape << endl;
  cout << CurrentState << CurrentState.size() <<endl;

  for (; SimulationSteps <= MaximumNumberOfSteps; SimulationSteps++) {

    //Checks to see whether the input to be read from tape is in Tape Alphabet
    if (!inTapeAlphabet(Tape[TapeIndex])) {
      cout << "Error" << endl;
      cout << "Tape " << '1' << ": " << Tape << endl << endl;
      return;
    }

    //This block is functioning as the transition function for the current state and input or *
    try {
      //First try the transition function with the symbol on the tape.
      DestinationPair = TransitionFunction.at(make_pair(CurrentState, Tape[TapeIndex]));
    }
    catch (const out_of_range& oor) {
      try {//if that doesnt work there is maybe a rule with the * for the current state
        DestinationPair = TransitionFunction.at(make_pair(CurrentState,'*'));
      }
      catch (const out_of_range& oor) {//if there is no such rule, then there is no transition available for the current configuration
        cout << "Error" << endl;
        //cout << CurrentState << ',' << Tape[TapeIndex] << endl;
        cout << "Tape " << '1' << ": " << Tape << endl << endl;
        return;
      }
    }

    //Extracting the new state, new tape character and the direction from the transition function result
    NewState = DestinationPair.first;
    Direction = NewState.back();
    NewState.pop_back();
    NewCharacter = NewState.back();
    NewState.pop_back();

    cout << SimulationSteps << ',' << DestinationPair.second << ',' << TapeIndex << ',' << CurrentState << ',' << Tape[TapeIndex] << ',' << NewState << ',' << NewCharacter << ',' << Direction << endl;

    if (NewState == RejectState) {
      cout << "Reject" << endl;
      cout << "Tape " << '1' << ": " << Tape << endl << endl;;
      return;
    }

    if (NewState == AcceptingState) {
      cout << "Accept" << endl;
      cout << "Tape " << '1' << ": " << Tape << endl << endl;;
      return;
    }

    //Updates the configuration of the Turing Machine
    CurrentState = NewState;
    writeOnTape(NewCharacter);

    if (Direction == 'L') {
      moveLeftOnTape();
      continue;
    }
    if (Direction == 'R') {
      moveRightOnTape();
      continue;
    }
    if (Direction == 'S') {
      continue;
    }
    else {
      cout << "Not LRS" << endl;
      //I should already have checked for this
    }

  }

  cout << "Error (Exceeded max number of steps)" << endl;
  cout << "Tape " << '1' << ": " << Tape << endl << endl;
  return;

}
