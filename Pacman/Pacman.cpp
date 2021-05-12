#include "Pacman.h"
#include <sstream>
#include <stdlib.h>
#include<iostream>
#include<time.h>

boolean Pacman::CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	//Right side
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	//Left side
	int left1 = x1;
	int left2 = x2;
	//Top side
	int top1 = y1;
	int top2 = y2;
	//Bottom side
	int bot1 = y1 + height1;
	int bot2 = y2 + height2;

	if (bot1 < top2)
		return false;

	if (top1 > bot2)
		return false;

	if (right1 < left2)
		return false;

	if (left1 > right2)
		return false;

	return true;
}

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(750)
{
	srand(time(NULL));

	int eatenMunchies = 0;

	_pacman = new Player();
	_paused = false;
	_pacman->currentFrameTime = 0; 
	_pacman->frame = 0;
	_munchieCurrentFrameTime = 0;
	_pacman->dead = false;

	_ghosts[0] = new MovingEnemy();
	_ghosts[0]->direction = 0;
	_ghosts[0]->speed = 0.2f;

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->position = new Vector2(rand() % (Graphics::GetViewportWidth() - 20), rand() %
			(Graphics::GetViewportHeight() - 20));
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->frameTime = rand() % 500 + 50;
		
		_munchies[i]->rect = new Rect(rand() % Graphics::GetViewportWidth(), rand() %
			Graphics::GetViewportHeight(), 12, 12);
		_munchies[i]->eaten = false;
	}

	if (_pacman->currentFrameTime > _cPacmanFrameTime) 
	{
		_pacman->frame++;
		if (_pacman->frame >= 2) _pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->frame = _pacman->frame + 1;
	}
	
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	Audio::Initialise();
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
	delete _pacman;
	delete _munchies[0]->texture;

	for (int i = 0; i < MUNCHIECOUNT; i++) 
	{ 
		delete _munchies[i]->sourceRect; 
		delete _munchies[i]->position; 
	} 
	delete[] _munchies;
}

void Pacman::LoadContent()
{
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.tga", true);
	for (int i = 0; i < MUNCHIECOUNT; i++) {
		_munchies[i]->texture = munchieTex;
	}

	//Load Blue Ghost
	_ghosts[0]->texture = new Texture2D();
	_ghosts[0]->texture->Load("Textures/GhostBlue.png", true);
	_ghosts[0]->posRect = new Rect(rand() % (Graphics::GetViewportWidth() - 20), rand() %
		(Graphics::GetViewportHeight() - 20), 20, 20);
	_ghosts[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->direction = 0;

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);
	

	

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth()
		/ 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	_pacman->currentFrameTime += elapsedTime;
	if (_pacman->position->X + _pacman->sourceRect->Width > 1024)
	{
		_pacman->position->X = 1024 - _pacman->sourceRect->Width;
	}

	if (_pacman->position->Y + _pacman->sourceRect->Height > 768)
	{
		_pacman->position->Y = 768 - _pacman->sourceRect->Height;
	}

	if (_pacman->position->Y < 0)
	{
		_pacman->position->Y = 0;
	}
	if (_pacman->position->X < 0)
	{
		_pacman->position->X = 0;
	}

		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
		_munchies[i]->currentFrameTime += elapsedTime;

		if (_munchies[i]->currentFrameTime > _cMunchieFrameTime)
		{
			_munchies[i]->frameCount++;

			if (_munchies[i]->frameCount >= 2)
				_munchies[i]->frameCount = 0;
			_munchies[i]->currentFrameTime = 0;
		}
	}
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}
	if (keyboardState->IsKeyDown(Input::Keys::P))
	{
		_paused = !_paused;
	}
	if (!_paused)
	{		
		if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
		{
			_pacmanSpeedMultiplier = 2.0f;

		}
		else {
			_pacmanSpeedMultiplier = 1.0f;
		}
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_pacman->position->X += 0.2f * elapsedTime * _pacmanSpeedMultiplier; //Moves Pacman across X axis
			_pacman->direction = 4;
		}
		if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_pacman->position->X -= 0.2f * elapsedTime * _pacmanSpeedMultiplier;
			_pacman->direction = 2;
		}
		if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_pacman->position->Y -= 0.2f * elapsedTime * _pacmanSpeedMultiplier;
			_pacman->direction = 3;
		}
		if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_pacman->position->Y += 0.2f * elapsedTime * _pacmanSpeedMultiplier;
			_pacman->direction = 1;
		}
		/*if (mouseState->LeftButton == Input::ButtonState::PRESSED)
		{
			_munchieRect->X = mouseState->X;
			_munchieRect->Y = mouseState->Y;
		}*/
		if (_pacman->position->X > Graphics::GetViewportWidth())
		{
			_pacman->position->X = -_pacman->sourceRect->Width;
		}
	}
	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;

	_munchieCurrentFrameTime += elapsedTime;
	if (_munchieCurrentFrameTime > _cMunchieFrameTime)
	{
		_frameCount++;
		if (_frameCount >= 2)
			_frameCount = 0;
		_munchieCurrentFrameTime = 0;
	}



	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	_pacman->currentFrameTime += elapsedTime;
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	UpdateGhosts(elapsedTime);
	CheckGhostCollisions(_ghosts[0]);

	CheckMunchiesCollisions();
	

}

void Pacman::UpdateMunchie(Enemy* munchie, int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		UpdateMunchie(_munchies[i], elapsedTime);
	}
}

void Pacman::UpdateGhosts(int elapsedTime)
{

	if (_ghosts[0]->direction == 0) //Moves Right
	{
		_ghosts[0]->posRect->X += _ghosts[0]->speed * elapsedTime;
	}
	else if (_ghosts[0]->direction == 1) //Move Left
	{
		_ghosts[0]->posRect->X -= _ghosts[0]->speed * elapsedTime;
	}
	if (_ghosts[0]->posRect->X + _ghosts[0]->posRect->Width >=
		Graphics::GetViewportWidth()) //Hits Right Edge
	{
		_ghosts[0]->direction = 1; //Change Direction
	}
	else if (_ghosts[0]->posRect->X <= 0) //Hits Left Edge
	{
		_ghosts[0]->direction = 0; //Change direction
	}
}

void Pacman::CheckGhostCollisions(MovingEnemy* _ghosts)
{
	if (CollisionCheck(_pacman->position->X, _pacman->position->Y, 
		_pacman->sourceRect->Width, _pacman->sourceRect->Height, 
		_ghosts->posRect->X, _ghosts->posRect->Y, _ghosts->sourceRect->Width, _ghosts->sourceRect->Height))
		_pacman->dead = true;
}

void Pacman::CheckMunchiesCollisions()
{
	for (int i = 0; i < MUNCHIECOUNT; i++) {

		if (CollisionCheck(_pacman->position->X, _pacman->position->Y,
			_pacman->sourceRect->Width, _pacman->sourceRect->Height,
			_munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->rect->Width, _munchies[i]->rect->Height) && !_munchies[i]->eaten)
		{
			_munchies[i]->eaten = true;
			eatenMunchies++;
		}
	}
}


void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y << endl;
	stream << "Pacman dead: " << _pacman->dead<<endl;
	stream << "Score: " << eatenMunchies;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_ghosts[0]->texture, _ghosts[0]->posRect, _ghosts[0]->sourceRect);
	if (!_pacman->dead)	
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (!_munchies[i]->eaten) {
			if (_munchies[i]->frameCount == 0)
				// Draws Red Munchie
			{
				SpriteBatch::Draw(_munchieInvertedTexture, _munchies[i]->position, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				_munchies[i]->frameCount++;
			}
			else
			{
				// Draw Blue Munchie
				SpriteBatch::Draw(_munchieBlueTexture, _munchies[i]->position, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				_munchies[i]->frameCount++;

				if (_frameCount == 0)
					_munchies[i]->frameCount = 0;
			}
		}
	}
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}



	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	
	SpriteBatch::EndDraw();
}


