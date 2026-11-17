// Optional geometry only. Legacy membership/order files retain their format.
struct GroupPosition {float x=0,y=0,w=400,h=240;};
[[clang::no_destroy]] std::map<std::shared_ptr<Folder>,GroupPosition> g_groupPositions;
[[clang::no_destroy]] std::map<std::shared_ptr<Folder>,int> g_groupRows;
bool g_freeGroupLayout=false,g_groupEditMode=false,g_groupPositionsReadOnly=false;
struct GroupGeometryGesture {
    std::shared_ptr<Folder> group;
    GroupPosition original,draft;
    POINT start{};
    float initialScroll=0;
    bool resize=false,valid=false,moved=false;
};
[[clang::no_destroy]] GroupGeometryGesture g_groupGesture;
struct ArrangementGeometry {
    size_t ordinal;std::wstring name;GroupPosition position;
    int row;bool collapsed;
};
[[clang::no_destroy]] std::vector<ArrangementGeometry> g_arrangementGeometry;
void BeforeArrangeGeometry() {
    g_arrangementGeometry.clear();
    if(!g_freeGroupLayout&&!g_groupEditMode)return;
    for(size_t i=0;i<g_tiles.size();i++)if(auto group=g_tiles[i].folder) {
        auto found=g_groupPositions.find(group);
        if(found!=g_groupPositions.end())g_arrangementGeometry.push_back({i,group->name,found->second,g_groupRows[group],group->collapsed});
    }
}
void AfterArrangeGeometry() {
    if(!g_freeGroupLayout&&!g_groupEditMode)return;
    g_groupPositions.clear();g_groupRows.clear();
    for(const auto& saved:g_arrangementGeometry)if(saved.ordinal<g_tiles.size()) {
        auto group=g_tiles[saved.ordinal].folder;
        if(group&&group->name==saved.name) {
            g_groupPositions[group]=saved.position;
            g_groupRows[group]=saved.row;
            group->collapsed=saved.collapsed;
        }
    }
    g_arrangementGeometry.clear();
}
std::wstring GroupPositionFile(){return StoragePath(L"group-geometry-"+LegacyLayoutFile());}
void ClearGroupPositions() {
    g_groupPositions.clear();
    g_groupRows.clear();
    g_groupGesture={};
    g_freeGroupLayout=false;
    g_groupEditMode=false;
    g_groupPositionsReadOnly=false;
}
bool PositionNumber(const std::wstring& text,float& value) {
    WCHAR* end=nullptr;value=wcstof(text.c_str(),&end);
    return !text.empty()&&end&&!*end&&std::isfinite(value)&&value>=0&&value<=1000000;
}
bool LoadGroupPositions() {
    auto path=GroupPositionFile();bool read=false;auto rows=ReadLines(path,&read);
    if(GetFileAttributesW(path.c_str())==INVALID_FILE_ATTRIBUTES&&GetLastError()==ERROR_FILE_NOT_FOUND)return true;
    bool valid=read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"1",L"group-geometry"};
    std::map<std::shared_ptr<Folder>,GroupPosition> parsed;
    std::map<std::shared_ptr<Folder>,bool> collapsed;
    for(size_t i=1;valid&&i<rows.size();i++) {
        auto& r=rows[i];GroupPosition p;std::wstring name;float ordinal=0;
        valid=r.size()==8&&r[0]==L"group"&&(r[7]==L"group"||r[7]==L"folder")&&PositionNumber(r[1],ordinal)&&ordinal==std::floor(ordinal)&&ordinal<g_tiles.size()&&
            EntryDecode(r[2],name)&&PositionNumber(r[3],p.x)&&PositionNumber(r[4],p.y)&&PositionNumber(r[5],p.w)&&PositionNumber(r[6],p.h)&&p.w>=48&&p.h>=60;
        if(!valid)break;
        auto group=g_tiles[(size_t)ordinal].folder;
        valid=group&&group->name==name&&parsed.emplace(group,p).second;
        if(valid)collapsed[group]=r[7]==L"folder";
    }
    if(valid)for(const auto& tile:g_tiles)if(tile.folder&&!parsed.count(tile.folder))valid=false;
    g_groupPositionsReadOnly=!valid;
    if(valid){g_groupPositions=std::move(parsed);for(const auto& [group,value]:collapsed)group->collapsed=value;g_freeGroupLayout=true;}
    else {g_layoutReadOnly=true;Wh_Log(L"Group geometry cannot be matched safely; layout writes disabled.");}
    return valid;
}
bool SaveGroupPositions() {
    if(g_groupPositionsReadOnly||g_layoutReadOnly||g_dataReadOnly)return false;
    if(!g_freeGroupLayout)return true;
    std::vector<std::wstring> lines{L"schema\t1\tgroup-geometry"};
    float bottom=0;for(const auto& [group,p]:g_groupPositions)bottom=std::max(bottom,p.y+p.h);
    for(size_t i=0;i<g_tiles.size();i++)if(auto group=g_tiles[i].folder) {
        // Only an explicit new-group operation can introduce an unmatched group.
        if(!g_groupPositions.count(group)){g_groupPositions[group]={24,bottom+32,400,240};bottom+=272;}
        auto p=g_groupPositions[group];
        lines.push_back(L"group\t"+std::to_wstring(i)+L"\t"+EntryEncode(group->name)+L"\t"+std::to_wstring(p.x)+L"\t"+
            std::to_wstring(p.y)+L"\t"+std::to_wstring(p.w)+L"\t"+std::to_wstring(p.h)+L"\t"+(group->collapsed?L"folder":L"group"));
    }
    return WriteLines(GroupPositionFile(),lines);
}
