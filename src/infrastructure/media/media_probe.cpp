#include "media_probe.h"
#include <filesystem>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

namespace nv::infrastructure {

nv::Result<nv::core::AssetMetadata> MediaProbe::probe(const std::string& filePath) {
    // Check if file exists first
    if (!std::filesystem::exists(filePath)) {
        return nv::Result<nv::core::AssetMetadata>::Err(
            nv::ErrorCode::NotFound,
            "File not found: " + filePath
        );
    }
    
    AVFormatContext* fmtCtx = nullptr;
    int ret = avformat_open_input(&fmtCtx, filePath.c_str(), nullptr, nullptr);
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, sizeof(errbuf));
        return nv::Result<nv::core::AssetMetadata>::Err(
            nv::ErrorCode::MediaError,
            "Failed to open input: " + std::string(errbuf)
        );
    }
    
    ret = avformat_find_stream_info(fmtCtx, nullptr);
    if (ret < 0) {
        avformat_close_input(&fmtCtx);
        return nv::Result<nv::core::AssetMetadata>::Err(
            nv::ErrorCode::MediaError,
            "Failed to find stream info"
        );
    }
    
    nv::core::AssetMetadata meta;
    meta.probe_status = "ok";
    meta.file_size_bytes = static_cast<int64_t>(std::filesystem::file_size(filePath));
    
    // Duration in milliseconds
    if (fmtCtx->duration != AV_NOPTS_VALUE) {
        meta.duration_ms = fmtCtx->duration / (AV_TIME_BASE / 1000);
    }
    
    // Format name
    if (fmtCtx->iformat && fmtCtx->iformat->name) {
        meta.format = fmtCtx->iformat->name;
    }
    
    // Iterate streams
    for (unsigned int i = 0; i < fmtCtx->nb_streams; i++) {
        AVStream* stream = fmtCtx->streams[i];
        AVCodecParameters* codecpar = stream->codecpar;
        
        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            meta.width = codecpar->width;
            meta.height = codecpar->height;
            
            // Calculate FPS
            if (stream->avg_frame_rate.den != 0) {
                meta.fps_numerator = stream->avg_frame_rate.num;
                meta.fps_denominator = stream->avg_frame_rate.den;
            } else if (stream->r_frame_rate.den != 0) {
                meta.fps_numerator = stream->r_frame_rate.num;
                meta.fps_denominator = stream->r_frame_rate.den;
            }
            
            const AVCodecDescriptor* desc = avcodec_descriptor_get(codecpar->codec_id);
            if (desc) {
                meta.codec = desc->name;
            }
            
            meta.has_video = true;
        } else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            meta.has_audio = true;
            if (meta.codec.empty()) {
                const AVCodecDescriptor* desc = avcodec_descriptor_get(codecpar->codec_id);
                if (desc) {
                    meta.codec = desc->name;
                }
            }
        }
    }
    
    avformat_close_input(&fmtCtx);
    return nv::Result<nv::core::AssetMetadata>::Ok(std::move(meta));
}

bool MediaProbe::isSupportedFormat(const std::string& filePath) {
    std::string ext = getFileExtension(filePath);
    // Basic check, FFmpeg will do the real check
    return !ext.empty();
}

std::string MediaProbe::getFileExtension(const std::string& filePath) {
    std::filesystem::path p(filePath);
    return p.extension().string();
}

} // namespace nv::infrastructure
