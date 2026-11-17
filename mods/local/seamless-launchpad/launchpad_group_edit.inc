bool GroupsOverlap(const GroupPosition& a,const GroupPosition& b) {
    return a.x<b.x+b.w && b.x<a.x+a.w && a.y<b.y+b.h && b.y<a.y+a.h;
}
void ToggleGroupEdit() {
    if(g_groupPositionsReadOnly||g_layoutReadOnly||g_dataReadOnly)return;
    if(g_groupEditMode) {
        g_groupGesture={};g_groupEditMode=false;
        if(!g_freeGroupLayout)g_groupPositions.clear();
        if(GetCapture()==g_wnd)ReleaseCapture();
    } else {
        if(!GroupedView())return;
        if(!g_freeGroupLayout) {
            g_groupPositions.clear();
            for(const auto& geometry:g_groupGeometry)if(!g_groupPositions.count(geometry.group)) {
                auto r=geometry.area;float s=g_layout.scale;
                g_groupPositions[geometry.group]={r.left/s,(r.top-g_layout.viewTop)/s,(r.right-r.left)/s,(r.bottom-r.top)/s};
            }
        }
        g_groupEditMode=true;
    }
    g_pressed=-1;g_pressedEmpty=false;Render();
}
bool FreeGroupPointerDown(HWND hwnd,int x,int y) {
    if(!g_groupEditMode||!GroupedView())return false;
    int index=GroupAtPoint(x,y);if(index<0)return y>=g_layout.viewTop&&y<g_layout.viewBottom;
    auto& geometry=g_groupGeometry[index];auto r=ProjectGroupRect(geometry.area);
    bool resize=x>=r.right-24*g_layout.scale&&y>=r.bottom-24*g_layout.scale;
    if(!resize&&!GroupContains(ProjectGroupRect(geometry.title),(float)x,(float)y))return true;
    g_groupGesture={};g_groupGesture.group=geometry.group;g_groupGesture.original=g_groupPositions[geometry.group];
    g_groupGesture.draft=g_groupGesture.original;g_groupGesture.start={x,y};g_groupGesture.resize=resize;
    g_groupGesture.initialScroll=g_scroll;g_groupGesture.valid=true;SetCapture(hwnd);Render();return true;
}
bool FreeGroupPointerMove(int x,int y) {
    auto& gesture=g_groupGesture;if(!gesture.group)return false;
    float s=g_layout.scale,scroll=g_scroll-gesture.initialScroll;
    float dx=(x-gesture.start.x+(g_settings.pages?scroll:0))/s,dy=(y-gesture.start.y+(g_settings.pages?0:scroll))/s;
    if(std::abs(dx)<GetSystemMetrics(SM_CXDRAG)/s&&std::abs(dy)<GetSystemMetrics(SM_CYDRAG)/s&&!gesture.moved)return true;
    gesture.moved=true;auto p=gesture.original;auto snap=[](float value){return std::round(value/16)*16;};
    float minWidth=g_layout.icon/s+24,minHeight=(36+24)+(g_layout.cellHeight/s);
    if(gesture.resize) {
        p.w=std::clamp(snap(p.w+dx),minWidth,std::max(minWidth,g_layout.width/s-p.x-16));
        p.h=std::max(minHeight,snap(p.h+dy));
    } else {
        p.x=std::clamp(snap(p.x+dx),16.0f,std::max(16.0f,g_layout.width/s-p.w-16));p.y=std::max(0.0f,snap(p.y+dy));
    }
    if(g_settings.pages)p.h=std::min(p.h,std::max(minHeight,(g_layout.viewBottom-g_layout.viewTop)/s-p.y));
    gesture.valid=true;
    for(const auto& [group,other]:g_groupPositions)if(group!=gesture.group&&GroupsOverlap(p,other)){gesture.valid=false;break;}
    gesture.draft=p;Render();return true;
}
bool FreeGroupPointerUp(HWND hwnd) {
    if(!g_groupEditMode)return false;
    auto gesture=g_groupGesture;g_groupGesture={};
    if(gesture.group&&gesture.moved&&gesture.valid) {
        bool wasFree=g_freeGroupLayout;
        g_groupPositions[gesture.group]=gesture.draft;g_freeGroupLayout=true;
        if(!SaveGroupPositions()) {
            g_groupPositions[gesture.group]=gesture.original;
            g_freeGroupLayout=wasFree;
            MessageBoxW(hwnd,Tr(L"Could not save group geometry."),Tr(L"Edit group layout"),MB_OK|MB_ICONWARNING);
        }
        Refresh();
    }
    if(GetCapture()==hwnd)ReleaseCapture();Render();return true;
}
bool ScrollFreeGroup(HWND hwnd,LPARAM lp,int delta) {
    if(!g_freeGroupLayout||!GroupedView()||g_groupEditMode||GetKeyState(VK_SHIFT)<0)return false;
    POINT point{GET_X_LPARAM(lp),GET_Y_LPARAM(lp)};ScreenToClient(hwnd,&point);
    int index=GroupAtPoint(point.x,point.y);if(index<0)return false;
    auto geometry=g_groupGeometry[index];int rows=((int)geometry.group->apps.size()+geometry.columns-1)/geometry.columns;
    if(geometry.group->collapsed)return false;
    if(rows<=geometry.visibleRows)return false;
    g_wheelDelta+=delta;int step=g_wheelDelta/WHEEL_DELTA;g_wheelDelta%=WHEEL_DELTA;
    g_groupRows[geometry.group]=std::clamp(g_groupRows[geometry.group]-step,0,rows-geometry.visibleRows);
    ComputeGroups();g_hover=-1;Render();return true;
}
