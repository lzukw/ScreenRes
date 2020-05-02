// SPDX-License-Identifier: MIT
// Copyright (c) 2020 Wolfgang Zukrigl 


#pragma once

#include <vector>
#include <string>
#include <exception>

#include <Windows.h>

/**
 * Principal class for retrieving information about available screen-resolutions,
 * the current screen-resolution and for changing the screen-resolution.
 */
class ResolutionManager
{

public:

  /**
   * Constructor
   */
  ResolutionManager();

  /**
   * Destructor
   */
  ~ResolutionManager();

  /**
   * Returns a string containing the initial resolution, which is the resolution,
   * that was set, when the this Resolution-Manager was instantiated.
   * If shortForm is true, the returned string is for example "640x480", otherwise
   * it is for example "640x480, 32 bits @ 64 Hz"
   */
  virtual std::string getInitialResolution(bool shortForm=false);

  /**
   * Returns a string containing the current screen-resolution.
   * If shortForm is true, the returned string is for example "640x480", otherwise
   * it is for example "640x480, 32 bits @ 64 Hz"
   */
  virtual std::string getCurrentResolution(bool shortForm=false);

  /**
   * Returns a vector containg string with all resolutions. 
   * If shortForm is true, the returned strings are for example "640x480", otherwise
   * they are for example "640x480, 32 bits @ 64 Hz"
   */
  virtual std::vector<std::string> getAllResolutions(bool shortForm=false);

  /**
   * Sets the screen-resolution to the give size. The argument passed
   * to this method is a string like "640x480". Color-depth and refresh-rate
   * get their highest possible values.
   */
  virtual bool setResolution(std::string resolutionToSet);


protected:

  DEVMODEA m_initialResolution;
  DWORD m_initialResolutionModeNum;

  DEVMODEA m_currentResolution;
  DWORD m_currentResolutionModeNum;

  std::vector<DEVMODEA*> m_resolutions;

  DEVMODEA* m_findResolution(std::string res);

  static bool m_areResolutionsEqual(DEVMODEA* resolutionA, DEVMODEA* resolutionB);
  static bool m_haveSameWidthAndHeight(DEVMODEA* resolutionA, DEVMODEA* resolutionB);
  static std::string m_resolutionToString(DEVMODEA* resolution, bool shortForm=false);
  
};

