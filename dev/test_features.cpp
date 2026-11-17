#define SEAMLESS_PREVIEW 1
#include "../mods/local/seamless-launchpad/seamless-launchpad.wh.cpp"
#include <cstdio>
void Preview_RunSmoke(HWND) {}
int failures=0, checks=0;
void Check(bool value,const char* label) { ++checks; if (!value) { ++failures; printf("FAIL: %s\n",label); } }
int wmain() {
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
    ToggleGroupEdit();Check(g_groupEditMode&&!g_freeGroupLayout&&!std::filesystem::exists(GroupPositionFile()),"entering edit mode alone does not persist or convert layout");
    auto group=g_tiles[0].folder,second=g_tiles[1].folder;
    g_groupGesture.group=group;g_groupGesture.original=g_groupPositions[group];g_groupGesture.draft=g_groupGesture.original;g_groupGesture.resize=true;
    g_groupGesture.start={0,0};g_groupGesture.initialScroll=g_scroll;
    FreeGroupPointerMove(-32,32);
    Check(g_groupGesture.valid&&g_groupGesture.moved,"resize previews a valid snapped rectangle");
    auto resized=g_groupGesture.draft;FreeGroupPointerUp(nullptr);
    Check(g_freeGroupLayout&&g_groupPositions[group].w==resized.w&&ArrangementLines()==membership,"resizing saves geometry without changing membership or order");
    auto firstPosition=g_groupPositions[group];g_groupPositions[second]={firstPosition.x,firstPosition.y+firstPosition.h+32,firstPosition.w,240};
    Check(SaveGroupPositions(),"two groups can be persisted in the same column");
    ComputeGroups();Check(g_groupGeometry.size()==2&&g_groupGeometry[0].area.left==g_groupGeometry[1].area.left&&g_groupGeometry[0].area.bottom<g_groupGeometry[1].area.top,"same-column groups are vertically independent");
    Check(GroupsOverlap(firstPosition,firstPosition)&&!GroupsOverlap(firstPosition,g_groupPositions[second]),"overlap detection distinguishes valid stacked groups");
    ClearGroupPositions();Check(LoadGroupPositions()&&g_freeGroupLayout&&g_groupPositions[group].w==resized.w,"group geometry reload retains individual dimensions");
    group->collapsed=true;Check(SaveGroupPositions(),"traditional folder presentation saved explicitly");
    group->collapsed=false;ClearGroupPositions();LoadGroupPositions();UpdateFilter();ComputeGroups();
    Check(group->collapsed&&!g_visible.empty()&&g_visible[0].folder==group&&ArrangementLines()==membership,"traditional folder coexists without altering membership");
    auto widthBefore=g_groupPositions[group].w;
    ArrangeTiles(membership);UpdateFilter();ComputeLayout();group=g_tiles[0].folder;
    Check(g_groupPositions.count(group)&&g_groupPositions[group].w==widthBefore&&group->collapsed,"drag cancel/rearrangement preserves geometry and traditional folders");
    Preview::PutSetting(L"themeMode",L"light");LoadSettings();Check(g_settings.textColor.r<0.2f&&g_settings.labelPlateColor.r>0.9f,"light theme uses dark text and a light name backplate");
    Preview::PutSetting(L"themeMode",L"dark");LoadSettings();Check(g_settings.textColor.r>0.9f&&g_settings.labelPlateColor.r<0.2f,"dark theme uses light text and a dark name backplate");
    Preview::PutSetting(L"themeMode",L"custom");Preview::PutSetting(L"textColor",L"#123456");LoadSettings();
    Check(std::abs(g_settings.textColor.r-0x12/255.0f)<0.001f&&g_settings.labelPlateOpacity==0.65f,"custom theme preserves chosen color and glare protection default");
    Check(ArrangementLines()==membership,"theme changes do not modify group contents");
    Check(WriteLines(GroupPositionFile(),{L"schema\t999\tgroup-geometry"}),"future geometry fixture saved");
    ClearGroupPositions();Check(!LoadGroupPositions()&&g_layoutReadOnly&&!SaveGroupPositions(),"unknown group geometry blocks layout writes");
    g_layoutReadOnly=false;g_groupPositionsReadOnly=false;g_freeGroupLayout=false;
    Check(WriteLines(StoragePath(L"entries-v1.txt"),{L"schema\t99\tentries"}),"future version fixture saved");
    Check(!LoadEntries()&&g_entriesReadOnly&&!SaveEntries()&&!SaveTiles(),"unsupported entries refuse entry and layout writes");
    printf("Feature checks: %d, failures: %d\n",checks,failures);
    return failures?1:0;
}
