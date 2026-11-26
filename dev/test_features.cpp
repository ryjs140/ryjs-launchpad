#define SEAMLESS_PREVIEW 1
#include "../mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp"
#include <cstdio>
void Preview_RunSmoke(HWND) {}
int failures=0, checks=0;
void Check(bool value,const char* label) { ++checks; if (!value) { ++failures; printf("FAIL: %s\n",label); } }
int wmain(int argc,wchar_t** argv) {
    setvbuf(stdout,nullptr,_IONBF,0);  // Keep the log up to a crash.
    CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    WCHAR executable[32768]{};GetModuleFileNameW(nullptr,executable,ARRAYSIZE(executable));
    auto cases=std::filesystem::path(executable).parent_path()/(L"feature-data-"+std::to_wstring(GetTickCount64()));
    std::filesystem::create_directories(cases);Preview::directory=cases.wstring();
    const auto displayA = reinterpret_cast<HMONITOR>(1);
    const auto displayB = reinterpret_cast<HMONITOR>(2);
    const auto displayC = reinterpret_cast<HMONITOR>(3);
    std::vector<LauncherDisplay> displays{{displayA, {}, true, L"display:a", L"A"},
                                          {displayB, {}, false, L"display:b", L"B"},
                                          {displayC, {}, false, L"display:c", L"C"}};
    Check(ResolveLauncherDisplay(displays, L"display:b", displayA) == displayB,
          "a connected fixed display is found by device");
    displays.erase(displays.begin() + 1);
    Check(ResolveLauncherDisplay(displays, L"display:b", displayA) == displayA &&
          ResolveLauncherDisplay(displays, L"2", displayA) == displayC,
          "an offline fixed display falls back while legacy ordinals remain available");
    displays.push_back({displayB, {}, false, L"display:b", L"B"});
    Check(ResolveLauncherDisplay(displays, L"display:b", displayA) == displayB,
          "a fixed display is chosen again after reconnecting in another order");
    {
        SetWindow settings;
        settings.fields = Set_MakeFields();
        Preview::PutSetting(L"monitor", L"display:offline-fixture");
        Set_LoadFields(settings);
        auto field = std::find_if(settings.fields.begin(), settings.fields.end(), [](const auto& item) {
            return wcscmp(item.key, L"monitor") == 0;
        });
        Check(field != settings.fields.end() && field->cur == L"display:offline-fixture" &&
              field->value == field->cur, "opening settings preserves the offline display selection");
        Preview::PutSetting(L"monitor", L"mouse");
    }
    if ((argc==3||argc==4)&&std::wstring_view(argv[1])==L"--web-icon-probe") {
        App site;
        site.target=argv[2];
        site.websiteUrl=ResolveWebsiteUrl(site);
        StartWebIconFetch(site);
        if (g_webWorkers.empty()||WaitForSingleObject(g_webWorkers.front(),45000)!=WAIT_OBJECT_0) return 2;
        StopWebIcons();
        bool loaded=LoadWebsiteIcon(64,site);
        // The loopback fixture serves a red icon behind a redirect and a bad candidate.
        return loaded&&(argc==4||site.pixels[32*64+32]==0xffff0000) ? 0 : 1;
    }
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
    candidates=WebsiteIconCandidates(L"<base href='/theme/'><link rel='not-an-icon' href='/junk'><link rel='icon'><link rel='apple-touch-icon' href='touch.png'><link rel='icon' href='logo.svg'>",L"https://example.org/page");
    Check(candidates==std::vector<std::wstring>{L"https://example.org/theme/logo.svg",L"https://example.org/theme/touch.png",L"https://example.org/favicon.ico"},
          "website candidates prefer actual icon links and honor the declared base");
    auto shortcut=cases/L"website.url";
    std::ofstream(shortcut)<<"[InternetShortcut]\r\nURL=https://example.org/site\r\n";
    App website;website.target=shortcut.wstring();
    Check(ResolveWebsiteUrl(website)==L"https://example.org/site","Internet shortcuts use the destination favicon");
    website.target=L"C:\\Browser\\chrome.exe";website.arguments=L"--profile-directory=Default --app=\"https://example.org/site\"";
    Check(ResolveWebsiteUrl(website)==L"https://example.org/site","browser app arguments use the destination favicon");
    App uncachedWebsite;
    uncachedWebsite.target=L"https://uncached.invalid/icon-preservation-test";
    uncachedWebsite.websiteUrl=uncachedWebsite.target;
    uncachedWebsite.pixels={0xff123456};uncachedWebsite.iconSize=1;uncachedWebsite.generation=7;
    Check(!LoadWebsiteIcon(64,uncachedWebsite)&&uncachedWebsite.pixels==std::vector<UINT32>{0xff123456}&&
          uncachedWebsite.iconSize==1&&uncachedWebsite.generation==7,
          "a missing website cache never erases the existing shell icon");
    std::ofstream(cases/L"invalid-favicon.ico")<<"<html>Unavailable</html>";
    Check(!LoadIconFile((cases/L"invalid-favicon.ico").wstring(),64,website,false),"HTML cannot become a cached shell icon");
    std::ofstream(cases/L"icon.svg")<<"<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 64 64'><rect width='64' height='64' fill='red'/></svg>";
    App svg;Check(LoadSvgIcon((cases/L"icon.svg").wstring(),128,svg)&&svg.iconSize==128&&!svg.pixels.empty(),"SVG icon rasterizes at requested DPI size");
    WriteLines(StoragePath(L"icons.txt"),{L"icon\t"+AppKey(*refreshed)+L"\t"+(cases/L"icon.svg").wstring()});
    ApplyCustomIcons(64);
    const auto savedIcon=g_customIcons.at(AppKey(*refreshed));
    Check(IsManagedIcon(savedIcon)&&std::filesystem::exists(CustomIconPath(savedIcon)),"legacy external icon migrates to launcher storage");
    std::filesystem::remove(cases/L"icon.svg");
    refreshed->pixels.clear();
    g_customIcons.clear();
    ApplyCustomIcons(64);
    Check(refreshed->iconSize==64&&!refreshed->pixels.empty()&&g_customIcons.at(AppKey(*refreshed))==savedIcon,
          "custom icon reloads after deleting its original and re-reading the mapping");
    UpdateFilter();g_layout.width=1600;g_layout.height=1000;g_layout.scale=1;ComputeLayout();
    // A fresh square-group autolayout must not overlap groups. When groups
    // overlap, the first cell in g_groupCells wins the hit test even though
    // the later group is the one drawn on top, which reads as pointer drift.
    g_settings.groups=true;
    ClearGroupPositions();
    g_squareGridColumns=18;g_wholeCellLayout=true;g_groupPositions.clear();
    ComputeLayout();
    Check(GroupedView(),"square group layout test starts in grouped view");
    for(int i=0;i<(int)g_visible.size();++i) {
        auto box=CellAt(i).highlight;
        int x=(int)((box.left+box.right)/2),y=(int)((box.top+box.bottom)/2);
        Check(ItemAt(x,y)==i,"every square-group tile hit-tests at its drawn centre");
    }
    for(int a=0;a<(int)g_groupGeometry.size();++a) {
        for(int b=a+1;b<(int)g_groupGeometry.size();++b) {
            auto A=g_groupGeometry[a].area,B=g_groupGeometry[b].area;
            Check(A.right<=B.left||B.right<=A.left||A.bottom<=B.top||B.bottom<=A.top,
                  "automatic square groups never overlap");
        }
    }
    ClearGroupPositions();g_squareGridColumns=0;g_wholeCellLayout=false;ComputeLayout();
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
    const bool showLabels = g_settings.showLabels;
    const bool pages = g_settings.pages;
    const float oldScale = g_layout.scale;
    g_settings.showLabels = true;
    g_squareGridColumns = 16;
    struct TestScreen { int width, height; float scale; };
    for (auto screen : {TestScreen{1920, 1080, 1.25f}, TestScreen{3120, 2080, 2.5f}, TestScreen{3840, 2160, 2.f}}) {
        g_layout.width = screen.width;
        g_layout.height = screen.height;
        g_layout.scale = screen.scale;
        for (bool paged : {false, true}) {
            g_settings.pages = paged;
            for (int requested : {56, 192}) {
                g_settings.iconSize = requested;
                ComputeLayout();
                const auto drawn = std::find_if(g_groupCells.begin(), g_groupCells.end(), [](const Cell& cell) {
                    return cell.icon.right > cell.icon.left;
                });
                Check(drawn != g_groupCells.end() &&
                      std::abs(drawn->icon.right - drawn->icon.left - requested * screen.scale) < .01f &&
                      std::abs(drawn->label.bottom - drawn->label.top - g_layout.labelHeight) < .01f,
                      "mixed-DPI group grid honors the configured icon and label sizes");
                if (requested == 192) Check(g_layout.columns < 16 && !g_layout.useSavedGroupGeometry,
                    "large icons reduce the display columns when saved columns cannot fit");
            }
        }
    }
    Check(g_squareGridColumns == 16 && ReadLines(gridFile) == savedGeometry,
          "responsive layout preserves the saved column count and geometry");
    g_squareGridColumns = committedColumns;
    g_layout.scale = oldScale;
    g_settings.showLabels = showLabels;
    g_settings.pages = pages;
    g_settings.iconSize = size;
    g_layout.width=oldScreenWidth;g_layout.height=oldScreenHeight;ComputeLayout();
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
    {
        const bool oldPages = g_settings.pages;
        g_settings.pages = true;
        g_scroll = g_scrollTarget = g_groupPagePan = 0;
        ComputeLayout();
        const auto first = g_groupGeometry[0];
        const auto area = ProjectGroupRect(first.area);
        const int perPage = first.columns * first.visibleRows;
        auto wheel = [&](int delta) {
            WndProc(nullptr, WM_MOUSEWHEEL, MAKEWPARAM(0, delta),
                    MAKELPARAM((int)(area.left + 10), (int)(area.top + 10)));
        };
        wheel(-WHEEL_DELTA);
        Check(g_groupGeometry[0].firstInGroup == perPage && !g_groupPagePan && !g_scrollTarget,
              "one wheel detent on a group moves exactly one page");
        wheel(WHEEL_DELTA / 2);
        Check(g_groupGeometry[0].firstInGroup == perPage, "partial group wheel input waits for a full detent");
        wheel(WHEEL_DELTA / 2);
        Check(g_groupGeometry[0].firstInGroup == 0, "partial wheel input completes one page in the opposite direction");
        g_groupWheelTarget.reset();
        g_groupWheelRemainder = 0;
        g_settings.pages = oldPages;
        ComputeLayout();
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
            Check(std::abs(expected.icon.left - actual.icon.left) < .01f &&
                  std::abs(expected.icon.top - actual.icon.top) < .01f &&
                  std::abs(expected.label.right - actual.label.right) < .01f &&
                  std::abs(expected.label.bottom - actual.label.bottom) < .01f &&
                  expected.icon.right-expected.icon.left<=std::round(97*g_layout.scale)+.01f&&
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
    auto probeExecutable=cases.parent_path()/L"test_launch_probe.exe";
    if (!std::filesystem::is_regular_file(probeExecutable)) {
        fputs("The windowless launch probe is missing. Run dev/Test-Features.ps1.\n",stderr);
        return 1;
    }
    AppSelection probes;
    for(int n=0;n<3;n++) {
        auto app=std::make_shared<App>();app->name=L"Probe "+std::to_wstring(n);
        app->target=n==1?(cases/L"missing.exe").wstring():probeExecutable.wstring();
        app->stableKey=L"selection-probe-"+std::to_wstring(n);
        app->arguments=std::to_wstring(n)+L".txt";
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
    auto waitForMarker=[&](PCWSTR filename){
        // Process startup on a busy machine can take several seconds; a short
        // window here reports a launch failure that never happened.
        for(int tries=0;tries<750&&!std::filesystem::exists(cases/filename);tries++)Sleep(20);
        return std::filesystem::exists(cases/filename);
    };
    if (argc==2&&std::wstring_view(argv[1])==L"--keyboard-batch") {
        for (int index : {0,2}) {
            auto app=std::make_shared<App>(*probes[index]);
            auto marker=cases/(L"keyboard-"+std::to_wstring(index)+L".txt");
            app->arguments=L"\""+marker.wstring()+L"\"";
            g_appSelection.push_back(std::move(app));
        }
        SelectionChanged();
        SendMessageW(g_wnd,WM_KEYDOWN,VK_RETURN,0);
        Check(waitForMarker(L"keyboard-0.txt")&&waitForMarker(L"keyboard-2.txt")&&g_appSelection.empty(),
              "Enter launches both selected applications through the native key handler");
        DestroyWindow(g_wnd);
        printf("Keyboard batch launch: %s\n",failures?"failed":"passed");
        return failures?1:0;
    }
    click(0,0);
    Check(waitForMarker(L"0.txt")&&g_appSelection.empty(),"ordinary click requests launch with entry arguments and working directory");
    std::filesystem::remove(cases/L"0.txt");
    std::vector<std::wstring> requested;
    auto launchFailures=RequestSelectedApps(probes,[&](const App& app){
        requested.push_back(app.name);
        return RequestLaunch(app.target,nullptr,&app,true);
    });
    Check(requested==std::vector<std::wstring>{L"Probe 0",L"Probe 1",L"Probe 2"},"batch sends requests in selection order");
    Check(launchFailures==std::vector<std::wstring>{L"Probe 1"},"batch reports the failed entry individually");
    Check(waitForMarker(L"0.txt")&&waitForMarker(L"2.txt"),"both valid native requests launch across an intervening failure");    std::filesystem::remove(cases/L"0.txt");std::filesystem::remove(cases/L"2.txt");
    click(2,MK_CONTROL);click(0,0);
    int openX=(int)bar.right-20;
    send(WM_LBUTTONDOWN,MK_LBUTTON,openX,cancelY);send(WM_LBUTTONUP,0,openX,cancelY);
    Check(waitForMarker(L"0.txt")&&waitForMarker(L"2.txt")&&g_appSelection.empty()&&g_batchBlend==0,
          "batch toolbar opens selected apps and returns to normal");
    auto dragTo = [&](int index, int x, int y, bool cancel = false) {
        auto icon = ItemCell(index).icon;
        int fromX = (int)((icon.left + icon.right) / 2);
        int fromY = (int)((icon.top + icon.bottom) / 2);
        send(WM_LBUTTONDOWN, MK_LBUTTON, fromX, fromY);
        send(WM_MOUSEMOVE, MK_LBUTTON, x, y);
        Check(g_drag.active && g_drag.tiles.size() == 2, "dragging a selected icon grabs both apps");
        if (cancel) SendMessageW(g_wnd, WM_KEYDOWN, VK_ESCAPE, 0);
        else send(WM_LBUTTONUP, 0, x, y);
    };
    g_apps = probes;
    g_tiles = {{probes[0], nullptr}, {probes[1], nullptr}, {probes[2], nullptr}};
    Refresh();
    click(2, MK_CONTROL); click(0, MK_CONTROL);
    auto middle = CellAt(1);
    dragTo(2, (int)((middle.icon.left + middle.icon.right) / 2), (int)middle.highlight.bottom - 2);
    Check(g_tiles == Tiles{{probes[1], nullptr}, {probes[0], nullptr}, {probes[2], nullptr}} &&
          g_appSelection.size() == 2, "quick drop reorders selected tiles as a block in layout order");
    middle = CellAt(0);
    dragTo(2, (int)((middle.icon.left + middle.icon.right) / 2), (int)((middle.icon.top + middle.icon.bottom) / 2));
    auto createdFolder = g_tiles.empty() ? nullptr : g_tiles.front().folder;
    Check(createdFolder && createdFolder->apps == Tiles{{probes[1], nullptr}, {probes[0], nullptr}, {probes[2], nullptr}},
          "dropping on another icon creates one folder with every dragged app");
    if (createdFolder) {
        OpenFolder(createdFolder);
        auto folderClick = [&](int index) {
            auto icon = ItemCell(index).icon;
            int x = (int)((icon.left + icon.right) / 2), y = (int)((icon.top + icon.bottom) / 2);
            send(WM_LBUTTONDOWN, MK_LBUTTON | MK_CONTROL, x, y);
            send(WM_LBUTTONUP, MK_CONTROL, x, y);
        };
        folderClick(1); folderClick(2);
        SendMessageW(g_wnd, WM_KEYDOWN, VK_DELETE, 0);
        Check(g_appSelection.empty() && g_hidden.contains(AppKey(*probes[0])) &&
              g_hidden.contains(AppKey(*probes[2])) && !g_hidden.contains(AppKey(*probes[1])),
              "Delete removes selected icons inside an open folder");
    }
    g_hidden.clear();
    g_openFolder.reset();
    g_settings.groups = true;
    ClearGroupPositions();
    auto sourceGroup = std::make_shared<decltype(g_openFolder)::element_type>();
    auto targetGroup = std::make_shared<decltype(g_openFolder)::element_type>();
    sourceGroup->name = L"Source";
    sourceGroup->apps = {{probes[0], nullptr}, {probes[1], nullptr}};
    targetGroup->name = L"Target";
    targetGroup->apps = {{probes[2], nullptr}};
    g_tiles = {{nullptr, sourceGroup}, {nullptr, targetGroup}};
    Refresh();
    click(0, MK_CONTROL); click(1, MK_CONTROL);
    auto targetArea = ProjectGroupRect(g_groupGeometry[1].area);
    int targetX = (int)(targetArea.right - 8), targetY = (int)(targetArea.top + 8);
    auto beforeDrag = ArrangementLines();
    dragTo(0, targetX, targetY, true);
    Check(beforeDrag == ArrangementLines() && g_appSelection.size() == 2,
          "Escape cancels the drag while keeping the selection");
    dragTo(0, targetX, targetY);
    auto afterDrop = std::find_if(g_tiles.begin(), g_tiles.end(), [](const Tile& tile) {
        return tile.folder && tile.folder->name == L"Target";
    });
    Check(afterDrop != g_tiles.end() &&
          afterDrop->folder->apps == Tiles{{probes[2], nullptr}, {probes[0], nullptr}, {probes[1], nullptr}},
          "drop moves both apps into the target group and preserves their order");
    ClearAppSelection();
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
    {
        const auto previousLayout = g_layout;
        const int previousIconSize = g_settings.iconSize;
        g_settings.iconSize = 192;
        g_layout.width = 3120;
        g_layout.height = 2080;
        g_layout.scale = 2.5f;
        g_squareGridColumns = 16;
        Refresh();
        const int fittedColumns = g_layout.columns;
        const auto geometryOnDisk = ReadLines(GroupPositionFile());
        Check(!g_layout.useSavedGroupGeometry && fittedColumns < 16,
              "the 3:2 high-DPI fixture starts with a responsive browsing grid");
        ToggleGroupEdit();
        Check(g_groupEditMode && g_squareGridColumns == fittedColumns && g_layout.columns == fittedColumns,
              "the 3:2 editor adopts the columns used in browsing");
        FinishGroupEdit();
        Check(g_squareGridColumns == 16 && ReadLines(GroupPositionFile()) == geometryOnDisk,
              "leaving the responsive editor without changes preserves the original layout");
        ToggleGroupEdit();
        auto titlePoint = [&](const auto& group) {
            auto found = std::find_if(g_groupGeometry.begin(), g_groupGeometry.end(),
                [&](const auto& geometry) { return geometry.group == group; });
            auto title = ProjectGroupRect(found->title);
            return POINT{(LONG)((title.left + title.right) / 2), (LONG)((title.top + title.bottom) / 2)};
        };
        const auto left = titlePoint(topLeft);
        const auto right = titlePoint(topRight);
        int startX = (int)(GridMargin() / 2);
        int startY = (int)std::ceil(g_layout.viewTop + 2);
        send(WM_LBUTTONDOWN, MK_LBUTTON, startX, startY);
        send(WM_MOUSEMOVE, MK_LBUTTON, right.x + 4, right.y + 4);
        send(WM_LBUTTONUP, 0, right.x + 4, right.y + 4);
        Check(g_groupSelection == GroupSelection{topLeft, topRight}, "marquee selects two distinct groups");
        send(WM_LBUTTONDOWN, MK_LBUTTON | MK_CONTROL, right.x, right.y);
        send(WM_LBUTTONUP, MK_CONTROL, right.x, right.y);
        Check(g_groupSelection == GroupSelection{topLeft}, "Ctrl-click toggles a group off");
        send(WM_LBUTTONDOWN, MK_LBUTTON | MK_CONTROL, right.x, right.y);
        send(WM_LBUTTONUP, MK_CONTROL, right.x, right.y);
        int destination = left.y + (int)std::round(GroupStepY() * g_layout.scale);
        auto first = g_groupPositions.at(topLeft), second = g_groupPositions.at(topRight);
        send(WM_LBUTTONDOWN, MK_LBUTTON, left.x, left.y);
        send(WM_MOUSEMOVE, MK_LBUTTON, left.x, destination);
        Check(g_groupGesture.valid && g_groupGesture.companions.contains(topRight),
              "dragging a selected group previews the full selection");
        send(WM_LBUTTONUP, 0, left.x, destination);
        auto movedFirst = g_groupPositions.at(topLeft), movedSecond = g_groupPositions.at(topRight);
        Check(movedFirst.gridY > first.gridY &&
              movedFirst.gridY - first.gridY == movedSecond.gridY - second.gridY &&
              movedFirst.gridX - first.gridX == movedSecond.gridX - second.gridX,
              "group drag keeps the selected groups' relative spacing");
        auto beforeFinish = g_groupGeometry;
        Check(FinishGroupEdit() && g_layout.columns == fittedColumns && g_layout.useSavedGroupGeometry,
              "saving the 3:2 edit keeps the preview density");
        Check(beforeFinish.size() == g_groupGeometry.size() &&
              std::equal(beforeFinish.begin(), beforeFinish.end(), g_groupGeometry.begin(), [](const auto& a, const auto& b) {
                  return a.group == b.group && SameGroupBounds(PositionFromArea(a.area), PositionFromArea(b.area));
              }), "saved 3:2 group rectangles match the final preview");
        g_layout = previousLayout;
        g_settings.iconSize = previousIconSize;
    }
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
    g_settings.groups=false;
    g_tiles={{probes[0],nullptr},{probes[1],nullptr}};
    g_apps={probes[0],probes[1]};
    UpdateFilter();ComputeLayout();
    g_shown=true;g_appear=1;
    g_requestedAnimations=true;g_systemAnimations=true;
    auto type=[&](std::wstring_view text){
        for (WCHAR character : text) SendMessageW(g_wnd,WM_CHAR,character,0);
    };
    auto clickBackground=[&]{
        const int y=g_layout.height/2;
        send(WM_LBUTTONDOWN,MK_LBUTTON,2,y);send(WM_LBUTTONUP,0,2,y);
    };
    type(L"Probe ");
    Check(g_searchGridTransition==0&&g_frameTimer,"typing starts the search grid animation");
    g_lastFrame=Now()-0.08;OnFrame();
    const float progress=g_searchGridTransition;
    type(L"1");
    Check(progress>0&&progress<1&&g_searchGridTransition==progress&&
          g_visible.size()==1&&g_visible[0].app==probes[1],"typing refines results without restarting the transition");
    clickBackground();
    Check(g_shown&&!g_closing&&!SearchExpanded()&&g_visible.size()==2&&
          g_searchGridTransition==0,"the first background click animates back to the main grid");
    g_lastFrame=Now()-0.2;OnFrame();
    Check(g_searchGridTransition==1,"the search return animation finishes");
    clickBackground();
    Check(g_closing,"the second background click closes the launcher");
    g_requestedAnimations=false;OnFrame();
    g_shown=true;g_appear=1;
    type(L"no matches");
    Check(g_visible.empty()&&g_searchGridTransition==1,"reduced motion switches immediately to empty results");
    clickBackground();
    Check(g_shown&&!g_closing&&!SearchExpanded()&&g_searchGridTransition==1,
          "empty results also return to the main grid without animation");
    HideNow();
    DestroyWindow(g_wnd);g_wnd=nullptr;
    // Deleting the ungrouped block must remove its apps, not rebuild the same
    // block: that was the "cannot delete it" behaviour.
    {
        g_shown=false;
        g_settings.groups=true;
        g_layoutReadOnly=false;g_groupPositionsReadOnly=false;g_dataReadOnly=false;g_entriesReadOnly=false;
        ClearGroupPositions();g_tiles.clear();
        auto inbox=InboxGroup();
        auto app=std::make_shared<App>();app->name=L"InboxProbe";app->target=L"C:\\inbox-probe.exe";
        g_apps={app};inbox->apps.push_back({app,nullptr});
        auto key=AppKey(*app);
        DeleteGroup(inbox);
        Check(g_hidden.count(key)==1,"deleting the ungrouped block removes its apps");
        Check(std::none_of(g_tiles.begin(),g_tiles.end(),[](const Tile& tile){
                  return tile.folder&&tile.folder->inbox;}),
              "deleting the ungrouped block does not recreate it");
        g_hidden.erase(key);g_apps.clear();g_tiles.clear();
    }
    // Multi-select delete takes exactly the selected apps off the grid.
    {
        auto first=std::make_shared<App>();first->name=L"DelA";first->target=L"C:\\del-a.exe";
        auto second=std::make_shared<App>();second->name=L"DelB";second->target=L"C:\\del-b.exe";
        auto keep=std::make_shared<App>();keep->name=L"Keep";keep->target=L"C:\\keep.exe";
        g_apps={first,second,keep};
        g_tiles={{first,nullptr},{second,nullptr},{keep,nullptr}};
        g_appSelection={first,second};SelectionChanged();
        DeleteSelectedApps();
        Check(g_hidden.count(AppKey(*first))==1&&g_hidden.count(AppKey(*second))==1,
              "multi-select delete removes every selected app");
        Check(!g_hidden.count(AppKey(*keep)),"multi-select delete leaves the other apps alone");
        Check(g_tiles.size()==1&&g_tiles[0].app==keep,"the grid keeps only the unselected app");
        Check(g_appSelection.empty()&&g_selectedAppKeys.empty(),"the selection is cleared after deleting");
        g_hidden.clear();g_apps.clear();g_tiles.clear();
    }
    Check(WriteLines(GroupPositionFile(),{L"schema\t999\tgroup-geometry"}),"future geometry fixture saved");
    ClearGroupPositions();Check(!LoadGroupPositions()&&g_layoutReadOnly&&!SaveGroupPositions(),"unknown group geometry blocks layout writes");
    g_layoutReadOnly=false;g_groupPositionsReadOnly=false;g_freeGroupLayout=false;
    Check(WriteLines(StoragePath(L"entries-v1.txt"),{L"schema\t99\tentries"}),"future version fixture saved");
    Check(!LoadEntries()&&g_entriesReadOnly&&!SaveEntries()&&!SaveTiles(),"unsupported entries refuse entry and layout writes");
    // A predecessor that cannot be validated must not be migrated halfway, and
    // it must not leave the mod read-only forever either.
    {
        auto root=std::filesystem::path(cases)/(L"migration-"+std::to_wstring(GetTickCount64()));
        auto source=root/L"local@seamless-launchpad",target=root/L"data-v1";
        std::filesystem::create_directories(source);
        { std::ofstream bad(source/L"groups-legacy.txt"); bad<<"app\tC:\\x.exe\n"; }
        auto readAll=[](const std::filesystem::path& file) {
            std::ifstream input(file,std::ios::binary);
            return std::string(std::istreambuf_iterator<char>(input),{});
        };
        auto before=readAll(source/L"groups-legacy.txt");
        Check(!ryjs_data::Prepare(source.wstring(),target.wstring(),[](const std::wstring&){return true;}),
              "an unreadable predecessor is rejected instead of half-migrated");
        Check(!std::filesystem::exists(target),"a rejected migration leaves no data-v1");
        Check(std::filesystem::exists(source/L"groups-legacy.txt")&&readAll(source/L"groups-legacy.txt")==before,
              "a rejected migration does not modify the predecessor folder");
        Check(ryjs_data::Prepare(L"",target.wstring(),[](const std::wstring&){return true;})&&
              std::filesystem::exists(target/L"ryjs-storage.ini"),
              "a fresh empty data folder is created after a rejected migration");
        WriteMigrationNote(target.wstring(),source.wstring());
        Check(std::filesystem::exists(target/L"migration-note.txt"),
              "the skipped migration is recorded beside the new data");
    }
    // Start-button reset lifetime is enforced in the real mod by the
    // Windhawk taskbar subclass API; no preview-only counter can model it.
    // Repeated enable/disable in one process: the teardown has to drop every
    // holder, not just the containers the grid draws from.
    {
        auto app=std::make_shared<App>();app->name=L"Probe";app->target=L"C:\\probe.exe";
        auto group=InboxGroup();group->name=L"Probe";
        group->apps.push_back({app,nullptr});
        g_tiles.push_back({nullptr,group});
        g_groupPositions[group]=GroupPosition{};
        g_groupRows[group]=1;
        g_groupEditSnapshot[group]=GroupPosition{};
        g_groupEditRows[group]=1;
        g_openFolder=group;
        g_closingFolder=group;
        g_groupResizeHover=group;
        g_groupHeaderPressed=group;
        g_groupDropFolder=group;
        g_groupHeaderDragging=group;
        g_drag.tile=Tile{app,nullptr};
        g_appSelection.push_back(app);
        g_selectionPointer.app=app;
        auto weak=std::weak_ptr(group);
        group.reset();app.reset();
        ReleaseLauncherResources();
        Check(weak.expired(),"the teardown releases every group and folder holder");
        Check(g_groupPositions.empty()&&g_groupRows.empty()&&g_groupEditSnapshot.empty()&&g_groupEditRows.empty(),
              "the group layout maps are cleared");
        Check(!g_openFolder&&!g_closingFolder&&!g_groupResizeHover&&!g_drag.tile.app&&!g_drag.tile.folder&&
              g_appSelection.empty()&&!g_selectionPointer.app,"every pointer holder is empty after the teardown");
        ReleaseLauncherResources();
        Check(g_tiles.empty()&&!g_wnd,"a second teardown on an empty state is harmless");
        App bad;bad.iconSize=64;
        Check(!WriteIconPng(L"unused.png",bad),"the PNG writer refuses a buffer that does not match the icon size");
    }
    // Review item 1, end to end: a saved group holds an app, the app holds a
    // GPU bitmap, and the bitmap holds the device. The teardown has to break
    // that chain, or every disable/update leaks the whole device tree.
    {
        for (int cycle=0;cycle<3;cycle++) {
            // StartThread creates the factories before the window; recreate
            // them each cycle so this matches one enable after each disable.
            if (!g_d2d) D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,g_d2d.GetAddressOf());
            if (!g_dwrite) DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),(IUnknown**)g_dwrite.GetAddressOf());
            if (!EnsureTarget()) { printf("SKIP: no D3D device in this environment; GPU release cycle not exercised\n"); break; }
            ComPtr<ID2D1Bitmap1> bitmap;
            D2D1_BITMAP_PROPERTIES1 props=D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED));
            Check(SUCCEEDED(g_target->CreateBitmap(D2D1::SizeU(4,4),nullptr,0,props,&bitmap))&&bitmap,
                  "a GPU bitmap can be created on the grid device");
            auto app=std::make_shared<App>();app->name=L"GpuProbe";app->target=L"C:\\gpu.exe";
            app->bitmap=bitmap;app->iconSize=4;app->pixels.assign(16,0);
            auto group=InboxGroup();group->apps.push_back({app,nullptr});
            g_groupPositions[group]=GroupPosition{};g_groupRows[group]=1;
            g_closingFolder=group;
            auto weak=std::weak_ptr(group);
            bitmap.Reset();app.reset();group.reset();
            ReleaseLauncherResources();
            Check(weak.expired(),"a group holding a GPU bitmap does not survive the teardown");
            Check(!g_target&&!g_d2dDevice&&!g_d3d,"the device objects are released with the groups");
            Check(g_groupPositions.empty()&&!g_closingFolder,"the layout holders are empty after the cycle");
        }
    }
    // The power dock's one shared warning source: the agreed colour ramp,
    // geometry and the continuous time constants, plus teardown.
    {
        const auto& spec = power_ambient::Get();
        Check(std::abs(spec.stop35 - .88f) < 1e-4f && std::abs(spec.stop65 - .48f) < 1e-4f,
              "the radial stops stay at 88% and 48% of the centre");
        Check(std::abs(spec.radiusX - 23.f) < .01f && std::abs(spec.radiusY - 23.f) < .01f &&
              std::abs(spec.radiusX - spec.radiusY) < 1e-4f,
              "the light is a circle (X radius == Y radius == 23 DIP)");
        Check(std::abs(spec.yOffset) <= 1.f,
              "the only vertical compensation allowed is -1..+1 DIP");
        Check(spec.positionMs == 35.f && spec.riseMs == 120 && spec.fallMs == 180,
              "position follows in 35 ms; intensity rises in 120 ms and falls in 180 ms");
        Check(std::abs(spec.pressScale - .95f) < 1e-4f,
              "press keeps its 0.95 scale");
        Check(kSetCornerDip == 20.f && kDwmRedirectionBitmapAlpha == 39,
              "the settings frame asks for a real 20 DIP mask through DWM alpha");
        Check(g_settings.powerWarning && g_settings.powerWarningStrength == 78 &&
              g_settings.powerWarningSpread == 100 && g_settings.powerWarningEdge == 65 &&
              g_settings.powerWarningBleed == 15,
              "the warning defaults are on, 78%, 100%, 65% and 15%");
        Check(g_settings.powerWarningActions[1] && g_settings.powerWarningActions[4] &&
              g_settings.powerWarningActions[5] && !g_settings.powerWarningActions[0],
              "the default warning actions are sign out, restart and shut down");
        Check(!g_settings.powerWarningFollowButton,
              "the default follow mode is the pointer");

        g_requestedAnimations = true;
        g_systemAnimations = true;
        ResetPowerGlass();
        g_layout.scale = 1.f;
        g_layout.powerItems = {0, 1, 2};
        g_layout.powerBar = D2D1::RectF(100, 900, 300, 944);
        g_layout.powerButton = 36;
        g_layout.powerGap = 12;
        g_powerHover = 1;  // Sign out, warning on by default.
        g_powerPointerInside = true;
        const float startX = PowerButtonCentreX(1);
        const float nextX = PowerButtonCentreX(2);
        g_powerPointerX = startX;
        g_powerGlass.tick = Now() - .05;
        UpdatePowerGlass();
        Check(g_powerGlass.targetAlpha == 1.f && g_powerGlass.alpha > 0,
              "hovering a warned button targets one light");
        Check(std::abs(g_powerGlass.y - 922.f) < .01f,
              "the light Y is the capsule geometry centre");

        const float beforeX = g_powerGlass.x;
        g_powerPointerX = startX + 12.f;
        g_powerGlass.tick = Now() - .05;
        UpdatePowerGlass();
        Check(g_powerGlass.x > beforeX && g_powerGlass.x < startX + 4.f,
              "pointer tracking stays local to the hovered button");

        g_settings.powerWarningActions[2] = true;
        g_settings.powerWarningFollowButton = true;
        g_powerHover = -1;
        const float middle = (startX + nextX) * .5f;
        g_powerPointerX = middle - 3.f;
        g_powerGlass.tick = Now() - .08;
        UpdatePowerGlass();
        const float nearShare = g_powerGlass.weights[2];
        Check(g_powerGlass.litSlot == 1 && nearShare > .20f && nearShare < .50f &&
              g_powerGlass.targetX == startX,
              "button mode starts to light the next anchor within the gap");
        g_powerPointerX = middle + 3.f;
        g_powerGlass.tick = Now() - .08;
        g_glowAnimating = false;
        UpdatePowerGlass();
        const float farShare = g_powerGlass.weights[2];
        Check(g_glowAnimating && g_powerGlass.litSlot == 1 && g_powerGlass.targetAlpha == 1.f &&
              farShare > nearShare + .25f && g_powerGlass.targetX == startX,
              "light follows travel across the gap before the next button is hovered");
        g_powerPointerX = middle - 3.f;
        g_powerGlass.tick = Now() - .08;
        UpdatePowerGlass();
        Check(g_powerGlass.litSlot == 1 && g_powerGlass.weights[2] < farShare - .25f,
              "reversing inside the gap reverses the transfer without a hover change");

        g_powerHover = 2;
        g_powerPointerX = nextX;
        g_powerGlass.tick = Now() - .04;
        UpdatePowerGlass();
        Check(g_powerGlass.litSlot == 2 && g_powerGlass.weights[2] > .60f,
              "entering the destination completes the pointer-directed transfer");
        g_powerHover = 1;
        g_powerPointerX = startX;
        g_powerGlass.tick = Now() - .025;
        UpdatePowerGlass();
        Check(g_powerGlass.litSlot == 1 && g_powerGlass.weights[2] > .1f,
              "a jump in the reverse direction fades the old lobe rather than snapping it off");
        for (int step = 0; step < 2; ++step) {
            g_powerGlass.tick = Now() - .25;
            UpdatePowerGlass();
        }
        Check(g_powerGlass.x == startX && g_powerGlass.weights[1] == 1 &&
              g_powerGlass.weights[2] == 0, "the two anchored lobes settle to one circle");
        g_requestedAnimations = false;
        g_powerHover = 2;
        g_powerPointerX = nextX;
        UpdatePowerGlass();
        Check(g_powerGlass.x == nextX && g_powerGlass.weights[1] == 0 &&
              g_powerGlass.weights[2] == 1, "disabled animations jump to the new anchor");
        g_requestedAnimations = true;

        g_settings.powerWarningFollowButton = false;

        // Moving to a normal button fades the remaining light.
        g_settings.powerWarningActions[0] = false;
        g_powerHover = 0;
        g_powerPointerX = 160.f;
        g_powerGlass.tick = Now() - .05;
        UpdatePowerGlass();
        Check(g_powerGlass.targetAlpha == 0.f && g_powerGlass.alpha > 0,
              "a normal button fades the light instead of switching it off instantly");
        g_powerGlass.tick = Now() - .25;
        UpdatePowerGlass();
        g_powerGlass.tick = Now() - .25;
        UpdatePowerGlass();
        Check(g_powerGlass.alpha < .05f, "the light reaches zero after the fall time");

        // The gap between two warned buttons is not a reset.
        g_powerHover = -1;
        g_powerPointerInside = true;
        g_powerPointerX = 200.f;  // Between sign-out and restart centres.
        g_powerGlass.tick = Now() - .05;
        UpdatePowerGlass();
        Check(g_powerGlass.targetAlpha == 1.f,
              "the gap between two warned buttons keeps the light on");

        g_powerHover = -1;
        g_powerPointerInside = false;
        UpdatePowerGlass();
        g_powerGlass.tick = Now() - .25;
        UpdatePowerGlass();
        g_powerGlass.tick = Now() - .25;
        UpdatePowerGlass();
        Check(g_powerGlass.alpha < .05f, "leaving the capsule fades the light out");
        ResetPowerGlass();
        Check(g_powerGlass.slot == -1 && g_powerGlass.alpha == 0 &&
              g_powerGlass.targetAlpha == 0,
              "closing the menu leaves no light or animation behind");
        g_frameTimer = false;
        g_layout.powerItems.clear();
    }
    // Glow reach: measured from the icon's hit box so the distance setting has
    // room to act, and resolved to the closest tile so the lit icon is always
    // the one the pointer is nearest — never a neighbour picked by paint order.
    {
        const D2D1_RECT_F box = D2D1::RectF(0, 0, 100, 80);
        Check(PointBoxDistance(box, 50, 40) == 0, "a point inside the icon is at distance 0");
        Check(std::abs(PointBoxDistance(box, 112, 40) - 12.f) < .01f,
              "the reach is measured from the icon's edge");
        Check(std::abs(PointBoxDistance(box, 112, 96) - 20.f) < .01f,
              "a diagonal point uses the box corner, not either axis");
        Check(std::abs(PointBoxDistance(box, -9, -12) - 15.f) < .01f,
              "the distance never goes negative outside the icon");
        Cell cell{};
        cell.unit = D2D1::RectF(0, 0, 120, 160);
        cell.icon = D2D1::RectF(30, 20, 90, 80);  // Icon centre is above the cell centre.
        const D2D1_POINT_2F centre{.5f, .3125f}, movedRight{.55f, .3125f};
        const auto neutral = TileRimLight(cell, centre, 80, 1.5f);
        const auto low = TileRimLight(cell, movedRight, 0, 1.5f);
        const auto mid = TileRimLight(cell, movedRight, 40, 1.5f);
        const auto high = TileRimLight(cell, movedRight, 80, 1.5f);
        const auto upper = TileRimLight(cell, {.5f, .27f}, 80, 1.5f);
        const auto lower = TileRimLight(cell, {.5f, .36f}, 80, 1.5f);
        Check(neutral.x == centre.x && neutral.y == centre.y,
              "the actual icon centre has no preferred rim");
        Check(low.x == movedRight.x && low.y == movedRight.y && mid.x > low.x &&
              mid.x < high.x && std::abs(high.x - 1.f) < .001f,
              "a higher setting increasingly steers small pointer movements to the facing edge");
        Check(upper.y < .001f && lower.y > .999f,
              "above and below the icon centre reach the top and bottom rim on a scaled layout");
        g_glowNearest = -1;
        g_glassScenePointer.reset();
        Check(!GlowNearestIs(nullptr, 24), "without a pointer nothing resolves as nearest");
    }
    // The settings window's own theme: the palette flips with themeMode, and
    // the dark table stays byte-for-byte what 1.4.x shipped.
    {
        const std::wstring previousTheme = GetStringSetting(L"themeMode");
        Preview::PutSetting(L"themeMode", L"light");
        settings_palette::Refresh();
        Check(pal().light && pal().text == 0x111111 && pal().base == 0xFFFFFF,
              "light puts ink text on white cards");
        Check(SetGlassTint() > .8f, "the light window is tinted more solidly than dark glass");
        Preview::PutSetting(L"themeMode", L"dark");
        settings_palette::Refresh();
        Check(!pal().light && pal().text == 0xFFFFFF && pal().base == 0x242424,
              "the dark palette is unchanged");
        Preview::PutSetting(L"themeMode", L"system");
        settings_palette::Refresh();
        Check(pal().light == settings_palette::SystemLight(),
              "system follows the Windows app theme");
        Preview::PutSetting(L"themeMode", previousTheme);
        settings_palette::Refresh();
    }
    // The material file reader is reachable from the settings window; nested
    // arrays must fail before they can exhaust the stack, and an oversized
    // import must be refused instead of being loaded into memory.
    {
        std::wstring shallow(8, L'[');
        shallow.append(8, L']');
        launchpad_material::Json shallowJson;
        Check(launchpad_material::JsonReader(shallow).Read(shallowJson),
              "shallow material JSON parses");
        std::wstring deep(100, L'[');
        deep.append(100, L']');
        launchpad_material::Json deepJson;
        Check(!launchpad_material::JsonReader(deep).Read(deepJson),
              "material JSON nesting is bounded");
        std::vector<launchpad_material::UserPreset> presets;
        Check(!launchpad_material::ParseUserPresets(deep, presets) && presets.empty(),
              "stored preset nesting is bounded");
    }
    {
        std::wstring oversized = cases / L"oversized-material.ryjsm";
        HANDLE file = CreateFileW(oversized.c_str(), GENERIC_WRITE, 0, nullptr,
                                  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        Check(file != INVALID_HANDLE_VALUE, "oversized material fixture created");
        if (file != INVALID_HANDLE_VALUE) {
            std::string bytes(8 * 1024 * 1024 + 1, 'x');
            DWORD written = 0;
            Check(WriteFile(file, bytes.data(), (DWORD)bytes.size(), &written, nullptr) &&
                  written == bytes.size(), "oversized material fixture written");
            CloseHandle(file);
        }
        std::wstring text;
        Check(!Set_ReadUtf8File(oversized, text) && text.empty(),
              "material import refuses files above the size cap");
        DeleteFileW(oversized.c_str());
    }
    // A real (hidden) settings window must give back its memory DC and DIB.
    // Three cycles expose the two GDI objects per close that the old
    // WM_NCDESTROY path leaked.
    {
        // Warm up the one-time GDI caches (icon decoding and the window class)
        // before measuring; the loop below is what must be flat.
        Set_Create(false);
        if (g_setWin && g_setWin->hwnd) DestroyWindow(g_setWin->hwnd);
        int before = (int)GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
        for (int i = 0; i < 3; ++i) {
            Check(Set_Create(false), "settings window creates for a resource cycle");
            if (g_setWin && g_setWin->hwnd) DestroyWindow(g_setWin->hwnd);
            Check(!g_setWin, "settings window releases its window object");
        }
        MSG message{};
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
        }
        int after = (int)GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
        Check(after <= before + 1, "settings window releases its GDI resources");
    }
    printf("Feature checks: %d, failures: %d\n",checks,failures);
    return failures?1:0;
}
