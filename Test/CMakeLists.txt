PROJECT( vtkMultiChannelTest ) 

SET( EXECUTABLE_OUTPUT_PATH "${vtkMultiChannel_BINARY_DIR}/bin" )

SET( VTK_LIBS vtkRendering )

SET( SRC vtkMultiChannelTest )
ADD_EXECUTABLE( vtkMultiChannelTest MACOSX_BUNDLE ${SRC} )
ADD_DEPENDENCIES( vtkMultiChannelTest vtkMultiChannel )
TARGET_LINK_LIBRARIES( vtkMultiChannelTest 
                       ${VTK_LIBS}
                 debug ${vtkMultiChannel_BINARY_DIR}/lib/debug/vtkMultiChannel.lib
             optimized ${vtkMultiChannel_BINARY_DIR}/lib/release/vtkMultiChannel.lib )