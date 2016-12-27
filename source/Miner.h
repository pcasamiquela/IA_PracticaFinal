/* ========================================================================
   File: Miner.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma  once

class Miner
{
public:
	enum LocationType
	{
		home,
		goldmine,
		bank,
		saloon
	};

	// Constants
	// The amount of gold a miner must have before he feels comfortable
	static constexpr int K_COMFORT_LEVEL = 5;
	// The amount of nuggets a miner can carry
	static constexpr int K_MAX_NUGGETS = 3;
	// Above this value a miner is thirsty
	static constexpr int K_THIRST_LEVEL = 5;
	// Above this value a miner is sleepy
	static constexpr int K_TIREDNESS_THRESHOLD = 5;

	// Data
	char stateInformation[256];
	int goldCarried = 0; // How many nuggets the miner has in his pockets
	int moneyInBank = 0;
	int thirst = 0; // The higher the value, the thirstier the miner
	int fatigue = 0; // The higher the value, the more tired the miner
	char name[48];
	LocationType currentLocation = home;

	// Public functions
	virtual void InitMiner(char* name);
	virtual void Update();
	void AddToWealth(const int val);
	void AddToGoldCarried(int val);
	bool Fatigued();
	bool Thirsty();

	LocationType Location()
	{
		return currentLocation;
	}
	void ChangeLocation(LocationType loc)
	{
		currentLocation = loc;
	}
	int GoldCarried()
	{
		return goldCarried;
	}
	void SetGoldCarried(int val)
	{
		goldCarried = val;
	}
	bool PocketsFull()
	{
		return goldCarried >= K_MAX_NUGGETS;
	}
	void DecreaseFatigue()
	{
		fatigue -= 1;
	}
	void IncreaseFatigue()
	{
		fatigue += 1;
	}
	int Wealth()
	{
		return moneyInBank;
	}
	void SetWealth(int val)
	{
		moneyInBank = val;
	}
	void BuyAndDrinkAWhiskey()
	{
		thirst = 0;
		moneyInBank -= 2;
	}
};

