/*=========================================================================

  Name:        vtkRenciRenderWindowManager.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/
// .NAME vtkRenciRenderWindowManager
// .SECTION Description
// vtkRenciRenderWindowManager is a subclass of 
// vtkMultiChannelRenderWindowManager that handles setting up rendering
// for the Dome and TeleImmersion visualization spaces at the RENCI
// Engagement Center at UNC-Chapel Hill, and the head-mounted display
// unit at the UNC-Chapel Hill Computer Science department.

// .SECTION see also
// vtkRenderWindow vtkMultiChannelRenderWindowHelper 
// vtkRenderWindowChannel

#ifndef __vtkRenciRenderWindowManager_h
#define __vtkRenciRenderWindowManager_h

#include "vtkMultiChannelConfigure.h"

#include "vtkMultiChannelRenderWindowManager.h"

class VTK_MULTICHANNEL_EXPORT vtkRenciRenderWindowManager : public vtkMultiChannelRenderWindowManager
{
public:
  static vtkRenciRenderWindowManager *New();
  vtkTypeRevisionMacro(vtkRenciRenderWindowManager,vtkMultiChannelRenderWindowManager);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Parses argc and argv from a command line to setup render modes
  // Usage:  -Dome 
  //         -DomePitch pitch
  //         -TeleImmersionHD
  //         -TeleImmersion4K
  //         -UncHmd
  vtkRenderWindow *GetRenciRenderWindow(int argc, char* argv[]);

  // Description:
  // Window for the 4-channel immersive dome
  vtkRenderWindow *GetDomeRenderWindow(double domePitch = 90.0);

  // Description:
  // Window for 2-channel HD stereo mode in the TeleImmersion room
  vtkRenderWindow *GetTeleImmersionHDRenderWindow();

  // Description:
  // Window for 2-channel 4K stereo mode in the TeleImmersion room
  vtkRenderWindow *GetTeleImmersion4KRenderWindow();

  // Description:
  // Window for 2-channel head-mounted display at UNC Computer Science
  vtkRenderWindow *GetUncHmdRenderWindow();

protected:
  vtkRenciRenderWindowManager();
  ~vtkRenciRenderWindowManager();

  void AddDomeChannel(double x1, double y1, double x2, double y2, 
                      double domePitch, double yaw);

  void PositionWindow(vtkRenderWindow*, int width, int height);

private:
  vtkRenciRenderWindowManager(const vtkRenciRenderWindowManager&);  // Not implemented.
  void operator=(const vtkRenciRenderWindowManager&);  // Not implemented.
};

#endif