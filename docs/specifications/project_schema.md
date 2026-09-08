# NirvanaEdit Project Schema Specification

**Version:** 1.0.0  
**Status:** Approved (Phase 1 Baseline)  
**Format:** ZIP-based Container (`.nvp`)

## 1. Container Structure (.nvp)

File project NirvanaEdit (`.nvp`) adalah arsip ZIP yang berisi struktur berikut:

```text
project.nvp (ZIP Archive)
├── project.json          # SOURCE OF TRUTH: Seluruh state project
── metadata.db           # CACHE/INDEX: SQLite untuk lookup cepat (dapat di-rebuild)
└── assets/               # PROJECT-GENERATED DATA:
    ├── thumbnails/       # Cached thumbnails (JPEG/WebP)
    └── waveforms/        # Audio waveform cache (binary/raw)
