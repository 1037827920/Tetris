#pragma once

class File
{
public:
	//保存存档
	void SaveMapFile(std::string filename, std::vector<std::vector<int>>map, int map_width, int map_height, Shape* now_shape, int now_type, int next_type, int score);
	//读取文档
	void ReadMapFile(std::string filename, std::vector<std::vector<int>>& map, int map_width, int map_height, int& x, int& y, int& now_type, int& next_type, int& score, bool& is_read_succeed);

	//保存用户信息
	void SaveUserInfo(std::string filename, std::string user_name, std::string password);
	//读取用户信息
	void ReadUserInfo(std::string filename, std::string user_name, std::string password, bool& is_log_in, std::string& now_user_name);

	//保存默认登录的信息
	void SaveDefauleLogin(std::string filename, std::string user_name);
	//读取默认登录信息
	void ReadDefaultLogin(std::string filename, std::string& user_name, bool& is_log_in);

	//保存地图大小信息
	void SaveMapSize(std::string filename, int width, int height);
	//读取地图大小信息
	void ReadMapSize(std::string filename, int& width, int& height);

	//保存键盘设置
	void SaveKeySet(std::string filename, int PauseKey, int Upkey, int DownKey, int LeftKey, int RightKey, int QuickDownKey, int QuitKey);
	//读取键盘设置
	void ReadKeySet(std::string filename, int& PauseKey, int& Upkey, int& DownKey, int& LeftKey, int& RightKey, int& QuickDownKey, int& QuitKey);

	//保存排行榜记录
	void SaveRank(std::string filename, std::vector<Rank>rank);
	//读取排行榜记录
	void ReadRank(std::string filename, std::vector<Rank>& rank);
};