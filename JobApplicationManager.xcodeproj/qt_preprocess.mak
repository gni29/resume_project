#############################################################################
# Makefile for building: JobApplicationManager.app/Contents/MacOS/JobApplicationManager
# Generated by qmake (3.1) (Qt 6.8.2)
# Project:  JobApplicationManager.pro
# Template: app
# Command: /opt/homebrew/bin/qmake -o JobApplicationManager.xcodeproj/project.pbxproj JobApplicationManager.pro -spec macx-xcode
#############################################################################

MAKEFILE      = project.pbxproj

EQ            = =

MOC       = /opt/homebrew/share/qt/libexec/moc
UIC       = /opt/homebrew/share/qt/libexec/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_CORE_LIB
INCPATH       = -I. -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/System/Library/Frameworks/OpenGL.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/System/Library/Frameworks/AGL.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/System/Library/Frameworks/OpenGL.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/System/Library/Frameworks/AGL.framework/Headers -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -F/opt/homebrew/lib
DEL_FILE  = rm -f
MOVE      = mv -f

preprocess: compilers
clean preprocess_clean: compiler_clean

mocclean: compiler_moc_header_clean compiler_moc_objc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_objc_header_make_all compiler_moc_source_make_all

check: first

benchmark: first

compilers: moc_predefs.h moc_company_add_window.cpp moc_company_window.cpp moc_first_window.cpp\
	 moc_num_issue_window.cpp moc_type_qualification_window.cpp moc_write_resume_window.cpp
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_predefs_make_all: moc_predefs.h
compiler_moc_predefs_clean:
	-$(DEL_FILE) moc_predefs.h
moc_predefs.h: /opt/homebrew/share/qt/mkspecs/features/data/dummy.cpp
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -pipe -stdlib=libc++ -O2 -std=gnu++1z -Wall -Wextra -dM -E -o moc_predefs.h /opt/homebrew/share/qt/mkspecs/features/data/dummy.cpp

compiler_moc_header_make_all: moc_company_add_window.cpp moc_company_window.cpp moc_first_window.cpp moc_num_issue_window.cpp moc_type_qualification_window.cpp moc_write_resume_window.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_company_add_window.cpp moc_company_window.cpp moc_first_window.cpp moc_num_issue_window.cpp moc_type_qualification_window.cpp moc_write_resume_window.cpp
moc_company_add_window.cpp: company_add_window.h \
		/opt/homebrew/lib/QtWidgets.framework/Headers/QDialog \
		/opt/homebrew/lib/QtCore.framework/Headers/QString \
		moc_predefs.h \
		/opt/homebrew/share/qt/libexec/moc
	/opt/homebrew/share/qt/libexec/moc $(DEFINES) --include /Users/pyoungjinji/resume_manager/qt_resume/qt_resume/moc_predefs.h -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -I/Users/pyoungjinji/resume_manager/qt_resume/qt_resume -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I/opt/homebrew/Cellar/fltk/1.4.2/include -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -F/opt/homebrew/lib company_add_window.h -o moc_company_add_window.cpp

moc_company_window.cpp: company_window.h \
		/opt/homebrew/lib/QtWidgets.framework/Headers/QDialog \
		/opt/homebrew/lib/QtCore.framework/Headers/QString \
		/opt/homebrew/lib/QtCore.framework/Headers/QVector \
		company_model.h \
		database_utils.h \
		/opt/homebrew/lib/QtSql.framework/Headers/QSqlDatabase \
		/opt/homebrew/lib/QtSql.framework/Headers/QSqlQuery \
		/opt/homebrew/lib/QtSql.framework/Headers/QSqlError \
		/opt/homebrew/lib/QtCore.framework/Headers/QVariant \
		/opt/homebrew/lib/QtCore.framework/Headers/QDebug \
		/opt/homebrew/lib/QtCore.framework/Headers/QDateTime \
		moc_predefs.h \
		/opt/homebrew/share/qt/libexec/moc
	/opt/homebrew/share/qt/libexec/moc $(DEFINES) --include /Users/pyoungjinji/resume_manager/qt_resume/qt_resume/moc_predefs.h -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -I/Users/pyoungjinji/resume_manager/qt_resume/qt_resume -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I/opt/homebrew/Cellar/fltk/1.4.2/include -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -F/opt/homebrew/lib company_window.h -o moc_company_window.cpp

moc_first_window.cpp: first_window.h \
		/opt/homebrew/lib/QtWidgets.framework/Headers/QMainWindow \
		/opt/homebrew/lib/QtCore.framework/Headers/QVector \
		company_model.h \
		database_utils.h \
		/opt/homebrew/lib/QtSql.framework/Headers/QSqlDatabase \
		/opt/homebrew/lib/QtSql.framework/Headers/QSqlQuery \
		/opt/homebrew/lib/QtSql.framework/Headers/QSqlError \
		/opt/homebrew/lib/QtCore.framework/Headers/QString \
		/opt/homebrew/lib/QtCore.framework/Headers/QVariant \
		/opt/homebrew/lib/QtCore.framework/Headers/QDebug \
		/opt/homebrew/lib/QtCore.framework/Headers/QDateTime \
		moc_predefs.h \
		/opt/homebrew/share/qt/libexec/moc
	/opt/homebrew/share/qt/libexec/moc $(DEFINES) --include /Users/pyoungjinji/resume_manager/qt_resume/qt_resume/moc_predefs.h -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -I/Users/pyoungjinji/resume_manager/qt_resume/qt_resume -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I/opt/homebrew/Cellar/fltk/1.4.2/include -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -F/opt/homebrew/lib first_window.h -o moc_first_window.cpp

moc_num_issue_window.cpp: num_issue_window.h \
		/opt/homebrew/lib/QtWidgets.framework/Headers/QDialog \
		/opt/homebrew/lib/QtCore.framework/Headers/QString \
		/opt/homebrew/lib/QtCore.framework/Headers/QDir \
		moc_predefs.h \
		/opt/homebrew/share/qt/libexec/moc
	/opt/homebrew/share/qt/libexec/moc $(DEFINES) --include /Users/pyoungjinji/resume_manager/qt_resume/qt_resume/moc_predefs.h -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -I/Users/pyoungjinji/resume_manager/qt_resume/qt_resume -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I/opt/homebrew/Cellar/fltk/1.4.2/include -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -F/opt/homebrew/lib num_issue_window.h -o moc_num_issue_window.cpp

moc_type_qualification_window.cpp: type_qualification_window.h \
		/opt/homebrew/lib/QtWidgets.framework/Headers/QDialog \
		/opt/homebrew/lib/QtCore.framework/Headers/QString \
		moc_predefs.h \
		/opt/homebrew/share/qt/libexec/moc
	/opt/homebrew/share/qt/libexec/moc $(DEFINES) --include /Users/pyoungjinji/resume_manager/qt_resume/qt_resume/moc_predefs.h -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -I/Users/pyoungjinji/resume_manager/qt_resume/qt_resume -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I/opt/homebrew/Cellar/fltk/1.4.2/include -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -F/opt/homebrew/lib type_qualification_window.h -o moc_type_qualification_window.cpp

moc_write_resume_window.cpp: write_resume_window.h \
		/opt/homebrew/lib/QtWidgets.framework/Headers/QDialog \
		/opt/homebrew/lib/QtCore.framework/Headers/QString \
		/opt/homebrew/lib/QtCore.framework/Headers/QVector \
		/opt/homebrew/lib/QtCore.framework/Headers/QTimer \
		/opt/homebrew/lib/QtCore.framework/Headers/QFile \
		moc_predefs.h \
		/opt/homebrew/share/qt/libexec/moc
	/opt/homebrew/share/qt/libexec/moc $(DEFINES) --include /Users/pyoungjinji/resume_manager/qt_resume/qt_resume/moc_predefs.h -I/opt/homebrew/Cellar/qt/6.8.2_1/share/qt/mkspecs/macx-clang -I/Users/pyoungjinji/resume_manager/qt_resume/qt_resume -I/opt/homebrew/lib/QtWidgets.framework/Headers -I/opt/homebrew/lib/QtGui.framework/Headers -I/opt/homebrew/lib/QtSql.framework/Headers -I/opt/homebrew/lib/QtCore.framework/Headers -I/opt/homebrew/Cellar/fltk/1.4.2/include -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk/usr/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -F/opt/homebrew/lib write_resume_window.h -o moc_write_resume_window.cpp

compiler_moc_objc_header_make_all:
compiler_moc_objc_header_clean:
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_predefs_clean compiler_moc_header_clean 

