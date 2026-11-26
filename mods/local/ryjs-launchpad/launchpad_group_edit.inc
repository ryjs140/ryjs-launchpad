bool GroupsOverlap(const GroupPosition& a,const GroupPosition& b,float gap=0) {
    return a.x<b.x+b.w+gap-.01f&&b.x<a.x+a.w+gap-.01f&&
        a.y<b.y+b.h+gap-.01f&&b.y<a.y+a.h+gap-.01f;
}
bool SameGroupBounds(const GroupPosition& a,const GroupPosition& b) {
    return std::abs(a.x-b.x)<.01f&&std::abs(a.y-b.y)<.01f&&
        std::abs(a.w-b.w)<.01f&&std::abs(a.h-b.h)<.01f;
}
bool g_groupClosePrompt=false;
int g_groupEditButton=-1;
void CancelGroupMarquee() {
    if (g_groupMarquee.pending) g_groupSelection = std::move(g_groupMarquee.before);
    g_groupMarquee = {};
}
void ResetGroupGesture(bool clearTransitions) {
    CancelGroupMarquee();
    g_groupGesture={};
    if(clearTransitions)g_groupTransitions.clear();
    g_groupEditButton=-1;
    if(g_wnd&&GetCapture()==g_wnd)ReleaseCapture();
}
void EndGroupEdit() {
    g_groupSelection.clear();
    g_groupResizeHover.reset();g_groupResizeGlow=0;
    g_groupEditMode=false;g_groupEditDirty=false;
    g_groupEditSnapshot.clear();g_groupEditRows.clear();
    g_pressed=-1;g_pressedEmpty=false;Refresh();
}
void CancelGroupEdit() {
    if(!g_groupEditMode)return;
    ResetGroupGesture(true);
    g_groupPositions=std::move(g_groupEditSnapshot);g_groupRows=std::move(g_groupEditRows);
    g_freeGroupLayout=g_groupEditWasFree;g_wholeCellLayout=g_groupEditWasWholeCell;g_squareGridColumns=g_groupEditPreviousColumns;
    EndGroupEdit();
}
bool FinishGroupEdit() {
    if(!g_groupEditMode)return true;
    if (!g_groupEditDirty) { CancelGroupEdit(); return true; }
    ResetGroupGesture(true);
    if(!SaveGroupPositions(true)) {
        bool modal=std::exchange(g_modal,true);
        MessageBoxW(g_wnd,Tr(L"Could not save group geometry."),Tr(L"Edit group layout"),MB_OK|MB_ICONWARNING);
        g_modal=modal;
        Refresh();return false;
    }
    EndGroupEdit();return true;
}
void AlignGroupsToMainGrid(int columns, const std::vector<ExpandedGroupGeometry>& oldGeometry,
                           bool responsive);
void ToggleGroupEdit() {
    if(g_groupEditMode){FinishGroupEdit();return;}
    if(g_groupPositionsReadOnly||g_layoutReadOnly||g_dataReadOnly||!GroupedView())return;
    UpdateFilter();
    ComputeLayout();
    auto oldGeometry = g_groupGeometry;
    const bool responsive = !g_layout.useSavedGroupGeometry;
    const int columns = FitSquareGridColumns(DesiredGridColumns(), true);
    g_groupEditSnapshot=g_groupPositions;g_groupEditRows=g_groupRows;
    g_groupEditPreviousColumns=g_squareGridColumns;
    g_groupEditWasWholeCell=g_wholeCellLayout;g_groupEditWasFree=g_freeGroupLayout;g_groupEditDirty=false;
    ClearAppSelection();
    g_groupEditMode=true;
    AlignGroupsToMainGrid(columns, oldGeometry, responsive);
    g_pressed=-1;g_pressedEmpty=false;Refresh();
}
bool ConfirmGroupEditClose() {
    if(g_groupClosePrompt)return false;
    if(!g_groupEditMode||!g_groupEditDirty)return true;
    ResetGroupGesture(true);Render();
    const TASKDIALOG_BUTTON buttons[]{
        {IDYES,Tr(L"Save layout")},{IDNO,Tr(L"Discard changes")},{IDCANCEL,Tr(L"Continue editing")}
    };
    TASKDIALOGCONFIG dialog{};
    dialog.cbSize=sizeof(dialog);dialog.hwndParent=g_wnd;
    dialog.dwFlags=TDF_ALLOW_DIALOG_CANCELLATION|TDF_POSITION_RELATIVE_TO_WINDOW;
    dialog.pszWindowTitle=Tr(L"Edit group layout");
    dialog.pszMainInstruction=Tr(L"Save changes to the group layout?");
    dialog.pButtons=buttons;dialog.cButtons=ARRAYSIZE(buttons);dialog.nDefaultButton=IDCANCEL;
    bool modal=std::exchange(g_modal,true);
    g_groupClosePrompt=true;
    int choice=IDCANCEL;
    // Resolved dynamically on purpose. A static import of TaskDialogIndirect
    // needs comctl32 v6, which only processes with the side-by-side manifest
    // get; everywhere else the whole module fails to load with
    // STATUS_ENTRYPOINT_NOT_FOUND (the test host does, and so can explorer).
    using TaskDialogProc=decltype(&TaskDialogIndirect);
    bool shown=false;
    if(HMODULE controls=LoadLibraryW(L"comctl32.dll")) {
        auto show=reinterpret_cast<TaskDialogProc>(GetProcAddress(controls,"TaskDialogIndirect"));
        shown=show&&SUCCEEDED(show(&dialog,&choice,nullptr,nullptr));
        FreeLibrary(controls);
    }
    if(!shown)choice=MessageBoxW(g_wnd,Tr(L"Save layout? Yes saves, No discards, Cancel continues editing."),
        dialog.pszWindowTitle,MB_YESNOCANCEL|MB_DEFBUTTON3|MB_ICONQUESTION);
    if(choice==IDNO)CancelGroupEdit();
    bool close=choice==IDNO||(choice==IDYES&&FinishGroupEdit());
    g_groupClosePrompt=false;g_modal=modal;
    return close;
}
int GroupResizeAt(int x,int y) {
    if(!g_groupEditMode||!GroupedView()||y<g_layout.viewTop||y>=g_layout.viewBottom)return -1;
    for(int i=(int)g_groupGeometry.size()-1;i>=0;--i)
        if(GroupContains(GroupResizeHit(VisualGroupRect(g_groupGeometry[i])),(float)x,(float)y))return i;
    return -1;
}
void UpdateGroupResizeHover(int x,int y) {
    int index=GroupResizeAt(x,y);
    auto group=index<0?nullptr:g_groupGeometry[index].group;
    if(group==g_groupResizeHover)return;
    g_groupResizeHover=group;g_groupResizeGlow=0;StartFrames();
}
std::array<D2D1_RECT_F,3> GroupEditButtons() {
    float s=g_layout.scale;
    float width=std::min(174*s,(g_layout.width-28*s)/3);
    float gap=6*s,left=(g_layout.width-3*width-2*gap)/2;
    float bottom=g_layout.height-12*s;
    if(!g_layout.powerItems.empty())bottom=g_layout.powerBar.top-6*s;
    std::array<D2D1_RECT_F,3> buttons{};
    for(auto& button:buttons) {
        button={left,bottom-36*s,left+width,bottom};
        left+=width+gap;
    }
    return buttons;
}
int GroupEditButtonAt(int x,int y) {
    if(!g_groupEditMode)return -1;
    auto buttons=GroupEditButtons();
    for(int i=0;i<(int)buttons.size();++i)
        if(GroupContains(buttons[i],(float)x,(float)y))return i;
    return -1;
}
int GroupEditHover() {
    return g_glassPointer ? GroupEditButtonAt((int)g_glassPointer->x, (int)g_glassPointer->y) : -1;
}

void DrawGroupEditBar() {
    if (!g_groupEditMode) return;
    const auto buttons = GroupEditButtons();
    const int hovered = GroupEditHover();
    const float s = g_layout.scale;
    const PCWSTR labels[]{Tr(L"Cancel"), Tr(L"Equalize group spacing"), Tr(L"Finish and save")};
    const D2D1_RECT_F panel{buttons.front().left - 5 * s, buttons.front().top - 4 * s,
                           buttons.back().right + 5 * s, buttons.back().bottom + 4 * s};
    LaunchpadMenu::DrawPanelSurface(panel, 13 * s, s, LaunchpadMenu::PanelBackground(g_material.average));
    for (int i = 0; i < (int)buttons.size(); ++i) {
        const auto rect = buttons[i];
        const bool hover = i == hovered, pressed = hover && g_groupEditButton == i;
        const bool primary = i == 2;
        const float radius = 9 * s;
        Fill(rect, D2D1::ColorF(0xFFFFFF, pressed ? .05f : hover ? .19f : primary ? .13f : .035f), radius);
        if (primary || hover) {
            const float inset = .5f * s;
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF, hover ? .44f : .26f));
            g_target->DrawRoundedRectangle(D2D1::RoundedRect(
                {rect.left + inset, rect.top + inset, rect.right - inset, rect.bottom - inset},
                radius - inset, radius - inset), g_brush.Get(), s);
        }
        if (hover)
            DrawControlGlass(g_target.Get(), g_tileGlass, rect, radius, s, *g_glassPointer,
                             pressed ? .4f : .66f + .34f * g_settings.glowStrength);
        const float offset = pressed ? s : 0;
        DrawLauncherText(labels[i], g_groupFormat.Get(),
                         {rect.left + 7 * s, rect.top + offset, rect.right - 7 * s, rect.bottom + offset});
    }
}

int GroupGridIndex(float value,float step) {
    return (int)std::clamp(std::round(value/step),0.f,1000000.f);
}
GroupPosition MoveGroupToGrid(GroupPosition p,int x,int y) {
    p.snapped=true;p.gridX=x;p.gridY=y;
    return EffectiveGroupPosition(p);
}
int GroupXIndex(float x,int page) {
    float local=x-page*g_layout.width/g_layout.scale-GridMargin()/g_layout.scale;
    return page*2*g_squareGridColumns+GroupGridIndex(local,GroupStepX());
}
GroupPosition EditorGroupPosition(const ExpandedGroupGeometry& geometry) {
    GroupPosition position;
    position.style=5;
    position.gridW=geometry.columns*2;position.gridH=geometry.visibleRows*4+2;
    auto area=PositionFromArea(geometry.area);
    int page=g_settings.pages?std::max(0,(int)std::floor(geometry.area.left/g_layout.width)):0;
    return MoveGroupToGrid(position,GroupXIndex(area.x,page),GroupGridIndex(area.y,GroupStepY()));
}
bool GroupAxisSharesSpan(float a,float aSize,float b,float bSize) {
    return a<b+bSize-.01f&&b<a+aSize-.01f;
}
struct GroupEditBounds {
    float width,height;
    int pages;
    float margin=0;
    float PageLeft(float x) const {return g_settings.pages?std::floor(x/width)*width:0;}
    bool Fits(const GroupPosition& p) const {
        float left=PageLeft(p.x+.001f);
        return p.x>=left+margin-.01f&&p.y>=0&&left<pages*width&&p.x+p.w<=left+width-margin+.01f&&p.y+p.h<=height+.01f;
    }
};
GroupEditBounds GroupGestureBounds() {
    const auto& L = g_layout;
    float height = g_settings.pages ? std::max(L.groupPageBottom - L.viewTop, L.contentHeight)
                                    : L.viewBottom - L.viewTop + MaxScroll();
    return {g_layout.width/g_layout.scale,height/L.scale,
        g_settings.pages?g_layout.pages:1,GridMargin()/g_layout.scale};
}

void AlignGroupsToMainGrid(int columns, const std::vector<ExpandedGroupGeometry>& oldGeometry,
                           bool responsive) {
    if(g_squareGridColumns==columns&&std::all_of(g_groupPositions.begin(),g_groupPositions.end(),
        [](const auto& entry){return entry.second.SquareCells();}))return;
    g_groupPositions.clear();
    g_squareGridColumns=columns;
    g_wholeCellLayout=true;
    ComputeLayout();
    auto bounds=GroupGestureBounds();
    const float sx=GroupStepX(),sy=GroupStepY();
    const int steps=2*columns;
    std::map<std::shared_ptr<Folder>,int> segments;
    for(const auto& geometry:oldGeometry)++segments[geometry.group];

    std::vector<GroupPosition> placed;
    for(const auto& geometry:oldGeometry) {
        if(segments[geometry.group]!=1)continue;
        auto old=PositionFromArea(geometry.area);
        int page=(int)std::floor(bounds.PageLeft(old.x)/bounds.width+.01f);
        int left=page*steps,right=left+steps;
        int bottom=(int)std::floor((bounds.height+.001f)/sy);
        GroupPosition grid;
        grid.style=5;
        int width=std::min(geometry.columns,columns);
        grid.gridW=width*2;
        int rows=std::max(geometry.visibleRows,(geometry.count+width-1)/width);
        if(g_settings.pages)rows=std::min(rows,std::max(1,(bottom-2)/4));
        grid.gridH=rows*4+2;
        int x=std::clamp(GroupXIndex(old.x,page),left,right-grid.gridW);
        int y=GroupGridIndex(old.y,sy);
        if(g_settings.pages)y=std::min(y,std::max(0,bottom-grid.gridH));
        auto position=MoveGroupToGrid(grid,x,y);
        auto overlaps=[&](const GroupPosition& candidate) {
            return std::any_of(placed.begin(),placed.end(),[&](const auto& other){return GroupsOverlap(candidate,other);});
        };
        if(overlaps(position)) {
            float best=std::numeric_limits<float>::infinity();
            std::vector<int> xs{x,left},ys{y,0};
            for(const auto& other:placed) {
                xs.push_back(other.gridX-grid.gridW);
                xs.push_back(other.gridX+other.gridW);
                ys.push_back(other.gridY-grid.gridH);
                ys.push_back(other.gridY+other.gridH);
            }
            for(int cx:xs)for(int cy:ys) {
                if(cx<left||cx>right-grid.gridW||cy<0||
                    (g_settings.pages&&cy>std::max(0,bottom-grid.gridH)))continue;
                float distance=std::hypot((cx-x)*sx,(cy-y)*sy);
                if(distance>=best)continue;
                auto candidate=MoveGroupToGrid(grid,cx,cy);
                if(!overlaps(candidate)){position=candidate;best=distance;}
            }
            if(!std::isfinite(best)) {
                if (g_settings.pages) {
                    int nextPage=page+1;
                    do {position=MoveGroupToGrid(grid,nextPage++*steps,0);} while(overlaps(position));
                } else {
                    int nextY = 0;
                    for (const auto& other : placed) nextY = std::max(nextY, other.gridY + other.gridH);
                    position = MoveGroupToGrid(grid, left, nextY);
                }
            }
        }
        placed.push_back(position);
        g_groupPositions[geometry.group]=position;
    }
    // Geometry for paged automatic groups is reflowed from their existing app order.
    g_freeGroupLayout=true;
    g_groupEditDirty=!responsive;
}

// Each branch pins the active group and the groups it has already moved.
// Resolving their clearance can bring additional groups into the chain.
struct GroupCollisionSearch {
    const std::vector<GroupEditNode>& baseline;
    GroupEditBounds bounds;
    float dx,dy;
    int budget=12000;
    double bestDistance=1e30,bestDirection=-1e30;
    std::vector<GroupEditNode> best;

    void Resolve(std::vector<GroupEditNode>& nodes,std::vector<size_t>& pinned,double distance,double direction) {
        if(--budget<0||distance>bestDistance+.0001)return;
        size_t next=nodes.size();
        for(size_t i=0;i<nodes.size();++i) {
            if(std::find(pinned.begin(),pinned.end(),i)!=pinned.end())continue;
            if(std::any_of(pinned.begin(),pinned.end(),[&](size_t j){return GroupsOverlap(nodes[i].position,nodes[j].position);})) {
                if(nodes[i].fixed)return;
                if(next==nodes.size())next=i;
            }
        }
        if(next==nodes.size()) {
            if(distance<bestDistance-.0001||direction>bestDirection) {
                best=nodes;bestDistance=distance;bestDirection=direction;
            }
            return;
        }
        const auto from=baseline[next].position;
        float sx=GroupStepX(),sy=GroupStepY();
        float left=bounds.PageLeft(from.x);
        int page=(int)std::floor(left/bounds.width+.01f);
        int minX=page*2*g_squareGridColumns,maxX=minX+2*g_squareGridColumns-from.gridW;
        int maxY=(int)std::floor((bounds.height-from.h+.001f)/sy);
        // Limit the search to the local neighborhood even for very tall scrolling layouts.
        int centerX=from.gridX,centerY=GroupGridIndex(from.y,sy);
        struct Candidate {int x,y;double distance,direction;};
        std::vector<Candidate> candidates;
        auto add=[&](int gridX,int gridY) {
            float x=GroupXAt(gridX),y=gridY*sy;
            float u=(x-from.x)/sx,v=(y-from.y)/sy;
            double cost=u*u+v*v;
            if(distance+cost>bestDistance+.0001)return;
            auto rect=from;rect.x=x;rect.y=y;
            if(!bounds.Fits(rect)||bounds.PageLeft(x+.001f)!=left)return;
            if(std::any_of(pinned.begin(),pinned.end(),[&](size_t j){return GroupsOverlap(rect,nodes[j].position);}))return;
            candidates.push_back({gridX,gridY,cost,u*dx+v*dy});
        };
        for(int y=std::max(0,centerY-32);y<=std::min(maxY,centerY+32);++y)
            for(int x=std::max(minX,centerX-32);x<=std::min(maxX,centerX+32);++x)
                add(x,y);
        std::sort(candidates.begin(),candidates.end(),[](const auto& a,const auto& b){
            if(a.distance!=b.distance)return a.distance<b.distance;
            if(a.direction!=b.direction)return a.direction>b.direction;
            if(a.y!=b.y)return a.y<b.y;
            return a.x<b.x;
        });
        pinned.push_back(next);
        for(const auto& candidate:candidates) {
            if(budget<=0||distance+candidate.distance>bestDistance+.0001)break;
            nodes[next].position=MoveGroupToGrid(from,candidate.x,candidate.y);
            Resolve(nodes,pinned,distance+candidate.distance,direction+candidate.direction);
        }
        pinned.pop_back();
        nodes[next].position=from;
    }
};
std::vector<GroupEditNode> SolveGroupGesture(const GroupGeometryGesture& gesture,GroupPosition target,
                                            GroupEditBounds bounds,float dx,float dy) {
    if(!bounds.Fits(target))return {};
    auto nodes=gesture.baseline;
    auto active=std::find_if(nodes.begin(),nodes.end(),[&](const auto& node){return node.group==gesture.group;});
    if(active==nodes.end())return {};
    if(SameGroupBounds(active->position,target))return nodes;
    const int deltaX = target.gridX - active->position.gridX;
    const int deltaY = target.gridY - active->position.gridY;
    active->position=target;
    std::vector<size_t> pinned{(size_t)(active-nodes.begin())};
    for (size_t i = 0; i < nodes.size(); ++i) {
        auto& node = nodes[i];
        if (!gesture.companions.contains(node.group)) continue;
        auto shifted = MoveGroupToGrid(node.position, node.position.gridX + deltaX, node.position.gridY + deltaY);
        if (!bounds.Fits(shifted) || std::any_of(pinned.begin(), pinned.end(), [&](size_t other) {
            return GroupsOverlap(shifted, nodes[other].position);
        })) return {};
        node.position = shifted;
        pinned.push_back(i);
    }
    GroupCollisionSearch search{gesture.baseline,bounds,dx,dy};
    search.Resolve(nodes,pinned,0,0);
    return search.best;
}

void CaptureGroupBaseline(GroupGeometryGesture& gesture) {
    std::map<std::shared_ptr<Folder>,int> counts;
    for(const auto& other:g_groupGeometry)++counts[other.group];
    GroupSelection captured;
    for(const auto& other:g_groupGeometry) {
        if(other.group==gesture.group&&other.start!=gesture.activeStart)continue;
        const bool moving = GroupMovesWithGesture(gesture, other.group);
        if (moving && !captured.insert(other.group).second) continue;
        gesture.baseline.push_back({other.group,EditorGroupPosition(other),
            !moving && counts[other.group]>1, other.start});
    }
}

bool FreeGroupPointerDown(HWND hwnd,int x,int y,bool control = false) {
    if(!g_groupEditMode||!GroupedView())return false;
    int resizeIndex=GroupResizeAt(x,y);
    int index=resizeIndex>=0?resizeIndex:GroupAtPoint(x,y);
    if (index < 0) {
        if (y < g_layout.viewTop || y >= g_layout.viewBottom) return false;
        g_groupMarquee = {g_groupSelection, {x,y}, {x,y}, true, false};
        if (hwnd) {
            SetCapture(hwnd);
            if (GetCapture() != hwnd) CancelGroupMarquee();
        }
        return true;
    }
    const auto& geometry=g_groupGeometry[index];
    if (control) {
        if (!g_groupSelection.erase(geometry.group)) g_groupSelection.insert(geometry.group);
        Render();
        return true;
    }
    auto area=VisualGroupRect(geometry);
    bool resize=resizeIndex>=0;
    auto title=area;title.bottom=title.top+geometry.title.bottom-geometry.title.top;
    if(!resize&&!GroupContains(title,(float)x,(float)y)&&!g_groupSelection.contains(geometry.group))return true;

    auto& gesture=g_groupGesture;
    gesture={};gesture.group=geometry.group;gesture.activeStart=geometry.start;
    if (!resize) {
        if (!g_groupSelection.contains(geometry.group)) {
            g_groupSelection.clear();
            g_groupSelection.insert(geometry.group);
        }
        gesture.companions = g_groupSelection;
        gesture.companions.erase(geometry.group);
    }
    gesture.original=EditorGroupPosition(geometry);
    gesture.draft=gesture.continuous=
        VisualGroupPosition(geometry.group,gesture.original);
    gesture.start=gesture.at={x,y};gesture.resize=resize;
    auto pointer=GroupPointerInLayout(x,y);
    const auto& visual=gesture.continuous;
    gesture.grab={pointer.x-visual.x,pointer.y-visual.y};
    gesture.handle={pointer.x-visual.x-visual.w,pointer.y-visual.y-visual.h};
    gesture.lastScroll=g_scroll;gesture.valid=true;
    CaptureGroupBaseline(gesture);
    if(hwnd){SetCapture(hwnd);if(GetCapture()!=hwnd)gesture={};}
    Render();
    return true;
}
void RetargetGroupTransitions(const GroupGeometryGesture& gesture) {
    const auto now=Now();
    for(size_t i=0;i<gesture.baseline.size();++i) {
        const auto& node=gesture.baseline[i];
        if(GroupMovesWithGesture(gesture,node.group)||node.fixed)continue;
        const auto& destination=gesture.valid?gesture.solution[i].position:node.position;
        auto t=g_groupTransitions.find(node.group);
        if(t!=g_groupTransitions.end()&&SameGroupBounds(t->second.to,destination))continue;
        auto visual=VisualGroupPosition(node.group,node.position);
        if(!SameGroupBounds(visual,destination))
            g_groupTransitions[node.group]={visual,destination,now};
        else g_groupTransitions.erase(node.group);
    }
}
GroupPosition SnapGroupBlock(const GroupGeometryGesture& gesture, const GroupPosition& raw,
                             const GroupEditBounds& bounds, float sy, int steps) {
    const auto& anchor = gesture.original;
    int minPageDelta = 1 - bounds.pages, maxPageDelta = bounds.pages - 1;
    int minX = -steps, maxX = steps;
    int minY = -1000000, maxY = 1000000;
    for (const auto& node : gesture.baseline) {
        if (!GroupMovesWithGesture(gesture, node.group)) continue;
        const auto& p = node.position;
        int page = p.gridX / steps, localX = p.gridX % steps;
        minPageDelta = std::max(minPageDelta, -page);
        maxPageDelta = std::min(maxPageDelta, bounds.pages - 1 - page);
        minX = std::max(minX, -localX);
        maxX = std::min(maxX, steps - localX - p.gridW);
        minY = std::max(minY, -p.gridY);
        maxY = std::min(maxY, std::max(0, (int)std::floor((bounds.height - p.y - p.h + .001f) / sy)));
    }
    int page = g_settings.pages
        ? (int)std::floor(std::clamp(raw.x, 0.f, bounds.width * bounds.pages - .01f) / bounds.width) : 0;
    int pageDelta = std::clamp(page - anchor.gridX / steps, minPageDelta, std::max(minPageDelta, maxPageDelta));
    int deltaX = GroupXIndex(raw.x, page) - page * steps - anchor.gridX % steps;
    int deltaY = GroupGridIndex(raw.y, sy) - anchor.gridY;
    return MoveGroupToGrid(anchor, anchor.gridX + pageDelta * steps + std::clamp(deltaX, minX, std::max(minX,maxX)),
                                 anchor.gridY + std::clamp(deltaY, minY, std::max(minY,maxY)));
}

bool FreeGroupPointerMove(int x,int y) {
    if (g_groupMarquee.pending) {
        auto& marquee = g_groupMarquee;
        marquee.end = {x, y};
        marquee.moved = marquee.moved || std::abs(x - marquee.start.x) >= GetSystemMetrics(SM_CXDRAG) ||
                                        std::abs(y - marquee.start.y) >= GetSystemMetrics(SM_CYDRAG);
        if (!marquee.moved) return true;
        auto selected = marquee.before;
        const auto box = GroupMarqueeRect();
        if (SelectionHasArea(box)) {
            for (const auto& group : g_groupGeometry) {
                if (SelectionHasArea(SelectionIntersection(box, VisualGroupRect(group))))
                    selected.insert(group.group);
            }
        }
        g_groupSelection = std::move(selected);
        Render();
        return true;
    }
    auto& gesture=g_groupGesture;if(!gesture.group)return false;
    if(!gesture.moved&&x==gesture.start.x&&y==gesture.start.y)return true;
    gesture.moved=true;gesture.at={x,y};gesture.lastScroll=g_scroll;
    float inset=26*g_layout.scale;
    int edge=g_settings.pages?(x<inset?-1:x>g_layout.width-inset?1:0):
        (y<g_layout.viewTop+inset?-1:y>g_layout.viewBottom-inset?1:0);
    if(edge!=gesture.edge){gesture.edge=edge;gesture.edgeSince=Now();}
    auto pointer=GroupPointerInLayout(x,y);
    auto raw=gesture.original,p=gesture.original;
    auto bounds=GroupGestureBounds();
    float sx=GroupStepX(),sy=GroupStepY();
    int steps=2*g_squareGridColumns;
    if(gesture.resize) {
        raw.w=std::max(2*sx,pointer.x-gesture.handle.x-raw.x);
        raw.h=std::max(6*sy,pointer.y-gesture.handle.y-raw.y);
        if(!g_settings.pages)bounds.height=std::max(bounds.height,raw.y+raw.h+2*sy);
        int maxColumns=(steps-p.gridX%steps)/2;
        int maxRows=std::max(1,(int)std::floor((bounds.height-p.y-2*sy+.001f)/(4*sy)));
        p.gridW=2*std::clamp(GroupGridIndex(raw.w,2*sx),1,std::max(1,maxColumns));
        p.gridH=2+4*std::clamp(GroupGridIndex(raw.h-2*sy,4*sy),1,maxRows);
        p=EffectiveGroupPosition(p);
    } else {
        raw.x=pointer.x-gesture.grab.x;raw.y=pointer.y-gesture.grab.y;
        if (!gesture.companions.empty()) {
            p = SnapGroupBlock(gesture, raw, bounds, sy, steps);
        } else {
            float pageLeft=bounds.PageLeft(std::clamp(raw.x,0.f,bounds.width*bounds.pages-.01f));
            int page=(int)std::floor(pageLeft/bounds.width+.01f);
            int first=page*steps,last=first+steps-p.gridW;
            int lastY=std::max(0,(int)std::floor((bounds.height-p.h+.001f)/sy));
            p=MoveGroupToGrid(p,std::clamp(GroupXIndex(raw.x,page),first,std::max(first,last)),
                              std::min(GroupGridIndex(raw.y,sy),lastY));
        }
    }
    gesture.continuous=raw;
    if(!gesture.solved||!SameGroupBounds(p,gesture.draft)) {
        float dx=(gesture.resize?p.w-gesture.original.w:p.x-gesture.original.x)/sx;
        float dy=(gesture.resize?p.h-gesture.original.h:p.y-gesture.original.y)/sy;
        // Group spacing is an arrangement preference. Only overlapping cells
        // block a drop or push a neighbor during a pointer gesture.
        gesture.solution=SolveGroupGesture(gesture,p,bounds,dx,dy);
        if(gesture.solution.empty()&&!g_settings.pages) {
            // A packed scrolling layout can grow to make room for a neighbor.
            float extra=0;
            for(const auto& node:gesture.baseline)extra=std::max(extra,node.position.h);
            bounds.height+=extra;
            gesture.solution=SolveGroupGesture(gesture,p,bounds,dx,dy);
        }
        gesture.valid=!gesture.solution.empty();
        gesture.draft=p;gesture.solved=true;
    }
    StartFrames();
    return true;
}
bool TickGroupGesture() {
    auto& gesture=g_groupGesture;
    bool canScroll=gesture.edge<0?g_scrollTarget>0:gesture.edge>0&&g_scrollTarget<MaxScroll();
    if(canScroll&&Now()-gesture.edgeSince>.34) {
        gesture.edgeSince=Now();ScrollRows(gesture.edge);
    }
    if(g_scroll!=gesture.lastScroll)FreeGroupPointerMove(gesture.at.x,gesture.at.y);
    return canScroll||g_scroll!=g_scrollTarget;
}
void ApplyGroupSolution(const GroupGeometryGesture& gesture) {
    for(size_t i=0;i<gesture.solution.size();++i) {
        const auto& node=gesture.solution[i];
        if(node.fixed)continue;
        if(!g_groupPositions.count(node.group)||!SameGroupBounds(node.position,gesture.baseline[i].position))
            g_groupPositions[node.group]=node.position;
    }
    g_freeGroupLayout=true;g_groupEditDirty=true;
    ComputeLayout();
}
// Use the closest preceding border in each row or column. Work from the
// original adjacency so adjusting one axis does not change the other axis's neighbors.
std::vector<GroupEditNode> EqualizedGroupPositions(const std::vector<GroupEditNode>& baseline,
                                                   GroupEditBounds bounds,float gap) {
    auto result=baseline;
    for(bool horizontal:{true,false}) {
        auto leadingEdge=[&](const GroupPosition& p){return horizontal?p.x:p.y;};
        auto trailingEdge=[&](const GroupPosition& p){return horizontal?p.x+p.w:p.y+p.h;};
        auto sharesSpan=[&](const GroupPosition& a,const GroupPosition& b){
            return horizontal?GroupAxisSharesSpan(a.y,a.h,b.y,b.h):GroupAxisSharesSpan(a.x,a.w,b.x,b.w);
        };
        std::vector<size_t> order;
        for(size_t i=0;i<baseline.size();++i)order.push_back(i);
        std::stable_sort(order.begin(),order.end(),[&](size_t a,size_t b){
            return leadingEdge(baseline[a].position)<leadingEdge(baseline[b].position);
        });
        for(size_t index:order) {
            const auto& before=baseline[index].position;
            if(baseline[index].fixed)continue;
            float previous=-1,destination=leadingEdge(before);
            for(size_t neighbor:order) {
                const auto& other=baseline[neighbor].position;
                if(leadingEdge(other)>=leadingEdge(before))break;
                if(bounds.PageLeft(before.x)!=bounds.PageLeft(other.x)||!sharesSpan(before,other))continue;
                float edge=trailingEdge(other);
                if(edge>leadingEdge(before)+.01f||edge<previous-.01f)continue;
                float target=trailingEdge(result[neighbor].position)+gap;
                destination=edge>previous+.01f?target:std::max(destination,target);
                previous=std::max(previous,edge);
            }
            auto& after=result[index].position;
            if(std::abs(leadingEdge(after)-destination)>.01f) {
                float step=horizontal?GroupStepX():GroupStepY();
                int page=(int)std::floor(bounds.PageLeft(after.x)/bounds.width+.01f);
                float origin=horizontal?bounds.PageLeft(after.x)+bounds.margin:0;
                int gridIndex=(int)std::ceil((destination-origin-.001f)/step)+(horizontal?page*2*g_squareGridColumns:0);
                after=MoveGroupToGrid(after,horizontal?gridIndex:after.gridX,horizontal?after.gridY:gridIndex);
            }
        }
    }
    if(!g_settings.pages)bounds.height=std::max(bounds.height,1000000.f);
    for(size_t i=0;i<result.size();++i) {
        const auto& p=result[i].position;
        if(!bounds.Fits(p)||bounds.PageLeft(p.x+.001f)!=bounds.PageLeft(baseline[i].position.x+.001f))return {};
        for(size_t j=0;j<i;++j) {
            const auto& q=result[j].position;
            float spacing=bounds.PageLeft(p.x)==bounds.PageLeft(q.x)?gap:0;
            if(GroupsOverlap(p,q,spacing))return {};
        }
    }
    return result;
}
void EqualizeGroupSpacing() {
    GroupGeometryGesture change;
    CaptureGroupBaseline(change);
    if(change.baseline.size()<2)return;
    change.solution=EqualizedGroupPositions(change.baseline,GroupGestureBounds(),g_settings.groupGap);
    if(change.solution.empty()) {
        bool modal=std::exchange(g_modal,true);
        MessageBoxW(g_wnd,Tr(L"Cannot equalize spacing in this arrangement."),Tr(L"Equalize group spacing"),MB_OK|MB_ICONINFORMATION);
        g_modal=modal;
        return;
    }
    for(size_t i=0;i<change.baseline.size();++i)
        if(!SameGroupBounds(change.baseline[i].position,change.solution[i].position)) {
            change.valid=true;
            RetargetGroupTransitions(change);
            ApplyGroupSolution(change);
            StartFrames();Render();
            return;
        }
}
bool FreeGroupPointerUp(HWND hwnd) {
    if(!g_groupEditMode)return false;
    if (g_groupMarquee.pending) {
        if (!g_groupMarquee.moved) g_groupSelection.clear();
        g_groupMarquee = {};
        if (hwnd && GetCapture() == hwnd) ReleaseCapture();
        Render();
        return true;
    }
    auto gesture=std::exchange(g_groupGesture,{});
    if(hwnd&&GetCapture()==hwnd)ReleaseCapture();
    if(!gesture.group||!gesture.moved)return true;
    bool changed=gesture.valid&&!SameGroupBounds(gesture.draft,gesture.original);
    if(changed)ApplyGroupSolution(gesture);
    if(Animate()) {
        g_groupTransitions[gesture.group]={gesture.continuous,changed?gesture.draft:gesture.original,Now()};
        for (size_t i = 0; i < gesture.baseline.size(); ++i) {
            const auto& node = gesture.baseline[i];
            if (!gesture.companions.contains(node.group)) continue;
            auto from = node.position;
            from.x += gesture.continuous.x - gesture.original.x;
            from.y += gesture.continuous.y - gesture.original.y;
            g_groupTransitions[node.group] = {from, changed ? gesture.solution[i].position : node.position, Now()};
        }
        StartFrames();
    }
    Render();return true;
}

void RestoreGroupGeometry(HWND hwnd) {
    bool modal=std::exchange(g_modal,true);
    int answer=MessageBoxW(hwnd,Tr(L"Restore the previous saved group geometry?"),
        Tr(L"Restore previous group geometry"),MB_OKCANCEL|MB_ICONQUESTION);
    if(answer==IDOK) {
        if(LoadGroupPositions(GroupPositionFile()+L".bak")) {
            LoadTiles();Refresh();
        } else {
            MessageBoxW(hwnd,Tr(L"Could not restore group geometry."),
                Tr(L"Restore previous group geometry"),MB_OK|MB_ICONWARNING);
        }
    }
    g_modal=modal;
}
bool ScrollFreeGroup(HWND hwnd, LPARAM position, int delta) {
    if (!g_freeGroupLayout || !g_layout.useSavedGroupGeometry || !GroupedView() || g_groupEditMode)
        return false;
    POINT point{GET_X_LPARAM(position), GET_Y_LPARAM(position)};
    ScreenToClient(hwnd, &point);
    const int index = GroupAtPoint(point.x, point.y);
    if (index < 0) return false;
    const auto& geometry = g_groupGeometry[index];
    if (geometry.group->collapsed || !g_groupPositions.contains(geometry.group)) return false;
    const int pages = GroupPageCount((int)geometry.group->apps.size(), geometry.columns, geometry.visibleRows);
    if (pages < 2) return false;
    if (g_groupWheelTarget.lock() != geometry.group) {
        g_groupWheelTarget = geometry.group;
        g_groupWheelRemainder = 0;
    }
    g_groupWheelRemainder += delta;
    const int step = g_groupWheelRemainder / WHEEL_DELTA;
    g_groupWheelRemainder %= WHEEL_DELTA;
    const int page = geometry.firstInGroup / (geometry.columns * geometry.visibleRows);
    const int next = std::clamp(page - step, 0, pages - 1);
    if (next != page) {
        if (g_selected >= geometry.start && g_selected < geometry.start + geometry.count)
            g_selected = -1;
        g_groupRows[geometry.group] = next * geometry.visibleRows;
        ComputeGroups();
        g_hover = -1;
        Render();
    }
    return true;
}
