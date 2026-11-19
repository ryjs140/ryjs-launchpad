// Included after CellAt: geometry uses the same MakeCell and DrawTile pipeline.
struct ExpandedGroupGeometry {
    std::shared_ptr<Folder> group;
    D2D1_RECT_F area, title;
    int start = 0, firstInGroup = 0, count = 0, columns = 1, entryOrder = 0;
    float cellWidth = 0, rowPitch = 0;
    int visibleRows = 1;
    bool wholeCells = false, squareCells = false;
};
[[clang::no_destroy]] std::vector<ExpandedGroupGeometry> g_groupGeometry;
[[clang::no_destroy]] std::vector<Cell> g_groupCells;
[[clang::no_destroy]] std::shared_ptr<Folder> g_groupDropFolder, g_groupHeaderDragging;
int g_groupDropSlot = -1;
void CancelGroupPointer();
void CancelGroupEdit();
bool ConfirmGroupEditClose();
void DrawGroupEditBar();
void GroupHeaderTick();
bool TickGroupGesture();
extern float g_appear;
extern bool g_closing;
float Ease(float t);

bool FlyInActive() {
    return g_settings.iconFlyIn && g_appear < 1.f && !g_groupEditMode;
}

float FlyInChromeOpacity() {
    return g_settings.iconFlyIn && !g_groupEditMode
        ? launchpad_fly_in::SmoothStep((g_appear - .05f) / .62f)
        : 1.f;
}

launchpad_fly_in::Pose FlyInPose(D2D1_POINT_2F center, float margin) {
    // Limit the spatial stagger to 45 ms at the default open duration.
    float delay = std::min(.18f, 45.f / g_settings.openMs);
    return launchpad_fly_in::Sample(g_appear, center.x, center.y,
                                    (float)g_layout.width, (float)g_layout.height,
                                    margin, delay);
}

[[clang::no_destroy]] std::map<const void*,int> g_groupEntryOrder;
float GroupEntryProgress(int order) {
    if (g_settings.iconFlyIn || !Animate() || g_closing || g_groupEditMode) return 1;
    float delay=std::min(order,6)*0.035f;
    return Ease(std::clamp((g_appear-delay)/(1-delay),0.0f,1.0f));
}
float GroupTileProgress(const Tile& tile) {
    if (!GroupedView() || !tile.app) return 1;
    auto p=g_groupEntryOrder.find(tile.app.get());
    return p==g_groupEntryOrder.end() ? 1 : GroupEntryProgress(p->second);
}

bool GroupContains(const D2D1_RECT_F& r, float x, float y) {
    return x >= r.left && x < r.right && y >= r.top && y < r.bottom;
}
D2D1_RECT_F ProjectGroupRect(D2D1_RECT_F r) {
    if (g_settings.pages) {
        r.left -= g_scroll; r.right -= g_scroll;
        r.top-=g_groupPagePan;r.bottom-=g_groupPagePan;
    }
    else { r.top -= g_scroll; r.bottom -= g_scroll; }
    return r;
}
// Complete pitch includes image, padding, text allocation and the user gap.
float LegacyGroupPitchX() {return std::max(g_layout.icon+2*g_layout.cellPad+g_settings.horizontalGap*g_layout.scale,
    (g_settings.labelSize+7)*4*g_layout.scale);}
float LegacyGroupPitchY() {return std::round(g_layout.icon+g_layout.labelHeight+(18+g_settings.verticalGap)*g_layout.scale);}
float GroupPitchX() {
    return (std::max(g_settings.iconSize+12.f,(g_settings.labelSize+7)*4)+g_settings.horizontalGap)*g_layout.scale;
}
float GroupPitchY() {
    return (g_settings.iconSize+g_settings.labelSize+7+18+g_settings.verticalGap)*g_layout.scale;
}
float GroupStepX() { return SquareCellSize(g_squareGridColumns)/g_layout.scale/2; }
float GroupStepY() { return GroupStepX()/2; }
float GroupXAt(int index) {
    int steps=2*g_squareGridColumns;
    int page=index/steps;
    return (page*g_layout.width+GridMargin())/g_layout.scale+(index%steps)*GroupStepX();
}
float LegacyGroupTitleHeight(bool legacyGrid=false) {
    float s=g_layout.scale;
    static UINT formatVersion=0;
    static float fontHeight=0;
    if(g_groupFormat&&g_dwrite&&formatVersion!=g_formatVersion) {
        formatVersion=g_formatVersion;
        ComPtr<IDWriteTextLayout> text;
        DWRITE_TEXT_METRICS metrics{};
        if(SUCCEEDED(g_dwrite->CreateTextLayout(L"Ag",2,g_groupFormat.Get(),1000,1000,&text))&&
           SUCCEEDED(text->GetMetrics(&metrics)))fontHeight=metrics.height/g_formatScale;
    }
    return std::max((std::max(fontHeight,24.f)+8)*s,legacyGrid?GroupPitchY()*.5f:36*s);
}
constexpr float kGroupPadding=12;
float GroupIconPitchX() { return std::round(GroupPitchX()); }
float GroupIconPitchY() { return std::round(GroupPitchY()); }
int FittingGroupColumns(float width) {
    float minimumCellWidth=g_layout.icon+2*g_layout.cellPad;
    return 1+std::max(0,(int)std::floor((width-2*kGroupPadding*g_layout.scale-minimumCellWidth+.01f)/GroupIconPitchX()));
}
GroupPosition EffectiveGroupPosition(GroupPosition p) {
    float s=g_layout.scale;
    if(p.SquareCells()&&g_squareGridColumns) {
        p.x=GroupXAt(p.gridX);p.y=p.gridY*GroupStepY();
        p.w=p.gridW*GroupStepX();p.h=p.gridH*GroupStepY();
        return p;
    }
    float stepX=GroupPitchX()/s,stepY=GroupPitchY()/s;
    if(!p.WholeCells()) {stepX*=.5f;stepY*=.5f;}
    if((p.style==0&&p.OnGrid())||p.snapped) {
        p.x=p.gridX*stepX;p.y=p.gridY*stepY;
    }
    if(p.OnGrid()) {
        p.w=p.gridW*stepX;p.h=p.gridH*stepY;
        if(p.style==2) {
            p.w+=2*kGroupPadding;
            p.h+=LegacyGroupTitleHeight()/s+2*kGroupPadding;
        }
    } else if(p.style==0&&p.columns>0&&p.rows>0) {
        p.w=24+p.columns*LegacyGroupPitchX()/s;
        p.h=60+p.rows*LegacyGroupPitchY()/s;
    }
    return p;
}
GroupPosition PositionFromArea(const D2D1_RECT_F& r) {
    float s=g_layout.scale;
    return {r.left/s,(r.top-g_layout.viewTop)/s,(r.right-r.left)/s,(r.bottom-r.top)/s};
}
D2D1_RECT_F GroupPositionRect(const GroupPosition& p) {
    float s=g_layout.scale;
    return {p.x*s,g_layout.viewTop+p.y*s,(p.x+p.w)*s,g_layout.viewTop+(p.y+p.h)*s};
}
// WM mouse coordinates and the 96-DPI D2D target use client pixels.
POINTF GroupPointerInLayout(int x,int y) {
    return {(x+(g_settings.pages?g_scroll:0))/g_layout.scale,
        (y-g_layout.viewTop+(g_settings.pages?g_groupPagePan:g_scroll))/g_layout.scale};
}
float GroupEase(double started) {
    if(!Animate())return 1;
    float t=std::clamp((float)((Now()-started)/.14),0.f,1.f);
    return 1-(1-t)*(1-t)*(1-t);
}
GroupPosition InterpolateGroup(const GroupPosition& a,const GroupPosition& b,float t) {
    return {std::lerp(a.x,b.x,t),std::lerp(a.y,b.y,t),std::lerp(a.w,b.w,t),std::lerp(a.h,b.h,t)};
}
D2D1_RECT_F GroupResizeHit(const D2D1_RECT_F& r) {
    float size=std::min({26*g_layout.scale,(r.right-r.left)/3,(r.bottom-r.top)/3});
    return {r.right-size,r.bottom-size,r.right,r.bottom};
}
GroupPosition VisualGroupPosition(const std::shared_ptr<Folder>& group,GroupPosition base) {
    if(g_groupGesture.moved) {
        if(group==g_groupGesture.group)return g_groupGesture.continuous;
        if(g_groupGesture.valid)for(const auto& node:g_groupGesture.solution)
            if(node.group==group&&!node.fixed)return node.position;
    }
    auto transition=g_groupTransitions.find(group);
    if(transition==g_groupTransitions.end())return base;
    const auto& t=transition->second;
    return InterpolateGroup(t.from,t.to,GroupEase(t.started));
}
D2D1_RECT_F VisualGroupRect(const ExpandedGroupGeometry& geometry) {
    if(g_groupTransitions.empty()&&!g_groupGesture.moved)return ProjectGroupRect(geometry.area);
    return ProjectGroupRect(GroupPositionRect(VisualGroupPosition(geometry.group,PositionFromArea(geometry.area))));
}
ExpandedGroupGeometry MakeFreeGroupGeometry(const std::shared_ptr<Folder>& group,GroupPosition position,
                                           int global,int order) {
    const auto& L=g_layout;
    float s=L.scale;
    auto p=EffectiveGroupPosition(position);
    float side=p.SquareCells()?SquareCellSize(g_squareGridColumns):0;
    float pad=(side||p.WholeCells())?0:kGroupPadding*s;
    float header=side?side/2:p.WholeCells()?GroupPitchY():LegacyGroupTitleHeight(p.style==0&&p.OnGrid());
    float storedPitch=p.OnGrid()?GroupPitchX():LegacyGroupPitchX();
    float storedRowPitch=p.OnGrid()?GroupPitchY():LegacyGroupPitchY();
    // Legacy viewport rules still determine the outside rectangle.
    float minWidth=(p.OnGrid()?storedPitch:L.icon)+2*pad;
    float width=p.style?p.w*s:std::max(minWidth,std::min(p.w*s,L.width-(p.OnGrid()?0:32*s)));
    float page=g_settings.pages?std::floor(p.x*s/std::max(1,L.width))*L.width:0;
    float x=(p.style||p.OnGrid()||p.snapped)?p.x*s:page+std::clamp(p.x*s-page,16*s,std::max(16*s,L.width-width-16*s));
    float y=L.viewTop+p.y*s;
    float height=p.style?p.h*s:std::max(header+2*pad+storedRowPitch,p.h*s);
    if(!p.style&&g_settings.pages&&!p.OnGrid())height=std::min(height,std::max(header+2*pad+storedRowPitch,L.viewBottom-y));
    if(!p.style&&!p.OnGrid()&&p.columns) {
        int oldColumns=std::clamp((int)std::floor((width-2*pad+.01f)/storedPitch),1,p.columns);
        width=2*pad+oldColumns*storedPitch;
    }
    if(!p.style&&!p.OnGrid()&&p.rows) {
        int oldRows=std::max(1,(int)std::floor((height-header-2*pad+.01f)/storedRowPitch));
        height=header+2*pad+oldRows*storedRowPitch;
    }

    int columns=FittingGroupColumns(width);
    if(side)columns=p.gridW/2;
    else if(p.WholeCells())columns=p.gridW;
    else if(p.OnGrid())columns=std::max(1,(int)std::floor((width-2*pad+.01f)/GroupPitchX()));
    else if(p.columns)columns=std::min(columns,p.columns);
    else if(g_settings.columns>0)columns=std::min(columns,g_settings.columns);
    float fitRowPitch=p.OnGrid()?GroupPitchY():GroupIconPitchY();
    int visibleRows=side?(p.gridH-2)/4:p.WholeCells()?p.gridH-1:std::max(1,(int)std::floor((height-header-2*pad+.01f)/fitRowPitch));
    int count=group->collapsed?1:(int)group->apps.size();
    int totalRows=(count+columns-1)/columns;
    auto storedRow=g_groupRows.find(group);
    int row=std::clamp(storedRow==g_groupRows.end()?0:storedRow->second,0,std::max(0,totalRows-visibleRows));
    int first=row*columns;

    ExpandedGroupGeometry geometry;
    geometry.group=group;geometry.area={x,y,x+width,y+height};
    geometry.title={x+pad,y,x+width-pad,y+header};
    geometry.start=global+first;geometry.firstInGroup=first;
    geometry.count=std::min(std::max(0,count-first),visibleRows*columns);
    geometry.columns=columns;geometry.entryOrder=order;
    geometry.cellWidth=side?side:p.WholeCells()?GroupPitchX():GroupIconPitchX();
    geometry.rowPitch=side?side:p.WholeCells()?GroupPitchY():GroupIconPitchY();
    geometry.visibleRows=visibleRows;geometry.wholeCells=p.WholeCells()||side;geometry.squareCells=side>0;
    return geometry;
}
D2D1_POINT_2F GroupCellsOrigin(const ExpandedGroupGeometry& geometry) {
    if(geometry.wholeCells)return {geometry.area.left,geometry.title.bottom};
    return {(geometry.area.left+geometry.area.right-geometry.columns*geometry.cellWidth)/2,
            geometry.title.bottom+kGroupPadding*g_layout.scale};
}
Cell MakeExpandedGroupCell(const ExpandedGroupGeometry& geometry,int slot) {
    auto origin=GroupCellsOrigin(geometry);
    float x=origin.x+(slot%geometry.columns)*geometry.cellWidth;
    float y=origin.y+(slot/geometry.columns)*geometry.rowPitch;
    auto cell=geometry.squareCells?MakeSquareCell(x,y,geometry.cellWidth):
        MakeCell(x,y,geometry.cellWidth,geometry.rowPitch,0);
    float pad=geometry.wholeCells?0:kGroupPadding*g_layout.scale;
    for(auto rect:{&cell.label,&cell.highlight}) {
        rect->left=std::max(rect->left,geometry.area.left+pad);
        rect->right=std::min(rect->right,geometry.area.right-pad);
    }
    const auto& group=*geometry.group;
    const Tile collapsed{nullptr,geometry.group};
    return SpaceTileContents(group.collapsed?collapsed:group.apps[geometry.firstInGroup+slot],cell);
}
std::optional<ExpandedGroupGeometry> GroupResizePreviewGeometry() {
    const auto& gesture=g_groupGesture;
    if(!gesture.moved||!gesture.resize||!gesture.valid)return std::nullopt;
    auto current=std::find_if(g_groupGeometry.begin(),g_groupGeometry.end(),[&](const auto& geometry){
        return geometry.group==gesture.group&&geometry.start==gesture.activeStart;
    });
    if(current==g_groupGeometry.end())return std::nullopt;
    auto before=PositionFromArea(current->area);
    const auto& draft=gesture.draft;
    if(std::abs(before.w-draft.w)<.01f&&std::abs(before.h-draft.h)<.01f)return std::nullopt;
    return MakeFreeGroupGeometry(gesture.group,draft,current->start-current->firstInGroup,current->entryOrder);
}
void ComputeFreeGroups() {
    if(g_groupPositions.empty())return;
    auto& L=g_layout;
    float bottom=L.contentHeight,right=L.pages*(float)L.width;
    int global=0,order=0;
    for(const auto& tile:g_tiles)if(auto group=tile.folder;GroupVisible(group)) {
        auto found=g_groupPositions.find(group);
        if(found!=g_groupPositions.end()) {
            auto geometry=MakeFreeGroupGeometry(group,found->second,global,order);
            g_groupGeometry.push_back(geometry);
            for(int i=0;i<geometry.count&&geometry.start+i<(int)g_groupCells.size();++i) {
                g_groupCells[geometry.start+i]=MakeExpandedGroupCell(geometry,i);
                if(!group->collapsed)if(auto app=group->apps[geometry.firstInGroup+i].app)g_groupEntryOrder[app.get()]=order;
            }
            bottom=std::max(bottom,geometry.area.bottom-L.viewTop);
            right=std::max(right,geometry.area.right);
        }
        global+=group->collapsed?1:(int)group->apps.size();
        ++order;
    }
    L.contentHeight=bottom;
    L.pages=std::max(1,(int)std::ceil((right-.01f)/std::max(1,L.width)));
}
void ComputeWholeCellGroups() {
    auto& L=g_layout;
    const float w=GroupPitchX(),h=GroupPitchY();
    const int gapX=(int)std::ceil(g_settings.groupGap*L.scale/w);
    const int gapY=(int)std::ceil(g_settings.groupGap*L.scale/h);
    const int available=std::max(1,(int)std::floor(L.width/w));
    const int viewRows=std::max(2,(int)std::floor((L.viewBottom-L.viewTop)/h));
    int visibleGroups=(int)std::count_if(g_tiles.begin(),g_tiles.end(),[](const Tile& tile){return GroupVisible(tile.folder);});
    float preferredWidth=g_settings.groupWidth>0?g_settings.groupWidth*L.scale:
        visibleGroups<=1?L.width:400*L.scale;
    int wanted=std::min(available,std::max(1,(int)std::round(preferredWidth/w)));
    if(g_settings.columns>0)wanted=std::min(wanted,g_settings.columns);
    L.cellWidth=w;L.cellHeight=h;L.cellSpare=0;L.columns=std::min(wanted,available);
    L.contentHeight=0;L.pages=0;
    L.perPage=std::max(1,L.columns*(viewRows-1));
    g_groupGeometry.clear();
    g_groupEntryOrder.clear();
    g_groupCells.assign(g_visible.size(),{});
    ComputeFreeGroups();

    // Keep automatic groups clear of existing manually placed groups.
    int page=g_settings.pages&&!g_groupGeometry.empty()?L.pages:0;
    int y=g_settings.pages||g_groupGeometry.empty()?0:(int)std::ceil((L.contentHeight-.01f)/h)+gapY;
    int left=(int)std::ceil(page*L.width/w-.0001f),x=left,rowHeight=0;
    int global=0,order=0;
    for(const auto& tile:g_tiles)if(auto group=tile.folder;GroupVisible(group)) {
        int count=group->collapsed?1:(int)group->apps.size();
        if(!g_groupPositions.count(group)) {
            int offset=0;
            do {
                int right=(int)std::floor((page+1)*L.width/w+.0001f);
                int columns=std::min(wanted,std::max(1,right-left));
                int rows=std::max(1,(count-offset+columns-1)/columns);
                if(g_settings.pages)rows=std::min(rows,viewRows-1);
                int height=rows+1;
                if(x>left&&(g_settings.groupRows||x+columns>right)) {x=left;y+=rowHeight+gapY;rowHeight=0;}
                if(g_settings.pages&&y+height>viewRows) {
                    ++page;x=left=(int)std::ceil(page*L.width/w-.0001f);y=0;rowHeight=0;
                    right=(int)std::floor((page+1)*L.width/w+.0001f);
                    columns=std::min(wanted,std::max(1,right-left));
                    rows=std::clamp((count-offset+columns-1)/columns,1,viewRows-1);
                    height=rows+1;
                }
                int chunk=std::min(count-offset,rows*columns);
                ExpandedGroupGeometry geometry;
                geometry.group=group;geometry.area={x*w,L.viewTop+y*h,(x+columns)*w,L.viewTop+(y+height)*h};
                geometry.title=geometry.area;geometry.title.bottom=geometry.title.top+h;
                geometry.start=global+offset;geometry.firstInGroup=offset;geometry.count=chunk;
                geometry.columns=columns;geometry.visibleRows=rows;geometry.entryOrder=order;
                geometry.wholeCells=true;geometry.cellWidth=w;geometry.rowPitch=h;
                g_groupGeometry.push_back(geometry);
                for(int i=0;i<chunk;++i) {
                    int index=geometry.start+i;
                    g_groupCells[index]=MakeExpandedGroupCell(geometry,i);
                    if(auto app=g_visible[index].app)g_groupEntryOrder[app.get()]=order;
                }
                L.contentHeight=std::max(L.contentHeight,(y+height)*h);
                L.pages=std::max(L.pages,page+1);
                x+=columns+gapX;rowHeight=std::max(rowHeight,height);
                offset+=chunk;
            } while(offset<count);
        }
        global+=count;++order;
    }
    L.pages=std::max(1,L.pages);
}
void ComputeSquareGroups() {
    auto& L=g_layout;
    const float side=SquareCellSize(g_squareGridColumns);
    const float quarter=side/4;
    const int columnsPerPage=g_squareGridColumns;
    const int availableY=std::max(6,(int)std::floor((L.viewBottom-L.viewTop+.01f)/quarter));
    const int maxRows=(availableY-2)/4;
    const int gapX=(int)std::ceil(g_settings.groupGap*L.scale/(side/2));
    const int gapY=(int)std::ceil(g_settings.groupGap*L.scale/quarter);
    int groups=(int)std::count_if(g_tiles.begin(),g_tiles.end(),[](const Tile& tile){return GroupVisible(tile.folder);});
    float preferred=g_settings.groupWidth>0?g_settings.groupWidth*L.scale:
        (groups<=1||g_settings.groupRows)?columnsPerPage*side:400*L.scale;
    int columns=std::clamp((int)std::round(preferred/side),1,columnsPerPage);

    L.contentHeight=0;L.pages=0;
    g_groupGeometry.clear();g_groupEntryOrder.clear();g_groupCells.assign(g_visible.size(),{});
    ComputeFreeGroups();
    int page=g_settings.pages&&!g_groupGeometry.empty()?L.pages:0;
    int y=g_settings.pages||g_groupGeometry.empty()?0:(int)std::ceil((L.contentHeight-.01f)/quarter)+gapY;
    int x=0,rowHeight=0,global=0,order=0;
    for(const auto& tile:g_tiles)if(auto group=tile.folder;GroupVisible(group)) {
        int count=group->collapsed?1:(int)group->apps.size();
        if(!g_groupPositions.count(group)) {
            int offset=0;
            do {
                int rows=std::max(1,(count-offset+columns-1)/columns);
                if(g_settings.pages)rows=std::min(rows,maxRows);
                int height=rows*4+2;
                if(x&&(g_settings.groupRows||x+columns*2>columnsPerPage*2)) {x=0;y+=rowHeight+gapY;rowHeight=0;}
                if(g_settings.pages&&y+height>availableY) {++page;x=0;y=0;rowHeight=0;}
                auto position=GroupPosition{};
                position.style=5;position.snapped=true;position.gridX=page*columnsPerPage*2+x;
                position.gridY=y;position.gridW=columns*2;position.gridH=height;
                auto geometry=MakeFreeGroupGeometry(group,position,global,order);
                int chunk=std::min(count-offset,rows*columns);
                geometry.firstInGroup=offset;geometry.start=global+offset;geometry.count=chunk;
                g_groupGeometry.push_back(geometry);
                for(int i=0;i<chunk;++i) {
                    int index=geometry.start+i;
                    g_groupCells[index]=MakeExpandedGroupCell(geometry,i);
                    if(auto app=g_visible[index].app)g_groupEntryOrder[app.get()]=order;
                }
                L.contentHeight=std::max(L.contentHeight,(y+height)*quarter);
                L.pages=std::max(L.pages,page+1);
                x+=columns*2+gapX;rowHeight=std::max(rowHeight,height);
                offset+=chunk;
            } while(offset<count);
        }
        global+=count;++order;
    }
    L.pages=std::max(1,L.pages);
}
void ComputeGroups() {
    if(g_squareGridColumns){ComputeSquareGroups();return;}
    if(g_wholeCellLayout){ComputeWholeCellGroups();return;}
    auto& L = g_layout;
    float s = L.scale, gap = g_settings.groupGap * s, pad = 12 * s, header = LegacyGroupTitleHeight();
    float margin = std::max(24 * s, L.width * 0.045f);
    float available = std::max(L.icon + 2 * pad, L.width - 2 * margin);
    float wanted = g_settings.groupWidth > 0 ? g_settings.groupWidth * s :
        (g_tiles.size() <= 1 ? available : 400 * s);
    wanted = std::max(wanted, L.icon + 2 * pad);
    int across = g_settings.groupRows ? 1 : std::max(1, (int)((available + gap) / (wanted + gap)));
    float groupWidth = (available - (across - 1) * gap) / across;
    int fitColumns=FittingGroupColumns(groupWidth);
    int columns=g_settings.columns>0?std::min(g_settings.columns,fitColumns):fitColumns;
    L.columns=columns;
    L.cellWidth=GroupIconPitchX();
    L.cellHeight=GroupIconPitchY();
    L.cellSpare = 0;
    float viewHeight = std::max(L.cellHeight + header + 2 * pad, L.viewBottom - L.viewTop);
    int maxRows = std::max(1, (int)((viewHeight - header - 2 * pad) / L.cellHeight));
    int capacity = columns * maxRows;
    g_groupGeometry.clear();
    g_groupEntryOrder.clear();
    g_groupCells.assign(g_visible.size(), {});
    float y = 0, rowHeight = 0;
    int col = 0, page = 0, global = 0, order = 0;
    bool custom=g_freeGroupLayout||g_groupEditMode;
    bool allCustom=custom&&std::all_of(g_tiles.begin(),g_tiles.end(),[](const Tile& tile){
        return !GroupVisible(tile.folder)||g_groupPositions.count(tile.folder);
    });
    for (const auto& tile : g_tiles) {
        if (!GroupVisible(tile.folder)||allCustom) continue;
        int count = tile.folder->collapsed?1:(int)tile.folder->apps.size(), offset = 0;
        bool override=custom&&g_groupPositions.count(tile.folder);
        do {
            int chunk = g_settings.pages ? std::min(capacity, count - offset) : count;
            int rows = std::max(1, (chunk + columns - 1) / columns);
            float height = header + 2 * pad + rows * L.cellHeight;
            if (col >= across) { col = 0; y += rowHeight + gap; rowHeight = 0; }
            if (g_settings.pages && y > 0 && y + height > viewHeight) {
                ++page; col = 0; y = 0; rowHeight = 0;
            }
            float x = margin + col * (groupWidth + gap) + page * (float)L.width;
            float top = L.viewTop + y;
            ExpandedGroupGeometry geometry;
            geometry.group = tile.folder;
            geometry.area = {x, top, x + groupWidth, top + height};
            geometry.title = {x + pad, top, x + groupWidth - pad, top + header};
            geometry.start = global + offset; geometry.firstInGroup = offset;
            geometry.count = chunk; geometry.columns = columns;
            geometry.cellWidth=L.cellWidth;geometry.rowPitch=L.cellHeight;geometry.visibleRows=rows;
            geometry.entryOrder = order;
            if(!override)g_groupGeometry.push_back(geometry);
            for (int i = 0; !override&&i < chunk; i++) {
                int index = global + offset + i;
                if (index < (int)g_groupCells.size()) g_groupCells[index] = MakeExpandedGroupCell(geometry,i);
                if (index < (int)g_visible.size() && g_visible[index].app) g_groupEntryOrder[g_visible[index].app.get()]=order;
            }
            ++col; rowHeight = std::max(rowHeight, height);
            offset += chunk;
        } while (offset < count);
        global += tile.folder->collapsed?1:count;
        ++order;
    }
    L.contentHeight = y + rowHeight;
    L.pages = allCustom?0:page+1;
    L.perPage = std::max(1, capacity * across);
    if(custom)ComputeFreeGroups();
    L.pages=std::max(1,L.pages);
}
float MaxPagePan(int page) {
    if(!g_settings.pages)return 0;
    if(!GroupedView())return g_visible.empty()?0:std::max(0.f,g_layout.viewTop+
        g_layout.visibleRows*g_layout.cellHeight-g_layout.viewBottom);
    if(!g_squareGridColumns)return 0;
    float start=page*(float)g_layout.width,end=start+g_layout.width,bottom=g_layout.viewBottom;
    for(const auto& geometry:g_groupGeometry)
        if(geometry.area.left>=start&&geometry.area.left<end)bottom=std::max(bottom,geometry.area.bottom);
    return bottom-g_layout.viewBottom;
}
Cell ProjectGroupCell(Cell c) {
    c.icon = ProjectGroupRect(c.icon);
    c.label = ProjectGroupRect(c.label);
    c.highlight = ProjectGroupRect(c.highlight);
    c.unit = ProjectGroupRect(c.unit);
    return c;
}
Cell GroupCellAt(int index) {
    if(index<0||index>=(int)g_groupCells.size())return {};
    return ProjectGroupCell(g_groupCells[index]);
}
bool GroupIconFits(const Cell& cell,const D2D1_RECT_F& area) {
    const auto& icon=cell.icon;
    return icon.left>=area.left&&icon.top>=area.top&&icon.right<=area.right&&icon.bottom<=area.bottom;
}
void EnsureFreeGroupVisible(int index) {
    if(!g_freeGroupLayout)return;
    for(const auto& geometry:g_groupGeometry) {
        int base=geometry.start-geometry.firstInGroup;
        if(index<base||index>=base+(geometry.group->collapsed?1:(int)geometry.group->apps.size()))continue;
        int row=(index-base)/geometry.columns;
        int next=std::clamp(g_groupRows[geometry.group],row-geometry.visibleRows+1,row);
        if(next!=g_groupRows[geometry.group]){g_groupRows[geometry.group]=next;ComputeGroups();}
        break;
    }
}
int GroupIndexAt(int x, int y) {
    if (y < g_layout.viewTop || y >= g_layout.viewBottom) return -1;
    for (int i = 0; i < (int)g_groupCells.size(); i++)
        if (GroupContains(GroupCellAt(i).highlight, (float)x, (float)y)) return i;
    return -1;
}
int GroupAtPoint(int x, int y, bool titleOnly = false) {
    if (!GroupedView() || y < g_layout.viewTop || y >= g_layout.viewBottom) return -1;
    for (int i = 0; i < (int)g_groupGeometry.size(); i++)
        if (GroupContains((titleOnly ? ProjectGroupRect(g_groupGeometry[i].title) : VisualGroupRect(g_groupGeometry[i])), (float)x, (float)y)) return i;
    return -1;
}
// Group headers render like the app labels: white semi-bold text over a soft
// shadow, pre-rendered once per name and size. Keyed by name: a rename simply
// misses, and two same-named groups can safely share one bitmap.
struct GroupTitleRender {
    std::wstring name;
    float width=0,height=0;
    UINT version=0;
    bool shrink=false;
    ComPtr<IDWriteTextLayout> layout;
    LabelShadowBitmap shadow;
};
[[clang::no_destroy]] std::map<std::wstring, GroupTitleRender> g_groupTitleRenders;

void DrawGroupHeaders(bool activeOnly,const ExpandedGroupGeometry* resizePreview) {
    const bool flying = FlyInActive();
    for(const auto& current:g_groupGeometry) {
        bool active=g_groupGesture.moved&&current.group==g_groupGesture.group&&
            current.start==g_groupGesture.activeStart;
        if(active!=activeOnly||(g_groupGesture.moved&&current.group==g_groupGesture.group&&!active))continue;
        const auto& geometry=active&&resizePreview?*resizePreview:current;
        auto area=VisualGroupRect(geometry);
        float pad=geometry.wholeCells?0:kGroupPadding*g_layout.scale;
        auto title=D2D1::RectF(area.left+pad,area.top,area.right-pad,
            std::min(area.bottom,area.top+geometry.title.bottom-geometry.title.top));
        if (area.bottom < g_layout.viewTop || area.top > g_layout.viewBottom || area.right < 0 || area.left > g_layout.width) continue;
        if(g_groupEditMode) {
            float s=g_layout.scale;
            // A small inset keeps the outlines distinct when group bounds touch.
            float inset=std::min({2.5f*s,(area.right-area.left)/4,(area.bottom-area.top)/4});
            auto outline=D2D1::RectF(area.left+inset,area.top+inset,area.right-inset,area.bottom-inset);
            g_brush->SetColor(D2D1::ColorF(active?0xEAF5FF:0x587187,.96f));
            g_target->DrawRoundedRectangle(D2D1::RoundedRect(outline,5*s,5*s),g_brush.Get(),1.6f*s);
            auto color=D2D1::ColorF(0xFFFFFF);
            float glow=geometry.group==g_groupResizeHover?g_groupResizeGlow:0;
            if(geometry.group==g_groupGesture.group&&g_groupGesture.resize)glow=1;
            float midX=area.right-12*s,midY=area.bottom-12*s,arm=(4+1.5f*glow)*s;
            color.a=.52f+.48f*glow;g_brush->SetColor(color);
            for(float sign:{-1.f,1.f}) {
                float x=midX+sign*arm,y=midY+sign*arm;
                auto end=D2D1::Point2F(x,y);
                g_target->DrawLine(D2D1::Point2F(midX,midY),end,g_brush.Get(),1.4f*s);
                g_target->DrawLine(end,D2D1::Point2F(x-sign*arm*.8f,y),g_brush.Get(),1.4f*s);
                g_target->DrawLine(end,D2D1::Point2F(x,y-sign*arm*.8f),g_brush.Get(),1.4f*s);
            }
        }
        float progress=GroupEntryProgress(geometry.entryOrder);
        float shift=(1-progress)*30*g_layout.scale;
        title.left+=shift; title.right+=shift;
        float dy = 0.f;
        if (flying) {
            auto anchor = geometry.count ? CellAt(geometry.start).icon : title;
            auto pose = FlyInPose({(anchor.left+anchor.right)*.5f, (anchor.top+anchor.bottom)*.5f},
                                   g_settings.iconSize*g_layout.scale);
            progress = pose.opacity * launchpad_fly_in::SmoothStep((g_appear - .13f) / .36f);
            if (progress <= 0) continue;
            // Tie the header to its first tile's phase with a shorter travel.
            shift = .40f * pose.dx;
            dy = .40f * pose.dy;
            title.left += shift; title.right += shift;
            title.top += dy; title.bottom += dy;
        }
        std::wstring name=geometry.group->name;
        if (!geometry.group->unavailable.empty()) name+=L" · "+std::to_wstring(geometry.group->unavailable.size())+Tr(L" unavailable");
        float titleOpacity=geometry.group==g_groupDropFolder||geometry.group==g_groupHeaderDragging?1.f:.90f;
        float titleHeight=title.bottom-title.top;
        if(!g_groupFormat||titleHeight<=0||title.right<=title.left)continue;
        float fontSize=g_groupFormat->GetFontSize();
        bool shrink=geometry.squareCells&&titleHeight<fontSize*1.25f;
        auto& cached=g_groupTitleRenders[name];
        float titleWidth=title.right-title.left;
        if(!cached.layout||cached.version!=g_formatVersion||cached.name!=name||
           cached.width!=titleWidth||cached.height!=titleHeight||cached.shrink!=shrink) {
            cached.name=name;cached.width=titleWidth;cached.height=titleHeight;
            cached.version=g_formatVersion;cached.shrink=shrink;
            cached.layout.Reset();cached.shadow.bitmap.Reset();
            if(SUCCEEDED(g_dwrite->CreateTextLayout(name.c_str(),(UINT32)name.size(),g_groupFormat.Get(),
                titleWidth,titleHeight,&cached.layout))) {
                if(shrink)cached.layout->SetFontSize(std::max(1.f,titleHeight/1.25f),{0,(UINT32)name.size()});
                RenderTextWithShadow(cached.layout.Get(),titleWidth,titleHeight,cached.shadow);
            } else cached.layout.Reset();
        }
        if (cached.shadow.bitmap) {
            g_target->DrawBitmap(cached.shadow.bitmap.Get(),
                D2D1::RectF(title.left-cached.shadow.padX,title.top-cached.shadow.padTop,
                            title.right+cached.shadow.padX,title.bottom+cached.shadow.padBottom),
                progress*titleOpacity,D2D1_INTERPOLATION_MODE_LINEAR);
        } else {
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF,progress*titleOpacity));
            if (cached.layout) g_target->DrawTextLayout({title.left,title.top},cached.layout.Get(),g_brush.Get(),D2D1_DRAW_TEXT_OPTIONS_CLIP);
            else g_target->DrawText(name.c_str(),(UINT32)name.size(),g_groupFormat.Get(),title,g_brush.Get(),D2D1_DRAW_TEXT_OPTIONS_CLIP);
        }
        if (!geometry.count) {
            auto hint = area; hint.top = title.bottom;
            if (flying) {
                hint.left += shift; hint.right += shift; hint.bottom += dy;
            }
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF,flying?progress:1.f));
            PCWSTR text = Tr(g_visible.empty() ? L"Search for an app, then use Move to group" : L"Drop apps here");
            g_target->DrawText(text, (UINT32)wcslen(text), g_tipFormat.Get(), hint, g_brush.Get());
        }
        if(g_freeGroupLayout && !geometry.group->collapsed && (int)geometry.group->apps.size()>geometry.visibleRows*geometry.columns) {
            auto hint=area;hint.top=hint.bottom-16*g_layout.scale;
            if (flying) {
                hint.left += shift; hint.right += shift; hint.top += dy; hint.bottom += dy;
            }
            std::wstring text=std::to_wstring(geometry.firstInGroup+1)+L"–"+std::to_wstring(geometry.firstInGroup+geometry.count)+L" / "+std::to_wstring(geometry.group->apps.size());
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF,flying?progress:1.f));g_target->DrawText(text.c_str(),(UINT32)text.size(),g_tipFormat.Get(),hint,g_brush.Get());
        }
        if (geometry.group == g_groupDropFolder && g_groupDropSlot >= geometry.firstInGroup &&
            g_groupDropSlot <= geometry.firstInGroup + geometry.count) {
            int slot = g_groupDropSlot - geometry.firstInGroup;
            auto origin=GroupCellsOrigin(geometry);
            float x=std::max(area.left,origin.x+area.left-geometry.area.left+(slot%geometry.columns)*geometry.cellWidth);
            float y=origin.y+area.top-geometry.area.top+(slot/geometry.columns)*geometry.rowPitch;
            auto color=D2D1::ColorF(0xFFFFFF,.75f);
            Fill({x,y,x+3*g_layout.scale,y+g_layout.icon+16*g_layout.scale},color,1.5f*g_layout.scale);
        }
    }
}
void DrawGroupGrid() {
    if(!g_groupEditMode||!g_squareGridColumns)return;
    const auto& L=g_layout;
    float side=SquareCellSize(g_squareGridColumns),margin=GridMargin(),s=L.scale;
    float scroll=g_settings.pages?g_groupPagePan:g_scroll;
    int row=std::max(0,(int)std::ceil(scroll/side));
    g_brush->SetColor(D2D1::ColorF(0x64778A,.32f));
    int page=g_settings.pages?std::max(0,(int)(g_scroll/L.width)):0;
    for(int i=0;i<(g_settings.pages?2:1);++i) {
        float left=margin+(g_settings.pages?(page+i)*L.width-g_scroll:0);
        float right=left+g_squareGridColumns*side;
        for(int column=0;column<=g_squareGridColumns;++column) {
            float x=left+column*side;
            g_target->DrawLine({x,L.viewTop},{x,L.viewBottom},g_brush.Get(),s);
        }
        for(float y=L.viewTop+row*side-scroll;y<=L.viewBottom;y+=side) {
            g_target->DrawLine({left,y},{right,y},g_brush.Get(),s);
        }
    }
}
void DrawGroupEditPreview() {
    const auto& gesture=g_groupGesture;
    if(!g_groupEditMode||!gesture.group||!gesture.moved)return;
    float s=g_layout.scale;
    auto color=D2D1::ColorF(gesture.valid?0xFFFFFF:0xC8C8C8,.75f);
    auto r=ProjectGroupRect(GroupPositionRect(gesture.continuous));
    if(gesture.valid) {
        auto target=ProjectGroupRect(GroupPositionRect(gesture.draft));
        float distance=std::max({std::abs(r.left-target.left),std::abs(r.top-target.top),
                                 std::abs(r.right-target.right),std::abs(r.bottom-target.bottom)});
        if(distance>4*s) {
            g_brush->SetColor(D2D1::ColorF(0x71C7FF));
            g_target->DrawRoundedRectangle(D2D1::RoundedRect(target,5*s,5*s),g_brush.Get(),1.8f*s);
        }
    }
    if(!gesture.resize&&gesture.valid)return;
    std::wstring text=gesture.valid?
        std::to_wstring(gesture.draft.gridW/2)+L" × "+std::to_wstring((gesture.draft.gridH-2)/4):
        Tr(L"Invalid placement: release to keep previous position");
    float width=std::min((gesture.valid?82.f:252.f)*s,g_layout.width-16*s);
    float right=std::clamp(r.right-8*s,width+8*s,g_layout.width-8*s);
    float bottom=std::clamp(r.bottom-8*s,g_layout.viewTop+34*s,g_layout.viewBottom-4*s);
    auto label=D2D1::RectF(right-width,bottom-26*s,right,bottom);
    Fill(label,D2D1::ColorF(0x303030,.92f),6*s);
    g_brush->SetColor(color);
    g_target->DrawText(text.c_str(),(UINT32)text.size(),g_tipFormat.Get(),label,g_brush.Get(),
                       D2D1_DRAW_TEXT_OPTIONS_CLIP);
}

Cell GroupVisualCell(const ExpandedGroupGeometry& geometry,Cell c) {
    auto visual=VisualGroupRect(geometry);
    auto base=ProjectGroupRect(geometry.area);
    float dx=visual.left-base.left,dy=visual.top-base.top;
    for(auto rect:{&c.icon,&c.label,&c.highlight,&c.unit}) {
        rect->left+=dx;rect->right+=dx;rect->top+=dy;rect->bottom+=dy;
    }
    return c;
}
