/*=========================================================================

  Name:        vtkMultiChannelRenderWindowHelper.cxx

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  License:     Licensed under the RENCI Open Source Software License v. 1.0.
               
               See included License.txt or 
               http://www.renci.org/resources/open-source-software-license
               for details.

=========================================================================*/

#include "vtkMultiChannelRenderWindowHelper.h"

#include "vtkCollection.h"
#include "vtkMultiChannelRenderWindowHelper.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowChannel.h"
#include "vtkRendererCollection.h"
#include "vtkRenderer.h"

vtkCxxRevisionMacro(vtkMultiChannelRenderWindowHelper, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkMultiChannelRenderWindowHelper);

//----------------------------------------------------------------------------
vtkMultiChannelRenderWindowHelper::vtkMultiChannelRenderWindowHelper() 
{
  this->Channels = vtkCollection::New();
}

//----------------------------------------------------------------------------
vtkMultiChannelRenderWindowHelper::~vtkMultiChannelRenderWindowHelper() 
{
  this->Channels->Delete();
}

//----------------------------------------------------------------------------
vtkCollection *vtkMultiChannelRenderWindowHelper::GetChannels() 
{
  return this->Channels;
}

//----------------------------------------------------------------------------
void vtkMultiChannelRenderWindowHelper::Render(vtkRendererCollection *renderers)
{
  // Render multiple channels.  
  for (int i = 0; i < this->Channels->GetNumberOfItems(); i++) 
    {
    vtkRenderWindowChannel* channel = vtkRenderWindowChannel::SafeDownCast(this->Channels->GetItemAsObject(i));
    
    vtkCollectionSimpleIterator iterator;
    vtkRenderer *renderer;
    for (renderers->InitTraversal(iterator); (renderer = renderers->GetNextRenderer(iterator)); )
      {
      channel->Render(renderer);
      }
    }
}

//----------------------------------------------------------------------------
void vtkMultiChannelRenderWindowHelper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Channels:\n"; 
  this->Channels->PrintSelf(os,indent.GetNextIndent());
}