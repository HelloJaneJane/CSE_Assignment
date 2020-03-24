#include "minimal_fighter.h"

using namespace std;

MinimalFighter::MinimalFighter(){
		mHp=0;
		mPower=0;
		mStatus=Invalid;
}

MinimalFighter::MinimalFighter(int _hp, int _power){
		mHp=_hp;
		mPower=_power;
		
		if (_hp<=0)
			mStatus=Dead;
		else
			mStatus=Alive;
}
int MinimalFighter::hp() const{
		return mHp;
}

int MinimalFighter::power() const{
		return mPower;
}

FighterStatus MinimalFighter::status() const{
		if (mHp<=0)
				return Dead;
		else
				return Alive;
}

void MinimalFighter::setHp(int _hp){
		mHp=_hp;
}

void MinimalFighter::hit(MinimalFighter *_enemy){
		_enemy->setHp((_enemy->hp())-power());
		setHp(hp()-(_enemy->power()));
}

void MinimalFighter::attack(MinimalFighter *_target){
		_target->setHp((_target->hp())-power());
		mPower=0;
}

void MinimalFighter::fight(MinimalFighter *_enemy){
		while(status()==Alive&&(_enemy->status())==Alive){
				_enemy->setHp((_enemy->hp())-power());
				setHp(hp()-(_enemy->power()));
		}

}
