﻿#define BOARD_W 10
#define BOARD_H 10
#define WIN_CHAIN 5

#include <iostream>
#include <cmath>

using namespace std;

class Game
{
private :

	enum class Stone
	{
		empty = 0,
		white = 1,
		black = 2
	};

	Stone gameBoard[BOARD_H * BOARD_W] = { };
	Stone turn = Stone::black;
	int turnPassed = 0;

public :
	Game()
	{
		cout << "[NEW GAME STARTED]" << endl;
	}

	~Game()
	{
		cout << "[GAME END]" << endl;
	}

	void Loop()
	{
		pair<int, int> coord;

		while (true)
		{
			DrawBoard();
			while (!GetInput(coord));
			PlaceStone(coord);
			if (IsVictory(coord.first, coord.second))
			{
				DrawBoard();
				cout << ((turn == Stone::white) ? "백돌" : "흑돌") << "의 승리!" << endl;
				break;
			}
			else if (turnPassed >= BOARD_H * BOARD_W)
			{
				cout << "칸이 모두 찼습니다, 무승부!" << endl;
				break;
			}
			SwitchTurn();
		}
	}

	void DrawBoard()
	{
		// x축 위치표시기
		cout << endl << "   ";
		for (size_t i = 0; i < BOARD_W; i++)
		{
			cout << i;

			if (i < 10) cout << " ";
		}
		cout << endl;

		for (int y = 0; y < BOARD_H; y++)
		{
			for (int x = 0; x < BOARD_W; x++)
			{
				// y축 표시기
				if (x == 0)
				{
					if (y < 10) cout << " ";
					cout << y << " ";
				}

				// 보드의 각 위치에 표시할 문자 (돌)
				string marker;

				switch (gameBoard[y * BOARD_W + x])
				{
				case Stone::white:
					marker = "●";
					break;
				case Stone::black:
					marker = "○";
					break;
				default:
					marker = "□";
					break;
				}
				cout << marker;
			}
			cout << endl;
		}
		cout << endl;
	}

	bool GetInput(pair<int, int> &output)
	{
		int xInput = -1;
		int yInput = -1;

		cout << ((turn == Stone::white) ? "백돌" : "흑돌") << "의 차례입니다." << endl
			<< "다음과 같이 좌표를 입력하여 돌을 놓습니다. : X Y" << endl;

		if (!(cin >> xInput >> yInput) ||
			!CanPlaceHere(xInput, yInput))
		{
			cout << "그렇게는 할 수 없습니다." << "\n\n";
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(256, '\n');
			}
			return false;
		}

		output.first = xInput;
		output.second = yInput;
		return true;
	}

	Stone GetPosColor(int x, int y) { return gameBoard[BOARD_W * y + x]; }

	bool CanPlaceHere(int x, int y)
	{
		if (x < 0 ||
			x >= BOARD_W ||
			y < 0 ||
			y >= BOARD_H ||
			GetPosColor(x, y) != Stone::empty)
			return false;

		return true;
	}

	void PlaceStone(pair<int, int> coord)
	{
		gameBoard[BOARD_W * coord.second + coord.first] = (turn == Stone::black) ? Stone::black : Stone::white;
		turnPassed++;
	}

	void SwitchTurn() { turn = (turn == Stone::black ? Stone::white : Stone::black); }

	bool IsVictory(int x, int y)
	{
		if (1 + GetStoneChain(x, y, 1, 0) + GetStoneChain(x, y, -1, 0) == WIN_CHAIN ||	// 가로 체크
			1 + GetStoneChain(x, y, 0, 1) + GetStoneChain(x, y, 0, -1) == WIN_CHAIN ||  // 세로 체크
			1 + GetStoneChain(x, y, 1, -1) + GetStoneChain(x, y, -1, 1) == WIN_CHAIN ||	// 우상향 체크
			1 + GetStoneChain(x, y, 1, 1) + GetStoneChain(x, y, -1, -1) == WIN_CHAIN )	// 우하향 체크
			return true;
		 return false;
	}

	int GetStoneChain(int x, int y, int xoff, int yoff)
	{
		if (x + xoff < 0 ||
			x + xoff >= BOARD_W ||
			y + yoff < 0 ||
			y + yoff >= BOARD_H ||
			GetPosColor(x + xoff, y + yoff) != turn)
			return 0;

		return 1 + GetStoneChain(x + xoff, y + yoff, xoff, yoff);
	}


};

int main()
{
	Game game;
	game.Loop();

	return 0;
}