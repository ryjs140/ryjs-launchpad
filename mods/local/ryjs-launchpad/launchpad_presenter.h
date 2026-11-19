#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d2d1_1.h>
#include <dxgi1_3.h>
#include <dcomp.h>
#include <wrl/client.h>

// Flip-model GPU composition. No GPU->CPU readback or layered-window bitmap
// upload is used in this path. The original presenter remains a fallback.
class LaunchpadPresenter {
    template<class T> using Ptr = Microsoft::WRL::ComPtr<T>;
    Ptr<IDXGISwapChain1> chain;
    Ptr<IDCompositionDevice> device;
    Ptr<IDCompositionTarget> target;
    Ptr<IDCompositionVisual> visual;
    Ptr<IDCompositionEffectGroup> opacityEffect;
    UINT width = 0, height = 0;
    RECT bounds{};
public:
    bool failed = false;
    bool Active() const { return chain != nullptr; }
    bool Statistics(DXGI_FRAME_STATISTICS& statistics) const {
        return chain && SUCCEEDED(chain->GetFrameStatistics(&statistics));
    }
    void Reset() {
        opacityEffect.Reset(); visual.Reset(); target.Reset(); device.Reset(); chain.Reset();
        width = height = 0;
        bounds = {};
        failed = false;
    }
    static void LegacyStyle(HWND hwnd) {
        auto style = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
        if (!(style & WS_EX_LAYERED) || (style & WS_EX_NOREDIRECTIONBITMAP))
            SetWindowLongPtrW(hwnd, GWL_EXSTYLE, (style | WS_EX_LAYERED) & ~WS_EX_NOREDIRECTIONBITMAP);
    }
    bool Ensure(HWND hwnd, ID3D11Device* d3d, UINT w, UINT h) {
        if (failed) return false;
        if (chain && width == w && height == h) return true;
        Reset();
        auto fail = [&] { Reset(); failed = true; LegacyStyle(hwnd); return false; };
        Ptr<IDXGIDevice> dxgi;
        Ptr<IDXGIAdapter> adapter;
        Ptr<IDXGIFactory2> factory;
        if (FAILED(d3d->QueryInterface(IID_PPV_ARGS(&dxgi))) || FAILED(dxgi->GetAdapter(&adapter)) ||
            FAILED(adapter->GetParent(IID_PPV_ARGS(&factory)))) return fail();
        DXGI_SWAP_CHAIN_DESC1 desc{};
        desc.Width = w; desc.Height = h;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 2;
        desc.Scaling = DXGI_SCALING_STRETCH;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
        if (FAILED(factory->CreateSwapChainForComposition(d3d, &desc, nullptr, &chain)) ||
            FAILED(DCompositionCreateDevice(dxgi.Get(), __uuidof(IDCompositionDevice), (void**)device.GetAddressOf())) ||
            FAILED(device->CreateTargetForHwnd(hwnd, TRUE, &target)) ||
            FAILED(device->CreateVisual(&visual)) || FAILED(visual->SetContent(chain.Get())) ||
            FAILED(device->CreateEffectGroup(&opacityEffect)) || FAILED(visual->SetEffect(opacityEffect.Get())) ||
            FAILED(target->SetRoot(visual.Get())) || FAILED(device->Commit())) return fail();
        Ptr<IDXGIDevice1> queued;
        if (SUCCEEDED(dxgi.As(&queued))) queued->SetMaximumFrameLatency(1);
        auto style = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
        SetWindowLongPtrW(hwnd, GWL_EXSTYLE, (style | WS_EX_NOREDIRECTIONBITMAP) & ~WS_EX_LAYERED);
        width = w; height = h;
        return true;
    }
    bool Present(ID3D11Device* d3d, ID2D1Bitmap1* image, float opacity) {
        if (!chain) return false;
        Ptr<IDXGISurface> surface;
        Ptr<ID3D11Texture2D> source, destination;
        Ptr<ID3D11DeviceContext> context;
        if (FAILED(image->GetSurface(&surface)) || FAILED(surface.As(&source)) ||
            FAILED(chain->GetBuffer(0, IID_PPV_ARGS(&destination)))) return false;
        d3d->GetImmediateContext(&context);
        context->CopyResource(destination.Get(), source.Get());
        if (FAILED(opacityEffect->SetOpacity(opacity)) || FAILED(device->Commit())) return false;
        // DWM composes at the display cadence; the high-resolution scheduler
        // submits one fresh image per monitor period, without blocking input
        // inside a vsync wait on the UI thread.
        return SUCCEEDED(chain->Present(0, 0));
    }
    void Position(HWND hwnd, const RECT& area) {
        if (!EqualRect(&area, &bounds)) {
            SetWindowPos(hwnd, nullptr, area.left, area.top, area.right-area.left, area.bottom-area.top, SWP_NOZORDER | SWP_NOACTIVATE);
            bounds = area;
        }
    }
};
