#include "TM.hpp"

int main(int argc, char** argv) {
  TM myTM(argv[1], argv[2]);
  myTM.readTM();
  myTM.runTapeFile();

}
