// Rasterize supported SVG icons once on load; no readback in the frame loop.
// https://learn.microsoft.com/windows/win32/direct2d/svg-support
bool LoadSvgIcon(const std::wstring& path,int size,App& app) {
    std::wstring text;
    if(!ryjs_data::ReadUtf8(path,text)||text.find(L"<svg")==std::wstring::npos)return false;
    ComPtr<IStream> stream;
    if(FAILED(SHCreateStreamOnFileEx(path.c_str(),STGM_READ|STGM_SHARE_DENY_WRITE,0,FALSE,nullptr,&stream)))return false;
    ComPtr<ID3D11Device> d3d;
    if(FAILED(D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,D3D11_CREATE_DEVICE_BGRA_SUPPORT,nullptr,0,D3D11_SDK_VERSION,&d3d,nullptr,nullptr))&&
       FAILED(D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_WARP,nullptr,D3D11_CREATE_DEVICE_BGRA_SUPPORT,nullptr,0,D3D11_SDK_VERSION,&d3d,nullptr,nullptr)))return false;
    ComPtr<IDXGIDevice> dxgi;ComPtr<ID2D1Factory1> factory;ComPtr<ID2D1Device> device;
    ComPtr<ID2D1DeviceContext> context;ComPtr<ID2D1DeviceContext5> svgContext;
    if(FAILED(d3d.As(&dxgi))||FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,IID_PPV_ARGS(&factory)))||
        FAILED(factory->CreateDevice(dxgi.Get(),&device))||FAILED(device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,&context))||FAILED(context.As(&svgContext)))return false;
    UINT side=(UINT)std::clamp(size,16,512);
    ComPtr<ID2D1SvgDocument> svg;ComPtr<ID2D1Bitmap1> target,readback;
    auto properties=D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED));
    if(FAILED(svgContext->CreateSvgDocument(stream.Get(),D2D1::SizeF((float)side,(float)side),&svg))||
        FAILED(context->CreateBitmap(D2D1::SizeU(side,side),nullptr,0,properties,&target)))return false;
    context->SetTarget(target.Get());context->BeginDraw();context->Clear(D2D1::ColorF(0,0));svgContext->DrawSvgDocument(svg.Get());
    if(FAILED(context->EndDraw()))return false;
    properties.bitmapOptions=D2D1_BITMAP_OPTIONS_CPU_READ|D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    if(FAILED(context->CreateBitmap(D2D1::SizeU(side,side),nullptr,0,properties,&readback))||FAILED(readback->CopyFromBitmap(nullptr,target.Get(),nullptr)))return false;
    D2D1_MAPPED_RECT mapped{};if(FAILED(readback->Map(D2D1_MAP_OPTIONS_READ,&mapped)))return false;
    std::vector<UINT32> pixels((size_t)side*side);
    for(UINT y=0;y<side;y++)std::copy_n((UINT32*)(mapped.bits+y*mapped.pitch),side,pixels.data()+y*side);
    readback->Unmap();
    if(!std::any_of(pixels.begin(),pixels.end(),[](UINT32 p){return (p>>24)!=0;}))return false;
    app.pixels=std::move(pixels);app.iconSize=side;app.bitmap.Reset();app.generation=0;
    return true;
}
