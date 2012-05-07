; libclaw.nsi
;
; Windows installer for Claw, a C++ Library Absolutely Wonderful

;--------------------------------

; The name of the installer
Name "Claw Windows installer"

; The file to write
OutFile libclaw.exe

; The default installation directory
InstallDir $PROGRAMFILES\claw
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libclaw" "UninstallString"

XPStyle on

;--------------------------------

Page license
Page components
Page directory
Page instfiles

;--------------------------------

LicenseData COPYING

InstType Runtime
InstType Development
InstType Full

;--------------------------------
; .dll files
Section "Runtime libraries (required)"

  SectionIn 1 2 3

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\lib"
  
  File "*.dll"

  SetOutPath "$INSTDIR\share\doc"

  File COPYING

SectionEnd ; end of "Runtime libraries"

;--------------------------------
; .dll.a files
Section "Static libraries"

  SectionIn 2 3

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\lib"
  
  File "*.dll.a"
SectionEnd ; end of "Static libraries"

;--------------------------------
; Library's headers
Section "Source headers"

  SectionIn 2 3

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\include\claw"
  
  File "claw\*.hpp"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\include\claw\impl"

  File "claw\impl\*.tpp"
  File "claw\impl\*.ipp"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\include\claw\meta"
  
  File "claw\meta\*.hpp"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\include\claw\tween"
  
  File "claw\tween\*.hpp"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\include\claw\tween\easing"
  
  File "claw\tween\easing\*.hpp"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\include\claw\tween\easing\impl"
  
  File "claw\tween\easing\impl\*.tpp"

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\win"

  File "win\definitions.txt"

SectionEnd ; end of "Source headers"

;--------------------------------
; documentation of the classes
Section "Classes documentation"

  SectionIn 2 3

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\share\doc\html"
  
  File /r "doc\html\"

SectionEnd ; end of "Classes documentation"

;--------------------------------
; CMake modules for using the library
Section "CMake modules"

  SectionIn 2 3

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\lib\cmake\libclaw"
  
  File "cmake-module\libclaw-config.cmake"

SectionEnd ; end of "CMake modules"

;--------------------------------
; Example programs
Section "Example programs"

  SectionIn 3

  ; Set output path to the installation directory.
  SetOutPath "$INSTDIR\share\doc\examples"
  
  File /r "examples\"

SectionEnd ; end of "Example programs"

;--------------------------------
Section -PostInstall
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libclaw" "DisplayName" "Claw library (uninstall)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libclaw" "UninstallString" '"$INSTDIR\uninst-libclaw.exe"'
  WriteUninstaller "uninst-libclaw.exe"
SectionEnd

UninstallText "Ready to uninstall Claw."

;--------------------------------
Section "Uninstall"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libclaw"
  Delete "$INSTDIR\uninst-libclaw.exe"
  RMDir /r "$INSTDIR\include"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\share"
  RMDir /r "$INSTDIR\win"

SectionEnd
