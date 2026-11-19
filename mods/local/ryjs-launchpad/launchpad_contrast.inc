// Read only the text-free GPU blur, once when its source changes. Never sample
// the presented desktop with our own text on it (which causes feedback).
struct ContrastPixels {
    int width=0,height=0;
    std::vector<UINT32> pixels;
    bool Read(ID2D1Bitmap* source) {
        *this={};if(!source)return false;
        auto size=source->GetPixelSize();ComPtr<ID2D1Bitmap1> staging;
        auto props=D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_CPU_READ|D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),96,96);
        if(FAILED(g_target->CreateBitmap(size,nullptr,0,props,&staging))||
           FAILED(staging->CopyFromBitmap(nullptr,source,nullptr)))return false;
        D2D1_MAPPED_RECT mapped{};
        if(FAILED(staging->Map(D2D1_MAP_OPTIONS_READ,&mapped)))return false;
        width=size.width;height=size.height;pixels.resize((size_t)width*height);
        for(int y=0;y<height;y++)memcpy(pixels.data()+(size_t)y*width,mapped.bits+(size_t)y*mapped.pitch,width*4);
        staging->Unmap();return true;
    }
    D2D1_COLOR_F At(float x,float y,float displayWidth,float displayHeight) const {
        if(pixels.empty())return D2D1::ColorF(0x808080);
        int px=std::clamp((int)(x*width/std::max(1.0f,displayWidth)),0,width-1);
        int py=std::clamp((int)(y*height/std::max(1.0f,displayHeight)),0,height-1);
        UINT32 p=pixels[(size_t)py*width+px];
        return D2D1::ColorF(((p>>16)&255)/255.0f,((p>>8)&255)/255.0f,(p&255)/255.0f);
    }
};
[[clang::no_destroy]] ContrastPixels g_backgroundContrast,g_folderContrast;
[[clang::no_destroy]] std::map<const void*,bool> g_textBlack;
float LinearChannel(float value) {return value<=0.04045f?value/12.92f:std::pow((value+0.055f)/1.055f,2.4f);}
float RelativeLuminance(D2D1_COLOR_F c) {return 0.2126f*LinearChannel(c.r)+0.7152f*LinearChannel(c.g)+0.0722f*LinearChannel(c.b);}
D2D1_COLOR_F CompositeContrast(D2D1_COLOR_F under,D2D1_COLOR_F over) {
    return D2D1::ColorF(under.r*(1-over.a)+over.r*over.a,under.g*(1-over.a)+over.g*over.a,under.b*(1-over.a)+over.b*over.a);
}
bool ChooseBlack(float luminance,const void* identity) {
    // Black/white contrast is equal at sqrt(.0525)-.05 (~.179).
    // A small hysteresis band prevents tiny animation/position changes toggling.
    auto previous=g_textBlack.find(identity);
    bool black=previous==g_textBlack.end()?luminance>0.1791288f:
        (previous->second?luminance>=0.155f:luminance>0.205f);
    g_textBlack[identity]=black;return black;
}
D2D1_COLOR_F AdaptiveTextColor(const D2D1_RECT_F& box,const void* identity);
