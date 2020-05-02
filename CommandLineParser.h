// SPDX-License-Identifier: MIT
// Copyright (c) 2020 Wolfgang Zukrigl 


#pragma once

#include <vector>
#include <cstdlib>
#include <exception>

/**
 * There are five use-cases, when calling this scree-resolution-setting-program:
 * screenres.exe /X:nnn /Y:mmm   ...set the screen-resolution to nnn x mmm
 * screenres.exe /S              ...display the current scree-resolution
 * screenres.exe /L              ...list all possible screen-resolutions
 * screenres.exe /H or /?        ...print usage-information
 * An additional /V command-line-argument is allowed in each case. If this 
 * argument is present, NO version-information is printed at the beginning.
 */
class CommandLineParser
{
public:

  /**
   * One of these enum-values is returned by getMode() to inidicate the
   * action desired by the user.
   */
  enum Mode 
  {
    SHOW_CURRENT_DISPLAY_SETTING,
    LIST_ALL_DISPLAY_MODES,
    SET_DISPLAY_MODE
  };

  /**
    * Constructor. argc and argv passed to main() have to be passed over to this Constructor.
    * If the command-line-arguments gaven by the user are not correct, a usage-information is
    * printed to stdout and the program exits with error-code EXIT_FAILURE.
    */
  CommandLineParser(int argc, char* argv[]);

  /** 
    * Returns the value, which the user gave with the command-line-parameter /X:nnn
    */
  virtual int getX();

  /**
   * Returns the value, which the user gave with the command-line-parameter /Y:nnn
   */
  virtual int getY();

  /**
   *  Returns the action to take (gaven by the user with command-line-arguments:
   *  Mode::SHOW_CURRENT_DISPLAY_SETTING is returned, if the user 
   */
  virtual CommandLineParser::Mode getMode();

  /**
   * Prints usage-information and exits the program with the error-code given by
   * exitStatus (this value should be either EXIT_SUCCESS, which is the default-value,
   * or EXIT_FAILURE).
   */
  virtual void printHelpAndExit(int exitStatus = EXIT_SUCCESS);


protected:

  // private copy ot the command-line-arguments
  std::vector<char*> m_argv;

  // The value of the command-line-argument /X:nnn is stored here
  int m_x;

  // The value of the command-line-argument /Ý:nnn is stored here
  int m_y;

  // true, if the command-line-argument /V is predent
  bool m_noVersionInfo;

  // See enum Mode 
  Mode m_mode;

  // Helper-function to strip the path of argv[0], which ist the full path to the
  // executed program (.exe-file)
  void changeCommandName();

  // returns the parsed integer value (nnn) of the command-line-arguments /X:nnn and /Y:nnn
  int parseXorY(char* arg);

  // If the command-line-argument /V is not present, then first some lines of version-
  // information is printed to stdout.
  void printVersionInfoIfDesired();
};

