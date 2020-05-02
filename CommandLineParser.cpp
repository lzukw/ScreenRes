// SPDX-License-Identifier: MIT
// Copyright (c) 2020 Wolfgang Zukrigl 


#include "CommandLineParser.h"

#include <iostream>
#include <exception>
#include <string>

using namespace std;

CommandLineParser::CommandLineParser(int argc, char* argv[])
{
  bool modeSet = false;
  m_x = 0;
  m_y = 0;
  m_noVersionInfo = false;

  // copy command-line-arguments in vector
  for (int i = 0; i < argc; i++)
  {
    m_argv.push_back(argv[i]);
  }

  // Strip path from program-name (argv[0])
  changeCommandName();

  // Process all Arguments
  for (size_t i = 1; i < m_argv.size(); i++)
  {
    size_t len = strlen(m_argv[i]);
    if (len < 2 || (m_argv[i][0] != '/' && m_argv[i][0] != '-') )
    {
      printHelpAndExit(EXIT_FAILURE);
    }
   
    switch (m_argv[i][1])
    {
    case 'X':
    case 'x':
      m_x = parseXorY(argv[i]);
      m_mode = CommandLineParser::Mode::SET_DISPLAY_MODE;
      modeSet = true;
      break;

    case 'Y':
    case 'y':
      m_y = parseXorY(argv[i]);
      m_mode = CommandLineParser::Mode::SET_DISPLAY_MODE;
      modeSet = true;
      break;

    case 'S':
    case 's':
      m_mode = CommandLineParser::Mode::SHOW_CURRENT_DISPLAY_SETTING;
      modeSet = true;
      break;

    case 'L':
    case 'l':
      m_mode = CommandLineParser::Mode::LIST_ALL_DISPLAY_MODES;
      modeSet = true;
      break;

    case 'V':
    case 'v':
      m_noVersionInfo = true;
      break;

    case '?':
    case 'h':
    case 'H':
      printHelpAndExit(EXIT_SUCCESS);
      break;

    default:
      printHelpAndExit(EXIT_FAILURE);
      break;
    }
   
  }

  if ( ! modeSet || m_mode == Mode::SET_DISPLAY_MODE && (m_x == 0 || m_y == 0) )
  {
    printHelpAndExit(EXIT_FAILURE);
  }

  printVersionInfoIfDesired();
}


int CommandLineParser::getX()
{
  return m_x;
}


int CommandLineParser::getY()
{
  return m_y;
}


CommandLineParser::Mode CommandLineParser::getMode()
{
  return m_mode;
}


void CommandLineParser::printHelpAndExit(int exitStatus)
{
  printVersionInfoIfDesired();

  cout << "Usage: " << endl;
  cout << m_argv[0] << "  { /X:px /Y:px | /S  | /L | /? | /H } [/V]" << endl;
  cout << endl;
  cout << "  /X:nnn      Width in pixels." << endl;
  cout << "  /Y:nnn      Height in pixels." << endl;
  cout << "  /S          Show current display settings." << endl;
  cout << "  /L          List all display modes." << endl;
  cout << "  /V          Does NOT display version information." << endl;
  cout << "  /H or /?    Displays usage information." << endl;
  cout << endl;
  cout << "Ex: '" << m_argv[0] << "  /x:640 /y:480' Changes resolution to 640 x 480" << endl << endl;
  exit(exitStatus);
}


////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////

/**
 * argv[0] is the full path to the executed command (exe-File). Strip the path
 * and just leave commandname.exe
 */
void CommandLineParser::changeCommandName()
{
  int lastOccurence = -1;
  int pos = 0;
  while (m_argv[0][pos] != '\0')
  {
    if (m_argv[0][pos] == '/' || m_argv[0][pos] == '\\')
      lastOccurence = pos;
    pos++;
  }
  m_argv[0] = &(m_argv[0][lastOccurence+1]);
}


/**
  * arg is one of the command-line-args /X:nnn or /Y:nnn. The number nnn is parsed and returned
  */
int CommandLineParser::parseXorY(char* arg)
{
  if (strlen(arg) < 4)
  {
    printHelpAndExit(EXIT_FAILURE);
  }

  if (arg[2] != ':' && arg[2] != '=')
  {
    printHelpAndExit(EXIT_FAILURE);
  }

  int val = 0;
  try
  {
    string valAsString(&(arg[3]));
    val = stoi(valAsString);
  }
  catch (...)
  {
    printHelpAndExit(EXIT_FAILURE);
  }

  return val;
}


void CommandLineParser::printVersionInfoIfDesired()
{
  if (!m_noVersionInfo)
  {
    cout << m_argv[0] << endl;
    cout << "Version 1.0" << endl;
    cout << endl;
  }
}
