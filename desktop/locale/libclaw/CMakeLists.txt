# replace the GETTEXT_CREATE_TRANSLATIONS macro to allow to specify the target
# name. So we can have translations in several subdirectories.
macro(GETTEXT_CREATE_TRANSLATIONS_CUSTOM _targetName _potFile _firstPoFile)

   set(_gmoFiles)
   get_filename_component(_potBasename ${_potFile} NAME_WE)
   get_filename_component(_absPotFile ${_potFile} ABSOLUTE)

   FOREACH (_currentPoFile ${_firstPoFile} ${ARGN})
      get_filename_component(_absFile ${_currentPoFile} ABSOLUTE)
      get_filename_component(_abs_PATH ${_absFile} PATH)
      get_filename_component(_lang ${_absFile} NAME_WE)
      set(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)

      add_custom_command( 
         OUTPUT ${_gmoFile} 
         COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE}
           --quiet --update --backup=none -s ${_absFile} ${_absPotFile}
         COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_absFile}
         DEPENDS ${_absPotFile} ${_absFile} 
      )

      install(
        FILES ${_gmoFile}
        DESTINATION share/locale/${_lang}/LC_MESSAGES
        RENAME ${_potBasename}.mo
        )
      
      set(_gmoFiles ${_gmoFiles} ${_gmoFile})
   ENDFOREACH (_currentPoFile )

   add_custom_target(${_targetName} ALL DEPENDS ${_gmoFiles})

endmacro(GETTEXT_CREATE_TRANSLATIONS_CUSTOM)

gettext_create_translations_custom(
  libclaw-translations
  libclaw.pot
  fr.po
  )
