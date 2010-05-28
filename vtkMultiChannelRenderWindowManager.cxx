/*=========================================================================

  Name:        vtkMultiChannelRenderWindowManager.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkMultiChannelRenderWindowManager.h"

#include "vtkCollection.h"
#include "vtkGraphicsFactory.h"
#include "vtkMultiChannelRenderWindowHelper.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowChannel.h"
#include "vtkRenderer.h"

// Include platform-specific headers via code borrowed from vtkGraphicsFactory.h

// Win32 specific stuff
#ifdef _WIN32
# ifndef VTK_USE_OGLR
#  include "vtkOpenGLMultiChannelCamera.h"
#  include "vtkWin32OpenGLMultiChannelRenderWindow.h"
#  define VTK_DISPLAY_WIN32_OGL
# endif // VTK_USE_OGLR
#endif

// Apple OSX stuff
#ifdef VTK_USE_CARBON
# include "vtkCarbonMultiChannelRenderWindow.h"
# define VTK_DISPLAY_CARBON
#endif

#ifdef VTK_USE_COCOA
# include "vtkCocoaMultiChannelRenderWindow.h"
# define VTK_DISPLAY_COCOA
#endif

// X OpenGL stuff
#ifdef VTK_USE_OGLR
# include "vtkXOpenGLMultiChannelRenderWindow.h"
# define VTK_DISPLAY_X11_OGL
#endif

// OSMESA OpenGL stuff
#ifdef VTK_USE_OSMESA
# include "vtkOSOpenGLMultiChannelRenderWindow.h"
//# define VTK_DISPLAY_X11_OGL
#endif

#if defined(VTK_USE_MANGLED_MESA)
#include "vtkXMesaRenderMultiChannelRenderWindow.h"
#endif

vtkCxxRevisionMacro(vtkMultiChannelRenderWindowManager, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkMultiChannelRenderWindowManager);

//----------------------------------------------------------------------------
vtkMultiChannelRenderWindowManager::vtkMultiChannelRenderWindowManager() 
{
  this->Helper = vtkMultiChannelRenderWindowHelper::New();

  this->NeedsStereo = false;
}

//----------------------------------------------------------------------------
vtkMultiChannelRenderWindowManager::~vtkMultiChannelRenderWindowManager() 
{
  this->Helper->Delete();
}

//----------------------------------------------------------------------------
void vtkMultiChannelRenderWindowManager::AddChannel(vtkRenderWindowChannel *channel)
{
  this->Helper->GetChannels()->AddItem(channel);

  if (channel->GetStereoType() != VTK_MULTICHANNEL_STEREO_NONE)
    {
    this->NeedsStereo = true;
    }
}

//----------------------------------------------------------------------------
void vtkMultiChannelRenderWindowManager::ClearChannels()
{
  this->Helper->GetChannels()->RemoveAllItems();
  this->NeedsStereo = false;
}

//----------------------------------------------------------------------------
vtkRenderWindow* vtkMultiChannelRenderWindowManager::GetRenderWindow() 
{
  // Return the correct platform-specific vtkRenderWindow subclass

  const char *rl = vtkGraphicsFactory::GetRenderLibrary();

#ifdef VTK_USE_OGLR
  if (!strcmp("OpenGL",rl))
    {
#if defined(VTK_USE_MANGLED_MESA)
    if ( vtkGraphicsFactory::UseMesaClasses )
      {
      vtkErrorMacro(<<"Multi-channel rendering not implemented for this platform yet.");
      return vtkXMesaRenderWindow::New();
      }
#endif
    vtkErrorMacro(<<"Multi-channel rendering not implemented for this platform yet.");
    return vtkXOpenGLRenderWindow::New();
    }
#endif

#if defined(VTK_USE_OSMESA)
  vtkErrorMacro(<<"Multi-channel rendering not implemented for this platform yet.");
  return vtkOSOpenGLRenderWindow::New();
#endif

#ifdef VTK_DISPLAY_WIN32_OGL
  if (!strcmp("Win32OpenGL",rl))
    {
    vtkWin32OpenGLMultiChannelRenderWindow* window = vtkWin32OpenGLMultiChannelRenderWindow::New();
    window->SetHelper(this->Helper);
    if (this->NeedsStereo)
      {
      window->StereoCapableWindowOn();
      window->StereoRenderOn();
      }

    // Create a new helper for the next window to be created
    this->Helper->Delete();
    this->Helper = vtkMultiChannelRenderWindowHelper::New();  
    this->NeedsStereo = false;

    return window;
    }
#endif

#ifdef VTK_USE_CARBON
  vtkErrorMacro(<<"Multi-channel rendering not implemented for this platform yet.");
  return vtkCarbonRenderWindow::New();
#endif
#ifdef VTK_USE_COCOA
  vtkErrorMacro(<<"Multi-channel rendering not implemented for this platform yet.");
  return vtkCocoaRenderWindow::New();
#endif
        
  return 0;
}

//----------------------------------------------------------------------------
vtkRenderer *vtkMultiChannelRenderWindowManager::GetRenderer()
{
  vtkRenderer* renderer = vtkRenderer::New();

#if defined(VTK_USE_MANGLED_MESA)
  if (vtkGraphicsFactor::UseMesaClasses)
    {
    vtkErrorMacro(<<"Multi-channel rendering not implemented for this rendering library yet.");
    return 0;
  }
#endif

  vtkOpenGLMultiChannelCamera *camera = vtkOpenGLMultiChannelCamera::New();
  renderer->SetActiveCamera(camera);

  return renderer;
}

//----------------------------------------------------------------------------
void vtkMultiChannelRenderWindowManager::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Helper:\n"; 
  this->Helper->PrintSelf(os,indent.GetNextIndent());

  os << indent << "NeedsStereo: " << this->NeedsStereo << "\n";
}