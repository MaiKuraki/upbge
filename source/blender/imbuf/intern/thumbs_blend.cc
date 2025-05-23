/* SPDX-FileCopyrightText: 2024 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup imbuf
 */

#include <cstdlib>
#include <cstring>

#include "BLO_readfile.hh"

#include "BKE_idtype.hh"
#include "BKE_main.hh"
#include "BKE_preview_image.hh"

#include "IMB_imbuf_types.hh"
#include "IMB_thumbs.hh"

#include "MEM_guardedalloc.h"

/* NOTE: we should handle all previews for a same group at once, would avoid reopening
 * `.blend` file for each and every ID. However, this adds some complexity,
 * so keep it for later. */
static ImBuf *imb_thumb_load_from_blend_id(const char *blen_path,
                                           const char *blen_group,
                                           const char *blen_id)
{
  ImBuf *ima = nullptr;
  BlendFileReadReport bf_reports = {};
  bf_reports.reports = nullptr;

  BlendHandle *libfiledata = BLO_blendhandle_from_file(blen_path, &bf_reports);
  if (libfiledata == nullptr) {
    return nullptr;
  }

  int idcode = BKE_idtype_idcode_from_name(blen_group);
  PreviewImage *preview = BLO_blendhandle_get_preview_for_id(libfiledata, idcode, blen_id);
  BLO_blendhandle_close(libfiledata);

  if (preview) {
    ima = BKE_previewimg_to_imbuf(preview, ICON_SIZE_PREVIEW);
    BKE_previewimg_freefunc(preview);
  }
  return ima;
}

static ImBuf *imb_thumb_load_from_blendfile(const char *blen_path)
{
  BlendThumbnail *data = BLO_thumbnail_from_file(blen_path);
  ImBuf *ima = BKE_main_thumbnail_to_imbuf(nullptr, data);

  if (data) {
    MEM_freeN(data);
  }
  return ima;
}

ImBuf *IMB_thumb_load_blend(const char *blen_path, const char *blen_group, const char *blen_id)
{
  if (blen_group && blen_id) {
    return imb_thumb_load_from_blend_id(blen_path, blen_group, blen_id);
  }
  return imb_thumb_load_from_blendfile(blen_path);
}
