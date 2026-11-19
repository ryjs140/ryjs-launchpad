#define SEAMLESS_PREVIEW 1
#include "../mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp"
#include <cstdio>
void Preview_RunSmoke(HWND) {}
int failures=0, checks=0;
void Check(bool value,const char* label) { ++checks; if (!value) { ++failures; printf("FAIL: %s\n",label); } }
int wmain(int argc,wchar_t** argv) {
    if(argc==3&&std::wstring_view(argv[1])==L"--launch-probe") {
        std::ofstream marker{std::filesystem::path(argv[2])};
        marker<<"launched";
        return marker?0:1;
    }
    CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    WCHAR executable[32768]{};GetModuleFileNameW(nullptr,executable,ARRAYSIZE(executable));
    auto cases=std::filesystem::path(executable).parent_path()/(L"feature-data-"+std::to_wstring(GetTickCount64()));
    std::filesystem::create_directories(cases);Preview::directory=cases.wstring();
    for (int hz : {60,120,144,165,240}) {
        float value=0;
        for (int i=0;i<hz;i++) value=AdvanceTransition(value,1,0.28/hz,280);
        Check(std::abs(value-1)<0.0001f,"elapsed duration independent of frame rate");
    }
    Check(AdvanceTransition(0,1,0.8,280)==1,"late frame completes animation");
    Check(AdvanceTransition(0.5f,0,0.18,180)==0,"reverse close clamps at target");
    Check(AdvanceTransition(0.5f,1,-1,280)==0.5f,"negative elapsed time does not reverse progress");
    {
        using namespace launchpad_fly_in;
        for (auto [width,height] : {std::pair{3840.f,2160.f}, {3440.f,1440.f}, {1080.f,1920.f}}) {
            for (float x : {.12f,.35f,.51f,.78f,.89f}) {
                for (float y : {.13f,.38f,.55f,.79f,.88f}) {
                    float centerX=x*width, centerY=y*height;
                    auto sample=[&](float phase) {
                        return Sample(phase,centerX,centerY,width,height,100.f,.16f);
                    };
                    auto start=sample(0), end=sample(1), previous=start;
                    Check(centerX+start.dx<=-99.9f||centerX+start.dx>=width+99.9f||
                          centerY+start.dy<=-99.9f||centerY+start.dy>=height+99.9f,
                          "each icon starts beyond a viewport edge");
                    Check(end.dx==0&&end.dy==0&&end.scale==1&&end.opacity==1&&start.opacity==0,
                          "flight lands at the exact original pose");
                    bool monotonic=true;
                    for (int frame=1;frame<=100;++frame) {
                        auto pose=sample(frame*.01f);
                        monotonic &= std::abs(pose.dx)<=std::abs(previous.dx)&&
                                     std::abs(pose.dy)<=std::abs(previous.dy)&&
                                     pose.scale<=previous.scale&&pose.opacity>=previous.opacity;
                        previous=pose;
                    }
                    Check(monotonic,"translation, scale and opacity converge without overshoot");
                }
            }
        }
        for (int hz : {60,120,144,160,240}) {
            float phase=0;
            double speed=0;
            double dt=.14/hz;
            for (int frame=0;frame<hz;++frame) phase=Advance(phase,speed,1,dt,280);
            Check(std::abs(phase-.5f)<.00001f,"fly-in phase is independent of frame rate");
            float reversed=Advance(phase,speed,0,0,180);
            Check(reversed==phase&&std::abs(speed-1000.0/280)<.00001,
                  "reversal retains phase and incoming speed");
            phase=Advance(phase,speed,0,.001,180);
            Check(phase>reversed&&speed>0,"reversal brakes the current motion first");
            phase=Advance(phase,speed,0,.03,180);
            Check(phase<reversed&&speed<0,"reversal then retraces the original path");
            phase=Advance(phase,speed,1,.001,280);
            Check(speed<0,"rapid reopen keeps the current closing velocity");
            phase=Advance(phase,speed,1,.5,280);
            Check(phase==1&&speed==0,"interrupted fly-in completes at rest");
            phase=Advance(phase,speed,0,.18,180);
            Check(phase==0&&speed==0,"full fly-out completes at rest");
        }
        Preview::PutSetting(L"launchpadAnimation", L"classic");
        LoadSettings();
        Check(!g_settings.iconFlyIn,"classic animation loads from persisted settings");
        Preview::PutSetting(L"launchpadAnimation", L"iconFlyIn");
        LoadSettings();
        Check(g_settings.iconFlyIn,"icon fly-in loads from persisted settings");
    }
    for (const auto& field:Set_MakeFields()) if (wcscmp(field.key,L"openDuration")==0 || wcscmp(field.key,L"closeDuration")==0 || wcscmp(field.key,L"folderDuration")==0)
        Check(Set_HasSlider(field),"duration has slider and exact numeric entry");
    std::wstring original=L"中文\tname%20\r\n";std::wstring decoded;
    Check(EntryDecode(EntryEncode(original),decoded)&&decoded==original,"entry text preserves Unicode and delimiters");
    Check(!EntryDecode(L"broken%2",decoded),"invalid entry escape rejected");
    auto automatic=std::make_shared<App>();automatic->name=L"Original";automatic->target=L"C:\\Old\\app.exe";
    g_apps={automatic};g_settings.groups=true;g_appsKey=0;
    ArrangeTiles({L"schema\t1\tgroups",L"folder\tTools",L"in\tc:\\old\\app.exe",L"folder\tOther"});
    auto arrangement=ArrangementLines();
    g_entries[AppKey(*automatic)]={false,L"C:\\New\\app.exe",L"My alias",L"--profile test",L"C:\\New"};
    g_entries[L"ryjs-entry:{test}"]={true,(cases/L"remote.rdp").wstring(),L"Remote",L"",L""};
    Check(SaveEntries(),"manual and override entry records saved");
    g_entries.clear();Check(LoadEntries()&&g_entries.size()==2,"entries reload without discovery");
    auto refreshed=std::make_shared<App>();refreshed->name=L"Original";refreshed->target=L"C:\\Old\\app.exe";
    g_apps={refreshed};ApplyEntries(64);ArrangeTiles(arrangement);
    Check(refreshed->name==L"My alias"&&refreshed->originalName==L"Original","rescan retains alias and original searchable name");
    Check(AppKey(*refreshed)==L"c:\\old\\app.exe"&&refreshed->arguments==L"--profile test","changed target keeps layout identity and launch parameters");
    Check(g_tiles.size()==2&&g_tiles[0].folder->apps.size()==1&&g_tiles[0].folder->apps[0].app==refreshed,"existing group membership and order retained");
    Check(ArrangementLines()==arrangement,"editing launch target preserves serialized layout identity exactly");
    auto discovered=std::make_shared<App>();discovered->name=L"New discovery";discovered->target=L"C:\\newly-discovered.exe";
    g_apps.push_back(discovered);ArrangeTiles(arrangement);
    Check(ArrangementLines()==arrangement,"new discovery never inserts into existing groups");g_apps.pop_back();
    Check(g_apps.size()==2&&g_apps[1]->manual&&g_apps[1]->unavailable,"missing RDP entry retained and marked repairable");
    g_query=L"Original";UpdateFilter();Check(g_visible.size()==1&&g_visible[0].app==refreshed,"original name remains searchable");
    g_query=L"alias";UpdateFilter();Check(g_visible.size()==1&&g_visible[0].app==refreshed,"alias participates in search");g_query.clear();
    Check(WebIconPath(L"ryjs-entry:C:\\?*test").find(L"?*")==std::wstring::npos,"cache name never contains path input");
    auto candidates=WebsiteIconCandidates(L"<link rel='shortcut icon' data-href='wrong' href='../icons/site.png?a=1&amp;b=2'>",L"https://example.org/path/page");
    Check(candidates.size()==2&&candidates[0]==L"https://example.org/icons/site.png?a=1&b=2"&&candidates[1]==L"https://example.org/favicon.ico","website icon declaration resolves relative URL and fallback");
    std::ofstream(cases/L"icon.svg")<<"<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 64 64'><rect width='64' height='64' fill='red'/></svg>";
    App svg;Check(LoadSvgIcon((cases/L"icon.svg").wstring(),128,svg)&&svg.iconSize==128&&!svg.pixels.empty(),"SVG icon rasterizes at requested DPI size");
    UpdateFilter();g_layout.width=1600;g_layout.height=1000;g_layout.scale=1;ComputeLayout();
    Check(!g_freeGroupLayout&&LoadGroupPositions()&&!g_freeGroupLayout,"missing geometry preserves legacy layout mode");
    auto membership=ArrangementLines();
    ToggleGroupEdit();Check(g_groupEditMode&&g_wholeCellLayout&&!std::filesystem::exists(GroupPositionFile()),"opening the editor aligns the draft without writing geometry");
    auto group=g_tiles[0].folder,second=g_tiles[1].folder;
    auto startGeometry=g_groupGeometry[0].area,untouched=g_groupGeometry[1].area;
    for(const auto& [group,position]:g_groupPositions)
        Check(position.SquareCells()&&SameGroupBounds(position,EffectiveGroupPosition(position)),"draft uses canonical whole-cell positions");
    auto handle=GroupResizeHit(startGeometry);
    int hx=(int)handle.left+10,hy=(int)handle.top+10;
    FreeGroupPointerDown(nullptr,hx,hy);
    FreeGroupPointerMove(hx-128,hy);
    Check(g_groupGesture.valid&&g_groupGesture.moved&&g_groupGesture.draft.SquareCells(),"resize previews a valid whole-cell rectangle");
    auto resized=g_groupGesture.draft;FreeGroupPointerUp(nullptr);
    Check(g_groupEditMode&&!std::filesystem::exists(GroupPositionFile())&&g_groupPositions[group].gridW==resized.gridW&&ArrangementLines()==membership,"pointer release commits only to edit session");
    ToggleGroupEdit();
    Check(g_freeGroupLayout&&std::filesystem::exists(GroupPositionFile()),"Finish explicitly commits geometry");
    auto other=std::find_if(g_groupGeometry.begin(),g_groupGeometry.end(),[&](const auto& geometry){return geometry.group==second;});
    Check(g_groupPositions.size()==2&&other!=g_groupGeometry.end()&&
          SameGroupBounds(PositionFromArea(other->area),PositionFromArea(untouched)),
          "saving materializes unaffected groups without changing their geometry");
    auto committed=g_groupPositions[group];
    ToggleGroupEdit();g_groupPositions[group].gridW+=1;g_groupEditDirty=true;CancelGroupEdit();
    Check(g_groupPositions[group].gridW==committed.gridW&&!g_groupEditMode,"Cancel restores the entire edit session");
    auto firstPosition=EffectiveGroupPosition(g_groupPositions[group]);
    g_groupPositions[second]=MoveGroupToGrid(firstPosition,firstPosition.gridX,firstPosition.gridY+firstPosition.gridH+1);
    Check(SaveGroupPositions(),"two groups can be persisted in the same column");
    ComputeGroups();Check(g_groupGeometry.size()==2&&g_groupGeometry[0].area.left==g_groupGeometry[1].area.left&&g_groupGeometry[0].area.bottom<g_groupGeometry[1].area.top,"same-column groups are vertically independent");
    Check(GroupsOverlap(firstPosition,firstPosition)&&!GroupsOverlap(firstPosition,g_groupPositions[second]),"overlap detection distinguishes valid stacked groups");
    ClearGroupPositions();Check(LoadGroupPositions()&&g_freeGroupLayout&&g_groupPositions[group].gridW==resized.gridW,"whole-cell shape survives a fresh read");
    group->collapsed=true;Check(SaveGroupPositions(),"traditional folder presentation saved explicitly");
    group->collapsed=false;ClearGroupPositions();LoadGroupPositions();UpdateFilter();ComputeGroups();
    Check(group->collapsed&&!g_visible.empty()&&g_visible[0].folder==group&&ArrangementLines()==membership,"traditional folder coexists without altering membership");
    auto widthBefore=g_groupPositions[group].w;
    ArrangeTiles(membership);UpdateFilter();ComputeLayout();group=g_tiles[0].folder;
    Check(g_groupPositions.count(group)&&g_groupPositions[group].w==widthBefore&&group->collapsed,"drag cancel/rearrangement preserves geometry and traditional folders");
    auto beforeRename=ArrangementLines();auto targetBefore=refreshed->target;auto keyBefore=AppKey(*refreshed);
    Check(SetDisplayName(refreshed,L"代码 编辑器 中文长名称"),"all discovered entries accept a launchpad display name");
    Check(refreshed->target==targetBefore&&AppKey(*refreshed)==keyBefore&&ArrangementLines()==beforeRename,"rename preserves target, identity, membership and order");
    auto renamed=refreshed->name;g_displayNames.clear();Check(LoadDisplayNames()&&g_displayNames.count(keyBefore),"display name survives storage reload");
    refreshed->name=L"Original";refreshed->originalName=L"Original";ApplyDisplayName(*refreshed);
    Check(refreshed->name==renamed&&refreshed->originalName==L"Original","automatic rescan retains custom and original names");
    g_query=L"代码";UpdateFilter();Check(g_visible.size()==1,"renamed title participates in search");
    g_query=L"Original";UpdateFilter();Check(g_visible.size()==1,"source title still participates in search");g_query.clear();
    Check(SetDisplayName(refreshed,L"")&&refreshed->name==L"Original"&&g_displayNames[keyBefore].custom.empty(),"restore source name removes only local display override");
    refreshed->name=L"My alias";refreshed->originalName=L"Original";ApplyDisplayName(*refreshed);
    Check(refreshed->name==L"Original","restored source name survives rescan even with a legacy editor alias");
    Check(!SetDisplayName(refreshed,L"bad\nname"),"rename rejects control characters");
    for(auto target:{L"C:\\example.exe",L"C:\\example.lnk",L"shell:AppsFolder\\Vendor.App!Main",L"https://example.org/"}) {
        auto entry=std::make_shared<App>();entry->name=L"Source name";entry->target=target;
        auto key=AppKey(*entry);Check(SetDisplayName(entry,L"Alias"),"EXE, shortcut, Store and web entry rename");
        Check(entry->target==target&&AppKey(*entry)==key,"rename never modifies any launch target type");
    }
    Check(WriteLines(StoragePath(L"display-names-v1.txt"),{L"schema\t99\tdisplay-names"}),"future name schema fixture written");
    Check(!LoadDisplayNames()&&g_displayNamesReadOnly&&!SetDisplayName(refreshed,L"loss"),"unsupported names data refuses destructive writes");
    g_displayNamesReadOnly=false;
    Check(ArrangementLines()==membership,"theme changes do not modify group contents");
    group=g_tiles[0].folder;second=g_tiles[1].folder;
    for(float scale:{1.0f,1.25f,1.5f,2.0f}) {
        g_layout.scale=scale;g_layout.width=(int)(1600*scale);g_layout.height=(int)(1200*scale);
        GroupPosition grid;grid.gridX=1;grid.gridY=1;grid.gridW=8;grid.gridH=10;
        grid.style=5;grid.snapped=true;
        g_groupPositions[group]=EffectiveGroupPosition(grid);
        auto stationary=grid;stationary.gridW=4;
        stationary=MoveGroupToGrid(stationary,2*g_squareGridColumns-4,0);
        g_groupPositions[second]=stationary;
        g_scroll=g_scrollTarget=0;ComputeLayout();
        ToggleGroupEdit();
        for(auto [w,h]:std::vector<std::pair<int,int>>{{10,10},{8,10},{12,14},{8,10},{10,10},{12,14}}) {
            g_groupTransitions.clear();
            auto start=EffectiveGroupPosition(g_groupPositions[group]);
            auto rect=ProjectGroupRect(GroupPositionRect(start));
            int x=(int)rect.right-12,y=(int)rect.bottom-12;
            FreeGroupPointerDown(nullptr,x,y);
            int dx=(int)std::lround((w-start.gridW)*GroupStepX()*scale);
            int dy=(int)std::lround((h-start.gridH)*GroupStepY()*scale);
            FreeGroupPointerMove(x+dx,y+dy);
            auto dims=g_groupGesture.draft;
            Check(g_groupGesture.valid&&dims.gridW==w&&dims.gridH==h,"size round trip and whole-cell targets are exact at each DPI");
            auto live=ProjectGroupRect(GroupPositionRect(g_groupGesture.continuous));
            Check(std::abs((live.right-rect.right)-dx)<.01f&&std::abs((live.bottom-rect.bottom)-dy)<.01f,
                "live bottom-right handle follows the pointer delta without interpolation");
            FreeGroupPointerUp(nullptr);
            Check(SameGroupBounds(EffectiveGroupPosition(g_groupPositions[group]),dims),"release uses the preview's exact geometry");
        }
        Check(g_groupPositions[second].x==stationary.x&&g_groupPositions[second].y==stationary.y,"unaffected resize neighbor remains in place");
        CancelGroupEdit();
    }
    {
        using TestFolder=typename decltype(group)::element_type;
        auto a=std::make_shared<TestFolder>(),b=std::make_shared<TestFolder>();
        float w=GroupStepX(),h=GroupStepY();
        GroupPosition cell;
        cell.style=5;cell.gridW=4;cell.gridH=6;
        GroupGeometryGesture gesture;
        gesture.group=a;
        gesture.baseline={{a,MoveGroupToGrid(cell,0,0)},{b,MoveGroupToGrid(cell,6,0)}};
        GroupEditBounds packed{(float)g_layout.width/g_layout.scale,6*h,1,GridMargin()/g_layout.scale};
        auto touching=SolveGroupGesture(gesture,MoveGroupToGrid(cell,2,0),packed,1,0);
        Check(touching.size()==2&&touching[1].position.gridX==6,"half-column move can place groups flush");
        auto wider=cell;wider.gridW=6;
        auto resized=SolveGroupGesture(gesture,MoveGroupToGrid(wider,0,0),packed,1,0);
        Check(resized.size()==2&&resized[1].position.gridX==6,"resize can place groups flush");
        auto target=MoveGroupToGrid(cell,4,0);
        auto result=SolveGroupGesture(gesture,target,packed,1,0);
        Check(result.size()==2&&result[1].position.gridX==8&&result[1].position.gridY==0,
              "actual overlap pushes the neighbor to a free grid line");

        auto inbox=std::make_shared<TestFolder>();inbox->inbox=true;
        auto tiles=g_tiles;
        g_tiles={{nullptr,inbox}};g_wholeCellLayout=true;
        UpdateFilter();ComputeLayout();
        Check(g_groupGeometry.empty()&&g_layout.contentHeight==0&&g_visible.empty(),"empty inbox reserves no view area");
        inbox->apps.push_back({refreshed,nullptr});UpdateFilter();ComputeLayout();
        Check(g_groupGeometry.size()==1&&g_visible.size()==1,"inbox reappears when an app is added");
        g_tiles=std::move(tiles);UpdateFilter();ComputeLayout();
        for(int rows:{1,2,3}) {
            auto p=cell;p.gridH=rows*4+2;p=MoveGroupToGrid(p,2,1);
            auto geometry=MakeFreeGroupGeometry(group,p,0,0);
            auto origin=GroupCellsOrigin(geometry);
            Check(std::abs(geometry.title.bottom-geometry.title.top-SquareCellSize(g_squareGridColumns)/2)<.01f&&
                  geometry.area.left==geometry.title.left&&geometry.area.right==geometry.title.right&&
                  origin.y==geometry.title.bottom&&geometry.visibleRows==rows&&
                  std::abs(geometry.area.bottom-geometry.area.top-(rows+.5f)*SquareCellSize(g_squareGridColumns))<.01f,
                  "one to three square app rows keep a half-height title");
        }
    }
    auto gridFile=GroupPositionFile();
    auto savedGeometry=ReadLines(gridFile);
    auto size=g_settings.iconSize,hgap=g_settings.horizontalGap,vgap=g_settings.verticalGap;
    auto saved=g_groupPositions[group];
    auto beforeSettings=EffectiveGroupPosition(saved);
    g_settings.iconSize+=9;ComputeLayout();
    auto largerIcons=EffectiveGroupPosition(saved);
    Check(SameGroupBounds(largerIcons,beforeSettings),"icon size does not change committed grid coordinates");
    g_settings.iconSize=size;g_settings.horizontalGap+=7;ComputeLayout();
    auto horizontalSpacing=EffectiveGroupPosition(saved);
    Check(SameGroupBounds(horizontalSpacing,beforeSettings),"legacy horizontal gap does not change the square grid");
    g_settings.horizontalGap=hgap;g_settings.verticalGap+=11;ComputeLayout();
    auto verticalSpacing=EffectiveGroupPosition(saved);
    Check(SameGroupBounds(verticalSpacing,beforeSettings),"legacy vertical gap does not change the square grid");
    Check(g_groupPositions[group].gridX==saved.gridX&&g_groupPositions[group].gridW==saved.gridW&&
          ReadLines(gridFile)==savedGeometry,"settings preserve grid indices and write no geometry");
    g_settings.verticalGap=vgap;ComputeLayout();
    int committedColumns=g_squareGridColumns;
    auto oldGeometry=EffectiveGroupPosition(saved);
    g_settings.globalColumns=std::max(3,committedColumns-2);
    ComputeLayout();
    Check(g_squareGridColumns==committedColumns&&SameGroupBounds(oldGeometry,EffectiveGroupPosition(saved)),
          "changing the requested columns does not remap the normal view");
    ToggleGroupEdit();
    Check(g_squareGridColumns==g_settings.globalColumns&&ReadLines(gridFile)==savedGeometry,
          "new column count is previewed before writing");
    float edge=SquareCellSize(g_squareGridColumns);
    auto tileCell=MakeSquareCell(GridMargin(),g_layout.viewTop,edge);
    Check(std::abs((tileCell.highlight.right-tileCell.highlight.left)-(tileCell.highlight.bottom-tileCell.highlight.top))<.01f&&
          std::abs((tileCell.icon.left+tileCell.icon.right)/2-(tileCell.label.left+tileCell.label.right)/2)<.01f,
          "square cell and its icon and label centers agree");
    auto oldScreenWidth=g_layout.width,oldScreenHeight=g_layout.height;
    g_layout.width=1024;g_layout.height=640;ComputeLayout();
    float newEdge=SquareCellSize(g_squareGridColumns);
    Check(std::abs(g_layout.width-g_squareGridColumns*newEdge-2*GridMargin())<.01f&&
          std::abs(g_layout.cellWidth-g_layout.cellHeight)<.01f&&ReadLines(gridFile)==savedGeometry,
          "resizing the screen keeps centered square cells without touching saved anchors");
    g_layout.width=oldScreenWidth;g_layout.height=oldScreenHeight;ComputeLayout();
    CancelGroupEdit();
    Check(g_squareGridColumns==committedColumns&&ReadLines(gridFile)==savedGeometry,"cancel restores the committed column count");
    g_settings.globalColumns=0;ComputeLayout();
    auto requestedAnimations=g_requestedAnimations.exchange(true),systemAnimations=g_systemAnimations.exchange(true);
    double clock=Now();
    Check(GroupEase(clock+.01)==0&&GroupEase(clock-.07)>.80f&&GroupEase(clock-.07)<1&&GroupEase(clock-.15)==1,
          "group preview and settle use bounded elapsed-time cubic easing");
    g_requestedAnimations=requestedAnimations;g_systemAnimations=systemAnimations;
    HANDLE originalFile=CreateFileW(gridFile.c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,0,nullptr);
    g_groupPositions[group].gridW++;
    Check(originalFile!=INVALID_HANDLE_VALUE&&!SaveGroupPositions()&&ReadLines(gridFile)==savedGeometry,"failed replacement leaves prior geometry intact");
    if(originalFile!=INVALID_HANDLE_VALUE)CloseHandle(originalFile);
    // Exercise the rendered preview with non-default icon and typography settings.
    g_settings.iconSize=97;g_settings.horizontalGap=29;g_settings.verticalGap=31;g_settings.labelSize=19;
    // Content overflow preserves all entries and exposes the final row.
    g_apps.clear();std::vector<std::wstring> overflow{L"schema\t1\tgroups",L"folder\tOverflow"};
    for(int n=0;n<31;n++){auto a=std::make_shared<App>();a->name=L"App "+std::to_wstring(n);a->target=L"C:\\fixture"+std::to_wstring(n)+L".exe";g_apps.push_back(a);overflow.push_back(L"in\t"+a->target);}
    ClearGroupPositions();ArrangeTiles(overflow);UpdateFilter();ComputeLayout();group=g_tiles[0].folder;
    g_groupPositions[group]=EffectiveGroupPosition({24,0,0,0,3,2});g_freeGroupLayout=true;ComputeGroups();auto allContents=ArrangementLines();
    Check(g_groupGeometry[0].count==6&&group->apps.size()==31,"small group uses internal viewport without removing overflow apps");
    {
        auto wide=MakeFreeGroupGeometry(group,{20,0,440,420},0,0);
        auto narrow=MakeFreeGroupGeometry(group,{20,0,301,420},0,0);
        Check(wide.columns>narrow.columns&&narrow.columns>=2,"different group widths fit different numbers of columns");
        auto distance=[](const auto& g,int first,int second) {
            auto a=MakeExpandedGroupCell(g,first),b=MakeExpandedGroupCell(g,second);
            return D2D1::Point2F(b.icon.left-a.icon.left,b.icon.top-a.icon.top);
        };
        Check(distance(wide,0,1).x==distance(narrow,0,1).x&&distance(wide,0,1).x==GroupIconPitchX(),
            "changing the group width does not stretch horizontal icon spacing");
        Check(distance(wide,0,wide.columns).y==distance(narrow,0,narrow.columns).y&&
              distance(wide,0,wide.columns).y==GroupIconPitchY(),
            "different group sizes use the same vertical icon spacing");
    }
    EnsureFreeGroupVisible(30);
    Check(g_groupGeometry[0].firstInGroup<=30&&g_groupGeometry[0].firstInGroup+g_groupGeometry[0].count>30&&ArrangementLines()==allContents,"keyboard navigation reaches final overflow entry in original order");
    Check(SaveGroupPositions(),"integer row/column geometry saves");ClearGroupPositions();LoadGroupPositions();
    Check(g_groupPositions[group].columns==3&&g_groupPositions[group].rows==2,"integer cell counts survive restart");
    g_scroll=g_scrollTarget=0;ComputeLayout();ToggleGroupEdit();
    auto legacy=PositionFromArea(g_groupGeometry[0].area);
    auto rect=ProjectGroupRect(g_groupGeometry[0].area);
    int hxLegacy=(int)rect.right-12,hyLegacy=(int)rect.bottom-12;
    FreeGroupPointerDown(nullptr,hxLegacy,hyLegacy);
    FreeGroupPointerMove(hxLegacy+330,hyLegacy+120);
    Check(g_groupGesture.valid&&g_groupGesture.draft.SquareCells(),"legacy resize can preview a grid size");
    auto previewGeometry=GroupResizePreviewGeometry();
    Check(previewGeometry&&previewGeometry->columns!=g_groupGeometry[0].columns,
        "resizing reflows icons inside the preview before release");
    FreeGroupPointerUp(nullptr);
    if(previewGeometry) {
        const auto& final=g_groupGeometry[0];
        Check(previewGeometry->columns==final.columns&&previewGeometry->count==final.count&&previewGeometry->start==final.start,
            "preview shows the same icons in the same slots as the committed group");
        for(int slot=0;slot<final.count;++slot) {
            auto expected=MakeExpandedGroupCell(*previewGeometry,slot);
            const auto& actual=g_groupCells[final.start+slot];
            Check(expected.icon.left==actual.icon.left&&expected.icon.top==actual.icon.top&&
                  expected.label.right==actual.label.right&&expected.label.bottom==actual.label.bottom&&
                  expected.icon.right-expected.icon.left<=std::round(97*g_layout.scale)&&
                  expected.label.right>expected.label.left,
                  "preview matches the committed icon size, spacing and label geometry");
        }
    }
    CancelGroupEdit();
    g_groupTransitions.clear();ComputeLayout();ToggleGroupEdit();
    rect=ProjectGroupRect(g_groupGeometry[0].area);
    int tx=(int)rect.left+38,ty=(int)rect.top+12;
    FreeGroupPointerDown(nullptr,tx,ty);
    FreeGroupPointerMove(tx+17,ty+47);
    Check(g_groupGesture.valid&&std::abs(g_groupGesture.continuous.x-legacy.x-17/g_layout.scale)<.01f&&
          std::abs(g_groupGesture.continuous.y-legacy.y-47/g_layout.scale)<.01f&&g_groupGesture.draft.snapped,
          "title uses the original mouse grab point and snaps the target independently");
    FreeGroupPointerUp(nullptr);CancelGroupEdit();
    // Deliver native messages to the same window procedure used in the preview and mod.
    g_settings.animations=false;g_requestedAnimations=false;
    g_settings.groups=false;g_scroll=g_scrollTarget=0;
    AppSelection probes;
    for(int n=0;n<3;n++) {
        auto app=std::make_shared<App>();app->name=L"Probe "+std::to_wstring(n);
        app->target=n==1?(cases/L"missing.exe").wstring():executable;
        app->stableKey=L"selection-probe-"+std::to_wstring(n);
        app->arguments=L"--launch-probe "+std::to_wstring(n)+L".txt";
        app->workingDirectory=cases.wstring();
        probes.push_back(app);
    }
    g_visible={{probes[0],nullptr},{probes[1],nullptr},{probes[2],nullptr}};
    ComputeLayout();
    WNDCLASSW window{};window.lpfnWndProc=WndProc;window.hInstance=GetModuleHandleW(nullptr);window.lpszClassName=L"RYJSSelectionTest";
    RegisterClassW(&window);
    g_wnd=CreateWindowExW(0,window.lpszClassName,L"",WS_POPUP,0,0,g_layout.width,g_layout.height,
                          nullptr,nullptr,window.hInstance,nullptr);
    Check(g_wnd!=nullptr,"isolated native launchpad window created");
    auto send=[&](UINT msg,WPARAM flags,int x,int y){SendMessageW(g_wnd,msg,flags,MAKELPARAM(x,y));};
    auto click=[&](int index,WPARAM mods){
        auto r=CellAt(index).icon;int x=(int)(r.left+r.right)/2,y=(int)(r.top+r.bottom)/2;
        send(WM_LBUTTONDOWN,MK_LBUTTON|mods,x,y);send(WM_LBUTTONUP,mods,x,y);
    };
    click(2,MK_CONTROL);click(0,0);
    Check(g_appSelection==AppSelection{probes[2],probes[0]}&&g_batchBlend==1,"Ctrl selects and ordinary click adds in selection order");
    click(2,MK_CONTROL);
    Check(g_appSelection==AppSelection{probes[0]},"Ctrl toggles off the selected entry");
    auto a=CellAt(0).highlight,b=CellAt(1).highlight,c=CellAt(2).highlight;
    auto marquee=[&](float from,float to){
        int y=(int)a.top-8;
        send(WM_LBUTTONDOWN,MK_LBUTTON,(int)from,y);
        send(WM_MOUSEMOVE,MK_LBUTTON,(int)to,(int)a.bottom);
        send(WM_LBUTTONUP,0,(int)to,(int)a.bottom);
    };
    marquee(a.left-6,b.right-2);marquee(b.left+2,c.right-2);
    Check(g_appSelection==probes&&g_batchDisplayCount==3,"overlapping consecutive marquees add entries without toggling prior hits");
    SendMessageW(g_wnd,WM_KEYDOWN,VK_ESCAPE,0);
    Check(g_appSelection.empty()&&g_batchBlend==0,"Escape clears selection and restores toolbar");
    click(0,MK_CONTROL);
    auto bar=BatchBar();int cancelX=(int)bar.left+12,cancelY=(int)(bar.top+bar.bottom)/2;
    send(WM_LBUTTONDOWN,MK_LBUTTON,cancelX,cancelY);send(WM_LBUTTONUP,0,cancelX,cancelY);
    Check(g_appSelection.empty()&&g_batchBlend==0,"batch toolbar Cancel clears selection through native input");
    click(0,0);
    auto waitForMarker=[&](PCWSTR filename){
        for(int tries=0;tries<50&&!std::filesystem::exists(cases/filename);tries++)Sleep(20);
        return std::filesystem::exists(cases/filename);
    };
    Check(waitForMarker(L"0.txt")&&g_appSelection.empty(),"ordinary click requests launch with entry arguments and working directory");
    std::filesystem::remove(cases/L"0.txt");
    std::vector<std::wstring> requested;
    auto launchFailures=RequestSelectedApps(probes,[&](const App& app){
        requested.push_back(app.name);
        return RequestLaunch(app.target,nullptr,&app,true);
    });
    Check(requested==std::vector<std::wstring>{L"Probe 0",L"Probe 1",L"Probe 2"},"batch sends requests in selection order");
    Check(launchFailures==std::vector<std::wstring>{L"Probe 1"},"batch reports the failed entry individually");
    Check(waitForMarker(L"0.txt")&&waitForMarker(L"2.txt"),"both valid native requests launch across an intervening failure");
    std::filesystem::remove(cases/L"0.txt");std::filesystem::remove(cases/L"2.txt");
    click(2,MK_CONTROL);click(0,0);
    int openX=(int)bar.right-20;
    send(WM_LBUTTONDOWN,MK_LBUTTON,openX,cancelY);send(WM_LBUTTONUP,0,openX,cancelY);
    Check(waitForMarker(L"0.txt")&&waitForMarker(L"2.txt")&&g_appSelection.empty()&&g_batchBlend==0,
          "batch toolbar opens selected apps and returns to normal");
    // Drive the layout toolbar through its normal mouse-message handlers.
    g_settings.groups=true;g_settings.pages=false;g_settings.groupGap=32;
    ClearGroupPositions();
    ArrangeTiles({L"schema\t1\tgroups",L"folder\tTop left",L"folder\tTop right",L"folder\tBottom left"});
    auto topLeft=g_tiles[0].folder,topRight=g_tiles[1].folder,bottomLeft=g_tiles[2].folder;
    g_groupPositions[topLeft]={30,0,416,300};g_groupPositions[topLeft].style=1;
    g_groupPositions[topRight]={457,0,413,300};g_groupPositions[topRight].style=1;
    g_groupPositions[bottomLeft]={30,338,413,300};g_groupPositions[bottomLeft].style=1;
    g_freeGroupLayout=true;Refresh();
    DeleteFileW(GroupPositionFile().c_str());
    Check(SaveGroupPositions(),"unequal geometry fixture saved");
    auto unequalGeometry=ReadLines(GroupPositionFile());
    auto toolbarClick=[&](int button) {
        auto rect=GroupEditButtons()[button];
        int x=(int)((rect.left+rect.right)/2),y=(int)((rect.top+rect.bottom)/2);
        send(WM_LBUTTONDOWN,MK_LBUTTON,x,y);send(WM_LBUTTONUP,0,x,y);
    };
    ToggleGroupEdit();toolbarClick(1);
    auto rightGrid=g_groupPositions[topRight],bottomGrid=g_groupPositions[bottomLeft];
    Check(g_groupEditDirty&&rightGrid.SquareCells()&&bottomGrid.SquareCells()&&
          ReadLines(GroupPositionFile())==unequalGeometry,"equalize only adjusts the draft on the whole-cell grid");
    toolbarClick(0);
    Check(!g_groupEditMode&&g_groupPositions[topRight].x==457&&g_groupPositions[bottomLeft].y==338,
          "Cancel restores legacy positions after grid alignment");
    ToggleGroupEdit();toolbarClick(1);toolbarClick(2);
    ClearGroupPositions();
    Check(LoadGroupPositions()&&g_wholeCellLayout&&g_groupPositions[topRight].gridX==rightGrid.gridX&&
          g_groupPositions[bottomLeft].gridY==bottomGrid.gridY,
          "the whole-cell draft survives a fresh geometry load");
    // The custom popup menu has to keep a submenu alive while the pointer
    // walks from the row that owns it to the submenu itself. That path stays
    // inside the parent panel from start to finish, so closing on any move
    // there made "Move to group" impossible to use.
    {
        using namespace LaunchpadMenu;
        Reset();
        State& menu = g_menu;
        menu.scale = 1;
        menu.work = RECT{0, 0, 1920, 1080};
        Node open, moveToGroup, rename, tools, remote;
        open.text = L"Open"; open.id = 1;
        moveToGroup.text = L"Move to group"; moveToGroup.id = 2;
        rename.text = L"Rename"; rename.id = 3;
        tools.text = L"Tools"; tools.id = 1000;
        remote.text = L"Remote"; remote.id = 1001;
        moveToGroup.sub = {tools, remote};
        menu.root = {open, moveToGroup, rename};
        Panel root;
        root.items = &menu.root;
        MeasurePanel(menu, root);
        root.rect = D2D1::RectF(100, 100, 100 + root.width,
                                100 + root.contentHeight);
        menu.rootTop = root.rect.top;
        menu.panels.push_back(root);
        OpenSubmenu(0, 1);
        Check(menu.panels.size() == 2 && menu.panels[1].parentItem == 1,
              "a row with a submenu opens the next level");
        const float rowY =
            RowTop(menu.panels[0], 1) + RowHeight(menu.root[1], 1) * 0.5f;
        const float edgeX = menu.panels[0].rect.right - 2.5f;
        const int edgeItem = HitTest(menu.panels[0], edgeX, rowY);
        Check(PanelAt(edgeX, rowY) == 0 && edgeItem == 1,
              "the row that owns the submenu stays hittable up to the shared edge");
        Check(DeeperLevelsAction(edgeItem, menu.panels[1].parentItem, true) ==
                  DeeperAction::Keep,
              "walking along the owner row keeps the submenu open");
        Check(PanelAt(menu.panels[1].rect.left + 4, rowY) == 1,
              "the submenu begins at the parent edge without a dead gap");
        Check(DeeperLevelsAction(2, 1, true) == DeeperAction::CloseNow &&
                  DeeperLevelsAction(2, 1, false) == DeeperAction::CloseSoon &&
                  DeeperLevelsAction(-1, 1, false) == DeeperAction::CloseSoon,
              "only the owner row keeps the submenu; other rows close or wait");
        // A class left by an earlier build must never be reused: the name is
        // unique to the loaded image, so an in-place update always registers
        // its own window procedure.
        const WCHAR* className = MenuClassName();
        Check(className && wcslen(className) > wcslen(kMenuClass) &&
                  wcsncmp(className, kMenuClass, wcslen(kMenuClass)) == 0,
              "the menu class name is unique per loaded image");
        Check(wcscmp(className, MenuClassName()) == 0,
              "the menu class name is stable within one image");
        Reset();
    }
    DestroyWindow(g_wnd);g_wnd=nullptr;
    Check(WriteLines(GroupPositionFile(),{L"schema\t999\tgroup-geometry"}),"future geometry fixture saved");
    ClearGroupPositions();Check(!LoadGroupPositions()&&g_layoutReadOnly&&!SaveGroupPositions(),"unknown group geometry blocks layout writes");
    g_layoutReadOnly=false;g_groupPositionsReadOnly=false;g_freeGroupLayout=false;
    Check(WriteLines(StoragePath(L"entries-v1.txt"),{L"schema\t99\tentries"}),"future version fixture saved");
    Check(!LoadEntries()&&g_entriesReadOnly&&!SaveEntries()&&!SaveTiles(),"unsupported entries refuse entry and layout writes");
    printf("Feature checks: %d, failures: %d\n",checks,failures);
    return failures?1:0;
}
