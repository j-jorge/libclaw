set(example_root "${repository_root}/example")
set(examples
  application
  automaton
  avl
  compress
  config_file
  curve
  dynamic_library
  game_ai
  graph
  image
  iterator
  kmp
  memory
  multi_type_map
  ordered_set
  singleton
  sockets
  string_algorithm
  system
  trie
  tween
  )

foreach(example ${examples})
  set(archive_file_name ${example}.tar.gz)
  set(archive_file_path ${CMAKE_CURRENT_BINARY_DIR}/${archive_file_name})
  set(archives ${archives} ${archive_file_name})

  file(GLOB RECURSIVE dependencies ${example_root}/*)
  add_custom_target(${archive_file_name}
    COMMAND ${CMAKE_COMMAND} -E tar cfz ${archive_file_path} ${example}
    DEPENDS ${dependencies}
    WORKING_DIRECTORY ${example_root}
    )
  install(
    FILES ${archive_file_path}
    DESTINATION ${claw_install_documentation_dir}/example/
    )
endforeach()

add_custom_target(example ALL DEPENDS ${archives})
