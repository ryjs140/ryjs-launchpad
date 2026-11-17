#define SEAMLESS_PREVIEW 1
#include "../mods/local/seamless-launchpad/seamless-launchpad.wh.cpp"
#include <cstdio>
void Preview_RunSmoke(HWND) {}
int failures=0, checks=0;
void Check(bool value,const char* label) { ++checks; if (!value) { ++failures; printf("FAIL: %s\n",label); } }
int wmain() {
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
    printf("Feature checks: %d, failures: %d\n",checks,failures);
    return failures?1:0;
}
