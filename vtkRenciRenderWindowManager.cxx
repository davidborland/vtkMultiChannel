/*=========================================================================

  Name:        vtkRenciRenderWindowManager.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkRenciRenderWindowManager.h"

#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowChannel.h"

vtkCxxRevisionMacro(vtkRenciRenderWindowManager, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkRenciRenderWindowManager);

//----------------------------------------------------------------------------
vtkRenciRenderWindowManager::vtkRenciRenderWindowManager() 
{
}

//----------------------------------------------------------------------------
vtkRenciRenderWindowManager::~vtkRenciRenderWindowManager() 
{
}

//----------------------------------------------------------------------------
vtkRenderWindow *vtkRenciRenderWindowManager::GetRenciRenderWindow(int argc, char* argv[]) 
{
  int windowType = -1;
  double domePitch = 90.0;


  for (int i = 1; i < argc; i++)
    {
    if (strcmp(argv[i], "-Dome") == 0)
      {
      windowType = 0;
      }
    else if (strcmp(argv[i], "-TeleImmersionHD") == 0)
      {
      windowType = 1;
      }
    else if (strcmp(argv[i], "-TeleImmersion4K") == 0)
      {
      windowType = 2;
      }
    else if (strcmp(argv[i], "-DomePitch") == 0) 
      {
      i++;
      if (i < argc)
        {
        domePitch = atof(argv[i]);
        }
      }
    }

  vtkRenderWindow* window;
  if (windowType == 0)
    {
    window = this->GetDomeRenderWindow(domePitch);
    }
  else if (windowType == 1)
    {
    window = this->GetTeleImmersionHDRenderWindow();
    }
  else if (windowType == 2)
    {
    window = this->GetTeleImmersion4KRenderWindow();
    }
  else
    {
    window = this->GetRenderWindow();
    window->SetSize(1024, 1024);
    }

  return window;
}

//----------------------------------------------------------------------------
vtkRenderWindow *vtkRenciRenderWindowManager::GetDomeRenderWindow(double domePitch) 
{
  domePitch = domePitch < 10.0 ? 10.0 : domePitch;
  domePitch = domePitch > 170.0 ? 170.0 : domePitch;
  domePitch = 90.0 - domePitch;

  // Clear all channels
  this->ClearChannels();

  // Set up dome channels
  this->AddDomeChannel(0.0, 0.0, 0.5, 0.5, domePitch,   45.0);
  this->AddDomeChannel(0.5, 0.0, 1.0, 0.5, domePitch,  -45.0);
  this->AddDomeChannel(0.0, 0.5, 0.5, 1.0, domePitch,  135.0);
  this->AddDomeChannel(0.5, 0.5, 1.0, 1.0, domePitch, -135.0);

  // Get a window
  vtkRenderWindow* window = this->GetRenderWindow();

  // Set up window for Dome
  this->PositionWindow(window, 1400 * 2, 1050 * 2);

  return window;
}

//----------------------------------------------------------------------------
vtkRenderWindow *vtkRenciRenderWindowManager::GetTeleImmersionHDRenderWindow() 
{
  // Clear all channels
  this->ClearChannels();

  double oneThird = 1.0 / 3.0;
  double twoThirds = 2.0 / 3.0;

  // Set up stereo channels
  vtkRenderWindowChannel* channel1 = vtkRenderWindowChannel::New();
  channel1->SetViewport(0.0, 0.0, oneThird, 1.0);
  channel1->SetStereoTypeToLeft();
  this->AddChannel(channel1);
  channel1->Delete();

  vtkRenderWindowChannel* channel2 = vtkRenderWindowChannel::New();
  channel2->SetViewport(twoThirds, 0.0, 1.0, 1.0);
  channel2->SetStereoTypeToRight();
  this->AddChannel(channel2);
  channel2->Delete();

  // Get a window
  vtkRenderWindow* window = this->GetRenderWindow();

  // Set up window for TeleImmersion
  this->PositionWindow(window, 1920 * 3, 1080);

  return window;
}

//----------------------------------------------------------------------------
vtkRenderWindow *vtkRenciRenderWindowManager::GetTeleImmersion4KRenderWindow() 
{
  // Clear all channels
  this->ClearChannels();

  // Set up stereo channels
  vtkRenderWindowChannel* channel1 = vtkRenderWindowChannel::New();
  channel1->SetViewport(0.0, 0.0, 0.5, 1.0);
  channel1->SetStereoTypeToLeft();
  this->AddChannel(channel1);
  channel1->Delete();

  vtkRenderWindowChannel* channel2 = vtkRenderWindowChannel::New();
  channel2->SetViewport(0.5, 0.0, 1.0, 1.0);
  channel2->SetStereoTypeToRight();
  this->AddChannel(channel2);
  channel2->Delete();

  // Get a window
  vtkRenderWindow* window = this->GetRenderWindow();

  // Set up window for TeleImmersion
  this->PositionWindow(window, 1920 * 4, 1080 * 2);

  return window;
}

//----------------------------------------------------------------------------
vtkRenderWindow *vtkRenciRenderWindowManager::GetUncHmdRenderWindow() 
{
  // Clear all channels
  this->ClearChannels();

  // Set up stereo channels
  vtkRenderWindowChannel* channel1 = vtkRenderWindowChannel::New();
  channel1->SetViewport(0.0, 0.0, 0.5, 1.0);
  channel1->SetStereoTypeToLeft();
  this->AddChannel(channel1);
  channel1->Delete();

  vtkRenderWindowChannel* channel2 = vtkRenderWindowChannel::New();
  channel2->SetViewport(0.5, 0.0, 1.0, 1.0);
  channel2->SetStereoTypeToRight();
  this->AddChannel(channel2);
  channel2->Delete();

  // Get a window
  vtkRenderWindow* window = this->GetRenderWindow();

  // Set up window for the HMD
  this->PositionWindow(window, 1280 * 2, 1024);

  return window;
}

//----------------------------------------------------------------------------
void vtkRenciRenderWindowManager::AddDomeChannel(double x1, double y1, double x2, double y2, 
                                                 double domePitch, double yaw)
{  
  // Values
  const double prePitch = -90;
  const double pitch = 52.5;
  const double viewAngle = 86.0;
  const double aspectRatio = tan(vtkMath::RadiansFromDegrees(56.0)) / 
                             tan(vtkMath::RadiansFromDegrees(43.0));

  vtkRenderWindowChannel* channel = vtkRenderWindowChannel::New();
  channel->SetViewport(x1, y1, x2, y2);
  channel->Pitch(prePitch / 2);
  channel->OrthogonalizeViewUp();
  channel->Pitch(prePitch / 2);
  channel->OrthogonalizeViewUp();
  channel->Pitch(domePitch);
  channel->OrthogonalizeViewUp();
  channel->Yaw(yaw);
  channel->Pitch(pitch);
  channel->SetViewAngle(viewAngle);
  channel->SetAspectRatio(aspectRatio);
  this->AddChannel(channel);
  channel->Delete();
}

//----------------------------------------------------------------------------
void vtkRenciRenderWindowManager::PositionWindow(vtkRenderWindow *window, int width, int height)
{  
  window->BordersOff();
#ifdef _WIN32
  // Border not properly accounted for on Windows
  width -= 8;
  height -= 34;
#endif
  window->SetSize(width, height);
  window->SetPosition(0, 0);
}


//----------------------------------------------------------------------------
void vtkRenciRenderWindowManager::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}