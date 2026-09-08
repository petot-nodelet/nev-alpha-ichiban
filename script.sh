#!/bin/bash

# Path utama
BASE_DIR="src/core/include/nvcore"

# 1. Buat struktur folder
mkdir -p "$BASE_DIR/project"
mkdir -p "$BASE_DIR/serialization"

# 2. Buat file di dalam direktori nvcore/ (jika belum ada)
touch "$BASE_DIR/types.h"
touch "$BASE_DIR/result.h"
touch "$BASE_DIR/version.h"

# 3. Buat file di dalam direktori project/
touch "$BASE_DIR/project/project.h"
touch "$BASE_DIR/project/project_info.h"
touch "$BASE_DIR/project/project_settings.h"
touch "$BASE_DIR/project/asset.h"
touch "$BASE_DIR/project/asset_metadata.h"
touch "$BASE_DIR/project/composition.h"
touch "$BASE_DIR/project/timeline.h"
touch "$BASE_DIR/project/track.h"
touch "$BASE_DIR/project/clip.h"

# 4. Buat file di dalam direktori serialization/
touch "$BASE_DIR/serialization/project_serializer.h"
touch "$BASE_DIR/serialization/project_deserializer.h"
touch "$BASE_DIR/serialization/schema_version.h"

echo "Struktur direktori dan file berhasil dibuat!"
