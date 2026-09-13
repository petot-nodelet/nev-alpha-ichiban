#include <vector>
#include <algorithm>
#include "media_probe.h"
#include <filesystem>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

namespace nv::infrastructure {

nv::Result<nv::core::AssetMetadata> MediaProbe::probe(const std::string& filePath) {
    // 1. Check file exists
    if (!std::filesystem::exists(filePath)) {
        return nv::Result<nv::core::AssetMetadata>::Err(
            nv::ErrorCode::NotFound, "File not found: " + filePath);
    }

    nv::core::AssetMetadata meta;
    meta.probe_status = "pending";
    meta.file_size_bytes = static_cast<int64_t>(std::filesystem::file_size(filePath));

    // 2. Open format context
    AVFormatContext* fmtCtx = nullptr;
    int ret = avformat_open_input(&fmtCtx, filePath.c_str(), nullptr, nullptr);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        meta.probe_status = "failed";
        meta.error_message = std::string("avformat_open_input failed: ") + errbuf;
        return nv::Result<nv::core::AssetMetadata>::Err(
            nv::ErrorCode::MediaError, meta.error_message);
    }

    // 3. Find stream info
    ret = avformat_find_stream_info(fmtCtx, nullptr);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        avformat_close_input(&fmtCtx);
        meta.probe_status = "failed";
        meta.error_message = std::string("avformat_find_stream_info failed: ") + errbuf;
        return nv::Result<nv::core::AssetMetadata>::Err(
            nv::ErrorCode::MediaError, meta.error_message);
    }

    // 4. Extract format name
    if (fmtCtx->iformat && fmtCtx->iformat->name) {
        meta.format = fmtCtx->iformat->name;
    }

    // 5. Extract duration
    if (fmtCtx->duration != AV_NOPTS_VALUE) {
        // duration is in AV_TIME_BASE units (microseconds), convert to ms
        meta.duration_ms = fmtCtx->duration / (AV_TIME_BASE / 1000);
    }

    // 6. Iterate streams
    for (unsigned int i = 0; i < fmtCtx->nb_streams; i++) {
        AVStream* stream = fmtCtx->streams[i];
        AVCodecParameters* codecpar = stream->codecpar;

        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            meta.width = codecpar->width;
            meta.height = codecpar->height;

            // FPS calculation
            if (stream->avg_frame_rate.den != 0) {
                meta.fps_numerator = stream->avg_frame_rate.num;
                meta.fps_denominator = stream->avg_frame_rate.den;
            } else if (stream->r_frame_rate.den != 0) {
                meta.fps_numerator = stream->r_frame_rate.num;
                meta.fps_denominator = stream->r_frame_rate.den;
            }

            // Codec name
            const AVCodecDescriptor* desc = avcodec_descriptor_get(codecpar->codec_id);
            if (desc) {
                meta.codec = desc->name;
            }

            meta.has_video = true;
        } else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            meta.has_audio = true;
            // If no video codec found, use audio codec name
            if (meta.codec.empty()) {
                const AVCodecDescriptor* desc = avcodec_descriptor_get(codecpar->codec_id);
                if (desc) {
                    meta.codec = desc->name;
                }
            }
        }
    }

    // 7. Cleanup
    avformat_close_input(&fmtCtx);

    meta.probe_status = "ok";
    return nv::Result<nv::core::AssetMetadata>::Ok(std::move(meta));
}

bool MediaProbe::isSupportedFormat(const std::string& filePath) {
    std::string ext = getFileExtension(filePath);
    // Common supported extensions
    static const std::vector<std::string> supported = {
        ".mp4", ".mov", ".avi", ".mkv", ".webm", ".flv", ".wmv",
        ".mp3", ".wav", ".aac", ".ogg", ".flac", ".m4a",
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp"
    };
    for (const auto& s : supported) {
        if (ext == s) return true;
    }
    return false;
}

std::string MediaProbe::getFileExtension(const std::string& filePath) {
    std::filesystem::path p(filePath);
    return p.extension().string();
}

} // namespace nv::infrastructure
