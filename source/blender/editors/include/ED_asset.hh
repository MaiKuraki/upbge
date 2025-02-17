/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup editors
 *
 * The public API for assets is defined in dedicated headers. This is a utility file that just
 * includes all of these.
 */

#pragma once

/* Barely anything here. Just general editor level functions. Actual asset level code is in
 * dedicated headers. */

#include "../asset/ED_asset_catalog.hh"           // IWYU pragma: export
#include "../asset/ED_asset_handle.hh"            // IWYU pragma: export
#include "../asset/ED_asset_library.hh"           // IWYU pragma: export
#include "../asset/ED_asset_list.hh"              // IWYU pragma: export
#include "../asset/ED_asset_mark_clear.hh"        // IWYU pragma: export
#include "../asset/ED_asset_temp_id_consumer.hh"  // IWYU pragma: export
#include "../asset/ED_asset_type.hh"              // IWYU pragma: export

#include "../asset/ED_asset_catalog.hh"  // IWYU pragma: export
#include "../asset/ED_asset_filter.hh"   // IWYU pragma: export
#include "../asset/ED_asset_import.hh"   // IWYU pragma: export
#include "../asset/ED_asset_list.hh"     // IWYU pragma: export

namespace blender::ed::asset {

std::string asset_tooltip(const asset_system::AssetRepresentation &asset,
                          bool include_name = true);

void operatortypes_asset();

}  // namespace blender::ed::asset
