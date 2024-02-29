#include"MyConsoleEnegine.h"

//保存存档
void File::SaveMapFile(std::string filename, std::vector<std::vector<int>>map, int map_width, int map_height, Shape* now_shape, int now_type, int next_type, int score)
{
	std::ofstream outfile(filename, std::ios::out);
	if (!outfile.is_open())
	{
		MessageBox(0, L"存档储存失败", 0, 0);
		return;
	}
	outfile << map_width << " " << map_height << std::endl;
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
			outfile << map[i][j] << (j == map_width - 1 ? "" : " ");
		outfile << std::endl;
	}
	outfile << now_shape->B[0].x << " " << now_shape->B[0].y << " " << now_type << " " << next_type << " " << score << std::endl;
	MessageBox(0, L"存档储存成功", L"恭喜！！！", 0);
	outfile.close();
}

//读取文档
void File::ReadMapFile(std::string filename, std::vector<std::vector<int>>& map, int map_width, int map_height, int& x, int& y, int& now_type, int& next_type, int& score, bool& is_read_succeed)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"存档读取失败!!!", 0, 0);
		return;
	}
	int temp_map_width, temp_map_height;
	infile >> temp_map_width >> temp_map_height;
	if (temp_map_width != map_width || temp_map_height != map_height)
	{
		std::string str = "您上次存档的屏幕宽度是";
		std::string s_width = std::to_string(temp_map_width * Block::width);
		std::string str1 = "高度是";
		std::string s_height = std::to_string(temp_map_height * Block::width);
		std::string str2 = "请重新设置后再读取存档";
		std::string all = str + s_width + str1 + s_height + str2;
		MessageBoxA(0, all.c_str(), 0, 0);
		is_read_succeed = false;
		return;
	}
	for (int i = 0; i < map_height; i++)
	{
		for (int j = 0; j < map_width; j++)
		{
			infile >> map[i][j];
		}
	}
	infile >> x >> y >> now_type >> next_type >> score;
	is_read_succeed = true;
	infile.close();
}

//保存用户信息
void File::SaveUserInfo(std::string filename, std::string user_name, std::string password)
{
	std::ofstream outfile(filename, std::ios::app | std::ios::out);
	if (!outfile.is_open())
	{
		MessageBox(0, L"用户注册失败", 0, 0);
		return;
	}
	outfile << user_name << " " << password << std::endl;
	MessageBox(0, L"用户注册成功", L"恭喜", 0);
	outfile.close();
}

//读取用户信息
void File::ReadUserInfo(std::string filename, std::string user_name, std::string password, bool& is_log_in, std::string& now_user_name)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"用户登录失败", 0, 0);
		return;
	}
	std::string temp_user_name;
	std::string temp_password;
	while (infile >> temp_user_name >> temp_password)
	{
		if (user_name == temp_user_name && password != temp_password)
		{
			MessageBox(0, L"密码错误！", 0, 0);
			infile.close();
			return;
		}
		if (user_name == temp_user_name && password == temp_password)
		{
			is_log_in = true;
			now_user_name = temp_user_name;
			MessageBox(0, L"登录成功！", 0, 0);
			infile.close();
			return;
		}
	}

	MessageBox(0, L"未找到该用户名！", L"恭喜", 0);
	infile.close();
}

//保存默认登录的信息
void File::SaveDefauleLogin(std::string filename, std::string user_name)
{
	std::ofstream outfile(filename, std::ios::trunc);
	if (!outfile.is_open())
	{
		MessageBox(0, L"默认登录信息保存失败", 0, 0);
		return;
	}
	outfile << user_name;
	outfile.close();
}

//读取默认登录信息
void File::ReadDefaultLogin(std::string filename, std::string& user_name, bool& is_log_in)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile.is_open())
	{
		return;
	}
	infile >> user_name;
	if (user_name == "")
	{
		is_log_in = false;
	}
	else
	{
		is_log_in = true;
	}
	infile.close();
}

//保存地图大小信息
void File::SaveMapSize(std::string filename, int width, int height)
{
	std::ofstream outfile(filename, std::ios::out);
	if (!outfile.is_open())
	{
		MessageBox(0, L"保存地图大小信息失败！", 0, 0);
		return;
	}
	outfile << width << " " << height;
	outfile.close();
}
//读取地图大小信息
void File::ReadMapSize(std::string filename, int& width, int& height)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile.is_open())
	{
		return;
	}
	infile >> width >> height;
	infile.close();
}

//保存键盘设置
void File::SaveKeySet(std::string filename, int PauseKey, int UpKey, int DownKey, int LeftKey, int RightKey, int QuickDownKey, int QuitKey)
{
	std::ofstream outfile(filename, std::ios::out);
	if (!outfile.is_open())
	{
		MessageBox(0, L"保存键盘设置失败", 0, 0);
		return;
	}
	outfile << PauseKey << " " << UpKey << " " << DownKey << " " << LeftKey << " " << RightKey << " " << QuickDownKey << " " << QuitKey;
	outfile.close();
}
//读取键盘设置
void File::ReadKeySet(std::string filename, int& PauseKey, int& UpKey, int& DownKey, int& LeftKey, int& RightKey, int& QuickDownKey, int& QuitKey)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile.is_open())
	{
		MessageBox(0, L"读取键盘设置失败", 0, 0);
		return;
	}
	infile >> PauseKey >> UpKey >> DownKey >> LeftKey >> RightKey >> QuickDownKey >> QuitKey;
	infile.close();
}

//保存排行榜记录
void File::SaveRank(std::string filename, std::vector<Rank>rank)
{
	std::ofstream outfile(filename, std::ios::trunc);
	if (!outfile.is_open())
	{
		MessageBox(0, L"排行榜保存失败！", 0, 0);
		return;
	}
	sort(rank.begin(), rank.end(), [](const Rank& a, const Rank& b) {return a.score > b.score; });
	while (rank.size() > 5)
	{
		rank.erase(rank.end() - 1);
	}
	for (int i = 0; i < rank.size(); i++)
	{
		outfile << rank.at(i).user_name << " " << rank.at(i).score << " " << rank.at(i).year << " " << rank.at(i).mouth << " " << rank.at(i).day << " " << rank.at(i).hour << " " << rank.at(i).min << std::endl;
	}
	outfile.close();
}
//读取排行榜记录
void File::ReadRank(std::string filename, std::vector<Rank>& rank)
{
	std::ifstream infile(filename, std::ios::in);
	if (!infile.is_open())
	{
		return;
	}
	rank.clear();
	Rank temp;
	while (infile.peek() != std::ifstream::traits_type::eof())
	{
		infile >> temp.user_name >> temp.score >> temp.year >> temp.mouth >> temp.day >> temp.hour >> temp.min;
		rank.push_back(temp);
	}
	rank.erase(rank.end() - 1);
	infile.close();
}