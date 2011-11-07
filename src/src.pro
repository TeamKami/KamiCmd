TEMPLATE = subdirs
SUBDIRS = KamiCmd \
          FS_Qt \
          FileCopy/FileCopy_Qt \
       	  FileOperationDummy \
          Operations_Queue \
          Panel_Classic \
          Unit_KamiShell \
          Unit_PictureView \
          Unit_WebKit \
          UnitManager_Tabs \
	  Unit_TextEditor \
	  Lib_Lexers

win32 {
          SUBDIRS += FS_Windows
}

INCLUDEPATH += .
