# vtkMultiChannel
Adds rendering multiple views of a VTK scene (to support passive stereo, for example) using a single window and renderer.

Python notes:	
* Should only build in Release mode.  
* When wrapping Python on Windows, need to change vtkMultiChannel.dll to vtkMultiChannel.pyd and add the containing directory to the PYTHONPATH environment variable.
