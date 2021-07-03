#include <GDMake.h>
#include "../BetterEdit.hpp"
#include "../tools/settings/BESettingsLayer.hpp"
#include "../tools/LiveCollab/pauseMenuHook.hpp"

using namespace gdmake;

class EditorPauseLayer_CB : public gd::EditorPauseLayer {
    public:
        void onBESettings(cocos2d::CCObject* pSender) {
            BESettingsLayer::create()->show();
        }
};

GDMAKE_HOOK(0x75660)
void __fastcall EditorPauseLayer_onExitEditor(gd::EditorPauseLayer* self, edx_t edx, cocos2d::CCObject* pSender) {
    GDMAKE_ORIG_V(self, edx, pSender);

    self->removeFromParentAndCleanup(true);
}

GDMAKE_HOOK(0x730e0)
bool __fastcall EditorPauseLayer_init(gd::EditorPauseLayer* self, edx_t edx, gd::LevelEditorLayer* el) {
    if (!GDMAKE_ORIG(self, edx, el))
        return false;

    auto menu = extra::as<cocos2d::CCMenu*>(self->m_pButton0->getParent());
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    auto btn = gd::CCMenuItemSpriteExtra::create(
        CCNodeConstructor()
            .fromFrameName("GJ_optionsBtn02_001.png")
            .scale(.8f)
            .done(),
        self,
        (cocos2d::SEL_MenuHandler)&EditorPauseLayer_CB::onBESettings
    );
    btn->setPosition(winSize.width / 2 - 70, winSize.height - 73);
    menu->addChild(btn);

    extra::getChild<cocos2d::CCMenu*>(menu, menu->getChildrenCount() - 2)
        ->setPositionX(winSize.width / 2 - 30);

    loadLiveButton(self);

    return true;
}