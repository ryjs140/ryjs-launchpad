// Included after CellAt: geometry uses the same MakeCell and DrawTile pipeline.
struct ExpandedGroupGeometry {
    std::shared_ptr<Folder> group;
    D2D1_RECT_F area, title;
    int start = 0, firstInGroup = 0, count = 0, columns = 1, entryOrder = 0;
};
[[clang::no_destroy]] std::vector<ExpandedGroupGeometry> g_groupGeometry;
[[clang::no_destroy]] std::vector<Cell> g_groupCells;
[[clang::no_destroy]] std::shared_ptr<Folder> g_groupDropFolder, g_groupHeaderDragging;
int g_groupDropSlot = -1;
void CancelGroupPointer();
void GroupHeaderTick();
extern float g_appear;
extern bool g_closing;
float Ease(float t);
[[clang::no_destroy]] std::map<const void*,int> g_groupEntryOrder;
float GroupEntryProgress(int order) {
    if (!Animate() || g_closing) return 1;
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
    if (g_settings.pages) { r.left -= g_scroll; r.right -= g_scroll; }
    else { r.top -= g_scroll; r.bottom -= g_scroll; }
    return r;
}
void ComputeGroups() {
    auto& L = g_layout;
    float s = L.scale, gap = g_settings.groupGap * s, pad = 12 * s, header = 36 * s;
    float margin = std::max(24 * s, L.width * 0.045f);
    float available = std::max(L.icon + 2 * pad, L.width - 2 * margin);
    float wanted = g_settings.groupWidth > 0 ? g_settings.groupWidth * s :
        (g_tiles.size() <= 1 ? available : 400 * s);
    wanted = std::max(wanted, L.icon + 2 * pad);
    int across = g_settings.groupRows ? 1 : std::max(1, (int)((available + gap) / (wanted + gap)));
    float groupWidth = (available - (across - 1) * gap) / across;
    int fitColumns = std::max(1, (int)((groupWidth - 2 * pad) / (L.icon + 8 * s)));
    int columns = g_settings.columns > 0 ? std::min(g_settings.columns, fitColumns) :
        std::max(1, (int)((groupWidth - 2 * pad) / (L.icon + g_settings.horizontalGap * s)));
    L.columns = columns;
    L.cellWidth = (groupWidth - 2 * pad) / columns;
    L.cellHeight = std::round(L.icon + L.labelHeight + (18+g_settings.verticalGap) * s);
    L.cellSpare = 0;
    float viewHeight = std::max(L.cellHeight + header + 2 * pad, L.viewBottom - L.viewTop);
    int maxRows = std::max(1, (int)((viewHeight - header - 2 * pad) / L.cellHeight));
    int capacity = columns * maxRows;
    g_groupGeometry.clear();
    g_groupEntryOrder.clear();
    g_groupCells.assign(g_visible.size(), {});
    float y = 0, rowHeight = 0;
    int col = 0, page = 0, global = 0, order = 0;
    for (const auto& tile : g_tiles) {
        if (!tile.folder) continue;
        int count = (int)tile.folder->apps.size(), offset = 0;
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
            geometry.entryOrder = order;
            g_groupGeometry.push_back(geometry);
            for (int i = 0; i < chunk; i++) {
                int index = global + offset + i;
                if (index < (int)g_groupCells.size()) g_groupCells[index] = MakeCell(
                    x + pad + (i % columns) * L.cellWidth,
                    top + header + pad + (i / columns) * L.cellHeight);
                if (index < (int)g_visible.size() && g_visible[index].app) g_groupEntryOrder[g_visible[index].app.get()]=order;
            }
            ++col; rowHeight = std::max(rowHeight, height);
            offset += chunk;
        } while (offset < count);
        global += count;
        ++order;
    }
    L.contentHeight = y + rowHeight;
    L.pages = page + 1;
    L.perPage = std::max(1, capacity * across);
}
Cell GroupCellAt(int index) {
    if (index < 0 || index >= (int)g_groupCells.size()) return {};
    Cell c = g_groupCells[index];
    c.icon = ProjectGroupRect(c.icon);
    c.label = ProjectGroupRect(c.label);
    c.highlight = ProjectGroupRect(c.highlight);
    return c;
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
        if (GroupContains(ProjectGroupRect(titleOnly ? g_groupGeometry[i].title : g_groupGeometry[i].area), (float)x, (float)y)) return i;
    return -1;
}
void DrawGroupHeaders() {
    for (const auto& geometry : g_groupGeometry) {
        auto area = ProjectGroupRect(geometry.area), title = ProjectGroupRect(geometry.title);
        if (area.bottom < g_layout.viewTop || area.top > g_layout.viewBottom || area.right < 0 || area.left > g_layout.width) continue;
        if (geometry.group == g_groupDropFolder || geometry.group == g_groupHeaderDragging) {
            auto color = g_settings.accent; color.a = 0.10f;
            Fill(area, color, 12 * g_layout.scale);
        }
        float progress=GroupEntryProgress(geometry.entryOrder);
        float shift=(1-progress)*30*g_layout.scale;
        title.left+=shift; title.right+=shift;
        g_brush->SetColor(D2D1::ColorF(0xCDD6F4,progress));
        std::wstring name=geometry.group->name;
        if (!geometry.group->unavailable.empty()) name+=L" · "+std::to_wstring(geometry.group->unavailable.size())+Tr(L" unavailable");
        g_target->DrawText(name.c_str(), (UINT32)name.size(), g_groupFormat.Get(), title, g_brush.Get());
        g_brush->SetColor(D2D1::ColorF(0x6C7086, 0.25f));
        g_target->DrawLine(D2D1::Point2F(title.left, title.bottom), D2D1::Point2F(title.right, title.bottom), g_brush.Get());
        if (!geometry.count) {
            auto hint = area; hint.top = title.bottom;
            g_brush->SetColor(D2D1::ColorF(0x9399B2));
            PCWSTR text = Tr(g_visible.empty() ? L"Search for an app, then use Move to group" : L"Drop apps here");
            g_target->DrawText(text, (UINT32)wcslen(text), g_tipFormat.Get(), hint, g_brush.Get());
        }
        if (geometry.group == g_groupDropFolder && g_groupDropSlot >= geometry.firstInGroup &&
            g_groupDropSlot <= geometry.firstInGroup + geometry.count) {
            int slot = g_groupDropSlot - geometry.firstInGroup;
            float x = area.left + 12 * g_layout.scale + (slot % geometry.columns) * g_layout.cellWidth;
            float y = title.bottom + 12 * g_layout.scale + (slot / geometry.columns) * g_layout.cellHeight;
            auto color = g_settings.accent; color.a=0.9f;
            Fill({x,y,x+3*g_layout.scale,y+g_layout.icon+16*g_layout.scale},color,1.5f*g_layout.scale);
        }
    }
}
