set(module_root "${lib_root}/tween")
set(source_root "${module_root}/src/claw/tween")

claw_library(
  TARGET claw_tween
  MODULE_ROOT ${module_root}
  SOURCES
    "${source_root}/base_tweener.cpp"
    "${source_root}/single_tweener.cpp"
    "${source_root}/tweener.cpp"
    "${source_root}/tweener_group.cpp"
    "${source_root}/tweener_sequence.cpp"
    "${source_root}/easing/easing_circ.cpp"
    "${source_root}/easing/easing_none.cpp"
    "${source_root}/easing/easing_linear.cpp"
    "${source_root}/easing/easing_elastic.cpp"
    "${source_root}/easing/easing_quint.cpp"
    "${source_root}/easing/easing_cubic.cpp"
    "${source_root}/easing/easing_back.cpp"
    "${source_root}/easing/easing_quad.cpp"
    "${source_root}/easing/easing_sine.cpp"
    "${source_root}/easing/easing_quart.cpp"
    "${source_root}/easing/easing_bounce.cpp"
    "${source_root}/easing/easing_expo.cpp"
  LINK claw_core
  )
