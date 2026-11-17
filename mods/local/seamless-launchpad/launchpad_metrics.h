#pragma once
#include <windows.h>
#include <dxgi.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <numeric>
#include <string>
#include <vector>

struct LaunchpadMetrics {
    bool active = false;
    std::string phase;
    double start = 0, last = 0, visibleMs = -1, hz = 60;
    int width = 0, height = 0;
    float scale = 1;
    bool havePresentation = false, presentationInvalid = false, presentationReliable = true;
    DXGI_FRAME_STATISTICS firstPresentation{}, lastPresentation{};
    std::vector<double> intervals, renders;
    void Begin(const char* name, double now, int w, int h, float dpiScale, double refresh) {
        phase = name; start = now; last = 0; visibleMs = -1;
        width = w; height = h; scale = dpiScale; hz = refresh;
        intervals.clear(); renders.clear();
        havePresentation=false; presentationInvalid=false;
        presentationReliable=GetSystemMetrics(SM_CMONITORS)==1;
        intervals.reserve(512); renders.reserve(512);
        active = true;
    }
    void Frame(double now) {
        if (!active) return;
        if (last && intervals.size() < 8192) intervals.push_back((now - last) * 1000);
        last = now;
    }
    void Rendered(double ms) { if (active && renders.size() < 8192) renders.push_back(ms); }
    void Presented(const DXGI_FRAME_STATISTICS& statistics) {
        if (!active || !presentationReliable || statistics.SyncQPCTime.QuadPart<=0) return;
        if (!havePresentation) { firstPresentation=statistics; havePresentation=true; }
        else if (statistics.PresentCount<lastPresentation.PresentCount || statistics.SyncQPCTime.QuadPart<lastPresentation.SyncQPCTime.QuadPart) presentationInvalid=true;
        lastPresentation=statistics;
    }
    std::wstring Finish(const std::wstring& path, double now, bool gpu) {
        if (!active) return {};
        active = false;
        auto mean = [](const auto& v) { return v.empty() ? 0.0 : std::accumulate(v.begin(), v.end(), 0.0) / v.size(); };
        auto p95 = [](auto v) {
            if (v.empty()) return 0.0;
            std::sort(v.begin(), v.end());
            return v[(size_t)std::ceil(v.size() * 0.95) - 1];
        };
        double intervalMean = mean(intervals), renderMean = mean(renders), variance = 0;
        size_t late = 0;
        for (double ms : intervals) { variance += (ms - intervalMean) * (ms - intervalMean); late += ms > 1500 / hz; }
        double jitter = intervals.empty() ? 0 : std::sqrt(variance / intervals.size());
        std::string presented="unavailable";
        if (gpu && havePresentation && !presentationInvalid && lastPresentation.PresentCount>firstPresentation.PresentCount &&
            lastPresentation.SyncQPCTime.QuadPart>firstPresentation.SyncQPCTime.QuadPart) {
            LARGE_INTEGER frequency; QueryPerformanceFrequency(&frequency);
            double fps=(lastPresentation.PresentCount-firstPresentation.PresentCount)*(double)frequency.QuadPart/
                (lastPresentation.SyncQPCTime.QuadPart-firstPresentation.SyncQPCTime.QuadPart);
            char number[64]; std::snprintf(number,sizeof(number),"%.3f",fps); presented=number;
        }
        double duration=(now-start)*1000;
        double animationMs=phase=="open"&&visibleMs>=0?std::max(0.0,duration-visibleMs):duration;
        char line[1024];
        std::snprintf(line, sizeof(line), "%s,%d,%d,%.3f,%.3f,%s,%.3f,%.3f,%zu,%.3f,%.3f,%.3f,%.3f,%.3f,%zu,%s,%.3f\r\n",
            phase.c_str(), width, height, scale, hz, gpu ? "gpu-composition" : "layered-readback", visibleMs,
            (now - start) * 1000, renders.size(), renderMean, p95(renders), intervalMean, jitter,
            intervalMean > 0 ? 1000 / intervalMean : 0, late, presented.c_str(), animationMs);
        // Bound log size. Rotation retains the previous capture for comparison.
        WIN32_FILE_ATTRIBUTE_DATA attributes{};
        if (GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &attributes) &&
            (attributes.nFileSizeHigh || attributes.nFileSizeLow > 4 * 1024 * 1024))
            MoveFileExW(path.c_str(), (path + L".previous.csv").c_str(), MOVEFILE_REPLACE_EXISTING);
        HANDLE file = CreateFileW(path.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file != INVALID_HANDLE_VALUE) {
            DWORD written;
            if (GetFileSize(file, nullptr) == 0) {
                constexpr char header[] = "phase,width,height,scale,target_hz,presenter,first_visible_ms,duration_ms,render_count,render_mean_ms,render_p95_ms,interval_mean_ms,interval_stddev_ms,submitted_fps,late_intervals,dxgi_presented_fps,animation_ms\r\n";
                WriteFile(file, header, sizeof(header) - 1, &written, nullptr);
            }
            WriteFile(file, line, (DWORD)strlen(line), &written, nullptr);
            CloseHandle(file);
        }
        std::string message = "performance: "; message += line;
        return std::wstring(message.begin(), message.end());
    }
};
