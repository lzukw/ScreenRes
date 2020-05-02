// SPDX-License-Identifier: MIT
// Copyright (c) 2020 Wolfgang Zukrigl 


#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "ResolutionManager.h"
#include "CommandLineParser.h"

using namespace std;

int main(int argc, char* argv[])
{
  ResolutionManager rm;
  CommandLineParser clp(argc, argv);
 
  vector<string> resolutions;
  ostringstream resolutionToSet;

  switch (clp.getMode())
  {
  case CommandLineParser::Mode::SHOW_CURRENT_DISPLAY_SETTING:
    cout << rm.getCurrentResolution(false) << endl; 
    break;

  case CommandLineParser::Mode::LIST_ALL_DISPLAY_MODES:
    resolutions = rm.getAllResolutions(false);
    for (size_t i = 0; i < resolutions.size(); i++)
    {
      cout << resolutions[i] << endl;
    }
    break;

  case CommandLineParser::Mode::SET_DISPLAY_MODE:
    resolutionToSet << clp.getX() << "x" << clp.getY();
    rm.setResolution(resolutionToSet.str());
    break;

  default:
    //should never get here
    clp.printHelpAndExit(EXIT_FAILURE);
    break;
  }
}
