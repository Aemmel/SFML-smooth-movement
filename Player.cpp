#include "Player.hpp"
#include "Global_Constants.hpp"

#include <iostream>
#include <math.h>

Player::Player()
	: cBall_Rad(50.0f), mPos(C::Window_Width / 2, C::Window_Height / 2), mSpeed(0, 0), mAcceleration(350.0f), mMax_speed(200.0f),
	mUp(0), mDown(0), mLeft(0), mRight(0)
{
	mBall.setOrigin(sf::Vector2f(cBall_Rad / 2, cBall_Rad / 2));
	mBall.setPosition(mPos);
	mBall.setFillColor(sf::Color::Green);
	mBall.setRadius(cBall_Rad);
}

void Player::Render(sf::RenderWindow &window)
{
	window.draw(mBall);
}

void Player::Update(sf::Time delta)
{
	Handle_Input();
 	Set_Vel();

	mPos += Calc_Speed(delta) * delta.asSeconds();
	
	mBall.setPosition(mPos);
}


///////////////////
//private functions

void Player::Handle_Input()
{
	//set position equal to keypresses
	mUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	mDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	mLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	mRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

	if (mUp && mDown) { //if trying to press both dont do anything on y-axis
		mUp = false;
		mDown = false;
	}

	if (mLeft && mRight) { //if trying to press both dont do anything on x-axis
		mLeft = false;
		mRight = false;
	}
}

bool Player::Set_Vel()
{
	if (!mSpeed.y && (!mUp || !mDown)) {
		mVel.y = 0;
	}
	if (!mSpeed.x) {
		mVel.x = 0;
	}

	if (mUp) {
		mVel.y = -1;
	}
	else if (mDown) {
		mVel.y = 1;
	}
	if (mLeft) {
		mVel.x = -1;
	}
	else if (mRight){
		mVel.x = 1;
	}

	return mVel.y || mVel.x;
}

sf::Vector2f Player::Calc_Speed(sf::Time delta)
{
	//accelerate
	////////////

	//Y-Axis
	if (mSpeed.y < mMax_speed && mSpeed.y > -mMax_speed && (mUp || mDown)) {	//if maximum speed isnt reached and its suppoesed to move
		mSpeed.y += mAcceleration * mVel.y * delta.asSeconds();					//increase speed by acceleration times delta for fps independent
	}																			//acceleration with direction of velovity (1 or -1)

	if (mSpeed.y > mMax_speed || mSpeed.y < -mMax_speed)	//if calculated speed is greater than the maximum speed, set speed to max speed
		mSpeed.y = mMax_speed * mVel.y;

	//X-Axis
	if (mSpeed.x < mMax_speed && mSpeed.x > -mMax_speed && (mLeft || mRight)) { //do the same with x-axis
		mSpeed.x += mAcceleration * mVel.x * delta.asSeconds();
	}

	if (mSpeed.x > mMax_speed || mSpeed.x < -mMax_speed)
		mSpeed.x = mMax_speed * mVel.x;

	//decelerate
	////////////

	//Y-Axis
	if (!mUp && !mDown) {										//if player isnt supposed to move anymore:
		if (mVel.y > 0) {							//get direction of movement
			mSpeed.y -= mAcceleration * delta.asSeconds();		//decelerate fps independent
			if (mSpeed.y <= 0) {				//dont want to decelerate into negative speed
				mSpeed.y = mVel.y =  0.0f;		//so set speed and velocity to zero
			}
		}
		else if (mVel.y < 0) {					//do the same for other direction
			mSpeed.y += mAcceleration * delta.asSeconds();
			if (mSpeed.y >= 0) {
				mSpeed.y = mVel.y =  0.0f;
			}
		}
	}

	//X-Axis
	if (!mLeft && !mRight) { //do the same for x-axis
		if (mVel.x > 0) {
			mSpeed.x -= mAcceleration * delta.asSeconds();
			if (mSpeed.x <= 0) {
				mSpeed.x = mVel.x =  0.0f;
			}
		}
		else if (mVel.x < 0) {
			mSpeed.x += mAcceleration * delta.asSeconds();
			if (mSpeed.x >= 0) {
				mSpeed.x = mVel.x =  0.0f;
			}
		}
	}

	return mSpeed;
}
