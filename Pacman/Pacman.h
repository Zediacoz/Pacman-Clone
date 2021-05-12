#pragma once
#define MUNCHIECOUNT 50
#define GHOSTCOUNT 1

#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#include "S2D/S2D.h"
using namespace S2D;

struct MovingEnemy
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	float speed;
};
struct Enemy
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	Texture2D* munchieTex;
	Rect* rect;
	int frameTime;
	int frame;
	int frameCount;
	int currentFrameTime;
	bool eaten;
};
struct Player
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	Rect* posRect;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	float _pacmanSpeedMultiplier;
	bool dead;

};
class Pacman : public Game
{
private:
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;

	// Data to represent Pacman
	//Vector2* _pacmanPosition;
	//Rect* _pacmanSourceRect;
	//Texture2D* _pacmanTexture;
	Player* _pacman;
	float _pacmanSpeedMultiplier;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;
	const int _cPacmanSpeed;
	int _munchieCurrentFrameTime;
	bool _pKeyDown;
	bool _paused;
	Enemy* _munchies[MUNCHIECOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];
	//void UpdateGhosts(int elapsedTime);
	//void CheckGhostCollisions();

public: boolean CollisionCheck(int x1, int y1, int width1, int height1,
	int x2, int y2, int width2, int height2);

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;
	int eatenMunchies;

	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void UpdateMunchie(Enemy* munchie, int elapsedTime);

	void UpdateGhosts(int elapsedTime);

	void CheckGhostCollisions(MovingEnemy* _ghosts);
	void CheckMunchiesCollisions();
};