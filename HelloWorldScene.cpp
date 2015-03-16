#include "HelloWorldScene.h"
#define COCOS2D_DEBUG 1

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
	scene->getPhysicsWorld()->setGravity(Vec2(0,-800));
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld() );

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	
	Size vSize = Director::getInstance()->getVisibleSize();
	auto wSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	FileUtils::getInstance()->addSearchResolutionsOrder("HD");

	
	// ********************************************  CAMERA  ************************************************************
	
	camera = Camera::createPerspective(60, (GLfloat)vSize.width/vSize.height, 1, 2000);
	cRot = camera->getRotation3D();
	cPos = Vec3(0,0,1000);
	cFoc = Vec3(wSize.width/2, wSize.height/2, 0);
	camera->setPosition3D(cPos);
	addChild(camera);

	// **************************************************** SPRITES *****************************************************

	// Get FrameCache
	auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("nyusz.plist");

	// Setup Layers With 3D Sprites
	auto lBackground = Sprite3D::create();
	lBackground->setAnchorPoint(Vec2(0.5,0.5));
	lBackground->setPosition3D(Vec3(0,0,150));
	addChild(lBackground);

	auto lMiddleground = Sprite3D::create();
	lMiddleground->setAnchorPoint(Vec2(0.5,0.5));
	lMiddleground->setPosition3D(Vec3(0,0,250));
	addChild(lMiddleground);

	auto lForeground = Sprite3D::create();
	lForeground->setAnchorPoint(Vec2(0.5,0.5));
	lForeground->setPosition3D(Vec3(0,0,350));
	addChild(lForeground);

	// Background
    auto spritebg = Sprite::create("dust.jpg");
    spritebg->setPosition(wSize.width/2, wSize.height/2);
	lBackground->addChild(spritebg, -1);

	// nyuszSprite
	nyuszSprite = Sprite::create(); // initialize the sprite without texture
	auto nyuszFrame = frameCache->getSpriteFrameByName("nyusz_run_front_01.png");
    nyuszSprite->setSpriteFrame(nyuszFrame);
	nyuszSprite->setPosition(Vec2(wSize.width/2, wSize.height/2));
    lMiddleground->addChild(nyuszSprite);

	// Ctrl Label
	label = Label::createWithSystemFont("Press the CTRL Key","Arial",32);
    label->setPosition(this->getBoundingBox().getMidX(),this->getBoundingBox().getMidY()-100);
    lForeground->addChild(label);

	// Setup Animations
	setupAnimations();

	// ******************************************* SLOPES  ****************************************************

	CCDrawNode* node = CCDrawNode::create();
	lMiddleground->addChild(node,10);//Make sure your z-order is large enough
	node->drawSegment(Vec2(30, 100),Vec2(166, 128),3.0,ccc4f(180,255,180,20));
	
	////******************************************** EVENT LISTENERS  ************************************************
	addMouse();
	addKeyboard();
	
    this->scheduleUpdate(); // Let cocos know we have an update function to be called.
    return true;
} 






void HelloWorld::update(float delta) {
    // Register an update function that checks to see if the CTRL key is pressed
    // and if it is displays how long, otherwise tell the user to press it
    Node::update(delta);

	Size vSize = Director::getInstance()->getVisibleSize();
	auto wSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////
	// Animations
	if(animRunningR->isDone()){
		animRunningR->startWithTarget(nyuszSprite);
    }
	if(animRunningF->isDone()){
		animRunningF->startWithTarget(nyuszSprite);
    }

	/////////////////////
	// PLayer Movement
	auto position = nyuszSprite->getPosition();

    if(isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
		position.x -= 250 * delta;
		animRunningR->step(delta);
		nyuszSprite->setFlippedX(true);

    }
	if(isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
		position.x += 250 * delta;		
		animRunningR->step(delta);
		nyuszSprite->setFlippedX(false);
		//CCLOG("right");
    }
	if(isKeyPressed(EventKeyboard::KeyCode::KEY_W) && !wasKeyPressed(EventKeyboard::KeyCode::KEY_W) ) {
		position.y += 250 * delta;
		animRunningF->step(delta);
		//CCLOG("up");
    }
	if(isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
		position.y -= 250 * delta;
		animRunningF->step(delta);
		//CCLOG("down");
    }
	if(isKeyPressed(EventKeyboard::KeyCode::KEY_R)) {
		cRot = Vec3(0,0,0);
		camera->setRotation3D(cRot);
		cFoc = Vec3(wSize.width/2,wSize.height/2,0);	
		camera->lookAt(cFoc);
    }
	
	nyuszSprite->setPosition(position);

	///////////////////////
	// The Control button
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL)) {
        std::stringstream ss;
        ss << "Control key has been pressed for " << 
            keyPressedDuration(EventKeyboard::KeyCode::KEY_CTRL) << " ms";
        label->setString(ss.str().c_str());
    }
    else
        label->setString("Press the CTRL Key");
}


void HelloWorld::addKeyboard() {

	EventDispatcher* dispatcher = this->getEventDispatcher();
    auto listener = EventListenerKeyboard::create();
    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);

    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){

		oldKeys = keys;
        if(keys.find(keyCode) == keys.end()){ // If a key already exists, do nothing as it will already have a time stamp, Otherwise, set's the timestamp to now
            keys[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };

    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		oldKeys = keys;
        keys.erase(keyCode); // remove the key.  std::map.erase() doesn't care if the key doesnt exist
    };	

    dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}


void HelloWorld::addMouse(){
	EventDispatcher* dispatcher = this->getEventDispatcher();
    EventListenerMouse* listener = EventListenerMouse::create();

    listener->onMouseDown = [=](Event *event) {
		EventMouse* e = (EventMouse*)event;
		CCLOG("Mouse button: %d",e->getMouseButton() );
		int buttonCode = e->getMouseButton();
		if(mouseState.find(buttonCode) == mouseState.end()) { 
			mouseState[buttonCode] = std::chrono::high_resolution_clock::now();
		}
	};	

	listener->onMouseUp = [=](Event *event) {
		EventMouse* e = (EventMouse*)event;
		int buttonCode = e->getMouseButton();
        mouseState.erase(buttonCode); // remove the code.  std::map.erase() doesn't care if the key doesnt exist
	};

    listener->onMouseMove = [=](Event *event) {
		EventMouse* e = (EventMouse*)event;
		auto delta = e->getLocation() - prevMouseLoc;
		if(isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL) && mouseState.find(2) != mouseState.end()) {

			cFoc = Vec3(cFoc.x - delta.x  , cFoc.y + delta.y , cFoc.z);
			Vec3 direction = cFoc - cPos;
			float normRatio = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
			Vec3 normToFocus = direction / normRatio;

			camera->lookAt(normToFocus * 1000);

			//cRot = Vec3(cRot.x - delta.x , cRot.y + delta.y, cRot.z);
			//camera->setRotation3D(cRot);
		}
		else if(mouseState.find(2) != mouseState.end()){
			cPos = Vec3(cPos.x - delta.x ,cPos.y + delta.y, cPos.z);
			camera->setPosition3D(cPos);
			cFoc = Vec3(cFoc.x - delta.x ,cFoc.y + delta.y, cFoc.z);
			camera->lookAt(cFoc);
		}
		prevMouseLoc = e->getLocation(); // Saving previous mouse position
	};	

    listener->onMouseScroll = [=](Event *event) {
		EventMouse* e = (EventMouse*)event;
		Vec3 direction = cFoc - cPos;
		float normRatio = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		Vec3 normToFocus = direction / normRatio; 
		cPos = cPos + e->getScrollY() * -normToFocus * 30;
		camera->setPosition3D(cPos); // Zoom
		//cFoc = e->getScrollY() * -normToFocus * 30;
		//camera->lookAt(cFoc);
	};	

    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::setupAnimations() {

    AnimationCache::destroyInstance(); // purge previously loaded animation
	auto animCache = AnimationCache::getInstance();

    animCache->addAnimationsWithFile("nyuszAnimations.plist"); // Add an animation to the Cache

	auto nyuszRnF = animCache->getAnimation("nyusz_run_front");
	auto nyuszRnL = animCache->getAnimation("nyusz_run_left");
	auto nyuszRnR = animCache->getAnimation("nyusz_run_right");

	animRunningF = Animate::create(nyuszRnF);
	animRunningL = Animate::create(nyuszRnL);
	animRunningR = Animate::create(nyuszRnR);

	animRunningF->setTarget(nyuszSprite);
	animRunningF->retain(); 
	animRunningR->setTarget(nyuszSprite);
	animRunningR->retain();
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode code) {
    if(keys.find(code) != keys.end())
        return true;
    return false;
}

bool HelloWorld::isMousePressed(int code) {
    if(mouseState.find(code) != mouseState.end())
        return true;
    return false;
}

bool HelloWorld::wasKeyPressed(EventKeyboard::KeyCode code) {
    if(oldKeys.find(code) != oldKeys.end())
        return true;
    return false;
}

double HelloWorld::keyPressedDuration(EventKeyboard::KeyCode code) {
    if(!isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL))
        return 0;  // Not pressed, so no duration obviously

    return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::high_resolution_clock::now() - keys[code]).count();
}


