// SPDX-License-Identifier: MIT
// Copyright (c) 2020 Wolfgang Zukrigl 


#include "ResolutionManager.h"

#include <sstream>
#include <algorithm>
//#include <iostream>

using namespace std;

ResolutionManager::ResolutionManager()
{
  // Get current display resolution and its modeid and store it in _currentResolution and
  // in _initialResolution
  bool success = EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &m_currentResolution);
  if (!success)
  {
    throw new exception("Error when calling EnumDisplaySettingsA()");
  }

  m_initialResolution = m_currentResolution; // In C++ this should be a copy of the whole struct (copy by value)
  
  // Iterate over all available resolutions and store them in _resolutions
  DWORD iModeNum = 0;
  DEVMODEA resolution;
  DEVMODEA* ptrToNewResoltion;
  while (EnumDisplaySettingsA(NULL, iModeNum, &resolution))
  {
    //std::cout << iModeNum << ": " << resolution.dmPelsWidth << "x" << resolution.dmPelsHeight << " " << resolution.dmDisplayFrequency << " " << resolution.dmBitsPerPel << " "  << resolution.dmDisplayFlags << "\n";

    ptrToNewResoltion = new DEVMODEA;
    *ptrToNewResoltion = resolution; // In C++ this should be a copy of the whole struct (copy by value)
    m_resolutions.push_back(ptrToNewResoltion);

    iModeNum++;
  }
}

ResolutionManager::~ResolutionManager()
{
  for (size_t i = 0; i < m_resolutions.size(); i++)
  {
    delete m_resolutions[i];
  }
}

string ResolutionManager::getInitialResolution(bool shortForm)
{
  return m_resolutionToString(&m_initialResolution, shortForm);
}

string ResolutionManager::getCurrentResolution(bool shortForm)
{
  return m_resolutionToString(&m_currentResolution, shortForm);
}

vector<string> ResolutionManager::getAllResolutions(bool shortForm)
{
  vector<string> resolutionsAsStrings;

  for (size_t i = 0; i < m_resolutions.size(); i++)
  {
    string res = m_resolutionToString(m_resolutions[i], shortForm);
    // Especially for shortForm == true: Don't add the same resolution more than once.
    if ( find(resolutionsAsStrings.begin(), resolutionsAsStrings.end(), res) == resolutionsAsStrings.end())
    {
      resolutionsAsStrings.push_back(res);
    }
  }
  return resolutionsAsStrings;
}

bool ResolutionManager::setResolution(string resolutionToSet)
{
  DEVMODEA* modeToSet;
  try
  {
    modeToSet = m_findResolution(resolutionToSet);
    //std::cout << "Found resolution " << ": " << modeToSet->dmPelsWidth << "x" << modeToSet->dmPelsHeight << " " << modeToSet->dmDisplayFrequency << " " << modeToSet->dmBitsPerPel << " " << modeToSet->dmDisplayFlags << "\n";
    
    modeToSet->dmSize = sizeof(DEVMODEA);
    modeToSet->dmDriverExtra = 0;
    modeToSet->dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY ;

   LONG retval = ChangeDisplaySettingsA(modeToSet, 0);
   if (retval != DISP_CHANGE_SUCCESSFUL)
   {
     return false;
   }
  }
  catch (...)
  {
    return false;
  }
  
  m_currentResolution = *modeToSet;
  return true;
}

void ResolutionManager::cloneScreens()
{
  LONG rv = SetDisplayConfig(0, NULL, 0, NULL, SDC_TOPOLOGY_CLONE | SDC_APPLY);
}


DEVMODEA* ResolutionManager::m_findResolution(string resolution)
{
  // First parse the string (for examnple '640x480')
  size_t posOfX = resolution.find('x');
  if (posOfX == string::npos)
  {
    throw new invalid_argument("Invalid string for resolution: '" + resolution + "'");
  }

  string widthstr = resolution.substr(0, posOfX);
  string heightstr = resolution.substr(posOfX + 1);

  DWORD width, height;
  try
  {
    width = (DWORD)stoi(widthstr);
    height = (DWORD)stoi(heightstr);
  }
  catch (...)
  {
    throw new invalid_argument("Invalid string for resolution: '" + resolution + "'");
  }

  // Iterate over m_resolutions and find best suited resolution with given width and heigt
  DEVMODEA* bestMatch = NULL;
  for (size_t i = 0; i < m_resolutions.size(); i++)
  {
    if (m_resolutions[i]->dmPelsWidth == width && m_resolutions[i]->dmPelsHeight == height)
    {
      if (bestMatch == NULL)
      {
        bestMatch = m_resolutions[i];
      }
      else if (bestMatch->dmBitsPerPel < m_resolutions[i]->dmBitsPerPel)
      {
        bestMatch = m_resolutions[i];
      }
      else if (bestMatch->dmDisplayFrequency < m_resolutions[i]->dmDisplayFrequency)
      {
        bestMatch = m_resolutions[i];
      }
    }
  }

  if (bestMatch == NULL)
  {
    throw new invalid_argument("No resolution found with '" + resolution + "'");
  }

  return bestMatch;
}


///////////////////////////////////////////////////////////////////
// static helper-methods
///////////////////////////////////////////////////////////////////

bool ResolutionManager::m_areResolutionsEqual(DEVMODEA* resolutionA, DEVMODEA* resolutionB)
{
  return resolutionA->dmBitsPerPel       == resolutionB->dmBitsPerPel &&
         resolutionA->dmPelsWidth        == resolutionB->dmPelsWidth &&
         resolutionA->dmPelsHeight       == resolutionB->dmPelsHeight &&
         resolutionA->dmDisplayFrequency == resolutionB->dmDisplayFrequency &&
         resolutionA->dmDisplayFlags     == resolutionB->dmDisplayFlags;
}

bool ResolutionManager::m_haveSameWidthAndHeight(DEVMODEA* resolutionA, DEVMODEA* resolutionB)
{
  return resolutionA->dmPelsWidth  == resolutionB->dmPelsWidth &&
         resolutionA->dmPelsHeight == resolutionB->dmPelsHeight;
}

string ResolutionManager::m_resolutionToString(DEVMODEA* resolution, bool shortForm)
{
  ostringstream s;
  s << resolution->dmPelsWidth << "x" << resolution->dmPelsHeight;
  if (!shortForm)
  {
    s << ", " << resolution->dmBitsPerPel << " bits @ " << resolution->dmDisplayFrequency << " Hz.";
  }
  return s.str();
}



