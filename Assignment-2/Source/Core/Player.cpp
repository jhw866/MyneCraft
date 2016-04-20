#include "Player.h"

Player::Player(Ogre::Camera* camera, GameObject* body, Ogre::SceneManager* sm) : _body(body), _playerCam(camera), _sceneManager(sm) {
	_body->getNode()->setVisible(false);
	_body->getBody()->setAngularFactor(btVector3(0, 0, 0));

	for(int i = 0; i < averageSize; i++) {
		camAvg[i] = Ogre::Vector3::ZERO;
	}

	_pickaxe = _sceneManager->createEntity("Pickaxe", "torch.mesh");
	_pickaxe->setCastShadows(true);
	_axeNode = _sceneManager->getRootSceneNode()->createChildSceneNode("Pickaxe");
	_axeNode->attachObject(_pickaxe);
	_axeNode->setDirection(Ogre::Vector3(0, 1 ,0));
	_axeNode->setScale(3, 3, 3);
}

void Player::update(OISManager* ois) {
	static int speed = 1000;
	static int camAvgIndex = 0;

	Ogre::Vector3 movePos = _playerCam->getDirection();
	movePos = Ogre::Vector3(movePos.x, 0, movePos.z);
	movePos.normalise();
	Ogre::Vector3 strafePos = movePos.crossProduct(Ogre::Vector3(0, 1, 0));
	float currentY = _body->getBody()->getLinearVelocity().y();

	OIS::KeyCode lastkey;
	bool moved = false;

	if (ois->isKeyDown(OIS::KC_W)) {
		_body->setVelocity(movePos.x*speed, currentY, movePos.z*speed);
		moved = true;
	}
	if (ois->isKeyDown(OIS::KC_S)) {
		_body->setVelocity(movePos.x*-speed, currentY, movePos.z*-speed);
		moved = true;
	}
	if (ois->isKeyDown(OIS::KC_D)) {
		_body->setVelocity(strafePos.x*speed, currentY, strafePos.z*speed);
		moved = true;
	}
	if (ois->isKeyDown(OIS::KC_A)) {
		_body->setVelocity(strafePos.x*-speed, currentY, strafePos.z*-speed);
		moved = true;
	}
	if (ois->isKeyDown(OIS::KC_SPACE) && _body->canJump) {
		_body->setVelocity(_body->getBody()->getLinearVelocity().x(), 2500, _body->getBody()->getLinearVelocity().z());
		moved = true;
		_body->canJump = false;
	}
	if (ois->isKeyDown(OIS::KC_LSHIFT)) {
		speed = 2500;
	}
	else {
		speed = 1000;
	}
	if (!moved) {
		_body->setVelocity(0, currentY, 0);
	}

	camAvg[camAvgIndex % averageSize] = _body->getNode()->getPosition() + Ogre::Vector3(0, 170, 0);
	camAvgIndex++;

	Ogre::Vector3 total(0,0,0);
	int n = 0;

	for(int i = 0; i < averageSize; i++) {
		if(camAvg[i] != Ogre::Vector3::ZERO) {
			total += camAvg[i];
			n++;
		}
	}

	if(n > 0)
		total /= n;

	_playerCam->setPosition(total);

	Ogre::Vector3 unit = _playerCam->getDirection().normalisedCopy();
	Ogre::Vector3 u = unit.crossProduct(Ogre::Vector3::UNIT_Y).normalisedCopy();
	Ogre::Vector3 v = unit.crossProduct(u).normalisedCopy();

	Ogre::Quaternion qt(unit, u, v);

	_axeNode->setOrientation(qt);

	Ogre::Vector3 offset = unit.crossProduct(qt.zAxis()).normalisedCopy();
	_axeNode->setPosition(total + unit*50 - offset*20);
	_axeNode->roll(Ogre::Degree(-25));
}