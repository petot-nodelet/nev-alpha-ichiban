🎬 NirvanaEdit

**Lightweight native Linux video editor** dengan motion graphics, layer workflow, keyframe, effects, masks, dan dukungan hardware modest.

> Dibangun dengan C++17, GTK3, FFmpeg, dan GStreamer. Tanpa Electron, tanpa Chromium, tanpa cloud.

---

## ✨ Visi

NirvanaEdit adalah editor video native Linux yang terinspirasi dari **Alight Motion** (workflow mobile-friendly) dengan pengalaman desktop ala **Canva** (drag-and-drop, intuitif). Dirancang untuk:

- ️ **PC lama & modest hardware** (Debian & turunannya)
- 🚀 **Ringan & cepat** (native C++, hemat RAM)
-  **Offline-first** (tidak butuh internet)
- 🧩 **Mudah dikembangkan** (arsitektur modular)

---

##  Status

| Phase | Nama | Status |
|-------|------|--------|
| **Phase 0** | Foundation & Build System | ✅ Complete |
| **Phase 1** | Core Domain & Serialization (.nvp) | ✅ **Complete** |
| **Phase 2** | Media Subsystem (Import, Probe, Thumbnails) |  In Progress |
| **Phase 3** | Timeline UI & Canvas | ⏳ Planned |
| **Phase 4** | Playback & Preview (GStreamer/OpenGL) | ⏳ Planned |
| **Phase 5–12** | Effects, Keyframes, Export, Packaging | ⏳ Planned |

---

## 🛠️ Technology Stack

| Komponen | Teknologi | Keterangan |
|----------|-----------|------------|
| **Bahasa** | C++17 | Native, performa tinggi |
| **UI** | GTK3 | Native Linux, ringan |
| **Build** | CMake | Cross-platform build system |
| **Serialization** | nlohmann/json + libzip | Format `.nvp` (ZIP + JSON) |
| **Media Probe** | FFmpeg (libavformat, libavcodec) | Metadata extraction |
| **Playback** | GStreamer | Preview & pipeline (Phase 4+) |
| **Rendering** | OpenGL | GPU-accelerated canvas (Phase 4+) |
| **Database** | SQLite | Metadata cache & index |

### ❌ Yang TIDAK Kami Gunakan

- Electron / Chromium
- Cloud services
- Framework berat (Qt, wxWidgets)
- Runtime interpreter

---

## 📦 Build Requirements

### Debian / Ubuntu

```bash
sudo apt install build-essential cmake pkg-config \
    libgtk-3-dev libzip-dev nlohmann-json3-dev \
    libavformat-dev libavcodec-dev libavutil-dev libswscale-dev
