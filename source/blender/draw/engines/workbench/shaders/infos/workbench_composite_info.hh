/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#ifdef GPU_SHADER
#  pragma once
#  include "BLI_utildefines_variadic.h"

#  include "gpu_glsl_cpp_stubs.hh"

#  include "workbench_shader_shared.hh"

#  include "draw_view_info.hh"
#  include "gpu_shader_fullscreen_info.hh"

#  define WORKBENCH_LIGHTING_STUDIO
#  define WORKBENCH_LIGHTING_MATCAP
#  define WORKBENCH_LIGHTING_FLAT
#  define WORKBENCH_CURVATURE
#  define WORKBENCH_CAVITY
#  define WORKBENCH_SHADOW
#endif

#include "gpu_shader_create_info.hh"
#include "workbench_defines.hh"

/* -------------------------------------------------------------------- */
/** \name Base Composite
 * \{ */

GPU_SHADER_CREATE_INFO(workbench_composite)
SAMPLER(3, sampler2DDepth, depth_tx)
SAMPLER(4, sampler2D, normal_tx)
SAMPLER(5, sampler2D, material_tx)
UNIFORM_BUF(WB_WORLD_SLOT, WorldData, world_data)
TYPEDEF_SOURCE("workbench_shader_shared.hh")
PUSH_CONSTANT(bool, force_shadowing)
FRAGMENT_OUT(0, float4, frag_color)
FRAGMENT_SOURCE("workbench_composite_frag.glsl")
ADDITIONAL_INFO(gpu_fullscreen)
ADDITIONAL_INFO(draw_view)
GPU_SHADER_CREATE_END()

/* Lighting */

GPU_SHADER_CREATE_INFO(workbench_resolve_opaque_studio)
DEFINE("WORKBENCH_LIGHTING_STUDIO")
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(workbench_resolve_opaque_matcap)
DEFINE("WORKBENCH_LIGHTING_MATCAP")
SAMPLER(WB_MATCAP_SLOT, sampler2DArray, matcap_tx)
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(workbench_resolve_opaque_flat)
DEFINE("WORKBENCH_LIGHTING_FLAT")
GPU_SHADER_CREATE_END()

/* Effects */

GPU_SHADER_CREATE_INFO(workbench_resolve_curvature)
DEFINE("WORKBENCH_CURVATURE")
SAMPLER(6, usampler2D, object_id_tx)
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(workbench_resolve_cavity)
DEFINE("WORKBENCH_CAVITY")
/* TODO(@pragma37): GPU_SAMPLER_EXTEND_MODE_REPEAT is set in CavityEffect,
 * it doesn't work here? */
SAMPLER(7, sampler2D, jitter_tx)
UNIFORM_BUF(5, float4, cavity_samples[512])
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(workbench_resolve_shadow)
DEFINE("WORKBENCH_SHADOW")
SAMPLER(8, usampler2D, stencil_tx)
GPU_SHADER_CREATE_END()

/* Variations */

#define WORKBENCH_RESOLVE_SHADOW_VARIATION(prefix, ...) \
  CREATE_INFO_VARIANT(prefix##_shadow, workbench_resolve_shadow, __VA_ARGS__) \
  CREATE_INFO_VARIANT(prefix##_no_shadow, __VA_ARGS__)

#define WORKBENCH_CURVATURE_VARIATIONS(prefix, ...) \
  WORKBENCH_RESOLVE_SHADOW_VARIATION( \
      prefix##_curvature, workbench_resolve_curvature, __VA_ARGS__) \
  WORKBENCH_RESOLVE_SHADOW_VARIATION(prefix##_no_curvature, __VA_ARGS__)

#define WORKBENCH_CAVITY_VARIATIONS(prefix, ...) \
  WORKBENCH_CURVATURE_VARIATIONS(prefix##_cavity, workbench_resolve_cavity, __VA_ARGS__) \
  WORKBENCH_CURVATURE_VARIATIONS(prefix##_no_cavity, __VA_ARGS__)

#define WORKBENCH_LIGHTING_VARIATIONS(prefix, ...) \
  WORKBENCH_CAVITY_VARIATIONS( \
      prefix##_opaque_studio, workbench_resolve_opaque_studio, __VA_ARGS__) \
  WORKBENCH_CAVITY_VARIATIONS( \
      prefix##_opaque_matcap, workbench_resolve_opaque_matcap, __VA_ARGS__) \
  WORKBENCH_CAVITY_VARIATIONS(prefix##_opaque_flat, workbench_resolve_opaque_flat, __VA_ARGS__)

WORKBENCH_LIGHTING_VARIATIONS(workbench_resolve, workbench_composite)

#undef WORKBENCH_FINAL_VARIATION
#undef WORKBENCH_CURVATURE_VARIATIONS
#undef WORKBENCH_CAVITY_VARIATIONS
#undef WORKBENCH_LIGHTING_VARIATIONS

/** \} */
