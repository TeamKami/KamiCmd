TEMPLATE = subdirs
SUBDIRS = KamiCmd \
          FileCopy/FileCopy_Widget \
          FS_Qt \
          Operations_Queue \
          Panel_Classic \
          Unit_KamiShell \
          Unit_PictureView \
          Unit_WebKit \
          UnitManager_Tabs \
	  Unit_TextEditor \
	  Lib_Lexers

win32 {
          SUBDIRS += FileCopy/FileCopy_Windows \
                     FS_Windows
}

INCLUDEPATH += .
