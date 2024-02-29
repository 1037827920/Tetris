#include"Game.h"

//游戏基本属性
GameAttribute Game::GA;
//游戏地图的状态
int Game::map_width = GA.game_width / 25;
int Game::map_height = GA.game_height / 25;
std::vector<std::vector<int>>Game::map;
//计时器
DWORD Game::tPre = GetTickCount();
DWORD Game::tNow = GetTickCount();
//游戏的状态
int Game::game_state = 0;
//鼠标的位置
MOUSEMSG Game::msg;
//目前方块和下一个方块
Shape* Game::now_shape = nullptr;
Shape* Game::next_shape = nullptr;
Shape* Game::show_next_shape = nullptr;
//方块核心位置
int Game::x = 0;
//产生方块我们需要给定一个范围，这样可以保证不会出现奇奇怪怪的错误
//其实就是使方块的落点比较整齐
std::vector<int>Game::x_scope;
//方块类型
int Game::now_type = 0;
int Game::next_type = 0;
//判断方块是否固定
bool Game::is_located = false;
//判断方块是否快速下降
bool Game::is_quick_down = false;
//游戏行数的消除积分
int Game::i_score = 0;
std::string Game::s_score = std::to_string(0);
//地图文件名种子   对应的用户名各自有一个存档
std::string Game::map_filename = "map.txt";
//检测存档是否读取成功
bool Game::is_read_succeed = false;
//判断用户是否登录，如果没有登录，不能进行游玩
bool Game::is_log_in = false;
//判断鼠标点击的是账号输入框还是密码输入框
int Game::account_or_password = 0;
//账号输入框的实时显示字符串
std::string Game::show_user_name = "";
//密码输入框的实时显示字符串
std::string Game::show_password = "";
//用来储存用户信息的文件
std::string Game::user_filename = "user.txt";
//用户名
std::string Game::user_name = "";
//默认登录用户储存文件
std::string Game::default_user_filename = "default_user.txt";
//地图大小文件
std::string Game::map_size_filename = "map_size.txt";
//键盘默认设置文件
std::string Game::default_key_filename = "defaultKey.txt";
//键盘设置文件
std::string Game::now_key_filename = "nowKey.txt";
//排行榜
std::vector<Rank>Game::rank;
//排行榜储存文件
std::string Game::rank_filename = "rank.txt";
//音乐目前曲目
int Game::music_index = 0;
//检测是否暂停音乐
bool Game::is_stop_music = true;
//是否播放消行音效
bool Game::is_play_audio1 = false;
//是否播放落方块音效
bool Game::is_play_audio2 = false;
//目前的背景颜色
int Game::bkcolor_index = 0;

//游戏最开始的初始化
void Game::GameInit()
{
	//随机数生成
	srand((unsigned int)time(NULL));
	//检查是否有默认的登录
	File file;
	file.ReadDefaultLogin(default_user_filename, user_name, is_log_in);
	//获取地图的大小
	file.ReadMapSize(map_size_filename, GA.game_width, GA.game_height);
	map_width = GA.game_width / 25;
	map_height = GA.game_height / 25;
	GA.W_ratio = (double)(GA.game_width + GA.extra_width) / 550.0;
	GA.H_ratio = (double)GA.game_height / 600.0;
	//读取按键设置
	file.ReadKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
	//初始化窗口
	initgraph(GA.game_width + GA.extra_width, GA.game_height);
	//读取排行榜
	file.ReadRank(rank_filename, rank);
	//播放音乐
	PlaySound(L"bgm/bgm1.wav", 0, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

//游戏状态函数
void Game::SelectGameState()
{
	switch (game_state)
	{
		//游戏开始菜单
	case Game::GameState::GS_StartMenu:
	{
		GameStartMenu();
	}
	break;
	//登录菜单
	case Game::GameState::GS_LogIn:
	{
		LogInMenu();
	}
	break;
	//注册菜单
	case Game::GameState::GS_Regist:
	{
		RegistMenu();
	}
	break;
	//设置
	case Game::GameState::GS_Set:
	{
		//进入设置前先清空输入缓冲区
		while (_kbhit())
		{
			_getch();
		}
		SetMenu();
	}
	break;
	//帮助界面
	case Game::GameState::GS_Help:
	{
		ShowHelpMenu();
	}
	break;
	//排行榜
	case Game::GameState::GS_Rank:
	{
		RankMenu();
	}
	break;
	//开始一个新游戏
	case Game::GameState::GS_StartNewGame:
	{
		//进入游戏前先初始化一下
		GamePlayInit();
		//进入游戏时先预生成下一个方块的形状
		GamePreGainNextBlock();
		//初始化一下即可转化为游戏进行状态
		game_state = Game::GameState::GS_GamePlay;
	}
	break;
	//游戏读取存档
	case Game::GameState::GS_StartReadMap:
	{
		ReadLastMap();
		//读取成功存档即可继续游戏
		if (is_read_succeed)
		{
			game_state = Game::GameState::GS_GamePlay;
		}
		else
		{
			game_state = Game::GameState::GS_StartMenu;
		}
	}
	break;
	//游戏进行
	case Game::GameState::GS_GamePlay:
	{
		GamePlay();
	}
	break;
	//游戏暂停
	case Game::GameState::GS_GamePause:
	{
		GamePause();
	}
	break;
	//游戏结束
	case Game::GameState::GS_GameOver:
	{
		GameOver();
	}
	break;
	//游戏结束提示是否保存游戏
	case Game::GameState::GS_GameOverSelectSave:
	{
		SelectSaveOrNo();
	}
	break;
	//保存存档
	case Game::GameState::GS_GameOverSave:
	{
		GameSave();
	}
	break;
	//退出程序
	case Game::GameState::GS_ProgramExit:
	{
		ExitProgram();
	}
	break;
	default:
		break;
	}
}

//游戏开始菜单
void Game::GameStartMenu()
{
	//设置背景为灰色
	setbkcolor(GA.background_color);
	cleardevice();
	//设置小方框
	setfillcolor(WHITE);
	solidrectangle(160 * GA.W_ratio, 150 * GA.H_ratio, 370 * GA.W_ratio, 200 * GA.H_ratio);
	solidrectangle(160 * GA.W_ratio, (150 + 75) * GA.H_ratio, 370 * GA.W_ratio, (200 + 75) * GA.H_ratio);
	solidrectangle(160 * GA.W_ratio, (150 + 2 * 75) * GA.H_ratio, 370 * GA.W_ratio, (200 + 2 * 75) * GA.H_ratio);
	solidrectangle(160 * GA.W_ratio, (150 + 3 * 75) * GA.H_ratio, 370 * GA.W_ratio, (200 + 3 * 75) * GA.H_ratio);
	solidrectangle(160 * GA.W_ratio, (150 + 4 * 75) * GA.H_ratio, 370 * GA.W_ratio, (200 + 4 * 75) * GA.H_ratio);

	//设置开始新游戏的文字
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(42 * GA.W_ratio * GA.H_ratio, 21, L"隶书");
	outtextxy(160 * GA.W_ratio, 150 * GA.H_ratio, L"开始新游戏");
	outtextxy(180 * GA.W_ratio, (150 + 75) * GA.H_ratio, L"继续游戏");
	outtextxy(200 * GA.W_ratio, (150 + 75 * 2) * GA.H_ratio, L"排行榜");
	outtextxy(220 * GA.W_ratio, (150 + 75 * 3) * GA.H_ratio, L"设置");
	outtextxy(180 * GA.W_ratio, (150 + 75 * 4) * GA.H_ratio, L"退出游戏");
	MenuMouseEvent();

	//设置登录的按钮
	if (!is_log_in)
	{
		setfillcolor(WHITE);
		solidrectangle(400 * GA.W_ratio, 100 * GA.H_ratio, 440 * GA.W_ratio, 120 * GA.H_ratio);

		setcolor(BLACK);
		settextstyle(20 * GA.H_ratio, 10 * GA.W_ratio, L"隶书");
		outtextxy(400 * GA.W_ratio, 100 * GA.H_ratio, L"登录");
	}
	else
	{
		setcolor(BLACK);
		settextstyle(20 * GA.H_ratio, 10 * GA.W_ratio, L"隶书");
		std::wstring w_user_name(user_name.begin(), user_name.end());
		outtextxy(400 * GA.W_ratio, 100 * GA.H_ratio, w_user_name.c_str());
		setfillcolor(WHITE);
		solidrectangle(450 * GA.W_ratio, 125 * GA.H_ratio, 530 * GA.W_ratio, 150 * GA.H_ratio);
		settextcolor(BLACK);
		settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
		outtextxy(452 * GA.W_ratio, 127 * GA.H_ratio, L"退出登录");
	}
}

//菜单页面的鼠标事件
void Game::MenuMouseEvent()
{
	//运用图形库的鼠标检测事件
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		//点击开始游戏即进入游戏进行状态
		if (msg.x > 160 * GA.W_ratio && msg.x < 370 * GA.W_ratio && msg.y > 150 * GA.H_ratio && msg.y < 200 * GA.H_ratio)
		{
			if (is_log_in)
				game_state = Game::GameState::GS_StartNewGame;
			else
				MessageBox(0, L"请先登录账号", 0, 0);
		}
		//点击继续游戏读取存档并进入上一次游戏中
		if (msg.x > 160 * GA.W_ratio && msg.x < 370 * GA.W_ratio && msg.y>(150 + 75) * GA.H_ratio && msg.y < (200 + 75) * GA.H_ratio)
		{
			if (is_log_in)
				game_state = Game::GameState::GS_StartReadMap;
			else
				MessageBox(0, L"请先登录账号", 0, 0);
		}
		//点击退出游戏即退出程序
		if (msg.x > 160 * GA.W_ratio && msg.x < 370 * GA.W_ratio && msg.y>(150 + 4 * 75) * GA.H_ratio && msg.y < (200 + 4 * 75) * GA.H_ratio)
		{
			game_state = Game::GameState::GS_ProgramExit;
		}
		//点击登录按钮登录
		if (msg.x > 400 * GA.W_ratio && msg.x < 440 * GA.W_ratio && msg.y>100 * GA.H_ratio && msg.y < 120 * GA.H_ratio && !is_log_in)
		{
			game_state = Game::GameState::GS_LogIn;
		}
		//点击退出登录按钮
		if (msg.x > 450 * GA.W_ratio && msg.x < 530 * GA.W_ratio && msg.y>125 * GA.H_ratio && msg.y < 150 * GA.H_ratio && is_log_in)
		{
			is_log_in = false;
			user_name = "";
			File file;
			file.SaveDefauleLogin(default_user_filename, user_name);
		}
		//点击设置按钮
		if (msg.x > 160 * GA.W_ratio && msg.x< 370 * GA.W_ratio && msg.y>(150 + 3 * 75) * GA.H_ratio && msg.y < (200 + 3 * 75) * GA.H_ratio)
		{
			game_state = Game::GameState::GS_Set;
		}
		//点击排行榜按钮
		if (msg.x > 160 * GA.W_ratio && msg.x< 370 * GA.W_ratio && msg.y>(150 + 2 * 75) * GA.H_ratio && msg.y < (200 + 2 * 75) * GA.H_ratio)
		{
			game_state = Game::GameState::GS_Rank;
		}
	}
}

//读取上一次的存档
void Game::ReadLastMap()
{
	//读取上一次方块的位置
	int x, y;
	File file;
	//读取存档
	std::string filename = user_name + map_filename;
	//保证地图为空
	if (map.size() == 0)
	{
		for (int i = 0; i < map_height; i++)
		{
			map.push_back(std::vector<int>(map_width));
			for (int j = 0; j < map_width; j++)
				map[i].push_back(0);
		}
	}
	else
	{
		for (int i = 0; i < map_height; i++)
		{
			for (int j = 0; j < map_width; j++)
				map[i][j] = 0;
		}
	}
	//初始化x_score
	if (x_scope.size() == 0)
	{
		for (int i = 0; i < map_width - 4; i++)
		{
			x_scope.push_back(25 * i);
		}
	}
	file.ReadMapFile(filename, map, map_width, map_height, x, y, now_type, next_type, i_score, is_read_succeed);
	switch (now_type)
	{
	case 0:
		now_shape = new ShapeO(x, y);
		break;
	case 1:
		now_shape = new ShapeI(x, y);
		break;
	case 2:
		now_shape = new ShapeS(x, y);
		break;
	case 3:
		now_shape = new ShapeZ(x, y);
		break;
	case 4:
		now_shape = new ShapeJ(x, y);
		break;
	case 5:
		now_shape = new ShapeL(x, y);
		break;
	case 6:
		now_shape = new ShapeT(x, y);
		break;
	default:
		break;
	}
	switch (next_type)
	{
	case 0:
		next_shape = new ShapeO(x, -2 * Block::width);
		break;
	case 1:
		next_shape = new ShapeI(x, -4 * Block::width);
		break;
	case 2:
		next_shape = new ShapeS(x, -2 * Block::width);
		break;
	case 3:
		next_shape = new ShapeZ(x, -2 * Block::width);
		break;
	case 4:
		next_shape = new ShapeJ(x, -3 * Block::width);
		break;
	case 5:
		next_shape = new ShapeL(x, -3 * Block::width);
		break;
	case 6:
		next_shape = new ShapeT(x, -2 * Block::width);
		break;
	default:
		break;
	}
}

//这个函数用来设置登录菜单
void Game::LogInMenu()
{
	cleardevice();
	//绘制框架
	setfillcolor(WHITE);
	solidrectangle(125 * GA.W_ratio, 200 * GA.H_ratio, 425 * GA.W_ratio, 400 * GA.H_ratio);
	setcolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	rectangle(125 * GA.W_ratio, 200 * GA.H_ratio, 425 * GA.W_ratio, 400 * GA.H_ratio);
	//绘制“×”号
	setlinestyle(PS_SOLID, 3);
	rectangle(400 * GA.W_ratio, 200 * GA.H_ratio, 425 * GA.W_ratio, 225 * GA.H_ratio);
	line(405 * GA.W_ratio, 205 * GA.H_ratio, 420 * GA.W_ratio, 220 * GA.H_ratio);
	line(420 * GA.W_ratio, 205 * GA.H_ratio, 405 * GA.W_ratio, 220 * GA.H_ratio);
	//绘制注册和登录按钮
	rectangle(175 * GA.W_ratio, 350 * GA.H_ratio, 225 * GA.W_ratio, 375 * GA.H_ratio);
	rectangle(310 * GA.W_ratio, 350 * GA.H_ratio, 360 * GA.W_ratio, 375 * GA.H_ratio);
	settextcolor(BLACK);
	settextstyle(20, 10, L"隶书");
	outtextxy(180 * GA.W_ratio, 352 * GA.H_ratio, L"注册");
	outtextxy(315 * GA.W_ratio, 352 * GA.H_ratio, L"登录");
	//绘制账号和密码输入框
	outtextxy(150 * GA.W_ratio, 250 * GA.H_ratio, L"账号");
	outtextxy(150 * GA.W_ratio, 300 * GA.H_ratio, L"密码");
	setcolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	rectangle(200 * GA.W_ratio, 250 * GA.H_ratio, 375 * GA.W_ratio, 275 * GA.H_ratio);
	rectangle(200 * GA.W_ratio, 300 * GA.H_ratio, 375 * GA.W_ratio, 325 * GA.H_ratio);

	//点击账号输入框高亮它  显示输入的账号
	if (account_or_password == 1)
	{
		setfillcolor(RGB(255, 255, 204));
		solidrectangle(200 * GA.W_ratio, 250 * GA.H_ratio, 375 * GA.W_ratio, 275 * GA.H_ratio);
	}
	//点击密码输入框高亮它   显示输入的密码
	if (account_or_password == 2)
	{
		setfillcolor(RGB(255, 255, 204));
		solidrectangle(200 * GA.W_ratio, 300 * GA.H_ratio, 375 * GA.W_ratio, 325 * GA.H_ratio);
	}

	//输入
	if (_kbhit())
	{
		//退格的ASCII码
		char c = _getch();
		if (c == 8)
		{
			if (account_or_password == 1 && !show_user_name.empty())
				show_user_name.erase(show_user_name.end() - 1);
			else if (account_or_password == 2 && !show_password.empty())
				show_password.erase(show_password.end() - 1);
		}
		else
		{
			if (account_or_password == 1)
				show_user_name.insert(show_user_name.end(), c);
			else if (account_or_password == 2)
				show_password.insert(show_password.end(), c);
		}
	}
	//显示输入的用户名和密码
	settextcolor(BLACK);
	settextstyle(20, 10, L"隶书");
	std::wstring show_w_user_name(show_user_name.begin(), show_user_name.end());
	outtextxy(200 * GA.W_ratio, 255 * GA.H_ratio, show_w_user_name.c_str());
	std::wstring show_w_password(show_password.begin(), show_password.end());
	outtextxy(200 * GA.W_ratio, 305 * GA.H_ratio, show_w_password.c_str());

	//登录菜单鼠标事件
	LogInMenuMouseEvent();
}

//这个函数用来处理登录菜单的鼠标事件
void Game::LogInMenuMouseEvent()
{
	//0x01是鼠标左键的虚拟键码
	if (IS_KEY_DOWN(0x01))
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			//点击右上角叉号关闭登录界面
			if (msg.x > 400 * GA.W_ratio && msg.x < 425 * GA.W_ratio && msg.y >  200 * GA.H_ratio && msg.y < 225 * GA.H_ratio)
			{
				//退出登录系统，需要把显示的字符串清空,顺便初始化account_or_password
				show_user_name = "";
				show_password = "";
				account_or_password = 0;
				game_state = Game::GameState::GS_StartMenu;
			}
			//点击账号输入框高亮它
			if (msg.x > 200 * GA.W_ratio && msg.x < 375 * GA.W_ratio && msg.y > 250 * GA.H_ratio && msg.y < 275 * GA.H_ratio)
			{
				account_or_password = 1;
			}
			//点击密码输入框高亮它
			if (msg.x > 200 * GA.W_ratio && msg.x < 375 * GA.W_ratio && msg.y >300 * GA.H_ratio && msg.y < 325 * GA.H_ratio)
			{
				account_or_password = 2;
			}
			//点击注册进入注册用户的界面
			if (msg.x > 175 * GA.W_ratio && msg.x < 225 * GA.W_ratio && msg.y >350 * GA.H_ratio && msg.y < 375 * GA.H_ratio)
			{
				//进入注册菜单，需要把显示的字符串清空,顺便初始化account_or_password
				show_user_name = "";
				show_password = "";
				account_or_password = 0;
				game_state = Game::GameState::GS_Regist;
			}
			//点击登录
			if (msg.x > 310 * GA.W_ratio && msg.x < 360 * GA.W_ratio && msg.y >350 * GA.H_ratio && msg.y < 375 * GA.H_ratio)
			{
				File file;
				file.ReadUserInfo(user_filename, show_user_name, show_password, is_log_in, user_name);
				//退出登录系统，需要把显示的字符串清空,顺便初始化account_or_password 顺便将用户名初始
				show_user_name = "";
				show_password = "";
				account_or_password = 0;
				if (!is_log_in)
					game_state = Game::GameState::GS_LogIn;
				else
				{
					file.SaveDefauleLogin(default_user_filename, user_name);
					game_state = Game::GameState::GS_StartMenu;
				}
			}
		}
	}
}

//注册菜单
void Game::RegistMenu()
{
	cleardevice();
	//绘制框架
	setfillcolor(WHITE);
	solidrectangle(125 * GA.W_ratio, 200 * GA.H_ratio, 425 * GA.W_ratio, 400 * GA.H_ratio);
	setcolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	rectangle(125 * GA.W_ratio, 200 * GA.H_ratio, 425 * GA.W_ratio, 400 * GA.H_ratio);
	//绘制“×”号
	setlinestyle(PS_SOLID, 3);
	rectangle(400 * GA.W_ratio, 200 * GA.H_ratio, 425 * GA.W_ratio, 225 * GA.H_ratio);
	line(405 * GA.W_ratio, 205 * GA.H_ratio, 420 * GA.W_ratio, 220 * GA.H_ratio);
	line(420 * GA.W_ratio, 205 * GA.H_ratio, 405 * GA.W_ratio, 220 * GA.H_ratio);
	//绘制“注册”
	settextcolor(RED);
	settextstyle(30 * GA.W_ratio, 15 * GA.H_ratio, L"隶书");
	outtextxy(250 * GA.W_ratio, 200 * GA.H_ratio, L"注册");
	//绘制账号和密码输入框
	settextcolor(BLACK);
	settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
	outtextxy(150 * GA.W_ratio, 250 * GA.H_ratio, L"账号");
	outtextxy(150 * GA.W_ratio, 300 * GA.H_ratio, L"密码");
	setcolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	rectangle(200 * GA.W_ratio, 250 * GA.H_ratio, 375 * GA.W_ratio, 275 * GA.H_ratio);
	rectangle(200 * GA.W_ratio, 300 * GA.H_ratio, 375 * GA.W_ratio, 325 * GA.H_ratio);
	//绘制确定按钮
	settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
	outtextxy(270 * GA.W_ratio, 350 * GA.H_ratio, L"确定");
	rectangle(265 * GA.W_ratio, 348 * GA.H_ratio, 320 * GA.W_ratio, 370 * GA.H_ratio);

	//点击账号输入框高亮它  显示输入的账号
	if (account_or_password == 1)
	{
		setfillcolor(RGB(255, 255, 204));
		solidrectangle(200 * GA.W_ratio, 250 * GA.H_ratio, 375 * GA.W_ratio, 275 * GA.H_ratio);
	}
	//点击密码输入框高亮它   显示输入的密码
	if (account_or_password == 2)
	{
		setfillcolor(RGB(255, 255, 204));
		solidrectangle(200 * GA.W_ratio, 300 * GA.H_ratio, 375 * GA.W_ratio, 325 * GA.H_ratio);
	}

	//输入
	if (_kbhit())
	{
		//退格的ASCII码
		char c = _getch();
		if (c == 8)
		{
			if (account_or_password == 1 && !show_user_name.empty())
				show_user_name.erase(show_user_name.end() - 1);
			else if (account_or_password == 2 && !show_password.empty())
				show_password.erase(show_password.end() - 1);
		}
		else
		{
			if (account_or_password == 1)
				show_user_name.insert(show_user_name.end(), c);
			else if (account_or_password == 2)
				show_password.insert(show_password.end(), c);
		}
	}
	//显示输入的用户名和密码
	settextcolor(BLACK);
	settextstyle(20, 10, L"隶书");
	std::wstring show_w_user_name(show_user_name.begin(), show_user_name.end());
	outtextxy(200 * GA.W_ratio, 255 * GA.H_ratio, show_w_user_name.c_str());
	std::wstring show_w_password(show_password.begin(), show_password.end());
	outtextxy(200 * GA.W_ratio, 305 * GA.H_ratio, show_w_password.c_str());

	//鼠标处理函数
	RegistMenuMouseEvent();
}

//注册菜单鼠标处理函数
void Game::RegistMenuMouseEvent()
{
	//0x01是鼠标左键的虚拟键码
	if (IS_KEY_DOWN(0x01))
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			//点击右上角叉号关闭登录界面
			if (msg.x > 400 * GA.W_ratio && msg.x < 425 * GA.W_ratio && msg.y >  200 * GA.H_ratio && msg.y < 225 * GA.H_ratio)
			{
				//退出登录系统，需要把显示的字符串清空
				show_user_name = "";
				show_password = "";
				game_state = Game::GameState::GS_LogIn;
			}
			//点击账号输入框高亮它
			if (msg.x > 200 * GA.W_ratio && msg.x < 375 * GA.W_ratio && msg.y > 250 * GA.H_ratio && msg.y < 275 * GA.H_ratio)
			{
				account_or_password = 1;
			}
			//点击密码输入框高亮它
			if (msg.x > 200 * GA.W_ratio && msg.x < 375 * GA.W_ratio && msg.y >300 * GA.H_ratio && msg.y < 325 * GA.H_ratio)
			{
				account_or_password = 2;
			}
			//点击确认注册用户
			if (msg.x > 265 * GA.W_ratio && msg.x < 320 * GA.W_ratio && msg.y>348 * GA.H_ratio && msg.y < 370 * GA.H_ratio)
			{
				File file;
				file.SaveUserInfo(user_filename, show_user_name, show_password);
				//退出登录系统，需要把显示的字符串清空
				show_user_name = "";
				show_password = "";
				game_state = Game::GameState::GS_LogIn;
			}
		}
	}
}

//设置界面
void Game::SetMenu()
{
	cleardevice();
	setfillcolor(WHITE);
	solidrectangle(250 * GA.W_ratio, 500 * GA.H_ratio, 300 * GA.W_ratio, 525 * GA.H_ratio);
	settextcolor(BLACK);
	settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
	outtextxy(255 * GA.W_ratio, 502 * GA.H_ratio, L"返回");
	//棋盘大小设置
	outtextxy(120 * GA.W_ratio, 100 * GA.H_ratio, L"棋盘宽");
	std::string s_game_width = std::to_string(GA.game_width);
	std::wstring ws_game_width(s_game_width.begin(), s_game_width.end());
	outtextxy(200 * GA.W_ratio, 100 * GA.H_ratio, ws_game_width.c_str());
	outtextxy(120 * GA.W_ratio, 130 * GA.H_ratio, L"棋盘高");
	std::string s_game_height = std::to_string(GA.game_height);
	std::wstring ws_game_height(s_game_height.begin(), s_game_height.end());
	outtextxy(200 * GA.W_ratio, 130 * GA.H_ratio, ws_game_height.c_str());
	setfillcolor(WHITE);
	solidrectangle(240 * GA.W_ratio, 115 * GA.H_ratio, 265 * GA.W_ratio, 135 * GA.H_ratio);
	solidrectangle(270 * GA.W_ratio, 115 * GA.H_ratio, 295 * GA.W_ratio, 135 * GA.H_ratio);
	setfillcolor(BLACK);
	POINT pos[] = { {255 * GA.W_ratio ,120 * GA.H_ratio} ,{245 * GA.W_ratio, 130 * GA.H_ratio} ,{260 * GA.W_ratio, 130 * GA.H_ratio} };
	solidpolygon((POINT*)pos, 3);
	POINT pos2[] = { {282 * GA.W_ratio,130 * GA.H_ratio},{275 * GA.W_ratio,120 * GA.H_ratio},{290 * GA.W_ratio,120 * GA.H_ratio} };
	solidpolygon((POINT*)pos2, 3);
	outtextxy(300 * GA.W_ratio, 115 * GA.H_ratio, L"地图大小修改后需要重启");
	//按键设置
	outtextxy(120 * GA.W_ratio, 160 * GA.H_ratio, L"旋转");
	outtextxy(120 * GA.W_ratio, 190 * GA.H_ratio, L"向左移动");
	outtextxy(120 * GA.W_ratio, 220 * GA.H_ratio, L"向右移动");
	outtextxy(120 * GA.W_ratio, 250 * GA.H_ratio, L"加速下落");
	outtextxy(120 * GA.W_ratio, 280 * GA.H_ratio, L"快速下落");
	outtextxy(120 * GA.W_ratio, 310 * GA.H_ratio, L"暂停");
	outtextxy(120 * GA.W_ratio, 340 * GA.H_ratio, L"中途退出游戏");
	setfillcolor(WHITE);
	solidrectangle(295 * GA.W_ratio, 158 * GA.H_ratio, 345 * GA.W_ratio, 180 * GA.H_ratio);
	outtextxy(315 * GA.W_ratio, 160 * GA.H_ratio, (wchar_t)GA.Upkey);
	solidrectangle(295 * GA.W_ratio, 188 * GA.H_ratio, 345 * GA.W_ratio, 210 * GA.H_ratio);
	outtextxy(315 * GA.W_ratio, 190 * GA.H_ratio, (wchar_t)GA.LeftKey);
	solidrectangle(295 * GA.W_ratio, 218 * GA.H_ratio, 345 * GA.W_ratio, 240 * GA.H_ratio);
	outtextxy(315 * GA.W_ratio, 220 * GA.H_ratio, (wchar_t)GA.RightKey);
	solidrectangle(295 * GA.W_ratio, 248 * GA.H_ratio, 345 * GA.W_ratio, 270 * GA.H_ratio);
	outtextxy(315 * GA.W_ratio, 250 * GA.H_ratio, (wchar_t)GA.DownKey);
	solidrectangle(295 * GA.W_ratio, 278 * GA.H_ratio, 345 * GA.W_ratio, 300 * GA.H_ratio);
	outtextxy(315 * GA.W_ratio, 280 * GA.H_ratio, (wchar_t)GA.QuickDownKey);
	solidrectangle(295 * GA.W_ratio, 308 * GA.H_ratio, 345 * GA.W_ratio, 330 * GA.H_ratio);
	outtextxy(315 * GA.W_ratio, 310 * GA.H_ratio, (wchar_t)GA.PauseKey);
	solidrectangle(295 * GA.W_ratio, 335 * GA.H_ratio, 345 * GA.W_ratio, 360 * GA.H_ratio);
	if (GA.QuitKey == 27)
		outtextxy(310 * GA.W_ratio, 340 * GA.H_ratio, L"Esc");
	else
		outtextxy(315 * GA.W_ratio, 340 * GA.H_ratio, (wchar_t)GA.QuitKey);
	//恢复默认按键设置
	setfillcolor(WHITE);
	solidrectangle(390 * GA.W_ratio, 195 * GA.H_ratio, 520 * GA.W_ratio, 220 * GA.H_ratio);
	settextcolor(BLACK);
	outtextxy(400 * GA.W_ratio, 200 * GA.H_ratio, L"恢复默认设置");
	//方块下落速度设置
	outtextxy(120 * GA.W_ratio, 400 * GA.H_ratio, L"方块下落速度");
	std::string temp = std::to_string(Block::y_speed);
	std::wstring w_temp(temp.begin(), temp.end());
	outtextxy(300 * GA.W_ratio, 400 * GA.H_ratio, w_temp.c_str());
	//画出调节方块下落速度的按键
	setfillcolor(WHITE);
	solidrectangle(320 * GA.W_ratio, 400 * GA.H_ratio, 345 * GA.W_ratio, 420 * GA.H_ratio);
	solidrectangle(350 * GA.W_ratio, 400 * GA.H_ratio, 375 * GA.W_ratio, 420 * GA.H_ratio);
	setfillcolor(BLACK);
	POINT pos3[] = { {332 * GA.W_ratio ,405 * GA.H_ratio} ,{320 * GA.W_ratio, 415 * GA.H_ratio} ,{335 * GA.W_ratio, 415 * GA.H_ratio} };
	solidpolygon((POINT*)pos3, 3);
	POINT pos4[] = { { 362 * GA.W_ratio,415 * GA.H_ratio},{355 * GA.W_ratio,405 * GA.H_ratio},{370 * GA.W_ratio,405 * GA.H_ratio} };
	solidpolygon((POINT*)pos4, 3);
	//切换歌曲
	setfillcolor(WHITE);
	solidrectangle(85 * GA.W_ratio, 460 * GA.H_ratio, 215 * GA.W_ratio, 490 * GA.H_ratio);
	settextcolor(BLACK);
	outtextxy(95 * GA.W_ratio, 465 * GA.H_ratio, L"切换背景音乐");
	//暂停播放音乐
	solidrectangle(85 * GA.W_ratio, 500 * GA.H_ratio, 215 * GA.W_ratio, 530 * GA.H_ratio);
	outtextxy(95 * GA.W_ratio, 505 * GA.H_ratio, L"暂停播放音乐");
	//帮助
	solidrectangle(255 * GA.W_ratio, 550 * GA.H_ratio, 300 * GA.W_ratio, 570 * GA.H_ratio);
	outtextxy(257 * GA.W_ratio, 552 * GA.H_ratio, L"帮助");
	//设置背景色
	solidrectangle(380 * GA.W_ratio, 460 * GA.H_ratio, 510 * GA.W_ratio, 490 * GA.H_ratio);
	outtextxy(382 * GA.W_ratio, 465 * GA.H_ratio, L"切换背景色");

	//鼠标事件处理
	SetMenuMouseEvent();
}

//设置界面鼠标处理事件
void Game::SetMenuMouseEvent()
{
	File file;
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		if (msg.x > 250 * GA.W_ratio && msg.x < 300 * GA.W_ratio && msg.y> 500 * GA.H_ratio && msg.y < 525 * GA.H_ratio)
		{
			game_state = Game::GameState::GS_StartMenu;
		}
		if (msg.x > 240 * GA.W_ratio && msg.x < 265 * GA.W_ratio && msg.y>115 * GA.H_ratio && msg.y < 135 * GA.H_ratio)
		{
			if (GA.game_height < 850 && GA.game_width < 650)
			{
				GA.game_width += 25;
				GA.game_height += 25;
			}
			else
				MessageBox(0, L"超出范围", 0, 0);
			file.SaveMapSize(map_size_filename, GA.game_width, GA.game_height);
		}
		if (msg.x > 270 * GA.W_ratio && msg.x < 295 * GA.W_ratio && msg.y>115 * GA.H_ratio && msg.y < 135 * GA.H_ratio)
		{
			if (GA.game_height > 500 && GA.game_width > 300)
			{
				GA.game_width -= 25;
				GA.game_height -= 25;
			}
			else
				MessageBox(0, L"超出范围", 0, 0);
			file.SaveMapSize(map_size_filename, GA.game_width, GA.game_height);
		}
		if (msg.x > 320 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y> 400 * GA.H_ratio && msg.y < 420 * GA.H_ratio)
		{
			if (Block::y_speed < 15)
				Block::y_speed++;
			else
				MessageBox(0, L"超出设置范围", 0, 0);
		}
		if (msg.x > 350 * GA.W_ratio && msg.x < 375 * GA.W_ratio && msg.y> 400 * GA.H_ratio && msg.y < 420 * GA.H_ratio)
		{
			if (Block::y_speed > 1)
				Block::y_speed--;
			else
				MessageBox(0, L"超出设置范围", 0, 0);
		}
		//按键修改
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y> 158 * GA.H_ratio && msg.y < 180 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if (key >= 65 && key <= 90)
			{
				if (key != GA.PauseKey && key != GA.DownKey && key != GA.LeftKey && key != GA.RightKey && key != GA.QuickDownKey && key != GA.QuitKey)
				{
					GA.Upkey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);

		}
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y> 188 * GA.H_ratio && msg.y < 210 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if (key >= 65 && key <= 90)
			{
				if (key != GA.PauseKey && key != GA.Upkey && key != GA.DownKey && key != GA.RightKey && key != GA.QuickDownKey && key != GA.QuitKey)
				{
					GA.LeftKey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);
		}
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y>218 * GA.H_ratio && msg.y < 240 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if (key >= 65 && key <= 90)
			{
				if (key != GA.PauseKey && key != GA.Upkey && key != GA.DownKey && key != GA.LeftKey && key != GA.QuickDownKey && key != GA.QuitKey)
				{
					GA.RightKey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);
		}
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y> 248 * GA.H_ratio && msg.y < 270 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if (key >= 65 && key <= 90)
			{
				if (key != GA.PauseKey && key != GA.Upkey && key != GA.LeftKey && key != GA.RightKey && key != GA.QuickDownKey && key != GA.QuitKey)
				{
					GA.DownKey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);
		}
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y> 278 * GA.H_ratio && msg.y < 300 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if (key >= 65 && key <= 90)
			{
				if (key != GA.PauseKey && key != GA.Upkey && key != GA.DownKey && key != GA.LeftKey && key != GA.RightKey && key != GA.QuitKey)
				{
					GA.QuickDownKey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);
		}
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y>  308 * GA.H_ratio && msg.y < 330 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if (key >= 65 && key <= 90)
			{
				if (key != GA.Upkey && key != GA.DownKey && key != GA.LeftKey && key != GA.RightKey && key != GA.QuickDownKey && key != GA.QuitKey)
				{
					GA.PauseKey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);
		}
		if (msg.x > 295 * GA.W_ratio && msg.x < 345 * GA.W_ratio && msg.y>   335 * GA.H_ratio && msg.y < 360 * GA.H_ratio)
		{
			char c = _getch();
			int key = (int)c;
			if (key >= 97 && key <= 122)
				key -= 32;
			if ((key >= 65 && key <= 90) || key == 27)
			{
				if (key != GA.PauseKey && key != GA.Upkey && key != GA.DownKey && key != GA.LeftKey && key != GA.RightKey && key != GA.QuickDownKey)
				{
					GA.QuitKey = key;
					file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
				}
				else
					MessageBox(0, L"您设置的按键已存在了", 0, 0);
			}
			else
				MessageBox(0, L"设置的按键无效", 0, 0);
		}
		if (msg.x > 390 * GA.W_ratio && msg.x < 520 * GA.W_ratio && msg.y>   195 * GA.H_ratio && msg.y < 220 * GA.H_ratio)
		{
			file.ReadKeySet(default_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
			file.SaveKeySet(now_key_filename, GA.PauseKey, GA.Upkey, GA.DownKey, GA.LeftKey, GA.RightKey, GA.QuickDownKey, GA.QuitKey);
		}
		//切换音乐和暂停音乐
		if (msg.x > 85 * GA.W_ratio && msg.x < 215 * GA.W_ratio && msg.y> 460 * GA.H_ratio && msg.y < 490 * GA.H_ratio)
		{
			if (music_index == 100)
				music_index = -1;
			music_index++;
			if (music_index == 5)
				music_index -= 5;
			PlayBGM();
		}
		if (msg.x > 85 * GA.W_ratio && msg.x < 215 * GA.W_ratio && msg.y> 500 * GA.H_ratio && msg.y < 530 * GA.H_ratio)
		{
			music_index = 100;
			PlayBGM();
		}
		//进入帮助界面
		if (msg.x > 255 * GA.W_ratio && msg.x < 300 * GA.W_ratio && msg.y>550 * GA.H_ratio && msg.y < 570 * GA.H_ratio)
		{
			game_state = GameState::GS_Help;
		}
		//切换背景色
		if (msg.x > 380 * GA.W_ratio && msg.x < 510 * GA.W_ratio && msg.y>460 * GA.H_ratio && msg.y < 490 * GA.H_ratio)
		{
			bkcolor_index++;
			if (bkcolor_index > 4)
				bkcolor_index -= 5;
			switch (bkcolor_index)
			{
			case 0:
			{
				GA.background_color = RGB(192, 192, 192);
			}break;
			case 1:
			{
				GA.background_color = RGB(255, 255, 153);
			}break;
			case 2:
			{
				GA.background_color = RGB(255, 204, 204);
			}break;
			case 3:
			{
				GA.background_color = RGB(0, 255, 128);
			}break;
			case 4:
			{
				GA.background_color = RGB(229, 204, 255);
			}break;
			}
			setbkcolor(GA.background_color);
			cleardevice();
		}
	}
}

//帮助界面
void Game::ShowHelpMenu()
{
	cleardevice();
	setfillcolor(WHITE);
	solidrectangle(250 * GA.W_ratio, 500 * GA.H_ratio, 300 * GA.W_ratio, 525 * GA.H_ratio);
	settextcolor(BLACK);
	settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
	outtextxy(255 * GA.W_ratio, 502 * GA.H_ratio, L"返回");

	outtextxy(25 * GA.W_ratio, 150 * GA.H_ratio, L"由小方块组成的不同形状的板块陆续从屏幕上方落下来，玩");
	outtextxy(25 * GA.W_ratio, 175 * GA.H_ratio, L"家通过调整板块的位置和方向，使它们在屏幕底部拼出完整");
	outtextxy(25 * GA.W_ratio, 200 * GA.H_ratio, L"的一条或几条。这些完整的横条会随即消失，给新落下来的");
	outtextxy(25 * GA.W_ratio, 225 * GA.H_ratio, L"板块腾出空间，与此同时，玩家得到分数奖励。没有被消除");
	outtextxy(25 * GA.W_ratio, 250 * GA.H_ratio, L"掉的方块不断堆积起来，一旦堆到屏幕顶端，玩家便告输，");
	outtextxy(25 * GA.W_ratio, 275 * GA.H_ratio, L"游戏结束。");
	HelpMenuMouseEvent();
}

//帮助界面鼠标处理函数
void Game::HelpMenuMouseEvent()
{
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		if (msg.x > 250 * GA.W_ratio && msg.x < 300 * GA.W_ratio && msg.y> 500 * GA.H_ratio && msg.y < 525 * GA.H_ratio)
		{
			game_state = Game::GameState::GS_Set;
		}
	}
}

//排行榜菜单
void Game::RankMenu()
{
	//更新排行榜
	File file;
	file.ReadRank(rank_filename, rank);

	cleardevice();
	setfillcolor(WHITE);
	//返回
	solidrectangle(250 * GA.W_ratio, 500 * GA.H_ratio, 300 * GA.W_ratio, 525 * GA.H_ratio);
	settextcolor(BLACK);
	settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
	outtextxy(255 * GA.W_ratio, 502 * GA.H_ratio, L"返回");
	solidrectangle(150 * GA.W_ratio, 100 * GA.H_ratio, 450 * GA.W_ratio, 155 * GA.H_ratio);
	solidrectangle(150 * GA.W_ratio, 170 * GA.H_ratio, 450 * GA.W_ratio, 225 * GA.H_ratio);
	solidrectangle(150 * GA.W_ratio, 240 * GA.H_ratio, 450 * GA.W_ratio, 295 * GA.H_ratio);
	solidrectangle(150 * GA.W_ratio, 310 * GA.H_ratio, 450 * GA.W_ratio, 365 * GA.H_ratio);
	solidrectangle(150 * GA.W_ratio, 380 * GA.H_ratio, 450 * GA.W_ratio, 435 * GA.H_ratio);

	settextstyle(20 * GA.W_ratio, 10 * GA.H_ratio, L"隶书");
	for (int i = 0; i < rank.size(); i++)
	{
		std::wstring w_user_name0(rank.at(i).user_name.begin(), rank.at(i).user_name.end());
		outtextxy(220 * GA.W_ratio, (115 + i * 70) * GA.H_ratio, w_user_name0.c_str());
		outtextxy(150 * GA.W_ratio, (115 + i * 70) * GA.H_ratio, L"第  名：      分数");
		std::string index = std::to_string(i + 1);
		std::wstring w_index(index.begin(), index.end());
		outtextxy(177 * GA.W_ratio, (115 + i * 70) * GA.H_ratio, w_index.c_str());
		std::string str0 = std::to_string(rank.at(i).score);
		std::wstring w_s_score0(str0.begin(), str0.end());
		outtextxy(380 * GA.W_ratio, (115 + i * 70) * GA.H_ratio, w_s_score0.c_str());
		std::string year0 = std::to_string(rank.at(i).year);
		std::wstring w_year0(year0.begin(), year0.end());
		outtextxy(250 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, w_year0.c_str());
		outtextxy(290 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, L"-");
		std::string mouth0 = std::to_string(rank.at(i).mouth);
		std::wstring w_mouth0(mouth0.begin(), mouth0.end());
		outtextxy(300 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, w_mouth0.c_str());
		outtextxy(310 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, L"-");
		std::string day0 = std::to_string(rank.at(i).day);
		std::wstring w_day0(day0.begin(), day0.end());
		outtextxy(320 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, w_day0.c_str());
		std::string hour0 = std::to_string(rank.at(i).hour);
		std::wstring w_hour0(hour0.begin(), hour0.end());
		outtextxy(350 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, w_hour0.c_str());
		outtextxy(370 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, L":");
		std::string min0 = std::to_string(rank.at(i).min);
		std::wstring w_min0(min0.begin(), min0.end());
		outtextxy(380 * GA.W_ratio, (135 + i * 70) * GA.H_ratio, w_min0.c_str());
	}

	RankMenuMouseEvent();
}

//排行榜菜单鼠标事件处理函数
void Game::RankMenuMouseEvent()
{
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		if (msg.x > 250 * GA.W_ratio && msg.x < 300 * GA.W_ratio && msg.y> 500 * GA.H_ratio && msg.y < 525 * GA.H_ratio)
		{
			game_state = Game::GameState::GS_StartMenu;
		}
	}
}

//播放音乐
void Game::PlayBGM()
{
	switch (music_index)
	{
	case 0:
	{
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound(L"bgm/bgm1.wav", 0, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	break;
	case 1:
	{
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound(L"bgm/bgm2.wav", 0, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	break;
	case 2:
	{
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound(L"bgm/bgm3.wav", 0, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	break;
	case 3:
	{
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound(L"bgm/bgm4.wav", 0, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	break;
	case 4:
	{
		PlaySound(NULL, NULL, SND_PURGE);
		PlaySound(L"bgm/bgm5.wav", 0, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	break;
	case 100:
	{
		PlaySound(NULL, NULL, SND_PURGE);
	}
	break;
	}
}

//这个函数用来初始化游戏
void Game::GamePlayInit()
{
	//保证地图为空
	if (map.size() == 0)
	{
		for (int i = 0; i < map_height; i++)
		{
			map.push_back(std::vector<int>(map_width));
			for (int j = 0; j < map_width; j++)
				map[i].push_back(0);
		}
	}
	else
	{
		for (int i = 0; i < map_height; i++)
		{
			for (int j = 0; j < map_width; j++)
				map[i][j] = 0;
		}
	}
	//初始化x_score
	if (x_scope.size() == 0)
	{
		for (int i = 0; i < map_width - 4; i++)
		{
			x_scope.push_back(25 * i);
		}
	}
	//清空分数
	i_score = 0;
	//生成现在方块的位置和形状
	int index = 0;
	index = rand() % (map_width - 4);
	x = x_scope[index];
	now_type = rand() % 7;
	switch (now_type)
	{
	case 0:
		now_shape = new ShapeO(x, -2 * Block::width);
		break;
	case 1:
		now_shape = new ShapeI(x, -4 * Block::width);
		break;
	case 2:
		now_shape = new ShapeS(x, -2 * Block::width);
		break;
	case 3:
		now_shape = new ShapeZ(x, -2 * Block::width);
		break;
	case 4:
		now_shape = new ShapeJ(x, -3 * Block::width);
		break;
	case 5:
		now_shape = new ShapeL(x, -3 * Block::width);
		break;
	case 6:
		now_shape = new ShapeT(x, -2 * Block::width);
		break;
	default:
		break;
	}
}

//游戏进行的总函数，包含下面的函数
void Game::GamePlay()
{
	Game::tNow = GetTickCount();
	if (Game::tNow - Game::tPre > 50)
	{
		//游戏中途退出 进行判断是否要保存存档
		//需要注意的是，保存存档一定要在GamePlaying函数之前保存，这样now_type和next_type才是正确的
		if (IS_KEY_DOWN(GA.QuitKey))
			game_state = Game::GameState::GS_GameOverSelectSave;

		//游戏进行
		GamePlayBackground();
		GamePlaying();


		Game::tPre = Game::tNow;
	}
}

//游戏进行背景的绘制
void Game::GamePlayBackground()
{
	//绘制游戏主要背景
	setbkcolor(GA.background_color);
	cleardevice();
	setfillcolor(RGB(160, 160, 160));
	solidrectangle(GA.game_width, 0, GA.game_width + GA.extra_width, GA.game_height);
}

//预先获得下一个方块
void Game::GamePreGainNextBlock()
{
	//生成方块的位置和形状
	int index = 0;
	index = rand() % (map_width - 4);
	x = x_scope[index];
	next_type = rand() % 7;
	switch (next_type)
	{
	case 0:
		next_shape = new ShapeO(x, -2 * Block::width);
		break;
	case 1:
		next_shape = new ShapeI(x, -4 * Block::width);
		break;
	case 2:
		next_shape = new ShapeS(x, -2 * Block::width);
		break;
	case 3:
		next_shape = new ShapeZ(x, -2 * Block::width);
		break;
	case 4:
		next_shape = new ShapeJ(x, -3 * Block::width);
		break;
	case 5:
		next_shape = new ShapeL(x, -3 * Block::width);
		break;
	case 6:
		next_shape = new ShapeT(x, -2 * Block::width);
		break;
	default:
		break;
	}
}

//给现在的方块赋值并预先获取写一个方块的形状
void Game::GameAssignNowBlock()
{
	now_shape = next_shape;
	now_type = next_type;//因为后面都是对next_shape进行随机处理，所以now_type的值不再改变，需要主动更新一下
	GamePreGainNextBlock();
}

//游戏进行时
void Game::GamePlaying()
{
	if (is_located || is_quick_down)
	{
		//一个方块到达底部或者快速下降，要生成另外一个方块
		GameAssignNowBlock();
		is_located = false;
		is_quick_down = false;
	}
	//画下落的方块
	DrawFourBlock(now_shape->B, now_shape->color);
	//画地图
	DrawMap();
	//方块移动   方块间碰撞检测    底部边界检测
	BlockMoveAndBlockCrash(now_shape->B);
	//两边的边界检测
	now_shape->TwoSideBorder(GA.game_width);
	//形状的旋转，顺便将暂停的按键也放入这个函数中
	now_shape->Spin(game_state, GA.PauseKey, GA.Upkey);
	//给地图状态赋值
	AssignMapState(now_shape->B, now_shape->color);
	//检查每一行是否满了
	CheckLine();
	//方块下落预期
	SimulateBlockDownAndBlockQuickDown(now_shape->B, now_shape->color);
	//画出下一个方块
	DrawNextBlock();
	//显示分数
	GameShowScore();
	//判断游戏是否失败
	IsGameOver();
}

//这个函数用来画出四个方块
void Game::DrawFourBlock(Block* B, int color)
{
	setfillcolor(color);
	solidrectangle(B[0].x, B[0].y, B[0].x + Block::width, B[0].y + Block::width);
	solidrectangle(B[1].x, B[1].y, B[1].x + Block::width, B[1].y + Block::width);
	solidrectangle(B[2].x, B[2].y, B[2].x + Block::width, B[2].y + Block::width);
	solidrectangle(B[3].x, B[3].y, B[3].x + Block::width, B[3].y + Block::width);

	//注意要先填充再画边框，如果先画边框再填充会被覆盖
	setcolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	rectangle(B[0].x, B[0].y, B[0].x + Block::width, B[0].y + Block::width);
	rectangle(B[1].x, B[1].y, B[1].x + Block::width, B[1].y + Block::width);
	rectangle(B[2].x, B[2].y, B[2].x + Block::width, B[2].y + Block::width);
	rectangle(B[3].x, B[3].y, B[3].x + Block::width, B[3].y + Block::width);
}

//用这个函数来画地图
void Game::DrawMap()
{
	//0代表空白  1代表O型  2代表I型	  3代表S型
	//4代表Z型   5代表J型  6代表L型   7代表T型
	for (int i = map_height - 1; i >= 0; i--)
	{
		for (int j = map_width - 1; j >= 0; j--)
		{
			switch (map[i][j])
			{
			case 0:
				break;
			case 1:
			{
				setfillcolor(YELLOW);
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			case 2:
			{
				setfillcolor(RGB(0, 0, 204));
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			case 3:
			{
				setfillcolor(RGB(255, 0, 0));
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			case 4:
			{
				setfillcolor(RGB(127, 0, 255));
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			case 5:
			{
				setfillcolor(RGB(0, 153, 0));
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			case 6:
			{
				setfillcolor(RGB(255, 153, 51));
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			case 7:
			{
				setfillcolor(RGB(0, 255, 255));
				solidrectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
				setcolor(BLACK);
				rectangle(j * 25, i * 25, j * 25 + Block::width, i * 25 + Block::width);
			}break;
			default:
				break;
			}
		}
	}
}

//这个函数用来模拟四个方块的移动以及检查方块间的碰撞,还有底部边界的检测
void Game::BlockMoveAndBlockCrash(Block* B)
{
	bool is_x_move = true;
	bool is_y_move = true;
	for (int i = 0; i < 4; i++)
	{
		if (B[i].y >= GA.game_height - Block::width)
		{
			is_located = true;
			is_play_audio2 = true;
			is_y_move = false;
		}
		else if (B[i].y >= 0)
		{
			if (map[((B[i].y + 2 * Block::width) / Block::width) - 1][B[i].x / Block::width] >= 1)//-1  防越界
			{
				is_y_move = false;
				is_located = true;
				is_play_audio2 = true;
			}
		}
	}
	for (int i = 0; i < 4 && is_y_move; i++)
	{
		B[i].y += Block::y_speed;
	}
	if (IS_KEY_DOWN(GA.LeftKey))
	{
		//检测左边有无方块阻挡，如果有，则方块不能移动
		for (int i = 0; i < 4; i++)
		{
			//进入棋盘中才开始判断
			if (B[i].y >= 0 && B[i].x >= Block::width)
			{
				if (map[B[i].y / Block::width][(B[i].x - Block::x_speed) / Block::width] >= 1)//+1  防越界
				{
					is_x_move = false;
				}
			}
		}
		for (int i = 0; i < 4 && is_x_move; i++)
		{
			B[i].x -= Block::x_speed;
		}
		//防止移动过快,稍微延迟一下
		Sleep(80);
	}
	if (IS_KEY_DOWN(GA.RightKey))
	{
		//检测左边有无方块阻挡
		for (int i = 0; i < 4; i++)
		{
			//进入棋盘中才开始判断     右边无空位才进行判断   如果方块都能移动过去，那判断有无方块阻挡没有意义
			if (B[i].y >= 0 && B[i].x <= GA.game_width - Block::width - Block::x_speed)
			{
				if (map[B[i].y / Block::width][((B[i].x + Block::width + Block::x_speed) / Block::width) - 1] >= 1)//-1  防越界
				{
					is_x_move = false;
				}
			}
		}
		for (int i = 0; i < 4 && is_x_move; i++)
		{

			B[i].x += Block::x_speed;
		}
		//防止移动过快,稍微延迟一下
		Sleep(80);
	}
	if (IS_KEY_DOWN(GA.DownKey))
	{
		for (int i = 0; i < 4 && is_y_move; i++)
		{
			B[i].y += Block::y_speed;
		}
		Sleep(40);
	}
}

//这个函数用来给地图的状态赋值，并重新生成方块
void Game::AssignMapState(Block* B, int color)
{
	if (is_located)
	{
		for (int i = 0; i < 4; i++)
		{
			if (B[i].y >= 0)
			{
				switch (color)
				{
				case YELLOW:
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 1;
				}break;
				case RGB(0, 0, 204):
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 2;
				}break;
				case RGB(255, 0, 0):
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 3;
				}break;
				case RGB(127, 0, 255):
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 4;
				}break;
				case RGB(0, 153, 0):
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 5;
				}break;
				case RGB(255, 153, 51):
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 6;
				}break;
				case RGB(0, 255, 255):
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 7;
				}break;
				default:
				{
					map[B[i].y / Block::width][B[i].x / Block::width] = 0;
				}break;
				}
			}
		}
	}
}

//这个函数用来每一行的检测，满了就清空
void Game::CheckLine()
{
	int sign = map_width;
	int count = 0;
	for (int i = map_height - 1; i >= 0; i--)
	{
		count = 0;
		for (int j = map_width - 1; j >= 0; j--)
		{
			if (map[i][j] != 0)
				count++;
		}
		if (count == sign)
		{
			//消除一个，分数+1
			i_score += count;
			for (int j = map_width - 1; j >= 0; j--)
				map[i][j] = 0;
			for (int k = i; k > 0; k--)
			{
				for (int l = map_width - 1; l >= 0; l--)
				{
					map[k][l] = map[k - 1][l];
				}
			}
			for (int l = map_width - 1; l >= 0; l--)
				map[0][l] = 0;
			i++;
			is_play_audio1 = true;
		}
	}
}

//这个函数用来模拟方块下落预期,并通过按下空格键能使方块快速下降
void Game::SimulateBlockDownAndBlockQuickDown(Block* B, int color)
{
	//下落预期
	int min_y_distance = GA.game_height - Block::width - B[0].y;
	for (int i = 1; i < 4; i++)
	{
		min_y_distance = min(min_y_distance, GA.game_height - Block::width - B[i].y);
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < map_height; j++)
		{
			if (map[j][B[i].x / Block::width] != 0 && B[i].x / Block::width < map_width)
			{
				min_y_distance = min(min_y_distance, (j - 1) * Block::width - B[i].y);
				break;
			}
		}
	}
	setcolor(BLACK);
	setlinestyle(PS_DASH, 3);
	rectangle(B[0].x, B[0].y + min_y_distance, B[0].x + Block::width, B[0].y + min_y_distance + Block::width);
	rectangle(B[1].x, B[1].y + min_y_distance, B[1].x + Block::width, B[1].y + min_y_distance + Block::width);
	rectangle(B[2].x, B[2].y + min_y_distance, B[2].x + Block::width, B[2].y + min_y_distance + Block::width);
	rectangle(B[3].x, B[3].y + min_y_distance, B[3].x + Block::width, B[3].y + min_y_distance + Block::width);

	//快速下落
	if (IS_KEY_DOWN(GA.QuickDownKey))
	{
		is_quick_down = true;
		is_play_audio2 = true;
		for (int i = 0; i < 4; i++)
		{
			switch (color)
			{
			case YELLOW:
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 1;
			}break;
			case RGB(0, 0, 204):
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 2;
			}break;
			case RGB(255, 0, 0):
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 3;
			}break;
			case RGB(127, 0, 255):
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 4;
			}break;
			case RGB(0, 153, 0):
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 5;
			}break;
			case RGB(255, 153, 51):
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 6;
			}break;
			case RGB(0, 255, 255):
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 7;
			}break;
			default:
			{
				if ((B[i].y + min_y_distance) / Block::width < map_height && B[i].x / Block::width < map_width)
					map[(B[i].y + min_y_distance) / Block::width][B[i].x / Block::width] = 0;
			}break;
			}
		}
		//防止快速下降多个方块
		Sleep(185);
	}
}

//这个函数用来画出下一个方块的形状
void Game::DrawNextBlock()
{
	setcolor(BLACK);
	setlinestyle(PS_SOLID, 5);
	int x = GA.game_width + 25;
	int y = 100;
	settextcolor(RGB(25, 51, 0));
	settextstyle(16 * GA.W_ratio, 8 * GA.H_ratio, L"隶书");
	outtextxy(x - 20, 75, L"下一个方块的形状");
	for (int i = 0; i < 5; i++)
	{
		line((x + Block::width * i), y, (x + Block::width * i), (y + 4 * Block::width));
	}
	for (int i = 0; i < 5; i++)
	{
		line(x, (y + Block::width * i), (x + 4 * Block::width), (y + Block::width * i));
	}
	switch (next_type)
	{
	case 0:
		show_next_shape = new ShapeO(x + Block::width, y);
		break;
	case 1:
		show_next_shape = new ShapeI(x + Block::width, y);
		break;
	case 2:
		show_next_shape = new ShapeS(x + Block::width, y);
		break;
	case 3:
		show_next_shape = new ShapeZ(x + Block::width, y);
		break;
	case 4:
		show_next_shape = new ShapeJ(x + Block::width, y);
		break;
	case 5:
		show_next_shape = new ShapeL(x + Block::width, y);
		break;
	case 6:
		show_next_shape = new ShapeT(x + Block::width, y);
		break;
	default:
		break;
	}
	DrawFourBlock(show_next_shape->B, show_next_shape->color);
}

//这个函数用来显示分数
void Game::GameShowScore()
{
	s_score = std::to_string(i_score);
	std::wstring ws_score(s_score.begin(), s_score.end());//因为要用outtextxy显示分数，参数得是宽字符，不然会乱码
	settextcolor(RGB(25, 51, 0));//设置文本颜色
	settextstyle(30 * GA.W_ratio, 15 * GA.H_ratio, L"隶书");
	outtextxy(GA.game_width + 25, 280 * GA.H_ratio, L"分数");
	outtextxy(GA.game_width + 25, 310 * GA.H_ratio, ws_score.c_str());
}

//这个函数用来判断游戏是否失败
void Game::IsGameOver()
{
	for (int j = 0; j < map_width; j++)
	{
		if (map[0][j] != 0)
		{
			//保存分数到排行榜
			Rank temp;
			temp.user_name = user_name, temp.score = i_score;
			time_t now = time(0);
			tm timeinfo;
			localtime_s(&timeinfo, &now);
			temp.year = timeinfo.tm_year + 1900, temp.mouth = timeinfo.tm_mon + 1;
			temp.day = timeinfo.tm_mday, temp.hour = timeinfo.tm_hour, temp.min = timeinfo.tm_min;
			rank.push_back(temp);
			File file;
			file.SaveRank(rank_filename, rank);
			//播放失败音效
			mciSendString(L"open bgm/失败.mp3", 0, 0, 0);
			mciSendString(L"play bgm/失败.mp3 wait", 0, 0, 0);
			mciSendString(L"stop bgm/失败.mp3", 0, 0, 0);
			mciSendString(L"close bgm/失败.mp3", 0, 0, 0);
			game_state = Game::GameState::GS_GameOver;
			break;
		}
	}
}


//游戏暂停
void Game::GamePause()
{
	settextcolor(RED);
	settextstyle(48 * GA.W_ratio, 24 * GA.H_ratio, L"隶书");
	outtextxy(125 * GA.W_ratio, 250 * GA.H_ratio, L"游戏暂停");
	//延时一下，防止暂停后又开始了
	Sleep(SLEEP_TIME + 10);
	if (IS_KEY_DOWN(GA.PauseKey))
	{
		game_state = Game::GameState::GS_GamePlay;
		Sleep(SLEEP_TIME + 10);
		//延时一下，防止开始后又暂停了
	}
	if (IS_KEY_DOWN(GA.QuitKey))
	{
		game_state = Game::GameState::GS_GameOverSelectSave;
	}
}



//这个函数用来提醒玩家是否要保存存档
void Game::SelectSaveOrNo()
{
	//清除屏幕
	cleardevice();
	//画两个矩形确定范围，方便进行鼠标事件的处理
	setfillcolor(WHITE);
	solidrectangle(200 * GA.W_ratio, 230 * GA.H_ratio, 230 * GA.W_ratio, 255 * GA.H_ratio);
	solidrectangle(200 * GA.W_ratio, 260 * GA.H_ratio, 230 * GA.W_ratio, 285 * GA.H_ratio);
	setcolor(RGB(148, 6, 6));
	settextstyle(30 * GA.W_ratio, 15 * GA.H_ratio, L"隶书");
	outtextxy(150 * GA.W_ratio, 175 * GA.H_ratio, L"是否保存游戏进度？");
	outtextxy(200 * GA.W_ratio, 230 * GA.H_ratio, L"是");
	outtextxy(200 * GA.W_ratio, 260 * GA.H_ratio, L"否");
	SelectSaveMouseEvent();
}

//这个函数用来选择是否存档的鼠标事件
void Game::SelectSaveMouseEvent()
{
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		//点击“是”，保存存档
		if (msg.x > 200 * GA.W_ratio && msg.x < 230 * GA.W_ratio && msg.y > 230 * GA.H_ratio && msg.y < 255 * GA.H_ratio)
		{
			game_state = Game::GameState::GS_GameOverSave;
		}
		//点击“否”，直接结束游戏即可
		if (msg.x > 200 * GA.W_ratio && msg.x < 230 * GA.W_ratio && msg.y > 260 * GA.H_ratio && msg.y < 285 * GA.H_ratio)
		{
			//保存分数到排行榜
			Rank temp;
			temp.user_name = user_name, temp.score = i_score;
			time_t now = time(0);
			tm timeinfo;
			localtime_s(&timeinfo, &now);//获取目前系统时间
			temp.year = timeinfo.tm_year + 1900, temp.mouth = timeinfo.tm_mon + 1;
			temp.day = timeinfo.tm_mday, temp.hour = timeinfo.tm_hour, temp.min = timeinfo.tm_min;
			rank.push_back(temp);
			File file;
			file.SaveRank(rank_filename, rank);
			//播放失败音效
			mciSendString(L"open bgm/失败.mp3", 0, 0, 0);
			mciSendString(L"play bgm/失败.mp3 wait", 0, 0, 0);
			mciSendString(L"stop bgm/失败.mp3", 0, 0, 0);
			mciSendString(L"close bgm/失败.mp3", 0, 0, 0);
			game_state = Game::GameState::GS_GameOver;
		}
	}
}


//这个函数用来保存存档并提示,然后返回开始菜单
void Game::GameSave()
{
	File file;
	std::string filename = user_name + map_filename;
	file.SaveMapFile(filename, map, map_width, map_height, now_shape, now_type, next_type, i_score);
	game_state = Game::GameState::GS_StartMenu;
}


//这个函数是结束的总函数
void Game::GameOver()
{
	GameOverMenu();
	GameOverSelect();
}

//游戏结束画面
void Game::GameOverMenu()
{
	setbkcolor(GA.background_color);
	cleardevice();
	settextcolor(RGB(255, 0, 0));
	settextstyle(50 * GA.W_ratio, 25 * GA.H_ratio, L"隶书");
	outtextxy(175 * GA.W_ratio, 175 * GA.H_ratio, L"游戏失败");
	outtextxy(120 * GA.W_ratio, 250 * GA.H_ratio, L"您的最终得分是：");
	std::wstring ws_score(s_score.begin(), s_score.end());//因为要用outtextxy显示分数，参数得是宽字符，不然会乱码
	outtextxy(275 * GA.W_ratio, 300 * GA.H_ratio, ws_score.c_str());

	//再来一局或者返回菜单
	setfillcolor(WHITE);
	solidrectangle(150 * GA.W_ratio, 400 * GA.H_ratio, (150 + 120) * GA.W_ratio, (400 + 35) * GA.H_ratio);
	solidrectangle(325 * GA.W_ratio, 400 * GA.H_ratio, (325 + 120) * GA.W_ratio, (400 + 35) * GA.H_ratio);
	setcolor(BLACK);
	settextstyle(30 * GA.W_ratio, 15 * GA.H_ratio, L"隶书");
	outtextxy(150 * GA.W_ratio, 400 * GA.H_ratio, L"再来一局");
	outtextxy(325 * GA.W_ratio, 400 * GA.H_ratio, L"返回菜单");
}

//游戏结束后选择
void Game::GameOverSelect()
{
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN)
	{
		//点击再来一句即进入游戏进行状态
		if (msg.x > 150 * GA.W_ratio && msg.x < (150 + 120) * GA.W_ratio && msg.y > 400 * GA.H_ratio && msg.y < (400 + 35) * GA.H_ratio)
		{
			game_state = Game::GameState::GS_StartNewGame;
		}
		//点击返回菜单
		if (msg.x > 325 * GA.W_ratio && msg.x < (325 + 120) * GA.W_ratio && msg.y>400 * GA.H_ratio && msg.y < (400 + 35) * GA.H_ratio)
		{
			game_state = Game::GameState::GS_StartMenu;
		}
	}
}


//程序结束时要进行的一些初始处理
void Game::ProgramOverDestory()
{
	if (now_shape == nullptr)
		delete now_shape;
	if (next_shape == nullptr)
		delete next_shape;
	if (show_next_shape == nullptr)
		delete show_next_shape;
}

//这个函数用来退出程序
void Game::ExitProgram()
{
	//在程序结束前delete指针，防止内存泄露
	ProgramOverDestory();
	MessageBox(0, L"欢迎下次继续！！！", L"游戏结束", MB_OK);
	exit(0);
}