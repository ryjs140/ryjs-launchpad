#pragma once
// Background material engine: parameters, presets, colour maths and the CPU
// mirror of the GPU chain that the settings preview uses.
//
// The GPU chain (In the mod source: BlurToBitmap, ApplyGrade, DrawMaterial) and
// this CPU mirror share the same order and the same numbers:
//
//   backdrop -> gaussian blur -> saturation/brightness/contrast -> luminosity
//   wash -> tint wash -> noise -> dim
//
// The preview works on a miniature of a 960 DIP wide screen, so every length
// (blur sigma, grain size) is scaled by the same factor as the drawing.

#include <windows.h>
#include <d2d1.h>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

namespace launchpad_material {

// -----------------------------------------------------------------------------
// Backends

enum class Backend { Custom, Acrylic, AcrylicSystem, Blur, Mica, MicaAlt, Transparent, Solid };

// The stored keys are the same strings the settings page uses. "none" is the
// legacy value from before the engine; it draws exactly what Transparent draws.
inline Backend BackendFromKey(const std::wstring& key) {
    if (key == L"acrylic") return Backend::Acrylic;
    if (key == L"acrylicSystem") return Backend::AcrylicSystem;
    if (key == L"blur") return Backend::Blur;
    if (key == L"mica") return Backend::Mica;
    if (key == L"micaAlt") return Backend::MicaAlt;
    if (key == L"solid") return Backend::Solid;
    if (key == L"transparent" || key == L"none") return Backend::Transparent;
    return Backend::Custom;
}

// Custom is the only backend that samples the screen and runs the effect chain.
inline bool UsesSnapshot(Backend b) { return b == Backend::Custom; }
// Native backends are drawn by DWM on the window behind the grid.
inline bool UsesDwmBackdrop(Backend b) {
    return b == Backend::Acrylic || b == Backend::Blur || b == Backend::AcrylicSystem ||
           b == Backend::Mica || b == Backend::MicaAlt;
}
// The Windows 11 system backdrops, which need DWMWA_SYSTEMBACKDROP_TYPE.
inline bool UsesSystemBackdrop(Backend b) {
    return b == Backend::AcrylicSystem || b == Backend::Mica || b == Backend::MicaAlt;
}

// What the backend can actually control. The settings page hides everything a
// backend cannot honour instead of showing a slider with no effect.
struct Capabilities {
    bool blur = false, grade = false, tint = false, tintOpacity = false, luminosity = false, noise = false, dim = true;
};

inline Capabilities CapabilitiesOf(Backend b) {
    Capabilities c;
    c.tint = true;
    switch (b) {
        case Backend::Custom:
            c.blur = c.grade = c.tintOpacity = c.luminosity = c.noise = true;
            break;
        case Backend::Solid:
            c.tintOpacity = false;  // A solid colour is the tint at full strength.
            break;
        default:
            c.tintOpacity = true;  // The tint layer is ours, even over a native material.
            break;
    }
    return c;
}

// -----------------------------------------------------------------------------
// Parameters

struct Params {
    float blurSigma = 36;     // DIP, 0..120; matches the old blurAmount 60.
    float saturation = 1.0f;  // 0..2
    float brightness = 1.0f;  // 0.4..1.6
    float contrast = 1.0f;    // 0.5..1.5
    float tintOpacity = 0.30f;  // 0..1, over the graded backdrop
    float luminosity = 0.0f;    // 0..1, adaptive haze
    float noise = 0.0f;         // 0..0.15
    float noiseScale = 1.0f;    // 0.5..2, grain size in DIP per texel
    float dim = 0.0f;           // 0..1, black readability layer
    D2D1_COLOR_F tint{0, 0, 0, 1};
};

// clang-format off
inline void Clamp(Params& p) {
    p.blurSigma = std::clamp(p.blurSigma, 0.0f, 120.0f);
    p.saturation = std::clamp(p.saturation, 0.0f, 2.0f);
    p.brightness = std::clamp(p.brightness, 0.4f, 1.6f);
    p.contrast = std::clamp(p.contrast, 0.5f, 1.5f);
    p.tintOpacity = std::clamp(p.tintOpacity, 0.0f, 1.0f);
    p.luminosity = std::clamp(p.luminosity, 0.0f, 1.0f);
    p.noise = std::clamp(p.noise, 0.0f, 0.15f);
    p.noiseScale = std::clamp(p.noiseScale, 0.5f, 2.0f);
    p.dim = std::clamp(p.dim, 0.0f, 1.0f);
    p.tint.r = std::clamp(p.tint.r, 0.0f, 1.0f);
    p.tint.g = std::clamp(p.tint.g, 0.0f, 1.0f);
    p.tint.b = std::clamp(p.tint.b, 0.0f, 1.0f);
    p.tint.a = 1;
}
// clang-format on

inline bool GradeIsNeutral(const Params& p) {
    return std::abs(p.saturation - 1) < 0.001f && std::abs(p.brightness - 1) < 0.001f &&
           std::abs(p.contrast - 1) < 0.001f;
}

inline bool NeedsGrade(const Params& a, const Params& b) {
    return std::abs(a.saturation - b.saturation) > 0.001f || std::abs(a.brightness - b.brightness) > 0.001f ||
           std::abs(a.contrast - b.contrast) > 0.001f;
}

// -----------------------------------------------------------------------------
// Colour matrix
//
// D2D1_MATRIX_5X4_F: row = input channel (R,G,B,A,1), column = output channel.
// Output = input * M. The colour matrix effect adds row 4 to every output.
struct Matrix5x4 {
    float m[5][4];
};

inline Matrix5x4 GradeMatrix(const Params& p) {
    Matrix5x4 out{};
    // Rec. 709 luminance, the same weights D2D1Saturation uses.
    const float lr = 0.2126f, lg = 0.7152f, lb = 0.0722f;
    const float s = p.saturation, k = p.brightness, c = p.contrast;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float luma = j == 0 ? lr : j == 1 ? lg : lb;
            float sat = (i == j ? s : 0.0f) + luma * (1.0f - s);
            out.m[i][j] = sat * k * c;
        }
        out.m[i][3] = 0;
        out.m[4][i] = 0.5f * (1.0f - c);  // Contrast pivots on mid grey, then brightness scales it.
    }
    out.m[3][0] = out.m[3][1] = out.m[3][2] = 0;
    out.m[3][3] = 1;  // The backdrop is opaque and stays opaque.
    out.m[4][3] = 0;
    return out;
}

inline void ApplyMatrix(const Matrix5x4& m, float& r, float& g, float& b) {
    float nr = r * m.m[0][0] + g * m.m[1][0] + b * m.m[2][0] + m.m[4][0];
    float ng = r * m.m[0][1] + g * m.m[1][1] + b * m.m[2][1] + m.m[4][1];
    float nb = r * m.m[0][2] + g * m.m[1][2] + b * m.m[2][2] + m.m[4][2];
    r = std::clamp(nr, 0.0f, 1.0f);
    g = std::clamp(ng, 0.0f, 1.0f);
    b = std::clamp(nb, 0.0f, 1.0f);
}

inline float Luminance(float r, float g, float b) { return 0.2126f * r + 0.7152f * g + 0.0722f * b; }

// The acrylic luminosity layer: a solid white or black wash over the backdrop.
// Dark backdrops get white haze, light backdrops get black haze, so the layer
// always works against the backdrop instead of flattening it.
inline D2D1_COLOR_F LuminosityColor(float meanLuminance) {
    return meanLuminance < 0.5f ? D2D1::ColorF(1, 1, 1, 1) : D2D1::ColorF(0, 0, 0, 1);
}

// -----------------------------------------------------------------------------
// Presets
//
// Numbers match the settings page units so a preset can be applied to the
// controls without rounding drift: blur in DIP, colour in percent, opacity and
// dim in percent, noise in tenths of a percent (0..15).

struct Preset {
    const wchar_t* id;    // Stable id, also the name of the translation entry.
    int blur;             // DIP
    int saturation;       // %
    int brightness;       // %
    int contrast;         // %
    int tintOpacity;      // %
    int noise;            // % (0..15)
    int dim;              // %
    int luminosity;       // %
    uint32_t tint;        // 0xRRGGBB
};

// Initial tuning values. Presets describe texture only: blur, transparency,
// brightness/contrast, haze, grain and dim. They deliberately do not carry a
// hue, so picking a preset never repaints the panel. Saturation stays mild; a
// preset should not turn the wallpaper's colours up to eleven either.
inline const std::vector<Preset>& Presets() {
    static const std::vector<Preset> presets = {
        {L"Default", 36, 100, 100, 100, 30, 0, 0, 0, 0x000000},
        {L"Clear Glass", 5, 100, 100, 100, 3, 0, 0, 0, 0x000000},
        {L"Light Glass", 12, 105, 106, 100, 10, 0, 0, 0, 0x000000},
        {L"Soft Frost", 55, 85, 105, 92, 25, 1, 0, 0, 0x000000},
        {L"Light Acrylic", 40, 105, 108, 98, 35, 3, 0, 0, 0x000000},
        {L"Dark Acrylic", 40, 100, 82, 100, 50, 3, 0, 0, 0x000000},
        {L"macOS Style", 75, 110, 95, 105, 18, 0, 0, 0, 0x000000},
        {L"Heavy Frost", 100, 65, 100, 85, 55, 5, 0, 0, 0x000000},
        {L"Vivid Glass", 45, 120, 108, 105, 12, 0, 0, 0, 0x000000},
        {L"Minimal Grey", 65, 25, 110, 85, 45, 0, 0, 0, 0x000000},
    };
    return presets;
}

inline D2D1_COLOR_F ColorFromRgb(uint32_t rgb) {
    return D2D1::ColorF(((rgb >> 16) & 0xFF) / 255.0f, ((rgb >> 8) & 0xFF) / 255.0f, (rgb & 0xFF) / 255.0f, 1);
}

inline uint32_t RgbFromColor(const D2D1_COLOR_F& c) {
    auto byte = [](float v) { return (uint32_t)std::lround(std::clamp(v, 0.0f, 1.0f) * 255); };
    return (byte(c.r) << 16) | (byte(c.g) << 8) | byte(c.b);
}

inline Params ParamsFromPreset(const Preset& preset) {
    Params p;
    p.blurSigma = (float)preset.blur;
    p.saturation = preset.saturation / 100.0f;
    p.brightness = preset.brightness / 100.0f;
    p.contrast = preset.contrast / 100.0f;
    p.tintOpacity = preset.tintOpacity / 100.0f;
    p.luminosity = preset.luminosity / 100.0f;
    p.noise = preset.noise / 100.0f;
    p.noiseScale = 1;
    p.dim = preset.dim / 100.0f;
    p.tint = ColorFromRgb(preset.tint);
    return p;
}

struct UserPreset {
    std::wstring name;
    Preset values;  // id unused
};

// -----------------------------------------------------------------------------
// Minimal JSON: only what the material file and the stored user presets need.
// Deliberately small and strict: anything it cannot parse is rejected, and the
// caller falls back to the defaults.

struct Json {
    enum Type { Null, Bool, Number, String, Array, Object } type = Null;
    bool boolean = false;
    double number = 0;
    std::wstring text;
    std::vector<Json> items;
    std::vector<std::pair<std::wstring, Json>> fields;

    const Json* Get(const std::wstring& key) const {
        for (const auto& field : fields)
            if (field.first == key) return &field.second;
        return nullptr;
    }
};

class JsonReader {
   public:
    explicit JsonReader(const std::wstring& text) : text_(text) {}

    bool Read(Json& out) {
        Skip();
        if (!Value(out, 0)) return false;
        Skip();
        return pos_ == text_.size();
    }

   private:
    const std::wstring& text_;
    size_t pos_ = 0;
    static constexpr int kMaxDepth = 64;

    void Skip() {
        while (pos_ < text_.size()) {
            WCHAR c = text_[pos_];
            if (c == L' ' || c == L'\t' || c == L'\r' || c == L'\n')
                pos_++;
            else
                break;
        }
    }

    bool Literal(const WCHAR* word) {
        size_t n = wcslen(word);
        if (text_.compare(pos_, n, word) != 0) return false;
        pos_ += n;
        return true;
    }

    bool String(std::wstring& out) {
        if (pos_ >= text_.size() || text_[pos_] != L'"') return false;
        pos_++;
        out.clear();
        while (pos_ < text_.size()) {
            WCHAR c = text_[pos_++];
            if (c == L'"') return true;
            if (c == L'\\') {
                if (pos_ >= text_.size()) return false;
                WCHAR e = text_[pos_++];
                switch (e) {
                    case L'"': out += L'"'; break;
                    case L'\\': out += L'\\'; break;
                    case L'/': out += L'/'; break;
                    case L'b': out += L'\b'; break;
                    case L'f': out += L'\f'; break;
                    case L'n': out += L'\n'; break;
                    case L'r': out += L'\r'; break;
                    case L't': out += L'\t'; break;
                    case L'u': {
                        if (pos_ + 4 > text_.size()) return false;
                        unsigned code = 0;
                        for (int i = 0; i < 4; i++) {
                            WCHAR h = text_[pos_ + i];
                            int d = h >= L'0' && h <= L'9' ? h - L'0'
                                    : h >= L'a' && h <= L'f' ? h - L'a' + 10
                                    : h >= L'A' && h <= L'F' ? h - L'A' + 10
                                                            : -1;
                            if (d < 0) return false;
                            code = code * 16 + (unsigned)d;
                        }
                        pos_ += 4;
                        out += (WCHAR)code;
                        break;
                    }
                    default: return false;
                }
            } else if (c < 0x20) {
                return false;
            } else {
                out += c;
            }
        }
        return false;
    }

    bool Number(Json& out) {
        size_t start = pos_;
        if (pos_ < text_.size() && (text_[pos_] == L'-' || text_[pos_] == L'+')) pos_++;
        bool digits = false;
        while (pos_ < text_.size() && text_[pos_] >= L'0' && text_[pos_] <= L'9') pos_++, digits = true;
        if (pos_ < text_.size() && text_[pos_] == L'.') {
            pos_++;
            while (pos_ < text_.size() && text_[pos_] >= L'0' && text_[pos_] <= L'9') pos_++, digits = true;
        }
        if (!digits) return false;
        if (pos_ < text_.size() && (text_[pos_] == L'e' || text_[pos_] == L'E')) {
            pos_++;
            if (pos_ < text_.size() && (text_[pos_] == L'-' || text_[pos_] == L'+')) pos_++;
            bool exp = false;
            while (pos_ < text_.size() && text_[pos_] >= L'0' && text_[pos_] <= L'9') pos_++, exp = true;
            if (!exp) return false;
        }
        out.type = Json::Number;
        out.number = wcstod(text_.substr(start, pos_ - start).c_str(), nullptr);
        return std::isfinite(out.number);
    }

    bool Value(Json& out, int depth = 0) {
        if (depth > kMaxDepth) return false;  // Corrupt or hostile nesting; do not grow the stack.
        Skip();
        if (pos_ >= text_.size()) return false;
        WCHAR c = text_[pos_];
        if (c == L'{') {
            pos_++;
            out.type = Json::Object;
            Skip();
            if (pos_ < text_.size() && text_[pos_] == L'}') { pos_++; return true; }
            for (;;) {
                Skip();
                std::wstring key;
                if (!String(key)) return false;
                Skip();
                if (pos_ >= text_.size() || text_[pos_] != L':') return false;
                pos_++;
                Json item;
                if (!Value(item, depth + 1)) return false;
                out.fields.emplace_back(std::move(key), std::move(item));
                Skip();
                if (pos_ >= text_.size()) return false;
                if (text_[pos_] == L',') { pos_++; continue; }
                if (text_[pos_] == L'}') { pos_++; return true; }
                return false;
            }
        }
        if (c == L'[') {
            pos_++;
            out.type = Json::Array;
            Skip();
            if (pos_ < text_.size() && text_[pos_] == L']') { pos_++; return true; }
            for (;;) {
                Json item;
                if (!Value(item, depth + 1)) return false;
                out.items.push_back(std::move(item));
                Skip();
                if (pos_ >= text_.size()) return false;
                if (text_[pos_] == L',') { pos_++; continue; }
                if (text_[pos_] == L']') { pos_++; return true; }
                return false;
            }
        }
        if (c == L'"') {
            out.type = Json::String;
            return String(out.text);
        }
        if (Literal(L"true")) { out.type = Json::Bool; out.boolean = true; return true; }
        if (Literal(L"false")) { out.type = Json::Bool; out.boolean = false; return true; }
        if (Literal(L"null")) { out.type = Json::Null; return true; }
        return Number(out);
    }
};

inline std::wstring JsonEscape(const std::wstring& text) {
    std::wstring out;
    for (WCHAR c : text) {
        switch (c) {
            case L'"': out += L"\\\""; break;
            case L'\\': out += L"\\\\"; break;
            case L'\n': out += L"\\n"; break;
            case L'\r': out += L"\\r"; break;
            case L'\t': out += L"\\t"; break;
            default:
                if (c < 0x20) {
                    WCHAR escape[8];
                    swprintf_s(escape, L"\\u%04X", (unsigned)c);
                    out += escape;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

inline std::wstring FormatNumber(double value, int decimals) {
    WCHAR text[64];
    swprintf_s(text, L"%.*f", decimals, value);
    return text;
}

// Params in the file and in stored presets. Tint is a hex string because the
// rest of the mod already speaks "#RRGGBB".
inline std::wstring ParamsJson(const Params& p, const std::wstring& indent = L"  ") {
    uint32_t rgb = RgbFromColor(p.tint);
    WCHAR hex[16];
    swprintf_s(hex, L"#%06X", (unsigned)(rgb & 0xFFFFFF));
    std::wstring out = L"{";
    out += L"\n" + indent + L"\"blurSigma\": " + FormatNumber(p.blurSigma, 1);
    out += L",\n" + indent + L"\"saturation\": " + FormatNumber(p.saturation * 100, 0);
    out += L",\n" + indent + L"\"brightness\": " + FormatNumber(p.brightness * 100, 0);
    out += L",\n" + indent + L"\"contrast\": " + FormatNumber(p.contrast * 100, 0);
    out += L",\n" + indent + L"\"tintOpacity\": " + FormatNumber(p.tintOpacity * 100, 0);
    out += L",\n" + indent + L"\"luminosity\": " + FormatNumber(p.luminosity * 100, 0);
    out += L",\n" + indent + L"\"noise\": " + FormatNumber(p.noise * 100, 1);
    out += L",\n" + indent + L"\"noiseScale\": " + FormatNumber(p.noiseScale * 100, 0);
    out += L",\n" + indent + L"\"dim\": " + FormatNumber(p.dim * 100, 0);
    out += L",\n" + indent + L"\"tint\": \"" + std::wstring(hex) + L"\"";
    out += L"\n";
    return out + L"}";
}

inline bool ParseHexColor(const std::wstring& text, uint32_t& rgb) {
    std::wstring digits = text;
    if (!digits.empty() && digits[0] == L'#') digits.erase(0, 1);
    if (digits.size() != 6) return false;
    uint32_t value = 0;
    for (WCHAR c : digits) {
        int d = c >= L'0' && c <= L'9' ? c - L'0'
                : c >= L'a' && c <= L'f' ? c - L'a' + 10
                : c >= L'A' && c <= L'F' ? c - L'A' + 10
                                        : -1;
        if (d < 0) return false;
        value = value * 16 + (uint32_t)d;
    }
    rgb = value;
    return true;
}

inline bool NumberField(const Json& object, const WCHAR* key, float& out, float fallback) {
    const Json* value = object.Get(key);
    if (!value || value->type != Json::Number || !std::isfinite(value->number)) {
        out = fallback;
        return false;
    }
    out = (float)value->number;
    return true;
}

// Reads a params object. Missing or invalid values take neutral defaults, and
// every value is clamped; an unknown backend or a corrupt document is the
// caller's problem, this only refuses a non-object.
inline bool ParseParams(const Json& object, Params& out, int* badFields = nullptr) {
    if (object.type != Json::Object) return false;
    int bad = 0;
    Params p;
    float v = 0;
    if (NumberField(object, L"blurSigma", v, 36)) p.blurSigma = v; else bad++;
    if (NumberField(object, L"saturation", v, 100)) p.saturation = v / 100.0f; else bad++;
    if (NumberField(object, L"brightness", v, 100)) p.brightness = v / 100.0f; else bad++;
    if (NumberField(object, L"contrast", v, 100)) p.contrast = v / 100.0f; else bad++;
    if (NumberField(object, L"tintOpacity", v, 30)) p.tintOpacity = v / 100.0f; else bad++;
    if (NumberField(object, L"luminosity", v, 0)) p.luminosity = v / 100.0f; else bad++;
    if (NumberField(object, L"noise", v, 0)) p.noise = v / 100.0f; else bad++;
    if (NumberField(object, L"noiseScale", v, 100)) p.noiseScale = v / 100.0f; else bad++;
    if (NumberField(object, L"dim", v, 0)) p.dim = v / 100.0f; else bad++;
    const Json* tint = object.Get(L"tint");
    uint32_t rgb = 0;
    if (tint && tint->type == Json::String && ParseHexColor(tint->text, rgb)) {
        p.tint = ColorFromRgb(rgb);
    } else {
        bad++;
    }
    Clamp(p);
    out = p;
    if (badFields) *badFields = bad;
    return true;
}

// User presets are stored in one setting as a JSON array. Names are limited so
// the row layout stays sane, and entries that fail to parse are dropped.
inline constexpr size_t kMaxUserPresets = 32;
inline constexpr size_t kMaxPresetName = 40;

inline std::wstring SanitizePresetName(const std::wstring& name) {
    std::wstring out;
    for (WCHAR c : name) {
        if (c == L'\t' || c == L'\n' || c == L'\r') continue;
        out += c;
        if (out.size() >= kMaxPresetName) break;
    }
    while (!out.empty() && out.front() == L' ') out.erase(0, 1);
    while (!out.empty() && out.back() == L' ') out.pop_back();
    return out;
}

inline std::wstring UserPresetsJson(const std::vector<UserPreset>& presets) {
    std::wstring out = L"[";
    for (size_t i = 0; i < presets.size(); i++) {
        const UserPreset& preset = presets[i];
        out += i ? L",\n  {" : L"\n  {";
        out += L"\"name\": \"" + JsonEscape(preset.name) + L"\", \"params\": ";
        out += ParamsJson(ParamsFromPreset(preset.values), L"    ");
        out += L"}";
    }
    if (!presets.empty()) out += L"\n";
    return out + L"]";
}

inline bool ParseUserPresets(const std::wstring& text, std::vector<UserPreset>& out) {
    out.clear();
    if (text.empty()) return true;
    Json root;
    JsonReader reader(text);
    if (!reader.Read(root) || root.type != Json::Array) return false;
    for (const Json& item : root.items) {
        if (out.size() >= kMaxUserPresets) break;
        if (item.type != Json::Object) continue;
        const Json* name = item.Get(L"name");
        const Json* params = item.Get(L"params");
        if (!name || name->type != Json::String || !params || params->type != Json::Object) continue;
        UserPreset preset;
        preset.name = SanitizePresetName(name->text);
        if (preset.name.empty()) continue;
        Params p;
        if (!ParseParams(*params, p)) continue;
        preset.values.id = nullptr;
        preset.values.blur = (int)std::lround(p.blurSigma);
        preset.values.saturation = (int)std::lround(p.saturation * 100);
        preset.values.brightness = (int)std::lround(p.brightness * 100);
        preset.values.contrast = (int)std::lround(p.contrast * 100);
        preset.values.tintOpacity = (int)std::lround(p.tintOpacity * 100);
        preset.values.luminosity = (int)std::lround(p.luminosity * 100);
        preset.values.noise = (int)std::lround(p.noise * 100);
        preset.values.dim = (int)std::lround(p.dim * 100);
        preset.values.tint = RgbFromColor(p.tint);
        out.push_back(std::move(preset));
    }
    return true;
}

// -----------------------------------------------------------------------------
// Preview wallpaper
//
// A fixed scene (never the desktop) so materials can be compared side by side:
// 0 dark, 1 light, 2 colourful. Deterministic: same input, same pixels.

inline void MakeWallpaper(int theme, int width, int height, std::vector<uint32_t>& out) {
    out.resize((size_t)width * height);
    auto mix = [](float a, float b, float t) { return a + (b - a) * t; };
    struct Blob { float x, y, r, r2, g2, b2; };
    // Neutral grey scene: the preview is there to judge blur, transparency and
    // grain, so it must not inject a hue of its own. Only the user's tint adds
    // colour to it.
    static const Blob dark[] = {
        {0.20f, 0.24f, 0.55f, 0.55f, 0.55f, 0.55f},
        {0.78f, 0.18f, 0.62f, 0.72f, 0.72f, 0.72f},
        {0.62f, 0.86f, 0.70f, 0.34f, 0.34f, 0.34f},
        {0.08f, 0.82f, 0.50f, 0.18f, 0.18f, 0.18f},
    };
    static const Blob light[] = {
        {0.18f, 0.20f, 0.62f, 0.96f, 0.96f, 0.96f},
        {0.84f, 0.26f, 0.60f, 0.52f, 0.52f, 0.52f},
        {0.55f, 0.88f, 0.66f, 0.88f, 0.88f, 0.88f},
        {0.30f, 0.70f, 0.48f, 0.68f, 0.68f, 0.68f},
    };
    static const Blob color[] = {
        {0.22f, 0.30f, 0.52f, 0.62f, 0.62f, 0.62f},
        {0.72f, 0.22f, 0.58f, 0.92f, 0.92f, 0.92f},
        {0.62f, 0.80f, 0.64f, 0.32f, 0.32f, 0.32f},
        {0.10f, 0.86f, 0.52f, 0.74f, 0.74f, 0.74f},
    };
    const Blob* blobs = theme == 1 ? light : theme == 2 ? color : dark;
    int count = 4;
    float baseR, baseG, baseB, topR, topG, topB;
    if (theme == 1) {
        baseR = 0.93f, baseG = 0.93f, baseB = 0.93f;
        topR = 0.82f, topG = 0.82f, topB = 0.82f;
    } else if (theme == 2) {
        baseR = 0.14f, baseG = 0.14f, baseB = 0.14f;
        topR = 0.19f, topG = 0.19f, topB = 0.19f;
    } else {
        baseR = 0.06f, baseG = 0.06f, baseB = 0.06f;
        topR = 0.12f, topG = 0.12f, topB = 0.12f;
    }
    for (int y = 0; y < height; y++) {
        float fy = (float)y / std::max(1, height - 1);
        for (int x = 0; x < width; x++) {
            float fx = (float)x / std::max(1, width - 1);
            float t = fy * 1.0f;
            float r = mix(topR, baseR, t), g = mix(topG, baseG, t), b = mix(topB, baseB, t);
            for (int i = 0; i < count; i++) {
                const Blob& blob = blobs[i];
                float dx = (fx - blob.x) * 1.65f, dy = fy - blob.y;
                float d2 = dx * dx + dy * dy;
                float w = std::exp(-d2 / (blob.r * blob.r) * 1.6f);
                r = mix(r, blob.r2, w * 0.85f);
                g = mix(g, blob.g2, w * 0.85f);
                b = mix(b, blob.b2, w * 0.85f);
            }
            // Vignette, so edges read as a desktop rather than a flat fill.
            float vx = fx - 0.5f, vy = fy - 0.5f;
            float vig = 1.0f - 0.35f * (vx * vx + vy * vy);
            r = std::clamp(r * vig, 0.0f, 1.0f);
            g = std::clamp(g * vig, 0.0f, 1.0f);
            b = std::clamp(b * vig, 0.0f, 1.0f);
            uint32_t B = (uint32_t)std::lround(b * 255), G = (uint32_t)std::lround(g * 255),
                     R = (uint32_t)std::lround(r * 255);
            out[(size_t)y * width + x] = (0xFFu << 24) | (R << 16) | (G << 8) | B;
        }
    }
}

// -----------------------------------------------------------------------------
// Noise texture
//
// One tile, generated once, shared by the GPU brush and the CPU preview. Value
// is in [-1, 1]: the sign picks white or black, the magnitude is the alpha.
// Hash based and tilable, it is never regenerated per frame.

inline constexpr int kNoiseTile = 256;

inline float NoiseSign(int x, int y) {
    // 32-bit integer hash (Wang/Murmur mix), mapped to [-1, 1].
    uint32_t h = (uint32_t)(x * 374761393 + y * 668265263);
    h = (h ^ (h >> 13)) * 1274126177u;
    h ^= h >> 16;
    return (float)(h & 0xFFFF) / 32767.5f - 1.0f;
}

inline void MakeNoiseTile(std::vector<uint8_t>& signedNoise) {
    signedNoise.resize((size_t)kNoiseTile * kNoiseTile);
    for (int y = 0; y < kNoiseTile; y++)
        for (int x = 0; x < kNoiseTile; x++) {
            float value = NoiseSign(x, y) * 0.7f + NoiseSign((x / 3) * 3, (y / 3) * 3) * 0.3f;
            value = std::clamp(value, -1.0f, 1.0f);
            signedNoise[(size_t)y * kNoiseTile + x] = (uint8_t)std::lround(value * 127 + 127);
        }
}

// BGRA premultiplied tile for a D2D bitmap brush: white or black, alpha = |v|.
inline void NoiseTileToBgra(const std::vector<uint8_t>& signedNoise, std::vector<uint32_t>& out) {
    out.resize(signedNoise.size());
    for (size_t i = 0; i < signedNoise.size(); i++) {
        int value = (int)signedNoise[i] - 127;  // -127..127
        uint32_t alpha = (uint32_t)std::min(255, std::abs(value) * 2);
        uint32_t colour = value > 0 ? 255u : 0u;
        out[i] = (alpha << 24) | (colour << 16) | (colour << 8) | colour;
    }
}

// -----------------------------------------------------------------------------
// CPU mirror of the GPU chain, used by the settings preview and the preset cards.

struct BakeResult {
    int width = 0, height = 0;
    float meanLuminance = 0.5f;
};

// Box radii (three passes) that approximate a gaussian of the given sigma.
inline void BoxRadii(float sigma, int radii[3]) {
    const float n = 3.0f;
    float ideal = std::sqrt(12.0f * sigma * sigma / n + 1.0f);
    int wl = (int)std::floor(ideal);
    if (wl % 2 == 0) wl--;
    wl = std::max(1, wl);
    int wu = wl + 2;
    float mIdeal = (12.0f * sigma * sigma - n * wl * wl - 4.0f * n * wl - 3.0f * n) / (-4.0f * wl - 4.0f);
    int m = (int)std::lround(mIdeal);
    for (int i = 0; i < 3; i++) radii[i] = std::max(0, ((i < m ? wl : wu) - 1) / 2);
}

inline void BoxBlurH(std::vector<float>& src, std::vector<float>& dst, int width, int height, int radius) {
    if (radius <= 0) {
        dst = src;
        return;
    }
    float scale = 1.0f / (2 * radius + 1);
    for (int y = 0; y < height; y++) {
        size_t row = (size_t)y * width;
        float sum = src[row] * (radius + 1);
        for (int i = 0; i < radius; i++) sum += src[row + std::min(width - 1, i)];
        for (int x = 0; x < width; x++) {
            dst[row + x] = sum * scale;
            float add = src[row + std::min(width - 1, x + radius + 1)];
            float sub = src[row + std::max(0, x - radius)];
            sum += add - sub;
        }
    }
}

inline void BoxBlurV(std::vector<float>& src, std::vector<float>& dst, int width, int height, int radius) {
    if (radius <= 0) {
        dst = src;
        return;
    }
    float scale = 1.0f / (2 * radius + 1);
    for (int x = 0; x < width; x++) {
        float sum = src[x] * (radius + 1);
        for (int i = 0; i < radius; i++) sum += src[(size_t)std::min(height - 1, i) * width + x];
        for (int y = 0; y < height; y++) {
            dst[(size_t)y * width + x] = sum * scale;
            float add = src[(size_t)std::min(height - 1, y + radius + 1) * width + x];
            float sub = src[(size_t)std::max(0, y - radius) * width + x];
            sum += add - sub;
        }
    }
}

inline void Blur(std::vector<float>& plane, int width, int height, float sigma) {
    if (sigma < 0.3f) return;
    int radii[3];
    BoxRadii(sigma, radii);
    std::vector<float> temp(plane.size());
    for (int i = 0; i < 3; i++) {
        BoxBlurH(plane, temp, width, height, radii[i]);
        BoxBlurV(temp, plane, width, height, radii[i]);
    }
}

// backdrop: opaque BGRA at bw x bh. scale: output pixels per DIP for this
// miniature. noise: signed tile; out: premultiplied BGRA at bw x bh.
inline BakeResult Bake(const std::vector<uint32_t>& backdrop, int bw, int bh, float scale, const Params& params,
                       const std::vector<uint8_t>& noise, std::vector<uint32_t>& out) {
    BakeResult result;
    result.width = bw;
    result.height = bh;
    size_t pixels = (size_t)bw * bh;
    out.resize(pixels);
    if (!pixels) return result;

    std::vector<float> red(pixels), green(pixels), blue(pixels);
    for (size_t i = 0; i < pixels; i++) {
        uint32_t c = backdrop[i];
        red[i] = (float)((c >> 16) & 0xFF) / 255.0f;
        green[i] = (float)((c >> 8) & 0xFF) / 255.0f;
        blue[i] = (float)(c & 0xFF) / 255.0f;
    }
    Blur(red, bw, bh, params.blurSigma * scale);
    Blur(green, bw, bh, params.blurSigma * scale);
    Blur(blue, bw, bh, params.blurSigma * scale);

    double sum = 0;
    for (size_t i = 0; i < pixels; i++) sum += Luminance(red[i], green[i], blue[i]);
    float mean = (float)(sum / (double)pixels);
    result.meanLuminance = mean;

    Matrix5x4 grade = GradeMatrix(params);
    D2D1_COLOR_F lum = LuminosityColor(mean);
    float lumA = params.luminosity;
    float tintA = params.tintOpacity;
    float noiseA = params.noise;
    float dim = params.dim;
    float noiseScale = std::max(0.01f, params.noiseScale);
    for (size_t i = 0; i < pixels; i++) {
        float r = red[i], g = green[i], b = blue[i];
        if (!GradeIsNeutral(params)) ApplyMatrix(grade, r, g, b);
        if (lumA > 0) {
            r += (lum.r - r) * lumA;
            g += (lum.g - g) * lumA;
            b += (lum.b - b) * lumA;
        }
        if (tintA > 0) {
            r += (params.tint.r - r) * tintA;
            g += (params.tint.g - g) * tintA;
            b += (params.tint.b - b) * tintA;
        }
        if (noiseA > 0 && !noise.empty()) {
            int x = i % bw, y = (int)(i / bw);
            int tx = (int)std::floor((float)x / scale / noiseScale) & (kNoiseTile - 1);
            int ty = (int)std::floor((float)y / scale / noiseScale) & (kNoiseTile - 1);
            float value = ((float)noise[(size_t)ty * kNoiseTile + tx] - 127.0f) / 127.0f;
            float alpha = std::min(1.0f, std::abs(value) * noiseA);
            float colour = value > 0 ? 1.0f : 0.0f;
            r += (colour - r) * alpha;
            g += (colour - g) * alpha;
            b += (colour - b) * alpha;
        }
        if (dim > 0) {
            r *= (1 - dim);
            g *= (1 - dim);
            b *= (1 - dim);
        }
        uint32_t R = (uint32_t)std::lround(std::clamp(r, 0.0f, 1.0f) * 255);
        uint32_t G = (uint32_t)std::lround(std::clamp(g, 0.0f, 1.0f) * 255);
        uint32_t B = (uint32_t)std::lround(std::clamp(b, 0.0f, 1.0f) * 255);
        out[i] = (0xFFu << 24) | (R << 16) | (G << 8) | B;
    }
    return result;
}

}  // namespace launchpad_material
