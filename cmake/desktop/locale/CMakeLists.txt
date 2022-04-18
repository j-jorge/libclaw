find_package(Gettext REQUIRED)

function(claw_create_translations)

  cmake_parse_arguments(ARG
    ""
    "TARGET;TEMPLATE"
    "FILES"
    ${ARGN}
    )

  get_filename_component(template_basename ${ARG_TEMPLATE} NAME_WE)

  foreach(po_file ${ARG_FILES})
    get_filename_component(lang ${po_file} NAME_WE)
    set(mo_file ${CMAKE_CURRENT_BINARY_DIR}/${lang}.mo)

    add_custom_command(
      OUTPUT ${mo_file}
      COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE}
      --quiet --update --backup=none -s ${po_file} ${ARG_TEMPLATE}
      COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${mo_file} ${po_file}
      DEPENDS ${ARG_TEMPLATE} ${po_file}
      )

    install(
      FILES ${mo_file}
      DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${lang}/LC_MESSAGES
      RENAME ${template_basename}.mo
      )

    set(all_mo_files ${all_mo_files} ${mo_file})
  endforeach()

  add_custom_target(${ARG_TARGET} ALL DEPENDS ${all_mo_files})
endfunction()

set(locale_root ${repository_root}/desktop/locale/libclaw)
claw_create_translations(
  TARGET claw-translations
  TEMPLATE "${locale_root}/libclaw.pot"
  FILES "${locale_root}/fr.po"
  )
