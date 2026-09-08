# NirvanaEdit

Lightweight native Linux video editor dengan motion graphics, layer workflow,
keyframe, effects, masks, dan dukungan hardware modest.

**Status**: Phase 0 — Foundation (Reboot Baseline)

## Technology Stack

- C++17
- GTK3 (UI)
- CMake (build)
- GStreamer (preview, Phase 4+)
- FFmpeg (export, Phase 12+)
- OpenGL (rendering, Phase 4+)

## Build Requirements

```bash
# Debian / Ubuntu
sudo apt install build-essential cmake pkg-config libgtk-3-dev
