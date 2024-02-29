#include"MyConsoleEnegine.h"
#include"Game.h"

//多线程播放音效
void PlayAudio1()
{
	//副线程自己一个循环
	while (1)
	{
		if (Game::is_play_audio1)
		{
			mciSendString(L"open bgm/消行.mp3", 0, 0, 0);
			mciSendString(L"play bgm/消行.mp3", 0, 0, 0);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			mciSendString(L"stop bgm/消行.mp3", 0, 0, 0);
			Game::is_play_audio1 = false;
			mciSendString(L"close bgm/消行.mp3", 0, 0, 0);
		}
	}
}
//多线程播放音效
void PlayAudio2()
{
	//副线程自己一个循环
	while (1)
	{
		if (Game::is_play_audio2)
		{
			mciSendString(L"open bgm/落方块.mp3", 0, 0, 0);
			mciSendString(L"play bgm/落方块.mp3", 0, 0, 0);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			mciSendString(L"stop bgm/落方块.mp3", 0, 0, 0);
			mciSendString(L"close bgm/落方块.mp3", 0, 0, 0);
			Game::is_play_audio2 = false;
		}
	}
}

int main()
{
	//两个副线程用来播放两种音效
	std::thread t1(PlayAudio1);
	std::thread t2(PlayAudio2);

	Game::GameInit();

	BeginBatchDraw();
	while (1)
	{
		Game::SelectGameState();
		FlushBatchDraw();
	}
	EndBatchDraw();

	//等待线程播放完毕
	t1.join();
	t2.join();

	return 0;
}