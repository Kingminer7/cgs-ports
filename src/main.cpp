#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto sapphireBtn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSpriteFrameName("sapphire.png"_spr, 1.2f, CircleBaseColor::Green, CircleBaseSize::MediumAlt),
			this,
			menu_selector(MyMenuLayer::onSapphire)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(sapphireBtn);

		sapphireBtn->setID("sapphire"_spr);

		menu->updateLayout();

		return true;
	}

	void onSapphire(CCObject*) {
		geode::openModsList();
	}
};