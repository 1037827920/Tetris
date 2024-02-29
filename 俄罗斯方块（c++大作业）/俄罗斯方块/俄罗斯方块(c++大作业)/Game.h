#include"MyConsoleEnegine.h"

namespace Game
{
	//游戏状态
	namespace GameState
	{
		//游戏开始页面状态
		const int GS_StartMenu = 0;
		const int GS_StartReadMap = 10;
		const int GS_StartNewGame = 100;//开始新游戏前先初始化
		const int GS_LogIn = 1000;//登录界面
		const int GS_Regist = 1001;//注册界面
		const int GS_Set = 10000;//设置界面
		const int GS_Help = 10001;//帮助界面
		const int GS_Rank = 100000;//排行榜界面
		//游戏进行状态
		const int GS_GamePlay = 1;
		//游戏暂停状态
		const int GS_GamePause = 2;
		//游戏结束
		const int GS_GameOver = 30;
		//游戏结束提示是否保存
		const int GS_GameOverSelectSave = 31;
		//保存存档
		const int GS_GameOverSave = 32;
		//退出程序
		const int GS_ProgramExit = 4;
	}

	//游戏基本属性  先声明有这个变量
	extern GameAttribute GA;
	//游戏地图的状态
	extern int map_width;
	extern int map_height;
	extern std::vector<std::vector<int>>map;
	//这里map_height是由GAME_HEIGHT/Block::width得出来的,还需要加上上面的四格
	//这里的map_width是由GAME_WIDTH/Block::width得出来的
	//0代表空白  1代表O型  2代表I型	  3代表S型
	//4代表Z型   5代表J型  6代表L型   7代表T型
	//计时器
	extern DWORD tPre;
	extern DWORD tNow;
	//游戏的状态
	extern int game_state;
	//鼠标信息
	extern MOUSEMSG msg;
	//目前方块和下一个方块
	extern Shape* now_shape;
	extern Shape* next_shape;
	extern Shape* show_next_shape;
	//方块核心位置
	extern int x;
	//产生方块我们需要给定一个范围，这样可以保证不会出现奇奇怪怪的错误
	//其实就是使方块的落点比较整齐
	extern std::vector<int>x_scope;
	//方块类型
	extern int now_type;
	extern int next_type;
	//判断方块是否固定
	extern bool is_located;
	//判断方块是否快速下降
	extern bool is_quick_down;

	//游戏行数的消除积分
	extern int i_score;
	extern std::string s_score;
	//地图文件名种子   对应的用户名各自有一个存档
	extern std::string map_filename;
	//检测存档是否读取成功
	extern bool is_read_succeed;
	//判断用户是否登录，如果没有登录，不能进行游玩
	extern bool is_log_in;
	//判断鼠标点击的是账号输入框还是密码输入框
	extern int account_or_password;//0 为空   1 为账号输入框   2 为密码输入框
	//账号输入框的实时显示字符串
	extern std::string show_user_name;
	//密码输入框的实时显示字符串
	extern std::string show_password;
	//用来储存用户信息的文件
	extern std::string user_filename;
	//用户名
	extern std::string user_name;
	//默认登录用户储存文件
	extern std::string default_user_filename;
	//地图大小文件
	extern std::string map_size_filename;
	//键盘默认设置文件
	extern std::string default_key_filename;
	//键盘设置文件
	extern std::string now_key_filename;
	//排行榜
	extern std::vector<Rank>rank;
	//排行榜储存文件
	extern std::string rank_filename;
	//音乐目前曲目
	extern int music_index;
	//检测是否暂停音乐
	extern bool is_stop_music;
	//是否播放消行音效
	extern bool is_play_audio1;
	//是否播放落方块音效
	extern bool is_play_audio2;
	//目前的背景颜色
	extern int bkcolor_index;

	//游戏最开始的初始化
	void GameInit();
	//游戏状态函数
	void SelectGameState();
	//游戏开始菜单（总函数，包含下面）
	void GameStartMenu();
	//菜单页面的鼠标时间
	void MenuMouseEvent();
	//读取上一次的存档
	void ReadLastMap();
	//这个函数用来设置登录菜单
	void LogInMenu();
	//这个函数用来处理登录菜单的鼠标事件
	void LogInMenuMouseEvent();
	//注册菜单
	void RegistMenu();
	//注册菜单鼠标处理函数
	void RegistMenuMouseEvent();
	//设置界面
	void SetMenu();
	//设置界面鼠标处理事件
	void SetMenuMouseEvent();
	//帮助界面
	void ShowHelpMenu();
	//帮助界面鼠标处理函数
	void HelpMenuMouseEvent();
	//排行榜菜单
	void RankMenu();
	//排行榜菜单鼠标事件处理函数
	void RankMenuMouseEvent();
	//播放音乐
	void PlayBGM();


	//这个函数用来初始化游戏
	void GamePlayInit();
	//游戏进行的总函数，包含下面的函数
	void GamePlay();
	//游戏进行背景的绘制
	void GamePlayBackground();
	//预先获得下一个方块
	void GamePreGainNextBlock();
	//给现在的方块赋值并预先获取写一个方块的形状
	void GameAssignNowBlock();
	//游戏进行时
	void GamePlaying();
	//这个函数用来画出四个方块
	void DrawFourBlock(Block* B, int color);
	//用这个函数来画地图
	void DrawMap();
	//这个函数用来模拟四个方块的移动
	void BlockMoveAndBlockCrash(Block* B);
	//这个函数用来给地图的状态赋值
	void AssignMapState(Block* B, int color);
	//这个函数用来每一行的检测，满了就清空
	void CheckLine();
	//这个函数用来模拟方块下落预期,并通过按下空格键能使方块快速下降
	void SimulateBlockDownAndBlockQuickDown(Block* B, int color);
	//这个函数用来画出下一个方块的形状
	void DrawNextBlock();
	//这个函数用来显示分数
	void GameShowScore();
	//这个函数用来判断游戏是否失败
	void IsGameOver();


	//游戏暂停
	void GamePause();


	//这个函数用来提醒玩家是否要保存存档
	void SelectSaveOrNo();
	//这个函数用来选择是否存档的鼠标事件
	void SelectSaveMouseEvent();
	//这个函数用来保存存档并提示
	void GameSave();


	//这个函数是结束的总函数
	void GameOver();
	//游戏结束画面
	void GameOverMenu();
	//游戏结束后选择  鼠标处理事件
	void GameOverSelect();


	//程序结束时要进行的一些初始处理
	void ProgramOverDestory();
	//这个函数用来退出程序
	void ExitProgram();
}