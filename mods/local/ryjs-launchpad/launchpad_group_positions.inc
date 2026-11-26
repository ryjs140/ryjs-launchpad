// Optional geometry only. Legacy membership/order files retain their format.
struct GroupPosition {
    float x=0,y=0,w=400,h=240; // Legacy DIP rectangle, also a compatibility copy for grid records.
    int columns=0,rows=0;   // Legacy whole-cell counts.
    int gridX=0,gridY=0,gridW=0,gridH=0;
    int style=0; // 0: legacy, 1: exact DIPs, 2: grid-sized icon body, 3: automatic columns in exact DIPs, 4: previous whole-cell bounds, 5: square-grid bounds.
    bool snapped=false;
    bool SquareCells() const { return style == 5; }
    bool WholeCells() const { return style == 4; }
    bool OnGrid() const { return gridW > 0; }
};
[[clang::no_destroy]] std::map<std::shared_ptr<Folder>,GroupPosition> g_groupPositions;
[[clang::no_destroy]] std::map<std::shared_ptr<Folder>,int> g_groupRows;
int g_squareGridColumns=0,g_groupEditPreviousColumns=0;
bool g_wholeCellLayout=false,g_groupEditWasWholeCell=false;
bool g_freeGroupLayout=false,g_groupEditMode=false,g_groupPositionsReadOnly=false;
[[clang::no_destroy]] std::map<std::shared_ptr<Folder>,GroupPosition> g_groupEditSnapshot;
[[clang::no_destroy]] std::map<std::shared_ptr<Folder>,int> g_groupEditRows;
bool g_groupEditWasFree=false,g_groupEditDirty=false;
using GroupSelection = std::set<std::shared_ptr<Folder>>;
[[clang::no_destroy]] GroupSelection g_groupSelection;
struct GroupMarquee {
    GroupSelection before;
    POINT start{}, end{};
    bool pending = false, moved = false;
};
[[clang::no_destroy]] GroupMarquee g_groupMarquee;
struct GroupEditNode {
    std::shared_ptr<Folder> group;
    GroupPosition position;
    bool fixed=false; // An untouched automatic group split across pages.
    int start = 0;
};
struct GroupVisualTransition {
    GroupPosition from,to;
    double started=0;
};
using GroupTransitions = std::map<std::shared_ptr<Folder>,GroupVisualTransition>;
[[clang::no_destroy]] GroupTransitions g_groupTransitions;
struct GroupGeometryGesture {
    std::shared_ptr<Folder> group;
    GroupPosition original,draft,continuous;
    std::vector<GroupEditNode> baseline,solution;
    GroupSelection companions;
    D2D1_POINT_2F grab{},handle{};
    bool solved=false;
    POINT start{},at{};
    double edgeSince=0;
    int edge=0,activeStart=0;
    float lastScroll=0;
    bool resize=false,valid=false,moved=false;
};
[[clang::no_destroy]] GroupGeometryGesture g_groupGesture;
bool GroupMovesWithGesture(const GroupGeometryGesture& gesture, const std::shared_ptr<Folder>& group) {
    return group == gesture.group || gesture.companions.contains(group);
}
[[clang::no_destroy]] std::shared_ptr<Folder> g_groupResizeHover;
float g_groupResizeGlow=0;

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
    g_groupGesture={};g_groupTransitions.clear();g_groupResizeHover.reset();g_groupResizeGlow=0;
    g_groupSelection.clear();g_groupMarquee={};
    g_freeGroupLayout=false;g_wholeCellLayout=false;g_squareGridColumns=0;
    g_groupEditMode=false;
    g_groupEditSnapshot.clear();g_groupEditRows.clear();g_groupEditDirty=false;
    g_groupPositionsReadOnly=false;
}
bool PositionNumber(const std::wstring& text,float& value) {
    WCHAR* end=nullptr;value=wcstof(text.c_str(),&end);
    return !text.empty()&&end&&!*end&&std::isfinite(value)&&value>=0&&value<=1000000;
}
bool LoadGroupPositions(const std::wstring& backup={}) {
    auto path=backup.empty()?GroupPositionFile():backup;
    bool read=false;auto rows=ReadLines(path,&read);
    if(backup.empty()&&GetFileAttributesW(path.c_str())==INVALID_FILE_ATTRIBUTES&&GetLastError()==ERROR_FILE_NOT_FOUND)return true;
    float columns=0;
    bool v6=read&&!rows.empty()&&rows[0].size()==4&&rows[0][0]==L"schema"&&rows[0][1]==L"6"&&
        rows[0][2]==L"group-geometry"&&PositionNumber(rows[0][3],columns)&&
        columns==std::floor(columns)&&columns>=1&&columns<=30;
    bool v5=v6||(read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"5",L"group-geometry"});
    bool v4=v5||(read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"4",L"group-geometry"});
    bool v3=read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"3",L"group-geometry"};
    bool v2=read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"2",L"group-geometry"};
    bool valid=v4||v3||v2||(read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"1",L"group-geometry"});
    std::map<std::shared_ptr<Folder>,GroupPosition> parsed;
    std::map<std::shared_ptr<Folder>,bool> collapsed;
    for(size_t i=1;valid&&i<rows.size();i++) {
        auto& r=rows[i];GroupPosition p;std::wstring name;float ordinal=0;
        valid=r.size()==(v4?18:v3?14:v2?10:8)&&r[0]==L"group"&&(r[7]==L"group"||r[7]==L"folder")&&PositionNumber(r[1],ordinal)&&ordinal==std::floor(ordinal)&&ordinal<g_tiles.size()&&
            EntryDecode(r[2],name)&&PositionNumber(r[3],p.x)&&PositionNumber(r[4],p.y)&&PositionNumber(r[5],p.w)&&PositionNumber(r[6],p.h)&&(v6?(p.w>0&&p.h>0):(p.w>=48&&p.h>=60));
        if(valid&&(v2||v3||v4)) {
            float columns=0,lines=0;
            valid=PositionNumber(r[8],columns)&&PositionNumber(r[9],lines)&&columns==std::floor(columns)&&lines==std::floor(lines)&&
                columns<=10000&&lines<=10000&&((columns==0&&lines==0)||(columns>=1&&lines>=1));
            p.columns=(int)columns;p.rows=(int)lines;
        }
        if(valid&&(v3||v4)) {
            int* fields[]{&p.gridX,&p.gridY,&p.gridW,&p.gridH};
            for(int j=0;j<4&&valid;j++) {
                float value=0;
                valid=PositionNumber(r[10+j],value)&&value==std::floor(value);
                if(valid)*fields[j]=(int)value;
            }
            if(v4&&valid) {
                float oldWidth=0,oldHeight=0;
                valid=(r[14]==L"0"||r[14]==L"1"||r[14]==L"2"||r[14]==L"3"||(v5&&r[14]==L"4")||(v6&&r[14]==L"5"))&&(r[15]==L"0"||r[15]==L"1")&&
                    PositionNumber(r[16],oldWidth)&&PositionNumber(r[17],oldHeight)&&
                    ((!oldWidth&&!oldHeight)||(oldWidth>=48&&oldHeight>=60));
                if(valid){p.style=r[14][0]-L'0';p.snapped=r[15]==L"1";}
            }
            valid=valid&&((p.gridW>0)==(p.gridH>0));
            if(p.SquareCells())valid=valid&&p.snapped&&p.gridW>=2&&p.gridW%2==0&&p.gridH>=6&&p.gridH%4==2&&
                p.gridX%(2*(int)columns)+p.gridW<=2*(int)columns;
            else if(p.WholeCells())valid=valid&&p.snapped&&p.gridW>=1&&p.gridH>=2;
            else if(p.style==0)valid=valid&&((p.OnGrid()&&!p.columns)||
                (!p.gridW&&(p.snapped||(!p.gridX&&!p.gridY))));
            else valid=valid&&(p.OnGrid()==(p.style==2))&&(p.snapped||(!p.gridX&&!p.gridY));
        }
        if(!valid)break;
        auto group=g_tiles[(size_t)ordinal].folder;
        valid=group&&group->name==name&&parsed.emplace(group,p).second;
        if(valid)collapsed[group]=r[7]==L"folder";
    }
    if(valid&&!v3&&!v4)for(const auto& tile:g_tiles)if(tile.folder&&!parsed.count(tile.folder))valid=false;
    if(!backup.empty()) {
        if(!valid)return false;
        std::vector<std::wstring> lines;
        for(const auto& fields:rows) {
            std::wstring line;
            for(size_t i=0;i<fields.size();++i){if(i)line+=L'\t';line+=fields[i];}
            lines.push_back(std::move(line));
        }
        if(!WriteLines(GroupPositionFile(),lines,false))return false;
    }
    g_groupPositionsReadOnly=!valid;
    if(valid){
        g_groupPositions=std::move(parsed);
        for(const auto& [group,value]:collapsed)group->collapsed=value;
        g_freeGroupLayout=true;
        g_wholeCellLayout=v5;
        g_squareGridColumns=v6?(int)columns:0;
    }
    else {g_layoutReadOnly=true;Wh_Log(L"Group geometry cannot be matched safely; layout writes disabled.");}
    return valid;
}
bool SaveGroupPositions(bool finishing) {
    if(g_groupPositionsReadOnly||g_layoutReadOnly||g_dataReadOnly)return false;
    if(!g_freeGroupLayout&&!finishing)return true;
    if(g_groupEditMode&&!finishing)return true; // The edit session commits only on Finish.
    auto path=GroupPositionFile();
    bool read=false;
    auto existingRows=ReadLines(path,&read);
    auto attributes=GetFileAttributesW(path.c_str());
    if(!read&&(attributes!=INVALID_FILE_ATTRIBUTES||GetLastError()!=ERROR_FILE_NOT_FOUND))return false;
    if(read) {
        if(existingRows.empty())return false;
        const auto& header=existingRows[0];
        bool oldShape=header.size()==3&&header[1].size()==1&&header[1][0]>=L'1'&&header[1][0]<=L'5';
        if(!(oldShape||(header.size()==4&&header[1]==L"6"))||
            header[0]!=L"schema"||header[2]!=L"group-geometry")return false;
    }
    bool wholeCells=g_wholeCellLayout||(read&&existingRows[0][1]==L"5");
    std::vector<std::wstring> lines{g_squareGridColumns?
        L"schema\t6\tgroup-geometry\t"+std::to_wstring(g_squareGridColumns):
        wholeCells?L"schema\t5\tgroup-geometry":L"schema\t4\tgroup-geometry"};
    for(size_t i=0;i<g_tiles.size();i++)if(auto group=g_tiles[i].folder) {
        auto found=g_groupPositions.find(group);
        if(found==g_groupPositions.end())continue;
        const auto& p=found->second;
        lines.push_back(L"group\t"+std::to_wstring(i)+L"\t"+EntryEncode(group->name)+L"\t"+std::to_wstring(p.x)+L"\t"+
            std::to_wstring(p.y)+L"\t"+std::to_wstring(p.w)+L"\t"+std::to_wstring(p.h)+L"\t"+(group->collapsed?L"folder":L"group")+
            L"\t"+std::to_wstring(p.columns)+L"\t"+std::to_wstring(p.rows)+
            L"\t"+std::to_wstring(p.gridX)+L"\t"+std::to_wstring(p.gridY)+
            L"\t"+std::to_wstring(p.gridW)+L"\t"+std::to_wstring(p.gridH)+
            L"\t"+std::to_wstring(p.style)+L"\t"+(p.snapped?L"1":L"0")+
            L"\t0\t0");
    }
    std::vector<std::wstring> existing;
    for(const auto& fields:existingRows) {
        std::wstring line;for(size_t i=0;i<fields.size();i++){if(i)line+=L'\t';line+=fields[i];}existing.push_back(line);
    }
    if(existing==lines)return true;
    return WriteLines(GroupPositionFile(),lines,false);
}
