#include "CIndieLib.h"
#include "IND_Surface.h"
#include "IND_Entity2d.h"
#include "IND_Animation.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <array>
/*
==================
Main
==================
*/



int IndieLib()
{

	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;


	// ----- Surface loading -----

	// Loading Background
	IND_Surface *mSurfaceBack = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../DragonBall_Indie/resources/dbs.jpg", IND_OPAQUE, IND_32)) return 0;

	//Loading sprite of a heart1
	IND_Surface *mSurfaceHeart1 = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceHeart1, "../DragonBall_Indie/resources/heart.png", IND_ALPHA, IND_32)) return 0;

	//Loading sprite of heart2
	IND_Surface *mSurfaceHeart2 = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceHeart2, "../DragonBall_Indie/resources/heart.png", IND_ALPHA, IND_32)) return 0;

	//Loading sprite of heart3
	IND_Surface *mSurfaceHeart3 = IND_Surface::newSurface();
	if (!mI->_surfaceManager->add(mSurfaceHeart3, "../DragonBall_Indie/resources/heart.png", IND_ALPHA, IND_32)) return 0;

	// ----- Animations loading -----

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter1 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter1, "../DragonBall_Indie/resources/animations/character1.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;

	// Characters animations, we apply a color key of (0, 48, 152)
	IND_Animation *mAnimationCharacter2 = IND_Animation::newAnimation();
	if (!mI->_animationManager->addToSurface(mAnimationCharacter2, "../DragonBall_Indie/resources/animations/character2.xml", IND_ALPHA, IND_32, 0, 48, 152)) return 0;


	// ----- Set the surface and animations into 2d entities -----

	// Creating 2d entity for the background
	IND_Entity2d *mBack = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mBack);					// Entity adding
	mBack->setSurface(mSurfaceBack);					// Set the surface into the entity

	//heart1
	IND_Entity2d *heart1 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(heart1);
	heart1->setSurface(mSurfaceHeart1);

	//heart2
	IND_Entity2d *heart2 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(heart2);
	heart2->setSurface(mSurfaceHeart2);

	////heart3
	IND_Entity2d *heart3 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(heart3);
	heart3->setSurface(mSurfaceHeart3);

	//Text
	IND_Entity2d *mTextSmallWhite = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mTextSmallWhite);

	// Character 1
	IND_Entity2d *mPlayer1 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer1);					// Entity adding
	mPlayer1->setAnimation(mAnimationCharacter1);				// Set the animation into the entity

	// Character 2
	IND_Entity2d *mPlayer2 = IND_Entity2d::newEntity2d();
	mI->_entity2dManager->add(mPlayer2);					// Entity adding
	mPlayer2->setAnimation(mAnimationCharacter2);				// Set the animation into the entity

	IND_Entity2d *Border = IND_Entity2d::newEntity2d();
	Border->setBoundingRectangle("left", 200, 500, 200, 600);
	mI->_entity2dManager->add(Border);




	// ----- Changing the attributes of the 2d entities -----

	// Player 1
	mPlayer1->setSequence(0);						// Choose sequence
	mPlayer1->setPosition(140, 350, 0);
	mPlayer1->setMirrorX(1);						// Horizontal mirroring
	mPlayer1->setBoundingCircle("circle", 85, 52, 55);



	// Player 2
	mPlayer2->setSequence(0);						// Choose sequence
	mPlayer2->setPosition(550, 350, 0);
	mPlayer2->setNumReplays(3);						// The animation will be displayed 3 times

	//heart1
	heart1->setPosition((mI->_window->getWidth() - mSurfaceHeart1->getWidth()) - 750, 0, 100);
	heart2->setPosition((mI->_window->getWidth() - mSurfaceHeart1->getWidth()) - 700, 0, 100);
	heart3->setPosition((mI->_window->getWidth() - mSurfaceHeart1->getWidth()) - 650, 0, 100);



	// ----- Main Loop -----
	int health = 2;
	std::array<IND_Entity2d *, 3> lives = { heart1, heart2, heart3 };
	float lastPositionY = mPlayer1->getPosY();
	float lastPositionX = mPlayer1->getPosX();
	float velocity = 0.0;
	float maxVelocity = 0.4;
	float acc = 0.0;
	float accspeed = 2;
	float maxAcc = 3;
	float minAcc = 0.2;
	bool border = false;
	
	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		mI->_input->update();

		if (mI->_input->onKeyPress(IND_SPACE)) mI->_render->toggleFullScreen();

		if (border && health >= 0){
			lives[health]->setFade(0, 0, 0, 0);
			border = false;
			health--;
		}
			
			while (mI->_input->isKeyPressed(IND_W)){
				acc += accspeed;
				if (acc > maxAcc)
					acc = maxAcc;
				else if (acc < minAcc)
					acc = minAcc;
				velocity += acc;

				if (velocity > maxVelocity)
					velocity = maxVelocity;
				else if (velocity < maxVelocity)
					velocity = maxVelocity;

				lastPositionY -= velocity;

				if (lastPositionY < 0.0){
					lastPositionY = 0.0;
					border = true;
				}
			
				mPlayer1->setPosition(lastPositionX, lastPositionY, 0);
				mI->_input->update();
				mI->_entity2dManager->renderEntities2d();
				mI->_render->endScene();

				

			}
		
			while (mI->_input->isKeyPressed(IND_S)){

				acc += accspeed;
				if (acc > maxAcc)
					acc = maxAcc;
				else if (acc < minAcc)
					acc = minAcc;
				velocity += acc;

				if (velocity > maxVelocity)
					velocity = maxVelocity;
				else if (velocity < maxVelocity)
					velocity = maxVelocity;

				lastPositionY += velocity;

				if (lastPositionY > 400.0){
					lastPositionY = 400.0;
					border = true;
				}
			

				mPlayer1->setPosition(lastPositionX, lastPositionY, 0);
				mI->_input->update();
				mI->_entity2dManager->renderEntities2d();
				mI->_render->endScene();
				
			}
		
	
			while (mI->_input->isKeyPressed(IND_D)){

				acc += accspeed;
				if (acc > maxAcc)
					acc = maxAcc;
				else if (acc < minAcc)
					acc = minAcc;
				velocity += acc;

				if (velocity > maxVelocity)
					velocity = maxVelocity;
				else if (velocity < maxVelocity)
					velocity = maxVelocity;

				lastPositionX += velocity;

				if (lastPositionX > 700){
					lastPositionX = 700;
					border = true;
				}
				mPlayer1->setPosition(lastPositionX, lastPositionY, 0);
				mI->_input->update();
				mI->_entity2dManager->renderEntities2d();
				mI->_render->endScene();
			
			}
		
	
		
			while (mI->_input->isKeyPressed(IND_A)){
				acc += accspeed;
				if (acc > maxAcc)
					acc = maxAcc;
				else if (acc < minAcc)
					acc = minAcc;
				velocity += acc;

				if (velocity > maxVelocity)
					velocity = maxVelocity;
				else if (velocity < maxVelocity)
					velocity = maxVelocity;

				lastPositionX -= velocity;
				if (lastPositionX < 0.0){
					lastPositionX = 0.0;
					border = true;
				}
				
				mPlayer1->setPosition(lastPositionX, lastPositionY, 0);
				mI->_input->update();
				mI->_entity2dManager->renderEntities2d();
				mI->_render->endScene();
			}
		


		mI->_render->beginScene();
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();
		
	}

	// ----- Free -----

	mI->end();

	return 0;
}