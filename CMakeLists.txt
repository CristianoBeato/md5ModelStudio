cmake_minimum_required(VERSION 3.13)

project( md5ModelStudio )

## STD 17
set( CMAKE_CXX_STANDARD 17 )

find_package( SDL3 REQUIRED )
find_package( SDL3_image REQUIRED )
find_package( SDL3_ttf REQUIRED )

include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/glm )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/crglLib )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/gwen )

set( GWEN_SOURCE
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Align.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Anim.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Anim.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/BaseRender.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/BaseRender.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Config.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/ControlList.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/ControlList.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/DragAndDrop.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/DragAndDrop.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/events.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Events.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Exports.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Font.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Gwen.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Gwen.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Hook.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Hook.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Input
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/inputhandler.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/InputHandler.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Macros.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Platform.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Platforms
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Renderers
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Skin.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Skin.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Skins
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Structures.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/TextObject.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Texture.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/ToolTip.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/ToolTip.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/UnitTest
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/UserData.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Util
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Utility.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Utility.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/WindowProvider.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Base.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Base.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Button.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Button.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Canvas.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Canvas.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CheckBox.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CheckBox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CollapsibleCategory.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CollapsibleCategory.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CollapsibleList.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ColorControls.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ColorControls.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ColorPicker.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ColorPicker.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ComboBox.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ComboBox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CrossSplitter.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/CrossSplitter.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/DockBase.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/DockBase.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/DockedTabControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/DockedTabControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dragger.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dragger.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/FieldLabel.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/FilePicker.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/FolderPicker.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/GroupBox.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/GroupBox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Highlight.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/HorizontalScrollBar.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/HorizontalScrollBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/HorizontalSlider.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/HorizontalSlider.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/HSVColorPicker.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/HSVColorPicker.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ImagePanel.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ImagePanel.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/LabelClickable.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/LabelClickable.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Label.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Label.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Layout
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ListBox.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ListBox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Menu.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Menu.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/MenuItem.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/MenuItem.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/MenuStrip.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/MenuStrip.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Modal.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/NumericUpDown.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/NumericUpDown.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/PageControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/PageControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ProgressBar.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ProgressBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Properties.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Properties.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/PropertyTree.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/PropertyTree.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/RadioButtonController.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/RadioButtonController.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/RadioButton.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/RadioButton.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Rectangle.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Rectangle.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ResizableControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ResizableControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Resizer.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Resizer.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/RichLabel.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/RichLabel.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollBarBar.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollBarBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollBarButton.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollBarButton.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollBar.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ScrollControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Slider.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Slider.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/SplitterBar.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/SplitterBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Splitters.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/StatusBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabButton.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabButton.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabStrip.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabStrip.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TabTitleBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TextBox.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TextBox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TextBoxNumeric.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Text.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Text.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/ToolBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TreeControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TreeControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TreeNode.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/TreeNode.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/VerticalScrollBar.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/VerticalScrollBar.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/VerticalSlider.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/VerticalSlider.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/WindowButtons.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/WindowCanvas.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/WindowCanvas.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/WindowControl.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/WindowControl.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/FileOpen.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/FileOpen.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/FileSave.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/FileSave.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/FolderOpen.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/FolderOpen.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/Query.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Dialogs/Query.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Layout/Position.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Layout/Table.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Layout/Tile.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/BaseProperty.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/Checkbox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/ColorSelector.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/ComboBox.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/File.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/Folder.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Controls/Property/Text.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Input/SDL3.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Platforms/SDL3.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Renderers/FontData.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Renderers/OpenGL.cpp
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Renderers/OpenGL.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Skins/Simple.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Skins/TexturedBase.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Skins/Texturing.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Util/ControlFactory.h
    ${CMAKE_SOURCE_DIR}/contrib/gwen/Gwen/Util/ImportExport.h
)

set( CRGL_SOURCE 
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglBuffer.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglBuffer.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglContext.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglContext.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglCore.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglCore.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglFence.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglFence.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglFormat.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglFormat.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglFrameBuffer.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglFrameBuffer.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglImage.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglImage.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglPrecompiled.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglSampler.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglSampler.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglShaders.cpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglShaders.hpp
    ${CMAKE_SOURCE_DIR}/contrib/crglLib/crglVertexArray.cpp
    )

set( MD5MESH_SOURCE 
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Anim.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Anim.hpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Joint.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Joint.hpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Mesh.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Mesh.hpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Model.cpp
    ${CMAKE_SOURCE_DIR}/md5Mesh/md5Model.hpp
    )

set( MAIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/precompiled.hpp
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/main.hpp
    )

set( SOURCES
    ${GWEN_SOURCE}
    ${CRGL_SOURCE}
    ${MD5MESH_SOURCE}
    ${MAIN_SOURCES}
    )

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID STREQUAL "Clang" )
	add_definitions(-Wnarrowing)
endif()

add_executable( md5ModelStudio ${SOURCES} )
target_link_libraries( md5ModelStudio PUBLIC SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf )
target_precompile_headers( md5ModelStudio PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/precompiled.hpp>" )
