TEMPLATE = subdirs
SUBDIRS = KamiCmd \
          FileCopy/FileCopy_Widget \
          FS_Qt \
          Operations_Queue \
          Panel_Classic \
          Unit_Console \
          Unit_PictureView \
          Unit_WebKit \
          UnitManager_Tabs \
	  Unit_TextEditor \
	  Unit_Lexers

win32 {
          SUBDIRS += FileCopy/FileCopy_Windows \
                     FS_Windows
}
