
// RandomSeatDlg.h: 头文件
//

#pragma once


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
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAbout();
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
	afx_msg void OnBnClickedSelect();
	// 讲台在下
	BOOL v_Down;

	// 生成讲台位置
	void PlatformInMiddle(std::ostream& is)
	{
		// TODO: 在此处添加实现代码.
		for (size_t i = 0; i < (int)(v_Num / 2) + 1 + 1; i++)	// 第一个+1是向上取整，第二个+1是因为有前后门，多了一列
		{
			is << ",";
		}
		is << "讲台" << std::endl;
	}

	// 检查名字。找到返回0，没找到返回1
	int check(std::vector<std::string> a, int flag)
	{
		// TODO: 在此处添加实现代码.
		for (size_t i = 0; i < flag; i++)
			if (a.at(i) == "韩镒名")
				return 0;
		return 1;
	}
};
