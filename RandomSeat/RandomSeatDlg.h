
// RandomSeatDlg.h: 头文件
//

#pragma once

#define ADMINISTRATOR	"韩镒名"


// CRandomSeatDlg 对话框
class CRandomSeatDlg : public CDialogEx
{
// 构造
public:
	CRandomSeatDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANDOMSEAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 当前状态
	CString v_Status;
	// 输出文件路径
	CString v_File_Path;
	// 处理名单
	std::vector<std::string> name_list;
	// 生成后是否打开文件
	BOOL v_isopen;
	// 是否两人一组
	BOOL v_isgroup;
	// 一排有几个人
	int v_Num;
	// 讲台在下
	BOOL v_Down;
	// 有人检查？
	BOOL ischeck;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedSelect();

	// 生成讲台位置
	void PlatformInMiddle(std::ostream& is) const
	{
		for (size_t i = 0; i <((v_Num % 2 == 0) ? (v_Num / 2) + 1 : (static_cast<unsigned long long>(v_Num / 2)) + 2); i++)	// 第一个+1是向上取整，第二个+1是因为有前后门，多了一列
		{
			is << ",";
		}
		is << "讲台" << std::endl;
	}

	// 检查名字。找到返回1，没找到返回0
	int check(std::string tar, std::vector<std::string> a, const unsigned int flag)
	{
		for (size_t i = 0; i < flag; i++)
			if (a.at(i) == tar)
				return TRUE;
		return FALSE;
	}
	int check(std::string tar, std::vector<std::string> a, const unsigned int bflag, const unsigned int eflag)
	{
		for (size_t i = bflag; i < eflag; i++)
			if (a.at(i) == tar)
				return 0;
		return 1;
	}
	// 换座位
	void changedeskmate(std::vector<std::string> &data, const std::string tar1, const std::string tar2)
	{
		using namespace std;
		vector<string>::iterator iterA = find(data.begin(), data.end(), tar1); //查找
		vector<string>::iterator iterB = find(data.begin(), data.end(), tar2); //查找
		string str;

		if ((iterA != data.end()) && (iterB != data.end()))	// 找到
		{
			if (distance(data.begin(), iterA) % 2 == 0)	// 找到TAR1_1的位置，即元素的序号。如果为偶数，即位于0，2，4...列
			{
				str = *(iterA + 1);
				*(iterA + 1) = *iterB;
				*iterB = str;
			}
			else
			{
				str = *(iterA - 1);
				*(iterA - 1) = *iterB;
				*iterB = str;
			}
		}
	}

	// 生成随机座位
	void CRandomSeatDlg::GenerateSeat(std::vector<std::string> &input);
	int GenerateMany;
	std::string Custom_Setting_File_Path;
	afx_msg void OnClose();
	BOOL IsDownload=false;
};
